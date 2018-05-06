/*
 * PENTA.c
 *
 *  Created on: May 3, 2018
 *      Author: kanta
 */

#include "PENTA.h"
#include "DSP.h"
#include "PIT.h"
#include "LCD_ILI9341.h"

#define SYSTEM_CLOCK 60000000
#define LOW_TEST_TEMPO .5f
#define NO_NOTE_THRESH 0.15f /** To be calibrated */

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

/**
 * 	Will have a buffer of two in case we need the extra space.
 */


void PENTA_startTimeMeassure()
{
	PIT_delay(PIT_1, SYSTEM_CLOCK, LOW_TEST_TEMPO); /**< The period is taken from the tempo value for the pentagram*/
	//checkingTime_flag = TRUE;
}

void PENTA_graphTempo()
{
	switch(timeCounter%8)
	{
	case 0:
		LCD_ILI9341_drawShape(30, 290, 40, 300, ILI9341_BLACK);
		LCD_ILI9341_drawShape(86, 290, 96, 300, ILI9341_CYAN);
		LCD_ILI9341_drawShape(142, 290, 152, 300, ILI9341_CYAN);
		LCD_ILI9341_drawShape(198, 290, 208, 300, ILI9341_CYAN);
		break;
	case 2:
		LCD_ILI9341_drawShape(86, 290, 96, 300, ILI9341_BLACK);
		break;
	case 4:
		LCD_ILI9341_drawShape(142, 290, 152, 300, ILI9341_BLACK);
		break;
	case 6:
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
