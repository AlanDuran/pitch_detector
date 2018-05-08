/**
 * \file
 * 		PENTA.c
 *
 * \brief
 * 		This is a driver for the internal ADC of a K64F board
 *
 * \authors
 *  	Dario Hoyo
 *     	Alan Duran
 *
 * \date
 * 		05/07/2018
 *
 *      */

#include "PENTA.h"
#include "DSP.h"
#include "PIT.h"
#include "NVIC.h"
#include "menu.h"
#include "LCD_ILI9341.h"

/**
 * 	Some notes on printing on the screen:
 *
 * 	First penta starts on y 40, in between lines there is a diff of 15
 */

float32 tempo = LOW_TEST_TEMPO; /**< Dynamically changed tempo */
uint16 timeCounter = RES_16_BIT;	/**< Counter for tempo, starts on 2pow16-1 so first value is 0 */
uint8 topOrBottom;		/**< Counter to alternate in top or bottom pentagram */
float32 avgData;		/**< Variable that stores averaged Data used to create new threshold */
uint8 tempoCounter;		/**< Counter that indicates the position of the note in the x axis of the pentagram */

/** Variables to manipulate printing of saved partitures */
static uint8 maxSaves = START_MAX_SAVES;
static uint8 currPage;

static uint8 saving_position;/**< Counter that indicates the position of the note being saved*/

uint8 clearPenta_flag; /**< Flag that indicates the pentagram needs to refresh */

/** Buffer where information on the notes and how to print them is stored. */
static PENTA_savedNotes_type savedNotes[MAX_SAVED_NOTES];

/** Save y position of note to be printed in pentagram */
void PENTA_saveYPos(uint8 y_pos)
{
	savedNotes[saving_position].y_pos =  y_pos;
}

/** Save x position of note to be printed in pentagram*/
void PENTA_saveXPos(uint8 x_pos)
{
	savedNotes[saving_position].x_pos =  x_pos;
}

/** Saves whether it is sharp or not */
void PENTA_saveSharp(uint8 sharp)
{
	savedNotes[saving_position].sharp =  sharp;
}

void PENTA_graph()
{
	/** If the value was not garbage (background noise, etc) save information*/
	if(FALSE != savedNotes[saving_position].y_pos)
	{
		/** If flag is set, refresh screen*/
		if(TRUE == PENTA_getClearPenta())
		{
			DisableInterrupts;
			PENTA_clearPage();
			PENTA_clearClearPenta();
			printTempo();
			EnableInterrupts;
		}

		/** If note to be graphed is sharp, print sharp symbol */
		if(TRUE == savedNotes[saving_position].sharp)
		{
			LCD_ILI9341_writeBigLetter(savedNotes[saving_position].x_pos,
									   savedNotes[saving_position].y_pos, 0, WHITE_SHARP);
		}
		else
		{
			LCD_ILI9341_writeBigLetter(savedNotes[saving_position].x_pos,
									   savedNotes[saving_position].y_pos, 0, WHITE);
		}

		/** Increase the position where info will be saved */
		saving_position++;

		/** If the menu is in the state of saving, stop after saving the desired ammount. */
		if(B1a == getMenuState() && saving_position >= maxSaves*16)
		{
			stopInterrupts();
		}
	}
}


void PENTA_startTimeMeassure()
{
	/** Starts timer to measure tempo
	 * 	Some notes:
	 *
	 * 	- To calculate the tempo, you divide 60 (seconds in a minute since tempo is beats per min)
	 * 	- We divide an additional four to use this as a measure checking the attack, but the tempo meter
	 * 	remains unchanged since we display it each four beats.
	 */
	float32 period = SEC_MIN/tempo/TEMPO_ATTACK_DIV;
	PIT_delay(PIT_1, SYSTEM_CLOCK, period); /**< The period is taken from the tempo value for the pentagram*/
}

void PENTA_graphTempo()
{
	/** Graph tempo each 4 beats */
	switch(timeCounter%TEMPO_DIV)
	{
	case 0:
		LCD_ILI9341_drawShape(30, 290, 40, 300, ILI9341_BLACK);
		LCD_ILI9341_drawShape(86, 290, 96, 300, ILI9341_CYAN);
		LCD_ILI9341_drawShape(142, 290, 152, 300, ILI9341_CYAN);
		LCD_ILI9341_drawShape(198, 290, 208, 300, ILI9341_CYAN);
		break;
	case 4:
		LCD_ILI9341_drawShape(86, 290, 96, 300, ILI9341_BLACK);
		break;
	case 8:
		LCD_ILI9341_drawShape(142, 290, 152, 300, ILI9341_BLACK);
		break;
	case 12:
		LCD_ILI9341_drawShape(198, 290, 208, 300, ILI9341_BLACK);
		break;
	}
}

void PENTA_stopTimeMeassure()
{
	/** Stop PIT and reset counter */
	PIT_stop(PIT_1);
	timeCounter = FALSE;
}

void PENTA_timeCount()
{
	/** Count a fourth of a beat */
	timeCounter++;
}

void PENTA_findNote(float32 freq)
{
	/** Values for the subtraction of the actual f0 with the ideal f0
	 *  in case it is greater or lesser
	 */
	float32 diff_greater;
	float32 diff_lesser;
	/**	Goes through all the possible notes */
	for(uint8 index = 0; index < MAX_NOTES; index++)
	{
		/** Makes the subtractions */
		diff_greater = freq - Notes[index].f0;
		diff_lesser = Notes[index].f0 - freq;

		/** If any of the subtractions result in a value in between 0 and
		 * the difference, it means that is the approximated value.
		 */
		if (((diff_greater > 0) && (diff_greater < Notes[index].diff)) ||
				((diff_lesser > 0) && (diff_lesser < Notes[index].diff)))
		{
			/** If it counted to 16, switch to top or bottom pentagram */
			if(tempoCounter == 16)
			{
				/** If it was on bottom, screen should be refreshed */
				if(TRUE == PENTA_getTopOrBottom())
				{
					clearPenta_flag = TRUE;
				}
				/** Reset counter, increment top or bottom counter.*/
				tempoCounter = FALSE;
				topOrBottom++;
			}

			/** Increase counter*/
			tempoCounter++;

			/** Depending on whether the note was in the top or bot pentagram, adds an offset to y position */
			if(TRUE == PENTA_getTopOrBottom())
			{
				PENTA_saveYPos(Notes[index].id + BOTTOM_OFF);
			}
			else
			{
				PENTA_saveYPos(Notes[index].id);
			}

			/** Saves x position depending on counter*/
			PENTA_saveXPos(PENTA_getTempoCounterPosition());
			PENTA_saveSharp(Notes[index].sharp);
		}
	}
}


void PENTA_increaseTempo()
{
	/** Validates if it is in range */
	tempo = (tempo + 5 > 120)? 120 : tempo + 5;
	PENTA_restartPit();
}

void PENTA_decreaseTempo()
{
	/** Validates range */
	tempo = (tempo - 5 < 60)? 60 : tempo - 5;
	PENTA_restartPit();
}

void PENTA_restartPit()
{
	/** Calculates period again and sends to pit */
	float32 period = 60/tempo/4;
	PIT_delay(PIT_1, SYSTEM_CLOCK, period);
}

void PENTA_clearSaves()
{
	/** Clears the save data with a loop */
	for(uint8 index = 0; index < MAX_SAVED_NOTES; index++)
	{
		savedNotes[index].x_pos = FALSE;
		savedNotes[index].y_pos = FALSE;
		savedNotes[index].sharp = FALSE;
	}
	/** resart variables for everything to work again from scratch */
	tempoCounter = FALSE;
	topOrBottom = FALSE;
	saving_position = FALSE;
	currPage = FALSE;
}

void PENTA_nextPage()
{
	/** print next page */
	if(maxSaves/2 - 1 > currPage)
	{
		currPage++;
		PENTA_clearPage();
		PENTA_printPage();
	}
}

void PENTA_prevPage()
{
	/** print prev page */
	if(FALSE != currPage)
	{
		currPage--;
		PENTA_clearPage();
		PENTA_printPage();
	}
}

void PENTA_printPage()
{
	/** Each page has 32 notes, so we start using this knowledge and the current page */
	uint8 index = currPage*32;
	for(; index < currPage*32 + 32; index++)
	{
		if(TRUE == savedNotes[index].sharp)
		{
			LCD_ILI9341_writeBigLetter(savedNotes[index].x_pos,
									   savedNotes[index].y_pos, 0, WHITE_SHARP);
		}
		else
		{
			LCD_ILI9341_writeBigLetter(savedNotes[index].x_pos,
												   savedNotes[index].y_pos, 0, WHITE);
		}
	}
}

void PENTA_clearPage()
{
	/** Refreshes page with partiture */
	LCD_ILI9341_fillScreen(ILI9341_CYAN);
	LCD_ILI9341_drawPartiture(FALSE);
	LCD_ILI9341_drawPartiture(TRUE);
}

/** GETTERS AND SETTERS */

void PENTA_setMaxSaves(uint8 new_max)
{
	maxSaves = new_max;
}

uint8 PENTA_getTempo()
{
	return tempo;
}

uint8 PENTA_getCurrPage()
{
	return currPage;
}

/** The length of our notes is 14, so calling this function, returns the position to be sent to the screen */
uint8 PENTA_getTempoCounterPosition()
{
	return (tempoCounter - 1)*14;
}

uint8 PENTA_getTimeCounter()
{
	return timeCounter;
}

uint8 PENTA_getTopOrBottom()
{
	/** Uses a modulo to avoid validation */
	return topOrBottom%2;
}

uint8 PENTA_getClearPenta()
{
	return clearPenta_flag;
}

void PENTA_clearClearPenta()
{
	clearPenta_flag = FALSE;
}

uint8 PENTA_getSavingPosition()
{
	return saving_position;
}
