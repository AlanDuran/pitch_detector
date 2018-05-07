/**
 * menu.c
 *
 *  Created on: Mar 18, 2018
 *      Authors: Alan Duran y Dario Hoyo
 */

#include "menu.h"
#include "PENTA.h"
#include "FLEX.h"
#include "ADC.h"

static uint8 currentState = MENU;
static uint8 currentBPM = INITIAL_BPM;
static float32 currentPIT = INITIAL_BPM / MIN;
static uint8 savedCount = FALSE;
static uint8 saveLimit = 2;
static uint8 string[SIZE];
static uint8 currentX;
static uint8 currentCompass;

/**Pointer to function containing all the possible states*/
void (*fp[B3])() = {menu, b0, b1, b1a, b2};

/**Executes current state*/
void updateMenu()
{
	(*fp[currentState])();
	/**Waits for button to be released*/
	while(FALSE != ((GPIO_readPORT(GPIO_C) & MASK) ^ MASK)){}
}

void initMenu()
{
	/**Peripheral System Clock Gating*/
	GPIO_clockGating(GPIO_C); /**< We use this port to read the buttons.*/
	GPIO_clockGating(GPIO_B);

	/** Sets up the masks to use in the rows and columns respectively.*/
	GPIO_pinControlRegisterType interruptPullUpIO = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;

	///Buttons
	GPIO_pinControlRegister(GPIO_C,PIN_B0,&interruptPullUpIO);
	GPIO_pinControlRegister(GPIO_C,PIN_B1,&interruptPullUpIO);
	GPIO_pinControlRegister(GPIO_C,PIN_B2,&interruptPullUpIO);
	GPIO_pinControlRegister(GPIO_C,PIN_B3,&interruptPullUpIO);
	NVIC_enableInterruptAndPriotity(PORTC_IRQ,PRIORITY_2);

	printMenu();
}

void menu()
{	/** B0: FREE MODE */
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B0))
	{
		PENTA_clearSaves();
		currentState = B0;
		printB0();
		/**
		 * 	INTERRUPTS:
		 * 	Start pit,
		 * 	start adc,
		 * 	start flex timer
		 */
		startInterrupts();
		/** Print tempo */
		printTempo();
	}
	/** B1: SAVE*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B1))
	{
		currentState = B1;
		printB1();
	}
	/** B2: SEE SAVED */
	else if(FALSE == GPIO_readPIN(GPIO_C, PIN_B2))
	{
		PENTA_clearPage();
		PENTA_printPage();
		clearPage();
		printPage();
		currentState = B2;
	}
}

void b0()
{
	/** B0: UP TEMPO*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B0))
	{ 	/**Printing block screen*/
		PENTA_decreaseTempo();
		clearTempo();
		printTempo();
	}
	/** B1: LOWER TEMPO */
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B1))
	{
		PENTA_increaseTempo();
		clearTempo();
		printTempo();
	}

	/** B2: EXIT TO MENU*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B2))
	{
		stopInterrupts();
		PENTA_clearSaves();
		currentState = MENU;
		printMenu();
		/** Clear saves xd*/
	}
}

void b1()
{
	/**If B0: start saving*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B0))
	{
		printB0();
		currentState = B1a;
		PENTA_clearSaves();
		startInterrupts();
		printTempo();
	}
	/**If B1: INCREASE COMPASES*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B1))
	{
		/** print a shape 45 15 to erase ILI9341_DARKCYAN*/
		LCD_ILI9341_drawShape(X_8, Y_1, X_8 + 45, Y_1 + 15, ILI9341_DARKCYAN);
		saveLimit = (saveLimit + 2 > 10)? 10: saveLimit + 2;
		uint8_to_string(saveLimit);
		LCD_ILI9341_sendString(X_8, Y_1, ILI9341_BLACK, string);
		PENTA_setMaxSaves(saveLimit);
	}
	/**If B1: DECREASE COMPASES*/
	else if(FALSE == GPIO_readPIN(GPIO_C, PIN_B2))
	{
		/** */
		LCD_ILI9341_drawShape(X_8, Y_1, X_8 + 45, Y_1 + 15, ILI9341_DARKCYAN);
		saveLimit = (saveLimit - 2 < 2)? 2 : saveLimit - 2;
		uint8_to_string(saveLimit);
		LCD_ILI9341_sendString(X_8, Y_1, ILI9341_BLACK, string);
		PENTA_setMaxSaves(saveLimit);
	}
}

void b1a()
{
	/**If B0: DECREASE TEMPO*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B0))
	{ 	/**Printing block screen*/
		PENTA_decreaseTempo();
		clearTempo();
		printTempo();
	}
	/**If B1: INCREASE TEMPO*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B1))
	{
		PENTA_increaseTempo();
		clearTempo();
		printTempo();
	}

	/**If B2: EXIT*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B2))
	{
		stopInterrupts();
		currentState = MENU;
		printMenu();
	}
}

void b2()
{
	/**If B0: PREV PAGE*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B0))
	{
		/** If currPage is first page, do nothing*/
		PENTA_prevPage();
		clearPage();
		printPage();
	}
	/**If B1: NEXT PAGE*/
	if(FALSE == GPIO_readPIN(GPIO_C, PIN_B1))
	{
		/** If currPage is last page, do nothing*/
		PENTA_nextPage();
		clearPage();
		printPage();
	}
	/**If B2: EXIT*/
	else if(FALSE == GPIO_readPIN(GPIO_C, PIN_B2))
	{
		printMenu();
		currentState = MENU;
	}
}


void printMenu()
{
	uint8 s0[] = "Selecciona:";
	uint8 s1[] = "0) Comenzar";
	uint8 s2[] = "1) Guardar";
	uint8 s3[] = "2) Ver partituras";
	LCD_ILI9341_fillScreen(ILI9341_CYAN);
	LCD_ILI9341_sendString(X_0, Y_1, ILI9341_BLACK, s0);
	LCD_ILI9341_sendString(X_0, Y_3, ILI9341_BLACK, s1);
	LCD_ILI9341_sendString(X_0, Y_4, ILI9341_BLACK, s2);
	LCD_ILI9341_sendString(X_0, Y_5, ILI9341_BLACK, s3);
}

/**This function just prints the corresponding strings*/
void printB0()
{
	LCD_ILI9341_fillScreen(ILI9341_CYAN);
	LCD_ILI9341_drawPartiture(FALSE);
	LCD_ILI9341_drawPartiture(TRUE);
}

void printB1()
{
	uint8 s1[] = "Guardar";
	uint8_to_string(saveLimit);
	uint8 s2[] = "compases";
	uint8 s3[] = "0) Comenzar";
	uint8 s4[] = "1) Aumentar";
	uint8 s5[] = "2) Disminuir";

	LCD_ILI9341_fillScreen(ILI9341_DARKCYAN);
	LCD_ILI9341_sendString(X_0, Y_1, ILI9341_BLACK, s1);
	LCD_ILI9341_sendString(X_8, Y_1, ILI9341_BLACK, string);
	LCD_ILI9341_sendString(X_0, Y_2, ILI9341_BLACK, s2);
	LCD_ILI9341_sendString(X_0, Y_4, ILI9341_BLACK, s3);
	LCD_ILI9341_sendString(X_0, Y_6, ILI9341_BLACK, s4);
	LCD_ILI9341_sendString(X_0, Y_8, ILI9341_BLACK, s5);
}

void printB2()
{

}

void updateB0(uint8 currentFreq)
{
}

void updateB1a(uint8 currentFreq)
{
}

void uint8_to_string(uint8 number)
{
	uint8 n = 1;
	uint8 numberTest = number;

	numberTest /= DEC;
	/**Increment size of string until number/10 = 0*/
	while(numberTest > 0)
	{
		numberTest /= DEC;
		n++;
	}
	/**String terminator*/
	string[n] = 0;
	n--;

	if( 0 == number)
	{
		string[n] = OFFSET;
	}
	/**Write number % 10 in string[n]*/
	while(number > 0)
	{
		string[n] = number % DEC + OFFSET;
		number = number / DEC;
		n--;
	}
}

void uint32_to_string(uint32 number)
{
	uint8 n = 1;
	uint32 numberTest = number;

	numberTest /= DEC;
	/**Increment size of string until number/10 = 0*/
	while(numberTest > 0)
	{
		numberTest /= DEC;
		n++;
	}
	/**String terminator*/
	string[n] = 0;
	n--;
	/**Write number % 10 in string[n]*/
	while(number > 0)
	{
		string[n] = number % DEC + OFFSET;
		number = number / DEC;
		n--;
	}
}

uint8 getMenuState()
{	/**Return currentState flag*/
	return currentState;
}

void stopInterrupts()
{
	FTM3_turnOff();
	ADC0_stop();
	PENTA_stopTimeMeassure();
}

void startInterrupts()
{
	FTM3_turnOn();
	PENTA_startTimeMeassure();
}

void printTempo()
{
	uint8 s0[] = "Tempo:";
	uint8 s1[] = "BPM";
	uint8_to_string(PENTA_getTempo());
	LCD_ILI9341_sendString(X_1, Y_19, ILI9341_BLACK, s0);
	LCD_ILI9341_sendString(X_7, Y_19, ILI9341_BLACK, string);
	LCD_ILI9341_sendString(X_10, Y_19, ILI9341_BLACK, s1);
}

void clearTempo()
{
	LCD_ILI9341_drawShape(X_1, Y_19, 240, 320, ILI9341_CYAN);
}

void printPage()
{
	uint8 s0[] = "Pag";
	uint8_to_string(PENTA_getCurrPage() + 1);
	LCD_ILI9341_sendString(X_3, Y_19, ILI9341_BLACK, s0);
	LCD_ILI9341_sendString(X_7, Y_19, ILI9341_BLACK, string);
}

void clearPage()
{
	LCD_ILI9341_drawShape(X_1, Y_19, 240, 320, ILI9341_CYAN);
}
