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
#include "LCD_ILI9341.h"

#define SYSTEM_CLOCK 60000000
#define LOW_TEST_TEMPO 60.00f
#define NO_NOTE_THRESH 0.15f /** To be calibrated */
#define MAX_ENTRIES 4
#define MAX_SAVED_NOTES 240

/**
 * 	Some notes on printing on the screen:
 *
 * 	First penta starts on y 40, in between lines there is a diff of 15
 */

uint8 timeCounter = 255;
uint8 topOrBottom;
uint8 checkTimeCounter;
float32 avgData;
uint8 tempoCounter;
static uint8 checkingTime_flag;

static uint8 saving_position;/**< Counter that indicates the position of the note being saved*/

uint8 clearPenta_flag;

/**
 * 	Will have a buffer of two in case we need the extra space.
 */

static PENTA_savedNotes_type savedNotes[MAX_ENTRIES][MAX_SAVED_NOTES];

/*
 * 	New functions:
 * 	Save note,
 * 	meassure time (make attack return the starting time to compare current to that)
 */

void PENTA_saveYPos(uint8 y_pos)
{
	savedNotes[1][saving_position].y_pos =  y_pos;
}

void PENTA_saveTopBottom(uint8 top_bottom)
{
	savedNotes[1][saving_position].top_bottom = top_bottom;
}

void PENTA_saveStartingTime()
{
	savedNotes[1][saving_position].start_time = PENTA_getTimeCounter();
}

void PENTA_saveDuration()
{
	/** Gets the current time, then subtracts starting time to get duration */
	uint8 current_time = PENTA_getTimeCounter();
	savedNotes[1][saving_position].duration = current_time - savedNotes[1][saving_position].start_time;
	/** Increases position, since the next sound will be the next to be saved */
	PENTA_graph();
	saving_position++;
	/** After this you can print */
}

void PENTA_graph()
{
	uint8 posX = PENTA_getTempoCounterPosition(); //check
	if(FALSE != savedNotes[1][saving_position].y_pos)
	{
		if(TRUE == savedNotes[1][saving_position].top_bottom)
		{
			LCD_ILI9341_writeBigLetter(posX, savedNotes[1][saving_position].y_pos + BOTTOM_OFF, 0, WHITE);
		}

		else
		{
			LCD_ILI9341_writeBigLetter(posX, savedNotes[1][saving_position].y_pos, 0, WHITE);
		}

		if(TRUE == PENTA_getClearPenta())
		{
			DisableInterrupts;
			LCD_ILI9341_fillScreen(ILI9341_CYAN);
			LCD_ILI9341_drawPartiture(FALSE);
			LCD_ILI9341_drawPartiture(TRUE);
			PENTA_clearClearPenta();
			EnableInterrupts;
		}
	}
	else
	{
		saving_position--;
		tempoCounter = (tempoCounter - 1 < 1)? 1: tempoCounter--;
	}

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

sint8 PENTA_findNote(float32 freq)
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
			return Notes[index].id;
		}
	}
	/** Returns an error */
	return FALSE;
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
