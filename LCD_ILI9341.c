/*
 * LCD_ILI9341.c
 *
 *  Created on: Apr 21, 2018
 *      Author: aland
 */

#include "LCD_ILI9341.h"

void LCD_ILI9341_init(void)
{
	GPIO_pinControlRegisterType pinControlRegister = GPIO_MUX1;

	///Configure control pins
	GPIO_clockGating(GPIO_D);
	GPIO_dataDirectionPIN(GPIO_D,GPIO_OUTPUT,DATA_OR_CMD_PIN);
	GPIO_pinControlRegister(GPIO_D,DATA_OR_CMD_PIN,&pinControlRegister);
	GPIO_dataDirectionPIN(GPIO_D,GPIO_OUTPUT,CS_PIN);
	GPIO_pinControlRegister(GPIO_D,CS_PIN,&pinControlRegister);

	/**Reset LCD and configure*/
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	GPIO_setPIN(GPIO_D, CS_PIN);

	/**Configure the DATA_OR_CMD PIN*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	/**Sends data*/


	SPI_startTranference(SPI_0);
	GPIO_clearPIN(GPIO_D, CS_PIN);
	SPI_sendOneByte(SPI_0, ADAFRUIT_CMD);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADA_1);
	SPI_sendOneByte(SPI_0, ADA_2);
	SPI_sendOneByte(SPI_0, ADA_3);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, VCOM_CMD);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, VCOM_1);
	SPI_sendOneByte(SPI_0, VCOM_2);
	SPI_sendOneByte(SPI_0, VCOM_3);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADAFRUIT_CMD2);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADA2_1);
	SPI_sendOneByte(SPI_0, ADA2_2);
	SPI_sendOneByte(SPI_0, ADA2_3);
	SPI_sendOneByte(SPI_0, ADA2_4);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADAFRUIT_CMD3);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADA3_1);
	SPI_sendOneByte(SPI_0, ADA3_2);
	SPI_sendOneByte(SPI_0, ADA3_3);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADAFRUIT_CMD4);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADA4_1);
	SPI_sendOneByte(SPI_0, ADA4_2);
	SPI_sendOneByte(SPI_0, ADA4_3);
	SPI_sendOneByte(SPI_0, ADA4_4);
	SPI_sendOneByte(SPI_0, ADA4_5);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADAFRUIT_CMD5);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADA5_1);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADAFRUIT_CMD6);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, ADA6);
	SPI_sendOneByte(SPI_0, ADA6);

	/**Power Control*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_PWCTR1);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_PWCTR1);
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_PWCTR2);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_PWCTR2);

	/**VCM Control*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_VMCTR1);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_VMCTR1);
	SPI_sendOneByte(SPI_0, DATA2_VMCTR1);
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_VMCTR2);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_VMCTR2);

	/**Memory Access Control*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_MADCTL);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_MADCTL);

	/**Vertical Scroll*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_VSCRSADD);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_VSCRSADD);
	/**Other*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_PIXFMT);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_PIXFMT);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_FRMCTR1);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_FRMCTR1);
	SPI_sendOneByte(SPI_0, DATA2_FRMCTR1);

	/**Display Function Control*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_DFUNCTR);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_DFUNCTR);
	SPI_sendOneByte(SPI_0, DATA2_DFUNCTR);
	SPI_sendOneByte(SPI_0, DATA3_DFUNCTR);

	/**3Gamma Function Disable*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_3DGAMMA);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_3DGAMMA);

	/**Gamma curve selected*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_GAMMASET);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA_GAMMASET);

	/**Set Gamma*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_GMCTRP1);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA0_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA2_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA3_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA4_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA5_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA6_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA7_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA8_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA9_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA10_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA11_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA12_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA13_GMCTRP1);
	SPI_sendOneByte(SPI_0, DATA14_GMCTRP1);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_GMCTRN1);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, DATA0_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA1_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA2_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA3_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA4_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA5_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA6_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA7_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA8_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA9_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA10_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA11_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA12_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA13_GMCTRN1);
	SPI_sendOneByte(SPI_0, DATA14_GMCTRN1);

	/**Exit sleep*/
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_SLPOUT);
	LCD_ILI9341_delay();
	/**Display on*/
	SPI_sendOneByte(SPI_0, CMD_DISPON);
	LCD_ILI9341_delay();
	SPI_stopTranference(SPI_0);

	LCD_ILI9341_fillScreen(ILI9341_BLACK);
}

void LCD_ILI9341_fillScreen(uint16 color)
{
	uint32 index;

	SPI_startTranference(SPI_0);
	GPIO_clearPIN(GPIO_D, CS_PIN);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_CASET);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, FALSE);
	SPI_sendOneByte(SPI_0, FALSE);
	SPI_sendOneByte(SPI_0, FALSE);
	SPI_sendOneByte(SPI_0, CA_LIMIT);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_PASET);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, FALSE);
	SPI_sendOneByte(SPI_0, FALSE);
	SPI_sendOneByte(SPI_0, PA_LIMIT1);
	SPI_sendOneByte(SPI_0, PA_LIMIT2);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_RAMWR);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);

	for(index = FALSE; index < SCREEN_SIZE; index++)
	{
		SPI_sendOneByte(SPI_0, color >> COLOR_SHIFT);
		SPI_sendOneByte(SPI_0, color & COLOR_MASK);
	}

	GPIO_setPIN(GPIO_D, CS_PIN);
	SPI_stopTranference(SPI_0);
}

void LCD_ILI9341_writeColor(uint16 x, uint16 y, uint16 data)
{
	/**Configure the DATA_OR_CMD PIN*/
	if(DataOrCmd)
		GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	else
		GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);

	/**Sends data*/
	SPI_startTranference(SPI_0);
	GPIO_clearPIN(GPIO_D, CS_PIN);
	SPI_sendOneByte(SPI_0, data >> COLOR_SHIFT);
	SPI_sendOneByte(SPI_0, data & COLOR_MASK);
	GPIO_setPIN(GPIO_D, CS_PIN);
	SPI_stopTranference(SPI_0);
}


void LCD_ILI9341_delay(void)
{
	uint32 counter;
	/**Does nothing over a certain amount of time*/
	for(counter=0; counter<18000; counter++)
	{

	}
}
