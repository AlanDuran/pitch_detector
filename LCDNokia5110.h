/**
 * \file LCDNokia5110.h
 *
 * \date Jun 11, 2014
 * \author Luis
 */

#ifndef LCDNOKIA5110_H_
#define LCDNOKIA5110_H_

#include "DataTypeDefinitions.h"
#define SCREENW 84
#define SCREENH 48

#define LCD_X 84
#define LCD_Y 48
/**SPI configuration*/
#define LCD_DATA 1
#define LCD_CMD 0
#define DATA_OR_CMD_PIN 3
#define RESET_PIN 0

/** \brief It configures the LCD*/
void LCDNokia_init(void);
/** \brief It writes a byte in the LCD memory. The place of writting is the last place that was indicated by LCDNokia_gotoXY. In the reset state
 * the initial place is x=0 y=0*/
void LCDNokia_writeByte(uint8, uint8);
/** \brief it clears all the figures in the LCD*/
void LCDNokia_clear(void);
/** \brief It is used to indicate the place for writing a new character in the LCD. The values that x can take are 0 to 84 and y can take values
 * from 0 to 5*/
void LCDNokia_gotoXY(uint8 x, uint8 y);
/** \brief It allows to write a figure represented by constant array
 *
 *	\param[in] uint8* Pointer to array with the bitmap to print.
 * */
void LCDNokia_bitmap(const uint8*);
/** \brief It writes a character in the LCD
 *
 * \param[in] uint8 ASCII code of the character to print
 * */
void LCDNokia_sendChar(uint8);
/** \brief It writes a string into the LCD
 *
 * \param[in] uint8* Pointer to array with the string to print.
 * */
void LCDNokia_sendString(uint8*);
/** \brief generates a delay with a blocking for function
 *
 * It is used in the initialization routine*/
void LCD_delay(void);



#endif /* LCDNOKIA5110_H_ */
