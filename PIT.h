/**
 * \file PIT.h
 *
 * \date 20/03/2018
 * \authors Dario Hoyo
 * 			Alan Duran
 */

#ifndef PIT_H_
#define PIT_H_

#include "DataTypeDefinitions.h"

typedef struct
{
	uint8 flagPIT0 : 1;
	uint8 flagPIT1 : 1;
	uint8 flagPIT2 : 1;
	uint8 flagPIT3 : 1;
}PIT_interruptFlags_t;



#define PIT_CLOCK_GATING_ENABLE 0x400000
#define PIT_MCR_ENABLE 0x00
#define ERROR 0x02

/*! This enumerated constant are used to select the PIT to be used*/
typedef enum {PIT_0,PIT_1,PIT_2,PIT_3} PIT_Timer_t;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configure the PIT to generate a delay base on the system clock.
 	 Internally it configures the clock gating and enables the PIT module.
 	 It is important to note that this strictly is not device driver since everything is
 	 contained in a single function, but in general you have to avoid this practices, this only
 	 for the propose of the homework

 	 @param[in]  pitTimer,systemClock,period
 	 \return void
 */
void PIT_delay(PIT_Timer_t pitTimer,float systemClock ,float period);

/**
 * 	\brief Disables pit on given channel.
 */

void PIT_stop(uint8 channel);

/**
 * 	\brief Enables PIT channels and lets them run on debug mode.
 */

void PIT_enable();

/**
 * 	\brief Sets up clockgating for PIT.
 */

void PIT_clockGating(void);

/**
 * 	\brief Returns the status of the flag of given pit channel.
 */

uint8 PIT_getIRQStatus(PIT_Timer_t pitTimer);

/**
 * 	\brief Clears status flag on given channel.
 */

void PIT_clearIRQStatus(PIT_Timer_t pitTimer);

/**
 * 	\brief Sets status flag to true on given channel.
 */

void PIT_setIRQStatus(PIT_Timer_t pitTimer);

/**
 * 	\brief Clears TFLG from pit on given channel.
 *
 * 	This Flag is from one of the registers of the pit, different from the ones declared by a bitmap.
 */

void PIT_clear(PIT_Timer_t);

#endif /* PIT_H_ */
