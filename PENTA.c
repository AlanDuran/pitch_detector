/*
 * PENTA.c
 *
 *  Created on: May 3, 2018
 *      Author: kanta
 */

#include "PENTA.h"
#include "DSP.h"
#include "PIT.h"
#include "NVIC.h"
#include "menu.h"
#include "LCD_ILI9341.h"

#define SYSTEM_CLOCK 60000000
#define LOW_TEST_TEMPO 80.00f
#define NO_NOTE_THRESH 0.15f /** To be calibrated */
#define MAX_ENTRIES 4
#define MAX_SAVED_NOTES 240

/**
 * 	Some notes on printing on the screen:
 *
 * 	First penta starts on y 40, in between lines there is a diff of 15
 */

float32 tempo = 80.00f;
uint16 timeCounter = 65535;
uint8 topOrBottom;
uint8 checkTimeCounter;
float32 avgData;
uint8 tempoCounter;
static uint8 checkingTime_flag;

/** Variables to manipulate printing of saved partitures */
static uint8 maxSaves = 2;
static uint8 currPage;

static uint8 saving_position;/**< Counter that indicates the position of the note being saved*/

uint8 clearPenta_flag;

/**
 * 	Will have a buffer of two in case we need the extra space.
 */

static PENTA_savedNotes_type savedNotes[MAX_SAVED_NOTES];

/*
 * 	New functions:
 * 	Save note,
 * 	meassure time (make attack return the starting time to compare current to that)
 */

void PENTA_saveYPos(uint8 y_pos)
{
	savedNotes[saving_position].y_pos =  y_pos;
}

void PENTA_saveXPos(uint8 x_pos)
{
	savedNotes[saving_position].x_pos =  x_pos;
}

void PENTA_saveSharp(uint8 sharp)
{
	savedNotes[saving_position].sharp =  sharp;
}

void PENTA_graph()
{
	if(FALSE != savedNotes[saving_position].y_pos)
	{
		if(TRUE == PENTA_getClearPenta())
		{
			DisableInterrupts;
			PENTA_clearPage();
			PENTA_clearClearPenta();
			printTempo();
			EnableInterrupts;
		}

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


		saving_position++;
		if(B1a == getMenuState() && saving_position >= maxSaves*16)
		{
			stopInterrupts();
		}
	}
}

void PENTA_stall()
{

}

void PENTA_startTimeMeassure()
{
	float32 period = 60/LOW_TEST_TEMPO/4;
	PIT_delay(PIT_1, SYSTEM_CLOCK, period); /**< The period is taken from the tempo value for the pentagram*/
	//checkingTime_flag = TRUE;
}

void PENTA_graphTempo()
{
	switch(timeCounter%16)
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
	PIT_stop(PIT_1);
	timeCounter = FALSE;
}

void PENTA_timeCount()
{
	timeCounter++;
}

void PENTA_checkTime(uint16 data)
{
	/** Stores data only if they are positive numbers */
	float32 newData = DSP_digToFloat(data);
	if(newData > 0)
	{
		avgData += newData;
		checkTimeCounter++;
	}
	/** If the samples reach the desired ammount, averages and checks */
	if(checkTimeCounter == AVG_MAX_SAMPLES)
	{
		/** Averages and checks if the sound enters no note threshold */
		avgData /= checkTimeCounter;
		if(NO_NOTE_THRESH > avgData)
		{
			/** If so, stops measuring time and graphs*/
			//PENTA_stopTimeMeassure();
			avgData = FALSE;
			checkTimeCounter = FALSE;
			checkingTime_flag = FALSE;
		}
		else
		{
			/** Keep averaging */
			avgData = FALSE;
			checkTimeCounter = FALSE;
		}
	}
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
			if(tempoCounter == 16)
			{
				if(TRUE == PENTA_getTopOrBottom())
				{
					clearPenta_flag = TRUE;
				}
				tempoCounter = FALSE;
				topOrBottom++;
			}
			tempoCounter++;
			if(TRUE == PENTA_getTopOrBottom())
			{
				PENTA_saveYPos(Notes[index].id + BOTTOM_OFF);
			}
			else
			{
				PENTA_saveYPos(Notes[index].id);
			}

			PENTA_saveXPos(PENTA_getTempoCounterPosition());
			PENTA_saveSharp(Notes[index].sharp);
		}
	}
}

uint8 PENTA_getCheckingTimeFlag()
{
	return checkingTime_flag;
}

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

uint8 PENTA_convertTimeToBeats(uint8 duration)
{
	uint8 result;

	switch(duration)
	{
	case 0:
	case 1:
	case 2:
		result = 2;
		break;
	case 3:
	case 4:
		result = 4;
		break;
	case 5:
	case 6:
		result = 6;
		break;
	case 7:
	case 8:
		result = 8;
		break;
	case 9:
	case 10:
		result = 10;
		break;
	case 11:
	case 12:
		result = 12;
		break;
	case 13:
	case 14:
		result = 14;
		break;
	case 15:
	case 16:
		result = 16;
		break;
	}

	return result;
}

void PENTA_increaseTempo()
{
	tempo = (tempo + 5 > 120)? 120 : tempo + 5;
	PENTA_restartPit();
}

void PENTA_decreaseTempo()
{
	tempo = (tempo - 5 < 60)? 60 : tempo - 5;
	PENTA_restartPit();
}

void PENTA_restartPit()
{
	float32 period = 60/tempo/4;
	PIT_delay(PIT_1, SYSTEM_CLOCK, period);
}

void PENTA_clearSaves()
{
	for(uint8 index = 0; index < MAX_SAVED_NOTES; index++)
	{
		savedNotes[index].x_pos = FALSE;
		savedNotes[index].y_pos = FALSE;
		savedNotes[index].sharp = FALSE;
	}
	/** resart top and bottom and counter*/
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
	LCD_ILI9341_fillScreen(ILI9341_CYAN);
	LCD_ILI9341_drawPartiture(FALSE);
	LCD_ILI9341_drawPartiture(TRUE);
}

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
