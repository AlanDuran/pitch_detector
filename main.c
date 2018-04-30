/**
 * main.c
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

#define SYSTEM_CLOCK 21000000
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


