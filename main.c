/** main.c
 *
 *  Created on: Apr 21, 2018
 *      Author: aland
 */

#include "SPI.h"
#include "GPIO.h"
#include "DataTypeDefinitions.h"
#include "LCDNokia5110.h"
#include "PIT.h"
#include "NVIC.h"
#include "LCD_ILI9341.h"
#include "MCG.h"

#define CLK_FREQ_HZ 50000000  /* CLKIN0 frequency */
#define SLOW_IRC_FREQ 32768	/*This is the approximate value for the slow irc*/
#define FAST_IRC_FREQ 4000000 /*This is the approximate value for the fast irc*/
#define EXTERNAL_CLOCK 0 /*It defines an external clock*/
#define PLL_ENABLE 1 /**PLL is enabled*/
#define PLL_DISABLE 0 /**PLL is disabled*/
#define CRYSTAL_OSC 1  /*It defines an crystal oscillator*/
#define LOW_POWER 0     /* Set the oscillator for low power mode */
#define SLOW_IRC 0 		/* Set the slow IRC */
#define CLK0_TYPE 0     /* Crystal or canned oscillator clock input */
#define PLL0_PRDIV 25    /* PLL predivider value */
#define PLL0_VDIV 30    /* PLL multiplier value*/
#define SYSTEM_CLOCK 30000000
#define DELAY_900MS 1

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

int main(void)
{
	int mcg_clk_hz;
	mcg_clk_hz = fbe_pbe(CLK_FREQ_HZ,PLL0_PRDIV,PLL0_VDIV);

	PIT_clockGating();
	PIT_enable();
	SPI_init(&SPI_Config); /*! Configuration function for the LCD port*/
	LCD_ILI9341_init();
	NVIC_enableInterruptAndPriotity(PIT_CH0_IRQ, PRIORITY_6);

	EnableInterrupts;
	PIT_delay(PIT_0,SYSTEM_CLOCK / 2,DELAY_900MS);

	LCD_ILI9341_drawPartiture(TRUE);
	LCD_ILI9341_drawPartiture(FALSE);

	uint16 color = ILI9341_GREENYELLOW;
	uint8 nShape = 3;

	LCD_ILI9341_writeBigLetter(0,0,0xFF,0);
	LCD_ILI9341_writeLetter(17,7,0xFF,'L');
	LCD_ILI9341_writeBigLetter(25,0,0xFF,0);
	LCD_ILI9341_writeLetter(40,7,0xFF,'N');

    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
	PORTC->PCR[1]   = PORT_PCR_MUX(0x4);

	FTM0->MODE |= FTM_MODE_WPDIS_MASK;
	FTM0->MODE &= ~FTM_MODE_FTMEN_MASK;
	FTM0->CONF |= FTM_CONF_BDMMODE(3);

	FTM0->CNT = 0x0;
	FTM0->COMBINE = 0;
	FTM0->MOD = 0x1;
	FTM0->CNTIN = 0;
	FTM0->SC = 0;
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSA_MASK| FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[0].CnV= 0x1;
	FTM0->SC |= FTM_SC_CLKS(1);

	while(TRUE)
	{
		if(PIT_getIRQStatus(PIT_0))
		{
			nShape = (nShape + 1) % 4;

			if(nShape != 3)
			{
				LCD_ILI9341_drawShape(60 + nShape *35, 305, 74 + nShape *35, 319, color);
			}

			else
			{
				LCD_ILI9341_drawNLines(305, 14, ILI9341_CYAN);
			}

			PIT_clearIRQStatus(PIT_0);

		}

		color++;
	}
}


>>>>>>> 379af5875bbdd9f137742409b023fb08ffe5e3d0
