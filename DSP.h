/**
 * \file
 * 			DSP.h
 *
 * \authors
 * 			Dario Hoyo
 *      	Alan Duran
 * \date
 * 			7/05/18
 *
 *      */

#ifndef DSP_H_
#define DSP_H_

#include "DataTypeDefinitions.h"
#include "FLEX.h"


#define RES_16_BIT 65535 /**< The max value the adc can represent */
#define SUPPLY_V 3.3f	 /**< The max value the adc will take by the electret condenser*/
#define ATTACK_THRESH .55f	/**< Value used to calibrate fixated threshold */
#define LOW_ATTACK_THRESH .3f	/**< Value of threshold to be considered as noise */
#define MAX_BUFFERS 16		/**< Number of buffers to save values */
#define THRESH_OFF .3f	/**< Offset to be added to dynamic threshold */

/**
 * 	Union of the status of the saving buffers, along with a bitmap in a structure and another value that can
 * 	contain them all.
 */

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

#define MAX_SAMPLES 1000 /**< Max number of samples to make an autocorrelation. */

/**
 * 	\brief converts digital values from adc to smaller values an autocorrelation can support.
 *
 * 	\param[in] data Data input from ADC.
 *
 * 	\return Float number ranging from -SUPPLY_VOLTAGE/2 TO SUPPLY_VOLTAGE/2
 *
 */

float32 DSP_digToFloat(uint16 data);

/**
 * 	\brief Check if the input value is greater than the threshold.
 *
 *	Also sets a flag indicating that a new value should be saved given the threshold was surpassed.
 *
 *	\param[in] data Data input from ADC.
 *	\return indicates if there was an attack {TRUE}, or not [FALSE]
 */

uint8 DSP_checkAttack(uint16 data);

/**
 * 	\brief Saves data into a buffer to later be processed.
 *
 * 	\param[in] data Data coming from ADC.
 */

void DSP_saveNote(uint16 data);

/**
 * 	\brief First step of PDA. Does a correlation of the signal with
 * 	itself delayed in time and saves it in a buffer to later be processed.
 */

void DSP_autocor();

/**
 * 	\brief Second step of PDA. Checks the highest non-zero peak of the autocorrelation.
 * 	the index of the array will correspond to the fundamental period of the sound.
 *
 * 	\return Index: fundamental period.
 */

uint16 DSP_detectPeak(void);

/**
 * 	\brief Divides the sampling rate by the fundamental period index to find the fundamental frequency.
 *
 * 	\param[in] pitch_period Fundamental period index.
 * 	\return Fundamental frequency.
 */

float32 DSP_findPitch(uint16 pitch_period);

/**
 * 	\brief Clears autocorrelation buffer to make another correlation.
 */

void DSP_clearBuffer(void);

/**
 * 	\return Flag that indicates whether values should be saved.
 */

uint8 DSP_getSavingFlag();

/**
 * 	\brief Set the status of a saving buffer indicating values are ready to be autocorrelated.
 *
 * 	\param[in] status_no Number of buffer.
 */

void DSP_setStatus(uint8 status_no);

/**
 * 	\brief Clears the status of a saving buffer, indicating it does not have values.
 *
 * 	\param[in] status_no Number of buffer;
 */

void DSP_clearStatus(uint8 status_no);

/**
 * 	\return Returns the status of the bitmap, corresponding to all statuses of the buffers.
 */

uint16 DSP_getGeneralStatus();

/**
 *	\return Returns the number of the buffer by doing a modulo of the counter with the number of max buffers.
 */

uint8 DSP_getBuffNo();

/**
 * 	\return Returns the number of the buffer the autocorrelation is in by doing a modulo of the counter
 * 	with the number of max buffers.
 */

uint8 DSP_getCorBuffNo();

/**
 * 	\brief Sets a new attack threshold by averaging positive values coming from ADC
 *
 * 	\param[in] data Data coming form adc.
 */

void DSP_setNewAttackThresh(uint16 data);

#endif /* DSP_H_ */
