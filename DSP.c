/*
 * DSP.c
 *
 *  Created on: Apr 28, 2018
 *      Author: kanta
 */

#include "DSP.h"


#define RES_16_BIT 65535
#define SUPPLY_V 3.3f
#define ATTACK_THRESH 2.2f	/**< Keep calibrating */
#define MAX_SAMPLES 312

float32 noteBuffer[MAX_SAMPLES];
uint16 noteBufferPointer;

uint8 savingData_flag;

float32 DSP_digToFloat(uint16 data)
{
	return (data*SUPPLY_V/RES_16_BIT); 	/** Multiplies by supply voltage and divides by pow(2,16) - 1*/
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

void DSP_saveNote(uint16 data)
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

uint8 DSP_getSavingFlag(){
	return savingData_flag;
}
