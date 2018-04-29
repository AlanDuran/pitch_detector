/**
	\file 
		main.c
	\brief 
		El main
	\author Dario Hoyo
	\author Alan Duran
	\date	18/03/2018
	\todo
	    The SPI device driver needs to be completed.
 */

#include "GPIO.h"
#include "DataTypeDefinitions.h"
#include "ADC.h"
#include "NVIC.h"
#include "FLEX.h"

#define ADC_channel 12

const ADC_ConfigType ADC_Config = {
							INTERRUPT_ENABLE,
							DIFFERENTIAL_DISABLE,
							MODE_16_BIT,
							BUS,
							ADC_channel,
							SAMPLE_32,
							CC_ENABLE,
							AVERAGE_DISABLE};


int main(void)
{
	NVIC_setBASEPRI_threshold(PRIORITY_10);
	FLEX_init();
	ADC0_init(&ADC_Config);

	EnableInterrupts;
	for(;;)
	{

	}
}

