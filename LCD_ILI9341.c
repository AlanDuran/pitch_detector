/*
 * LCD_ILI9341.c
 *
 *  Created on: Apr 28, 2018
 *      Author: Raymundo
 */

#include "LCD_ILI9341.h"

static const uint8 ASCII[][6] =
{
		{0x00, 0x00, 0x00, 0x00, 0x00,0x00}, // 20
		{0x00, 0x00, 0x5f, 0x00, 0x00,0x00}, // 21 !
		{0x00, 0x07, 0x00, 0x07, 0x00,0x00}, // 22 "
		{0x14, 0x7f, 0x14, 0x7f, 0x14,0x00}, // 23 #
		{0x24, 0x2a, 0x7f, 0x2a, 0x12,0x00}, // 24 $
		{0x23, 0x13, 0x08, 0x64, 0x62,0x00}, // 25 %
		{0x36, 0x49, 0x55, 0x22, 0x50,0x00}, // 26 &
		{0x00, 0x05, 0x03, 0x00, 0x00,0x00}, // 27 '
		{0x00, 0x1c, 0x22, 0x41, 0x00,0x00}, // 28 (
		{0x00, 0x41, 0x22, 0x1c, 0x00,0x00}, // 29 )
		{0x14, 0x08, 0x3e, 0x08, 0x14,0x00}, // 2a *
		{0x08, 0x08, 0x3e, 0x08, 0x08,0x00}, // 2b +
		{0x00, 0x50, 0x30, 0x00, 0x00,0x00}, // 2c ,
		{0x08, 0x08, 0x08, 0x08, 0x08,0x00}, // 2d -
		{0x00, 0x60, 0x60, 0x00, 0x00,0x00}, // 2e .
		{0x20, 0x10, 0x08, 0x04, 0x02,0x00}, // 2f /

		{0x00, 0x7C, 0x82, 0x82, 0x7C, 0x00},	//0			30
		{0x00, 0x00, 0x42, 0xFE, 0x02, 0x00},	//1			31
		{0x00, 0x46, 0x8A, 0x92, 0x62, 0x00},	//2			32
		{0x00, 0x44, 0x82, 0x92, 0x6C, 0x00},	//3			33
		{0x00, 0xF0, 0x10, 0x10, 0xFE, 0x00},	//4			34
		{0x00, 0xF2, 0x92, 0x92, 0x9E, 0x00},	//5			35
		{0x00, 0xFE, 0x92, 0x92, 0x9E, 0x00},	//6			36
		{0x00, 0x80, 0x80, 0x80, 0xFE, 0x00},	//7			37
		{0x00, 0xFE, 0x92, 0x92, 0xFE, 0x00},	//8			38
		{0x00, 0xF0, 0x90, 0x90, 0xFE, 0x00},	//9			39

		{0x00, 0x36, 0x36, 0x00, 0x00, 0x00}, // 3a :
		{0x00, 0x56, 0x36, 0x00, 0x00, 0x00}, // 3b ;
		{0x08, 0x14, 0x22, 0x41, 0x00, 0x00}, // 3c <
		{0x14, 0x14, 0x14, 0x14, 0x14, 0x00}, // 3d =
		{0x00, 0x41, 0x22, 0x14, 0x08, 0x00}, // 3e >
		{0x02, 0x01, 0x51, 0x09, 0x06, 0x00}, // 3f ?
		{0x32, 0x49, 0x79, 0x41, 0x3e, 0x00}, // 40 @

		{0x00, 0x7C, 0x90, 0x90, 0x7C, 0x00},	//A			41
		{0x00, 0xFC, 0xA4, 0xA4, 0x58, 0x00},	//B			42
		{0x00, 0x78, 0x84, 0x84, 0x48, 0x00},	//C			43
		{0x00, 0xFC, 0x84, 0x48, 0x30, 0x00},	//D			44
		{0x00, 0xFC, 0xA4, 0xA4, 0x84, 0x00},	//E			45
		{0x00, 0xFC, 0x90, 0x90, 0x80, 0x00},	//F			46
		{0x00, 0x78, 0x84, 0x94, 0x58, 0x00},	//G			47
		{0x00, 0xFC, 0x20, 0x20, 0xFC, 0x00},	//H			48
		{0x00, 0x84, 0xFC, 0x84, 0x00, 0x00},	//I			49
		{0x00, 0x0C, 0x40, 0xFC, 0x00, 0x00},	//J			4A
		{0x00, 0xFC, 0x20, 0x50, 0x8C, 0x00},	//K			4B
		{0x00, 0xFC, 0x04, 0x04, 0x04, 0x00},	//L			4C
		{0xFC, 0x40, 0x20, 0x40, 0xFC, 0x00},	//M			4D
		{0xFC, 0xC0, 0x30, 0x0C, 0xFC, 0x00},	//N			4E
		{0x00, 0x78, 0x84, 0x84, 0x78, 0x00},	//O			4F
		{0x00, 0xFC, 0x90, 0xA0, 0x40, 0x00},	//P			50
		{0x00, 0x78, 0x84, 0x84, 0x7E, 0x00},	//Q			51
		{0x00, 0xFC, 0x90, 0xA8, 0x44, 0x00},	//R			52
		{0x00, 0x62, 0x92, 0x92, 0x8C, 0x00},	//S			53
		{0x80, 0x80, 0xFC, 0x80, 0x80, 0x00},	//T			54
		{0x00, 0xF8, 0x04, 0x04, 0xF8, 0x00},	//U			55
		{0xE0, 0x18, 0x04, 0x18, 0xE0, 0x00},	//V			56
		{0xF8, 0x04, 0x18, 0x18, 0x04, 0xF8},	//W			57
		{0x00, 0xCC, 0x30, 0x30, 0xCC, 0x00},	//X			58
		{0x00, 0xE0, 0x10, 0x1E, 0x10, 0xE0},	//Y			59
		{0x00, 0x8C, 0x94, 0xA4, 0xC4, 0x00},	//Z			5A
		{0x02, 0x05, 0x05, 0x02, 0x00, 0x00}	//silencio 1		5B
};

static const uint16 ASCII_BIG[][15] =
{
		{0x0000, 0x3FFC, 0x3FFC, 0xC180, 0xC180, 0xC180, 0xC180, 0xC180, 0xC180, 0xC180, 0xC180, 0xC180, 0x3FFC, 0x3FFC, 0x0000},
		{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}
};
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

	LCD_ILI9341_fillScreen(ILI9341_CYAN);
}

void LCD_ILI9341_fillScreen(uint16 color)
{
	uint32 index;

	SPI_startTranference(SPI_0);
	GPIO_clearPIN(GPIO_D, CS_PIN);

	LCD_ILI9341_gotoXY(CA_LIMIT, PA_LIMIT);

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

void LCD_ILI9341_writeColor(uint16 x, uint16 y, uint16 color)
{
	uint32 newX = (x << GOTO_XY_SHIFT16) | x;
	uint32 newY = (y << GOTO_XY_SHIFT16) | y;

	/**Configure the DATA_OR_CMD PIN*/

	SPI_startTranference(SPI_0);
	GPIO_clearPIN(GPIO_D, CS_PIN);

	LCD_ILI9341_gotoXY(newX, newY);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_RAMWR);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, color >> COLOR_SHIFT);
	SPI_sendOneByte(SPI_0, color & COLOR_MASK);

	GPIO_setPIN(GPIO_D, CS_PIN);
	SPI_stopTranference(SPI_0);
}

void LCD_ILI9341_writeLetter(uint16 x, uint16 y, uint16 color, uint8 letter)
{
	uint8 x_index= FALSE;
	uint8 x_index2= FALSE;
	uint8 y_index= FALSE;
	uint8 temporalValue = FALSE;
	uint8 shift_helper;

	for(x_index = x; x_index < x + X_CHAR_LIMIT; x_index++)
	{
		shift_helper = Y_CHAR_LIMIT;

		for(y_index = y; y_index < y + Y_CHAR_LIMIT; y_index++)
		{
			temporalValue = ((ASCII[letter - 0x20][x_index2]) >> shift_helper) & TRUE;
			if(temporalValue)
				LCD_ILI9341_writeColor(x_index, y_index , color);
			shift_helper--;
		}

		x_index2++;
	}
}

void LCD_ILI9341_writeBigLetter(uint16 x, uint16 y, uint16 color, uint8 letter)
{
	uint8 x_index= FALSE;
	uint8 x_index2= FALSE;
	uint8 y_index= FALSE;
	uint32 temporalValue = FALSE;
	uint32 temporalASCII = FALSE;
	uint8 shift_helper;

	for(x_index = x; x_index < x + X_BIGCHAR_LIMIT; x_index++)
	{
		shift_helper = Y_BIGCHAR_LIMIT;
		temporalASCII = (ASCII_BIG[letter][x_index2]);

		for(y_index = y; y_index < y + Y_BIGCHAR_LIMIT; y_index++)
		{
			temporalValue = (temporalASCII >> shift_helper);
			temporalValue =  temporalValue & TRUE;
			if(temporalValue)
				LCD_ILI9341_writeColor(x_index, y_index , color);
			shift_helper--;
		}

		x_index2++;
	}
}

void LCD_ILI9341_drawNLines(uint16 y, uint16 nLines, uint16 color)
{
	uint32 index;
	uint32 newY = (y << GOTO_XY_SHIFT16) | (y + nLines - 1);

	SPI_startTranference(SPI_0);
	GPIO_clearPIN(GPIO_D, CS_PIN);

	LCD_ILI9341_gotoXY(CA_LIMIT, newY);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_RAMWR);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);

	for(index = FALSE; index < (nLines * ILI_LCD_X); index++)
	{
		SPI_sendOneByte(SPI_0, color >> COLOR_SHIFT);
		SPI_sendOneByte(SPI_0, color & COLOR_MASK);
	}

	GPIO_setPIN(GPIO_D, CS_PIN);
	SPI_stopTranference(SPI_0);
}

void LCD_ILI9341_gotoXY(uint32 x, uint32 y)
{
	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_CASET);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, x >> GOTO_XY_SHIFT24);
	SPI_sendOneByte(SPI_0, (x >> GOTO_XY_SHIFT16) & COLOR_MASK);
	SPI_sendOneByte(SPI_0, (x >> COLOR_SHIFT) & COLOR_MASK);
	SPI_sendOneByte(SPI_0, x & COLOR_MASK);

	GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, CMD_PASET);
	GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);
	SPI_sendOneByte(SPI_0, y >> GOTO_XY_SHIFT24);
	SPI_sendOneByte(SPI_0, (y >> GOTO_XY_SHIFT16) & COLOR_MASK);
	SPI_sendOneByte(SPI_0, (y >> COLOR_SHIFT) & COLOR_MASK);
	SPI_sendOneByte(SPI_0, y & COLOR_MASK);
}

void LCD_ILI9341_drawPartiture(uint8 position)
{
	uint32 newY;

	if(TRUE == position)
	{
		newY = 135;
		LCD_ILI9341_drawNLines(newY, P_SIZE, ILI9341_CYAN);
		newY += P1_OFFSET;
	}

	else
	{
		newY = FALSE;
		LCD_ILI9341_drawNLines(newY, P_SIZE, ILI9341_CYAN);
		newY += P2_OFFSET;
	}

	LCD_ILI9341_drawNLines(newY, TRUE, ILI9341_BLACK);
	newY += TEXT_SIZE;
	LCD_ILI9341_drawNLines(newY, TRUE, ILI9341_BLACK);
	newY += TEXT_SIZE;
	LCD_ILI9341_drawNLines(newY, TRUE, ILI9341_BLACK);
	newY += TEXT_SIZE;
	LCD_ILI9341_drawNLines(newY, TRUE, ILI9341_BLACK);
	newY += TEXT_SIZE;
	LCD_ILI9341_drawNLines(newY, TRUE, ILI9341_BLACK);
}

void LCD_ILI9341_drawShape(uint16 x, uint16 y, uint16 x2, uint16 y2, uint16 color)
{
	uint32 index;
	uint16 y_diff = y2 - y;
	uint16 x_diff = x2 - x;
	uint32 newX = (x << GOTO_XY_SHIFT16) | (x + x_diff -1);
	uint32 newY = (y << GOTO_XY_SHIFT16) | y;
	uint16 nLines;

	y++;

	for(nLines = FALSE; nLines < y_diff; nLines++)
	{
		SPI_startTranference(SPI_0);
		GPIO_clearPIN(GPIO_D, CS_PIN);

		LCD_ILI9341_gotoXY(newX, newY);

		GPIO_clearPIN(GPIO_D, DATA_OR_CMD_PIN);
		SPI_sendOneByte(SPI_0, CMD_RAMWR);
		GPIO_setPIN(GPIO_D, DATA_OR_CMD_PIN);

		for(index = FALSE; index < x_diff; index++)
		{
			SPI_sendOneByte(SPI_0, color >> COLOR_SHIFT);
			SPI_sendOneByte(SPI_0, color & COLOR_MASK);
		}

		GPIO_setPIN(GPIO_D, CS_PIN);
		SPI_stopTranference(SPI_0);

		newY = (y << GOTO_XY_SHIFT16) | y++;
	}
}

void LCD_ILI9341_delay(void)
{
	uint32 counter;
	/**Does nothing over a certain amount of time*/
	for(counter=0; counter<18000; counter++)
	{

	}
}
