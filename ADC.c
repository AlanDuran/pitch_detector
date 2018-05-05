/**
 * \file
 * 			ADC.c
 *
 * \authors
 * 			Dario Hoyo
 *      	Alan Duran
 * \date
 * 			19/03/18
 *
 *      */

#include <ADC.h>
#include <stdio.h>
#include "GPIO.h"
#include "NVIC.h"
#include "DSP.h"
#include "PENTA.h"
#include "FLEX.h"
#include "LCD_ILI9341.h"
#define LIMIT 0xFF
#define SHIFT 8

void ADC0_IRQHandler()
{
	ADC0->SC1[0] |=  0x1f;
	//If the value is greater than [something], start saving values
	/** If not saving, check if it should save*/
	if(FALSE == DSP_getSavingFlag())
	{
		DSP_checkAttack(ADC0_readValue());	//Detects
	}

	/** If saving, save*/
	if(TRUE == DSP_getSavingFlag())
	{
		DSP_saveNote(ADC0_readValue());		//If note is detected, saves it
	}

}

void ADC0_init(const ADC_ConfigType* ADC_Config)
{
	ADC0_clockGating(); /**Activates System Clock Gating for ADC0*/
	setRegisters(ADC_Config); /**Writes the configurations contained in structure into registers*/
	NVIC_enableInterruptAndPriotity(ADC0_IRQ,PRIORITY_5);
}

void ADC0_clockGating()
{   /**System Clock Gating Control Register 6 Mask*/
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
}

void setRegisters(const ADC_ConfigType* ADC_Config)
{
	GPIO_clockGating(GPIO_B);
	/**Selects channel, interrupt and differential mode configurations*/
	ADC0->SC1[0] =  ADC_Config->channel |  ADC_SC1_AIEN(ADC_Config->interrupt)
			|  ADC_SC1_DIFF(ADC_Config->differential);
	/**Selects resolution (mode) and internal clock reference*/
	ADC0->CFG1 = ADC_CFG1_MODE(ADC_Config->mode) | ADC_CFG1_ADICLK(ADC_Config->clockSelect);
	/**Selects average or continuous modes enables*/
	ADC0->SC3 |= ADC_SC3_AVGS(ADC_Config->nSamples) | ADC_SC3_AVGE(ADC_Config->averageEnable)
		| ADC_SC3_ADCO(ADC_Config->cc);
	ADC0->SC2 &= ~ADC_SC2_ACFE_MASK;
	startConversion(ADC_Config->channel);
	//while(TRUE == ADC0_conversionComplete()){}
	//ADC0_readValue();
}

uint16 ADC0_readValue()
{
	/**Returns the first 16 bits of R register, which stores the result of a conversion*/
	return ADC0->R[0] & 0xFFFF;
}

uint8 ADC0_conversionComplete()
{
	/**Returns the COCO bit stored in SC1*/
	return ADC0->SC1[0] & (ADC_SC1_COCO_MASK >> ADC_SC1_COCO_SHIFT);
}

void startConversion(uint8 channel)
{
	/**Writes in SC1 to start a conversion*/
	ADC0->SC1[0] |=  channel;
}

void DSP_clearSC1()
{
	ADC0->SC1[0] &= ~(ADC_SC1_ADCH_MASK);
}
