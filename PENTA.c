/*
 * PENTA.c
 *
 *  Created on: May 3, 2018
 *      Author: kanta
 */

#include "PENTA.h"
#include "DSP.h"

#define MAX_NOTES 22
#define SYSTEM_CLOCK 30000000
#define LOW_TEST_TEMPO 0.25f
#define AVG_MAX_SAMPLES 100
#define NO_NOTE_THRESH 0.1f /** To be calibrated */

uint8 timeCounter;
uint8 checkTimeCounter;
float32 avgData;
uint8 checkingTime_flag;

/**
 * 	Will have a buffer of two in case we need the extra space.
 */

static ToBePlayedNote_type nextNote[4];
/*
* 	15.55, 16.48, 17.47, 18.5, 19.6, 20.76, 22.01, 23.3, 24.7, 26.16, 27.72, 29.37,
* 	31.12, 32.96, 34.92, 37, 39.21, 41.53, 44, 46.62, 49.39, 52.
*/

const PENTA_note_type Notes[MAX_NOTES] =
		{
				{C4, 0, 0, DIFF1/2 - CONSTAT_SUB},
				{CS4, 1, 0, DIFF2/2 - CONSTAT_SUB},
				{D4, 0, 0, DIFF3/2 - CONSTAT_SUB},
				{DS4, 1, 0, DIFF4/2 - CONSTAT_SUB},
				{E4, 0, 0, DIFF5/2 - CONSTAT_SUB},
				{F4, 0, 0, DIFF6/2 - CONSTAT_SUB},
				{FS4, 1, 0, DIFF7/2 - CONSTAT_SUB},
				{G4, 0, 0, DIFF8/2 - CONSTAT_SUB},
				{GS4, 1, 0, DIFF9/2 - CONSTAT_SUB},
				{A4, 0, 0, DIFF10/2 - CONSTAT_SUB},
				{AS4, 1, 0, DIFF11/2 - CONSTAT_SUB},
				{B4, 0, 0, DIFF12/2 - CONSTAT_SUB},
				{C5, 0, 0, DIFF13/2 - CONSTAT_SUB},
				{CS5, 1, 0, DIFF14/2 - CONSTAT_SUB},
				{D5, 0, 0, DIFF15/2 - CONSTAT_SUB},
				{DS5, 1, 0, DIFF16/2 - CONSTAT_SUB},
				{E5, 0, 0, DIFF17/2 - CONSTAT_SUB},
				{F5, 0, 0, DIFF18/2 - CONSTAT_SUB},
				{FS5, 1, 0, DIFF19/2 - CONSTAT_SUB},
				{G5, 0, 0, DIFF20/2 - CONSTAT_SUB},
				{GS5, 1, 0, DIFF21/2 - CONSTAT_SUB},
				{A5, 0, 0, DIFF22/2 - CONSTAT_SUB},

		};

void PENTA_setId(sint8 id, uint8 sharp)
{
	nextNote[0].id = id;
	nextNote[0].sharp = sharp;
}

void PENTA_graph(uint8 duration)
{
	nextNote[0].duration = duration;
	/** Paste in next position:
	 *
	 * 	Determine with duration what will be printed as follows:
	 * 	1 - a sixteenth (double flag)
	 * 	2 - an eight (flag)
	 * 	3 - an eight with a point (flag with point)
	 * 	4 - a quarter note (black)
	 * 	5 - a quarter note with a sixteenth together with a rainbow on top (black and double flag with rainbow on top)
	 * 	6 - a quarter note with a point (black with a point)
	 * 	7 - black with a flag with a point with rainbow
	 * 	8 - half note (white)
	 * 	9 - white with a double flag with rainbow
	 * 	10 - white with a flag with rainbow
	 * 	11 - white with a flag with point with rainbow
	 * 	12 - white with a point
	 * 	13 - white with a point with a double flag with rainbow
	 * 	14 - white with a point with a flag with rainbow
	 * 	15 - white with a point with a a flag with point with rainbow
	 * 	16 - whole note
	 */

	// Some of this sounds ridiculous and probably won't understand the myself, so will probably round them up.

	/**
	 * 	Determine the position of the pentagram with notes played (counter)(horizontally) and with id (vertically)
	 */
}

void PENTA_startTimeMeassure()
{
	PIT_delay(PIT_1, SYSTEM_CLOCK, LOW_TEST_TEMPO); /**< The period is taken from the tempo value for the pentagram*/
	checkingTime_flag = TRUE;
}

void PENTA_stopTimeMeassure()
{
	PIT_stop(PIT_1);
	PENTA_graph(timeCounter);
	timeCounter = FALSE;
}

void PENTA_timeCount()
{
	timeCounter++;
}

void PENTA_checkTime(uint16 * data)
{
	float32 newData = DSP_digToFloat(data);
	if(newData > 0)
	{
		avgData += newData;
		timeCheckCounter++;
	}

	if(timeCheckCounter == AVG_MAX_SAMPLES)
	{
		/** Averages and checks if the sound enters no note threshold */
		avgData /= timeCheckCounter;
		if(NO_NOTE_THRESH > avgData)
		{
			/** If so, stops measuring time and graphs*/
			PENTA_stopTimeMeassure();
			avgData = FALSE;
			timeCheckCounter = FALSE;
			checkingTime_flag = FALSE;
		}
		else
		{
			/** Keep averaging */
			avgData = FALSE;
			timeCheckCounter = FALSE;
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


