/*
 * DSP.c
 *
 *  Created on: Apr 28, 2018
 *      Author: kanta
 */

#include "DSP.h"
#include "PIT.h"


#define RES_16_BIT 65535
#define SUPPLY_V 3.3f
#define ATTACK_THRESH .40f
#define MAX_BUFFERS 4

uint16 noteBufferPointer;

uint8 savingData_flag;
uint8 checkingTime_flag;
uint8 autoCorCo_flag = TRUE;

uint16 buff_no;
uint16 corBuff_no;
float32 noteBuffer[MAX_BUFFERS][MAX_SAMPLES] = {0};
float32 corrBuffer[MAX_SAMPLES] = {0};

typedef union
{
	struct
	{
		uint8 status1 : 1;
		uint8 status2 : 1;
		uint8 status3 : 1;
		uint8 status4 : 1;
	};
	uint8 general_status: 4;
}Buffer_status_type;

Buffer_status_type buffer_status;

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
		/** Start pit to count the time */

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
	}
}

uint8 DSP_getGeneralStatus()
{
	return buffer_status.general_status;
}
