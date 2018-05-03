/*
 * LCD_ILI9341.h
 *
 *  Created on: Apr 21, 2018
 *      Author: aland
 */

#ifndef LCD_ILI9341_H_
#define LCD_ILI9341_H_

#include "DataTypeDefinitions.h"
#include "SPI.h"

/**Screen size*/
#define ILI_LCD_X 320
#define ILI_LCD_Y 240
#define SCREEN_SIZE 76800
#define X_CHAR_LIMIT 6
#define Y_CHAR_LIMIT 8
#define X_BIGCHAR_LIMIT 15
#define Y_BIGCHAR_LIMIT 16

/**SPI configuration*/
#define LCD_DATA 1
#define LCD_CMD 0
#define DATA_OR_CMD_PIN 3
#define CS_PIN 0

/**Commands*/
#define ADAFRUIT_CMD 0xEF
#define ADA_1 0x03
#define ADA_2 0x80
#define ADA_3 0x02
#define VCOM_CMD 0xCF
#define VCOM_1 0
#define VCOM_2 0xC1
#define VCOM_3 0x30
#define ADAFRUIT_CMD2 0xED
#define ADA2_1 0x64
#define ADA2_2 3
#define ADA2_3 0x12
#define ADA2_4 0x81
#define ADAFRUIT_CMD3 0xE8
#define ADA3_1 0x85
#define ADA3_2 0x0
#define ADA3_3 0x78
#define ADAFRUIT_CMD4 0xCB
#define ADA4_1 0x39
#define ADA4_2 0x2C
#define ADA4_3 0x0
#define ADA4_4 0x34
#define ADA4_5 0x02
#define ADAFRUIT_CMD5 0xF7
#define ADA5_1 0x20
#define ADAFRUIT_CMD6 0xEA
#define ADA6 0

#define CMD_MADCTL 0x36
#define CMD_VSCRSADD 0x37
#define CMD_PWCTR1 0xC0
#define CMD_PWCTR2 0xC1
#define CMD_VMCTR1 0xC5
#define CMD_VMCTR2 0xC7
#define CMD_PIXFMT 0x3A
#define CMD_FRMCTR1 0xB1
#define CMD_DFUNCTR 0xB6
#define CMD_3DGAMMA 0xF2
#define CMD_GAMMASET 0x26
#define CMD_GMCTRP1 0xE0
#define CMD_GMCTRN1 0xE1
#define CMD_SLPOUT 0x11
#define CMD_DISPON 0x29

#define DATA_PWCTR1 0x23
#define DATA_PWCTR2 0x10
#define DATA_VMCTR1 0x3E
#define DATA2_VMCTR1 0x28
#define DATA_VMCTR2 0x86
#define DATA_MADCTL 0x48
#define DATA_VSCRSADD 0
#define DATA_PIXFMT 0x55
#define DATA_FRMCTR1 0
#define DATA2_FRMCTR1 0x18
#define DATA_DFUNCTR 0x08
#define DATA2_DFUNCTR 0x82
#define DATA3_DFUNCTR 0x27
#define DATA_3DGAMMA 0
#define DATA_GAMMASET 1
#define DATA0_GMCTRP1 0xF
#define DATA_GMCTRP1 0x31
#define DATA2_GMCTRP1 0x2B
#define DATA3_GMCTRP1 0x0C
#define DATA4_GMCTRP1 0x0E
#define DATA5_GMCTRP1 0x08
#define DATA6_GMCTRP1 0x4E
#define DATA7_GMCTRP1 0xF1
#define DATA8_GMCTRP1 0x37
#define DATA9_GMCTRP1 0x07
#define DATA10_GMCTRP1 0x10
#define DATA11_GMCTRP1 0x03
#define DATA12_GMCTRP1 0x0E
#define DATA13_GMCTRP1 0x09
#define DATA14_GMCTRP1 0
#define DATA0_GMCTRN1 0
#define DATA1_GMCTRN1 0x0E
#define DATA2_GMCTRN1 0x14
#define DATA3_GMCTRN1 0x03
#define DATA4_GMCTRN1 0x11
#define DATA5_GMCTRN1 0x07
#define DATA6_GMCTRN1 0x31
#define DATA7_GMCTRN1 0xC1
#define DATA8_GMCTRN1 0x48
#define DATA9_GMCTRN1 0x08
#define DATA10_GMCTRN1 0x0F
#define DATA11_GMCTRN1 0x0C
#define DATA12_GMCTRN1 0x31
#define DATA13_GMCTRN1 0x36
#define DATA14_GMCTRN1 0x0F

/**Write to RAM*/
#define CMD_CASET      0x2A
#define CMD_PASET      0x2B
#define CMD_RAMWR      0x2C

/**Fill screen*/
#define CA_LIMIT 0xEF
#define PA_LIMIT 0x13F

/**Colors*/
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

/**Data for graphics*/
#define COLOR_SHIFT 8
#define COLOR_MASK 0xFF
#define GOTO_XY_SHIFT16 16
#define GOTO_XY_SHIFT24 24
#define TEXT_SIZE 15
#define HALF_TEXT_SIZE 8
#define P_SIZE 150
#define PGEN_OFFSET 135
#define P1_OFFSET 52
#define P2_OFFSET 53



/** \brief It configures the LCD*/
void LCD_ILI9341_init(void);

/** \brief Writes a given color in every pixel*/
void LCD_ILI9341_fillScreen(uint16 color);

void LCD_ILI9341_drawNLines(uint16, uint16, uint16);

void LCD_ILI9341_gotoXY(uint32, uint32);

void LCD_ILI9341_drawShape(uint16, uint16, uint16, uint16, uint16);

/** \brief It writes a byte in the LCD memory. The place of writting is the last place that was indicated by LCDNokia_gotoXY. In the reset state
 * the initial place is x=0 y=0*/
void LCD_ILI9341_writeColor(uint16, uint16, uint16);

void LCD_ILI9341_writeBigLetter(uint16 x, uint16 y, uint16 color, uint8 letter);

void LCD_ILI9341_writeLetter(uint16 x, uint16 y, uint16 color, uint8 letter);
/** \brief generates a delay with a blocking for function
 *
 * It is used in the initialization routine*/
void LCD_ILI9341_delay(void);

void LCD_ILI9341_drawPartiture(uint8);

#endif /* LCD_ILI9341_H_ */
