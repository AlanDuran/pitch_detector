/**
 * menu.h
 *
 *  Created on: Mar 18, 2018
 *      Author: aland
 */

#ifndef MENU_H_
#define MENU_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "SPI.h"
#include "GPIO.h"
#include "LCD_ILI9341.h"
#include "ADC.h"
#include "NVIC.h"
#include "PIT.h"
#include "FLEX.h"

/**PIN button values*/
#define PIN_B0 5
#define PIN_B1 7
#define PIN_B2 0
#define PIN_B3 9
/**MASK for button pins*/
#define MASK 0x2A1 //0010 1010 0001

/**Menu constants*/
#define OFFSET 48
#define SIZE 15
#define INITIAL_BPM 60
#define MIN 60.0f
#define DEC 10
/**PIT values*/
#define PIT_CLOCK 10500000
#define DELAY 0.5
/**LCD XY*/
#define X_0 0
#define X_1 15
#define X_2 30
#define X_3 45
#define X_4 60
#define X_5 75
#define X_6 90
#define X_7 105
#define X_8 120
#define X_9 135
#define X_10 150
#define X_11 165
#define X_12 180
#define X_13 195
#define X_14 210
#define X_15 225
#define Y_0 0
#define Y_1 0x10
#define Y_2 0x20
#define Y_3 0x30
#define Y_4 0x40
#define Y_5 0x50
#define Y_6 0x60
#define Y_7 0x70
#define Y_8 0x80
#define Y_9 0x90
#define Y_10 0xA0
#define Y_11 0xB0
#define Y_12 0xC0
#define Y_13 0xD0
#define Y_14 0xE0
#define Y_15 0xF0
#define Y_16 0x100
#define Y_17 0x110
#define Y_18 0x120
#define Y_19 0x130

/**Type for state machine menus*/
typedef enum {MENU,B0,B1,B1a,B2,B3} menus;

/**\brief Initializes necessary GPIO and interrupt configurations for menu*/
void initMenu();
/**\brief Handles the possible options of IDLE state for a given button entry*/
void menu();
/**\brief Handles first menu available options*/
void b0();
/**\brief Handles alarm configurations*/
void b1();
/**\brief Handles temperature display configurations*/
void b1a();
/**\brief Handles decrement configurations*/
void b2();
/**\brief Handles increment configurations*/
void b3();
/**\brief Handles manual motor control configurations*/

/**\brief Prints default screen with currTemp and currMotorSpeed*/
void printMenu();

/**\brief Prints menu screen*/
void printB0();

/**\brief Prints alarm configuration screen
 *
 * \param[in] number value to be shown in screen.
 * */
void printB1();

/**\brief Prints decrement configuration screen
 *
 * \param[in] number value to be shown in screen.
 * */
void printB2();

/**\brief Prints motor configuration screen
 *
 * \param[in] number value to be shown in screen.
 * */
void printB3();

/**\brief Converts an uint8 type to be printed in screen and stores it
 * 			in static variable string
 *
 *\param[in] number value to be converted.
 */
void uint8_to_string(uint8 number);

void uint32_to_string(uint32 number);

/**\brief executes current machine state with a pointer to function
 */
void updateMenu();

/**\brief updates frequency value in screen
 *
 * \param[in] currentFreq value to be printed.
 */
void updateB0(uint8 currentFreq);

void updateB1a(uint8 currentFreq);

/**\brief returns currentState
 *
 * \return currentState  value*/
uint8 getMenuState();

void startInterrupts();

void stopInterrupts();

#endif /* MENU_H_ */
