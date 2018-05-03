/*
 * DSP.c
 *
 *  Created on: Apr 28, 2018
 *      Author: kanta
 */

#include "DSP.h"


#define RES_16_BIT 65535
#define SUPPLY_V 3.3f
#define ATTACK_THRESH 0.55f


float32 secondBuffer[MAX_SAMPLES];
uint16 noteBufferPointer;

uint8 savingData_flag;
uint8 autoCorCo_flag = TRUE;

const DSP_note_type Notes[22] =
		{
				{C4, 0},
				{CS4, 1},
				{D4, 0},
				{DS4, 1},
				{E4, 0},
				{F4, 0},
				{FS4, 1},
				{G4, 0},
				{GS4, 1},
				{A4, 0},
				{AS4, 1},
				{B4, 0},
				{C5, 0},
				{CS5, 1},
				{D5, 0},
				{DS5, 1},
				{E5, 0},
				{F5, 0},
				{FS5, 1},
				{G5, 0},
				{GS5, 1},
				{A5, 0},

		};

float32 DSP_digToFloat(uint16 data)
{
	return (data*SUPPLY_V/RES_16_BIT) - SUPPLY_V/2; 	/** Multiplies by supply voltage and divides by pow(2,16) - 1*/
}

uint8 DSP_checkAttack(uint16 data)
{
	/** Checks if value converted to float surpasses the defined threshold*/
	uint8 isAttack =(DSP_digToFloat(data) >= ATTACK_THRESH);
	/** If so, turns on flag that indicates to save values*/
	if(TRUE == isAttack)
	{
		savingData_flag = TRUE;
	}
	return isAttack;
}

void DSP_saveNote(uint16 data, float32 * noteBuffer)
{
	/** Saves float value in array and increases the pointer*/
	noteBuffer[noteBufferPointer] = DSP_digToFloat(data);
	noteBufferPointer++;

	/** If the sampling is done, reinitialize pointer and turn off flag*/
	if(MAX_SAMPLES == noteBufferPointer)
	{
		noteBufferPointer = FALSE;
		savingData_flag = FALSE;
	}
}

void DSP_autocor(float32 * noteBuffer, float32 * corrBuffer)
{
	uint16 lag_index;
	uint16 sample_index;
	autoCorCo_flag = FALSE;

	/** Autocorrelation formula */
	for(lag_index = 0; lag_index < MAX_SAMPLES; lag_index++)
	{
		for(sample_index = 0; sample_index < MAX_SAMPLES - lag_index; sample_index++)
		{
			if(sample_index - lag_index > 0)
			{
				corrBuffer[lag_index] += noteBuffer[sample_index]*noteBuffer[sample_index - lag_index];
			}
		}
	}

	autoCorCo_flag = TRUE;
}

uint16 DSP_detectPeak(float32 * corrBuffer)
{
	uint16 pitch_period;
	uint16 index;

	float32 peak;
	float32 prev_value = corrBuffer[0];
	float32 curr_value = corrBuffer[1];

	for(index = 2; prev_value > curr_value; index++)
	{
		prev_value = curr_value;
		curr_value = corrBuffer[index];
	}

	peak = curr_value;
	for(; index < MAX_SAMPLES; index++)
	{
		if(corrBuffer[index] > peak)
		{
			peak = corrBuffer[index];
			pitch_period = index;
		}
	}

	return pitch_period;
}

void DSP_findNote()
{

}

void DSP_clearBuffer(float32 * buffer)
{
	for(uint16 index = 0; index < MAX_SAMPLES; index++)
	{
		buffer[index] = FALSE;
	}
}

float32 DSP_findPitch(uint16 pitch_period)
{
	return SAMPLE_FS/pitch_period;
}

uint8 DSP_getSavingFlag()
{
	return savingData_flag;
}

uint8 DSP_getAutoCorFlag()
{
	return autoCorCo_flag;
}
