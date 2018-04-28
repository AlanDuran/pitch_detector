/**
 * \file PIT.C
 *
 *  \date Feb 21, 2018
 *
 *  \authors Alan Duran
 *  		Dario Hoyo
 */
#include "MK64F12.h"

#include "PIT.h"

static PIT_interruptFlags_t PIT_intrStatusFlag;

void PIT0_IRQHandler()
{
	///Pit status flag is set to true so interrupt can be detected elsewhere and TFLG is turned off.
	PIT_intrStatusFlag.flagPIT0 = TRUE;
	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK;
}

void PIT1_IRQHandler()
{
	///Pit status flag is set to true so interrupt can be detected elsewhere and TFLG is turned off.
	PIT_intrStatusFlag.flagPIT1 = TRUE;
	PIT->CHANNEL[PIT_1].TFLG |= PIT_TFLG_TIF_MASK;
}

void PIT2_IRQHandler()
{
	///Pit status flag is set to true so interrupt can be detected elsewhere and TFLG is turned off.
	PIT_intrStatusFlag.flagPIT2 = TRUE;
	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK;
}

void PIT3_IRQHandler()
{
	///Pit status flag is set to true so interrupt can be detected elsewhere and TFLG is turned off.
	PIT_intrStatusFlag.flagPIT3 = TRUE;
	PIT->CHANNEL[PIT_3].TFLG |= PIT_TFLG_TIF_MASK;
}

uint8 PIT_getIRQStatus(PIT_Timer_t pitTimer)
{
	///Return the status flag
	switch (pitTimer)
	{
		case PIT_0:
			return(PIT_intrStatusFlag.flagPIT0);
			break;
		case PIT_1:
			return(PIT_intrStatusFlag.flagPIT1);
			break;
		case PIT_2:
			return(PIT_intrStatusFlag.flagPIT2);
			break;
		case PIT_3:
			return(PIT_intrStatusFlag.flagPIT3);
			break;
		default:
			return(ERROR);
			break;
	}

}

void PIT_clearIRQStatus(PIT_Timer_t pitTimer)
{
	///Clear the status flag on given channel.
	switch (pitTimer) {
		case PIT_0:
			PIT_intrStatusFlag.flagPIT0 = FALSE;
			break;
		case PIT_1:
			PIT_intrStatusFlag.flagPIT1 = FALSE;
			break;
		case PIT_2:
			PIT_intrStatusFlag.flagPIT2 = FALSE;
			break;
		case PIT_3:
			PIT_intrStatusFlag.flagPIT3 = FALSE;
			break;
		default:
			break;
	}

}

void PIT_setIRQStatus(PIT_Timer_t pitTimer)
{
	///Set to true a flag on given channel.
	switch (pitTimer) {
		case PIT_0:
			PIT_intrStatusFlag.flagPIT0 = TRUE;
			break;
		case PIT_1:
			PIT_intrStatusFlag.flagPIT1 = TRUE;
			break;
		case PIT_2:
			PIT_intrStatusFlag.flagPIT2 = TRUE;
			break;
		case PIT_3:
			PIT_intrStatusFlag.flagPIT3 = TRUE;
			break;
		default:
			break;
	}

}

void PIT_delay(PIT_Timer_t pitTimer,float systemClock ,float period)
{
	///Fills LDVAL with the value to count then sets the enable and interrupt on the timer.
	PIT->CHANNEL[pitTimer].LDVAL = (period*systemClock);
	PIT->CHANNEL[pitTimer].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//enables timer0
}

void PIT_enable()
{
	///Zero on MDIS enables the PIT and a zero on FRZ enables the timers to run on debug mode.
	PIT->MCR &= ~(PIT_MCR_MDIS_MASK);
	PIT->MCR &= ~(PIT_MCR_FRZ_MASK);
}

void PIT_stop(PIT_Timer_t channel)
{
	///Turns off enable on given channel.
	PIT->CHANNEL[channel].TCTRL &= ~(PIT_TCTRL_TEN_MASK);
}

void PIT_clockGating()
{
	///Clock gating using PIT MASK
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8 PIT_getIntrStatus(PIT_Timer_t pitTimer)
{
	///returns the flag on TFLG
	return PIT->CHANNEL[pitTimer].TFLG & PIT_TFLG_TIF_MASK;
}


void PIT_clear(PIT_Timer_t pitTimer)
{
	///Clears the flag on TFLG
	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK;
}
