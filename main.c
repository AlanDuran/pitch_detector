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
#define DELAY_900MS 5

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

	uint16 i, x, y;

	LCD_ILI9341_writeColor(120, 160, 0xFFFF);
	LCD_ILI9341_writeColor(121, 160, 0xFFFF);
	LCD_ILI9341_writeColor(122, 160, 0xFFFF);
	LCD_ILI9341_writeColor(123, 160, 0xFFFF);
	LCD_ILI9341_writeColor(124, 160, 0xFFFF);
	LCD_ILI9341_writeColor(125, 160, 0xFFFF);
	LCD_ILI9341_writeColor(126, 160, 0xFFFF);
	LCD_ILI9341_writeColor(100, 180, 0xFFFF);
	LCD_ILI9341_writeColor(100, 181, 0xFFFF);
	LCD_ILI9341_writeColor(100, 182, 0xFFFF);
	LCD_ILI9341_writeColor(100, 183, 0xFFFF);
	LCD_ILI9341_writeColor(100, 184, 0xFFFF);

	while(TRUE)
	{
		if(PIT_getIRQStatus(PIT_0))
		{
			LCD_ILI9341_writeColor(x, y, i);
			PIT_clearIRQStatus(PIT_0);
		}

		i++;
		x = (x + 1) % 240;
		y = (y + 1) % 320;
	}
}


