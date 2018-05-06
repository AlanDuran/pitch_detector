/*
 * DSP.c
 *
 *  Created on: Apr 28, 2018
 *      Author: kanta
 */

#include "DSP.h"
#include "PIT.h"
#include "PENTA.h"


#define RES_16_BIT 65535
#define SUPPLY_V 3.3f
#define ATTACK_THRESH .55f
#define LOW_ATTACK_THRESH .3f
#define MAX_BUFFERS 16
#define THRESH_OFF .3f

uint16 noteBufferPointer;

uint8 savingData_flag;
uint8 checkingTime_flag;
uint8 autoCorCo_flag = TRUE;

uint8 currentTempo;

/** Variables to set new threshold*/
float32 new_threshold;
float32 temp_threshold;
uint8 threshold_counter;

/** Flag that indicates to take duration from diff between attacks*/
uint8 duration_attacks_flag = TRUE;

uint16 buff_no;
uint16 corBuff_no;
float32 noteBuffer[MAX_BUFFERS][MAX_SAMPLES] = {0};
float32 corrBuffer[MAX_SAMPLES] = {0};

typedef union
{
	struct
	{
		uint16 status1 : 1;
		uint16 status2 : 1;
		uint16 status3 : 1;
		uint16 status4 : 1;
		uint16 status5 : 1;
		uint16 status6 : 1;
		uint16 status7 : 1;
		uint16 status8 : 1;
		uint16 status9 : 1;
		uint16 status10 : 1;
		uint16 status11 : 1;
		uint16 status12 : 1;
		uint16 status13 : 1;
		uint16 status14 : 1;
		uint16 status15 : 1;
		uint16 status16 : 1;
	};
	uint16 general_status;
}Buffer_status_type;

Buffer_status_type buffer_status;

float32 DSP_digToFloat(uint16 data)
{
	return (data*SUPPLY_V/RES_16_BIT) - SUPPLY_V/2; 	/** Multiplies by supply voltage and divides by pow(2,16) - 1*/
}

uint8 DSP_checkAttack(uint16 data)
{
	/** Checks if value converted to float surpasses the defined threshold*/
	uint8 isAttack =(DSP_digToFloat(data) >= new_threshold + THRESH_OFF);
	/** If so, turns on flag that indicates to save values*/

	/** If silence, save duration with difference in times and turn on a flag that indicates
	 * 	not to take duration from difference between attacks
	 */
	if (new_threshold == LOW_ATTACK_THRESH && currentTempo != FALSE && TRUE == duration_attacks_flag)
	{
		duration_attacks_flag = FALSE;
		/** Save data */
		PENTA_saveDuration(); /**< Need to be careful, the first entry might record this forever*/
	}

	if(TRUE == isAttack && PENTA_getTimeCounter() != currentTempo)
	{
		if(TRUE == duration_attacks_flag)
		{
			/** Save last note's duration with current time*/
			PENTA_saveDuration();
		}
		/** Save starting time on current note */

		PENTA_saveStartingTime();
		savingData_flag = TRUE;
		currentTempo = PENTA_getTimeCounter();
		duration_attacks_flag = TRUE; /**< Reset the flag to indicate it can measure between attacks */
	}
	return isAttack;
}

void DSP_saveNote(uint16 data)
{
	uint8 currBuff = DSP_getBuffNo();
	/** Saves float value in array and increases the pointer*/
	noteBuffer[currBuff][noteBufferPointer] = DSP_digToFloat(data);
	noteBufferPointer++;

	/** If the sampling is done, reinitialize pointer and turn off flag to sample*/
	if(MAX_SAMPLES == noteBufferPointer)
	{
		noteBufferPointer = FALSE;
		savingData_flag = FALSE;
		DSP_setStatus(currBuff);
		buff_no++;
	}
}

void DSP_autocor()
{
	uint8 currCorBuff = DSP_getCorBuffNo();
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
				corrBuffer[lag_index] += noteBuffer[currCorBuff][sample_index]*noteBuffer[currCorBuff][sample_index - lag_index];
			}
		}
	}
	DSP_clearStatus(currCorBuff);
	corBuff_no++;
	autoCorCo_flag = TRUE;
}

uint16 DSP_detectPeak()
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

void DSP_setNewAttackThresh(uint16 data)
{
	/** Stores data only if they are positive numbers */
	float32 newData = DSP_digToFloat(data);
	if(newData > 0)
	{
		temp_threshold += newData;
		threshold_counter++;
	}
	/** If the samples reach the desired ammount, averages and sets */
	if(threshold_counter == AVG_MAX_SAMPLES)
	{
		/** Averages and sets the new threshold*/
		new_threshold = (temp_threshold/threshold_counter)*2;
		new_threshold = (new_threshold < LOW_ATTACK_THRESH)? LOW_ATTACK_THRESH : new_threshold;
		temp_threshold = FALSE;
		threshold_counter = FALSE;
	}
}


void DSP_clearBuffer()
{
	for(uint16 index = 0; index < MAX_SAMPLES; index++)
	{
		corrBuffer[index] = FALSE;
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

uint8 DSP_getBuffNo()
{
	return buff_no%MAX_BUFFERS;
}

uint8 DSP_getCorBuffNo()
{
	return corBuff_no%MAX_BUFFERS;
}

void DSP_setStatus(uint8 status_no)
{
	switch(status_no)
	{
	case 0:
		buffer_status.status1 = TRUE;
		break;
	case 1:
		buffer_status.status2 = TRUE;
		break;
	case 2:
		buffer_status.status3 = TRUE;
		break;
	case 3:
		buffer_status.status4 = TRUE;
		break;
	case 4:
		buffer_status.status5 = TRUE;
		break;
	case 5:
		buffer_status.status6 = TRUE;
		break;
	case 6:
		buffer_status.status7 = TRUE;
		break;
	case 7:
		buffer_status.status8 = TRUE;
		break;
	case 8:
		buffer_status.status9 = TRUE;
		break;
	case 9:
		buffer_status.status10 = TRUE;
		break;
	case 10:
		buffer_status.status11 = TRUE;
		break;
	case 11:
		buffer_status.status12 = TRUE;
		break;
	case 12:
		buffer_status.status13 = TRUE;
		break;
	case 13:
		buffer_status.status14 = TRUE;
		break;
	case 14:
		buffer_status.status15 = TRUE;
		break;
	case 15:
		buffer_status.status16 = TRUE;
		break;

	}
}

void DSP_clearStatus(uint8 status_no)
{
	switch(status_no)
	{
	case 0:
		buffer_status.status1 = FALSE;
		break;
	case 1:
		buffer_status.status2 = FALSE;
		break;
	case 2:
		buffer_status.status3 = FALSE;
		break;
	case 3:
		buffer_status.status4 = FALSE;
		break;
	case 4:
		buffer_status.status5 = FALSE;
		break;
	case 5:
		buffer_status.status6 = FALSE;
		break;
	case 6:
		buffer_status.status7 = FALSE;
		break;
	case 7:
		buffer_status.status8 = FALSE;
		break;
	case 8:
		buffer_status.status9 = FALSE;
		break;
	case 9:
		buffer_status.status10 = FALSE;
		break;
	case 10:
		buffer_status.status11 = FALSE;
		break;
	case 11:
		buffer_status.status12 = FALSE;
		break;
	case 12:
		buffer_status.status13 = FALSE;
		break;
	case 13:
		buffer_status.status14 = FALSE;
		break;
	case 14:
		buffer_status.status15 = FALSE;
		break;
	case 15:
		buffer_status.status16 = FALSE;
		break;
	}
}

uint16 DSP_getGeneralStatus()
{
	return buffer_status.general_status;
}
