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

#include "FLEX.h"
#include "MK64F12.h"
#include "NVIC.h"
#include "GPIO.h"
#include "ADC.h"
#include <stdio.h>


#define MINIMUM_DECRE 12
#define MOD_4_ADC 0x41 //+ps128 ~~ 2.5KHz
#define ADC_CHANNEL 12
#define DEC 10

//static uint8 FTM3_flag = FALSE;

void FTM3_IRQHandler()
{
	//sample
	startConversion(ADC_CHANNEL);
	FTM3->SC &= ~FLEX_TIMER_TOF;
}

void FLEX_init()
{
	init_adcTimer();
}

void init_adcTimer()
{
	SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;						/**< Clock gating for the FlexTimer 3*/
	NVIC_enableInterruptAndPriotity(FTM3_IRQ,PRIORITY_4);	/**< Enable interrupt for FTM1*/
	FTM3->MOD = MOD_4_ADC;									/**< Mod value for 4 Hz with PS 128*/
	FTM3->SC = FLEX_TIMER_CLKS_1 | FLEX_TIMER_TOIE | FLEX_TIMER_PS_128;/**< Configures FTM1 with OF interrupts enabled and PS 128 but TURNED OFF*/
}
