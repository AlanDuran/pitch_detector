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
#define MOD_4_ADC 0x1d //+ps128 ~~ 2.5KHz
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
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
	PORTC->PCR[1]   = PORT_PCR_MUX(0x4);

	FTM0->MODE |= FTM_MODE_WPDIS_MASK;
	FTM0->MODE &= ~FTM_MODE_FTMEN_MASK;
	FTM0->CONF |= FTM_CONF_BDMMODE(3);

	FTM0->CNT = 0x0;
	FTM0->COMBINE = 0;
	FTM0->MOD = MOD_4_ADC;
	FTM0->CNTIN = 0;
	FTM0->SC = 0;
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSA_MASK| FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[0].CnV= 0x0;
	FTM0->SC |= FLEX_TIMER_CLKS_1| FLEX_TIMER_PS_16;
}

void init_adcTimer()
{
	SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;						/**< Clock gating for the FlexTimer 3*/
	NVIC_enableInterruptAndPriotity(FTM3_IRQ,PRIORITY_4);	/**< Enable interrupt for FTM1*/
	FTM3->MOD = MOD_4_ADC;									/**< Mod value for 4 Hz with PS 128*/
	FTM3->SC = FLEX_TIMER_CLKS_1 | FLEX_TIMER_TOIE | FLEX_TIMER_PS_16;/**< Configures FTM1 with OF interrupts enabled and PS 128 but TURNED OFF*/
}
