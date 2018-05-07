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
#include "MCG.h"
#include "PIT.h"
#include "SPI.h"
#include "PENTA.h"
#include "LCD_ILI9341.h"
#include "DSP.h"
#include <stdio.h>



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

const SPI_ConfigType SPI_Config = {
		SPI_DISABLE_FIFO,
		SPI_LOW_POLARITY,
		SPI_LOW_PHASE,
		SPI_MSB,
		SPI_0,
		SPI_MASTER,
		GPIO_MUX2,
		SPI_BAUD_RATE_2,
		SPI_FSIZE_8,
		{GPIO_D,BIT1,BIT2}
};

int main()
{
	int mcg_clk_hz = pll_init(CLK_FREQ_HZ, LOW_POWER, EXTERNAL_CLOCK, PLL0_PRDIV, PLL0_VDIV, PLL_ENABLE);
	printf("%d\n", mcg_clk_hz);
	NVIC_setBASEPRI_threshold(PRIORITY_10);
	PIT_clockGating();
	PIT_enable();
	NVIC_enableInterruptAndPriotity(PIT_CH1_IRQ,PRIORITY_3);
	SPI_init(&SPI_Config);
	LCD_ILI9341_init();
	LCD_ILI9341_drawPartiture(FALSE);
	LCD_ILI9341_drawPartiture(TRUE);
	FLEX_init();
	ADC0_init(&ADC_Config);
	EnableInterrupts;
	PENTA_startTimeMeassure();


	for(;;)
	{
		/** Do correlations and save stuff */
		if(0 != DSP_getGeneralStatus())
		{
			DSP_autocor();
			uint16 pitch = DSP_detectPeak();
			float32 f0 = DSP_findPitch(pitch);
			DSP_clearBuffer();

			/** Find y position of note */
			PENTA_findNote(f0);

			PENTA_graph();
		}
		/** */

		/** Draw little squares to indicate tempo */
	}
}

