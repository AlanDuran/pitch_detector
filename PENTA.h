/**
 * \file
 * 		PENTA.h
 *
 * \brief
 * 		This is a driver for the internal ADC of a K64F board
 *
 * \authors
 *  	Dario Hoyo
 *     	Alan Duran
 *
 * \date
 * 		05/07/2018
 *
 *      */

#ifndef PENTA_H_
#define PENTA_H_

#include "DataTypeDefinitions.h"

/*
 * 	Notes on differences between f0s starting with c4
 * 	15.55, 16.48, 17.47, 18.5, 19.6, 20.76, 22.01, 23.3, 24.7, 26.16, 27.72, 29.37,
 * 	31.12, 32.96, 34.92, 37, 39.21, 41.53, 44, 46.62, 49.39, 52.
 *
 * 	To identify the notes with a little bit of error, we would need to take half of this differences
 * 	(to separate them from one another), and subtract a little, say, three.
 */

#define DIFF1 15.55f
#define DIFF2 16.48f
#define DIFF3 17.47f
#define DIFF4 18.5f
#define DIFF5 19.6f
#define DIFF6 20.76f
#define DIFF7 22.01f
#define DIFF8 23.3f
#define DIFF9 24.7f
#define DIFF10 26.16f
#define DIFF11 27.72f
#define DIFF12 29.37f
#define DIFF13 31.12f
#define DIFF14 32.96f
#define DIFF15 34.92f
#define DIFF16 37
#define DIFF17 39.21f
#define DIFF18 41.53f
#define DIFF19 44
#define DIFF20 46.62f
#define DIFF21 49.39f
#define DIFF22 52

#define CONSTAT_SUB 0.8F

/** Notes frequencies */
#define C_4 261.63f
#define CS4 277.18f
#define D4 293.66f
#define DS4 311.13f
#define E4 329.63f
#define F4 349.23f
#define FS4 369.99f
#define G4 392
#define GS4 415.30f
#define A4 440
#define AS4 466.16f
#define B4 493.88f
#define C_5 523.25f
#define CS5 554.37f
#define D5 587.33f
#define DS5 622.25f
#define E5 659.25f
#define F5 698.46f
#define FS5 739.99f
#define G5 783.99f
#define GS5 830.61f
#define A5 880

#define BOTTOM_OFF 134 /**< Offset to print in the bottom pentagram */

#define AVG_MAX_SAMPLES 250 /**< Max samples for averaging */

#define MAX_NOTES 22		/**< Maximum different frequencies*/

#define SYSTEM_CLOCK 60000000 /**< Clock that PIT uses */
#define LOW_TEST_TEMPO 80.00f /**< Starting tempo */
#define MAX_SAVED_NOTES 240	  /**< Maximum notes you can save */
#define TEMPO_DIV 16		/**< Number of virtual beats */
#define START_MAX_SAVES 2	/**< Starting number for number of compases*/
#define SEC_MIN 60			/**< Number of seconds in a minute (used in operation) */
#define TEMPO_ATTACK_DIV 4	/**< Divider of tempo to create virtual beats */

#define DIFF_DIV 2 /**< Divider of the differences */

/** Defines for position of tempo beats */

#define XPOS1_TEMPO1 30
#define XPOS1_TEMPO2 86
#define XPOS1_TEMPO3 142
#define XPOS1_TEMPO4 198

#define XPOS2_TEMPO1 40
#define XPOS2_TEMPO2 96
#define XPOS2_TEMPO3 152
#define XPOS2_TEMPO4 208

#define YPOS1_TEMPO 290
#define YPOS2_TEMPO 300

/** Defines for increments and limits of tempo */

#define TEMPO_INC 5
#define TEMPO_MAX 120
#define TEMPO_MIN 60

/** Page defines */

#define MIN_COMP_PAGE 2
#define SUB_CONST_PAGE 1
#define NOTES_PER_PAGE 32

#define SYM_LENGTH 14 /**< Lenght of our symbols for our font */
#define SUB_TEMPO_COUNTER 1

#define COMP_PER_PAGE 2


/** Type of structure to identify the note with the fundamental frequency */

typedef struct
{
	float32 f0;
	uint8 sharp;
	sint8 id;
	float32 diff;
} PENTA_note_type;

/** Structure to be made an array to save the notes */

typedef struct
{
	uint8 y_pos;
	uint8 x_pos;
	uint8 sharp;
}PENTA_savedNotes_type;

/*
* 	15.55, 16.48, 17.47, 18.5, 19.6, 20.76, 22.01, 23.3, 24.7, 26.16, 27.72, 29.37,
* 	31.12, 32.96, 34.92, 37, 39.21, 41.53, 44, 46.62, 49.39, 52.
*/

/** Defines of the positions of the notes in the pentagram */

#define POS_C4 115
#define POS_D4 108
#define POS_E4 100
#define POS_F4 93
#define POS_G4 85
#define POS_A4 78
#define POS_B4 70
#define POS_C5 63
#define POS_D5 55
#define POS_E5 48
#define POS_F5 40
#define POS_G5 33
#define POS_A5 25

/** Table that has all the necessary information to identify the notes and print them */

static const PENTA_note_type Notes[MAX_NOTES] =
		{
				{C_4, FALSE, POS_C4, DIFF1/DIFF_DIV - CONSTAT_SUB},
				{CS4, TRUE, POS_C4, DIFF2/DIFF_DIV - CONSTAT_SUB},
				{D4, FALSE, POS_D4, DIFF3/DIFF_DIV - CONSTAT_SUB},
				{DS4, TRUE, POS_D4, DIFF4/DIFF_DIV - CONSTAT_SUB},
				{E4, FALSE, POS_E4, DIFF5/DIFF_DIV - CONSTAT_SUB},
				{F4, FALSE, POS_F4, DIFF6/DIFF_DIV - CONSTAT_SUB},
				{FS4, TRUE, POS_F4, DIFF7/DIFF_DIV - CONSTAT_SUB},
				{G4, FALSE, POS_G4, DIFF8/DIFF_DIV - CONSTAT_SUB},
				{GS4, TRUE, POS_G4, DIFF9/DIFF_DIV - CONSTAT_SUB},
				{A4, FALSE, POS_A4, DIFF10/DIFF_DIV - CONSTAT_SUB},
				{AS4, TRUE, POS_A4, DIFF11/DIFF_DIV - CONSTAT_SUB},
				{B4, FALSE, POS_B4, DIFF12/DIFF_DIV - CONSTAT_SUB},
				{C_5, FALSE, POS_C5, DIFF13/DIFF_DIV - CONSTAT_SUB},
				{CS5, TRUE, POS_C5, DIFF14/DIFF_DIV - CONSTAT_SUB},
				{D5, FALSE, POS_D5, DIFF15/DIFF_DIV - CONSTAT_SUB},
				{DS5, TRUE, POS_D5, DIFF16/DIFF_DIV - CONSTAT_SUB},
				{E5, FALSE, POS_E5, DIFF17/DIFF_DIV - CONSTAT_SUB},
				{F5, FALSE, POS_F5, DIFF18/DIFF_DIV - CONSTAT_SUB},
				{FS5, TRUE, POS_F5, DIFF19/DIFF_DIV - CONSTAT_SUB},
				{G5, FALSE, POS_G5, DIFF20/DIFF_DIV - CONSTAT_SUB},
				{GS5, TRUE, POS_G5, DIFF21/DIFF_DIV - CONSTAT_SUB},
				{A5, FALSE, POS_A5, DIFF22/DIFF_DIV - CONSTAT_SUB},

		};

/**
 *	\brief Save the position of the note in the y axis
 *
 *	\param[in] y_pos Y position
 */

void PENTA_saveYPos(uint8 y_pos);

/**
 *	\brief Save the position of the note in the x axis
 *
 *	\param[in] y_pos X position
 */

void PENTA_saveXPos(uint8 x_pos);

/**
 *	\brief Save whether the note is sharp or not
 *
 *	\param[in] y_pos Sharp (TRUE or FALSE)
 */

void PENTA_saveSharp(uint8 sharp);

/**
 * 	\brief Graph with the saving information and increment the index of the saving.
 *
 * 	If the menu is in state of saving, this will turn off once the desired notes were reached.
 */

void PENTA_graph();

/**
 * 	\brief identifies the note with the table and saves the values in the index.
 *
 * 	\param[in] freq Fundamental frequency of the note that will be compared with the values of the table.
 */

void PENTA_findNote(float32 freq);

/**
 * 	\brief Start the pit with the desired tempo.
 */

void PENTA_startTimeMeassure();

/**
 * 	\brief Graphs the tempo each four beats (since the rest are virtual for other processes)
 */

void PENTA_graphTempo();

/**
 * 	\brief Count each time the pit interrupts.
 */

void PENTA_timeCount();

/**
 * 	\brief Stop the PIT.
 */

void PENTA_stopTimeMeassure();

/**
 * 	\brief Clear the flag that indicates pentagram should be refreshed.
 */

void PENTA_clearClearPenta();

/**
 * 	\brief Validates tempo limit and then restarts the pit.
 */

void PENTA_increaseTempo();

/**
 * 	\brief Validates tempo limits and then restarts pit.
 */

void PENTA_decreaseTempo();

/**
 * 	\brief Calls again the pit delay function with the new parameters.
 */

void PENTA_restartPit();

/**
 * 	\brief Clears all the notes saved and counters so everything works back from scratch.
 */

void PENTA_clearSaves();

/**
 * 	\brief Set the maximum ammount of notes to be saved.
 *
 * 	\param[in] new_max New value to be set.
 */

void PENTA_setMaxSaves(uint8 new_max);

/**
 *	\brief Clears the page number in the bottom of the screen.
 */

void PENTA_clearPage();

/**
 * 	\brief Prints the page number in the bottom of the screen.
 */

void PENTA_printPage();

/**
 * 	\brief Returns to the previous saved notes page.
 *
 * 	Checks if it is a valid page number and calls the cuntion to print it with new parameters.
 */

void PENTA_prevPage();

/**
 * 	\brief Returns to the next saved notes page.
 *
 * 	Checks if it is a valid page number and calls the cuntion to print it with new parameters.
 */


void PENTA_nextPage();

/**
 * 	\return Returns the current tempo.
 */

uint8 PENTA_getTempo();

/**
 * 	\return Returns the current page.
 */

uint8 PENTA_getCurrPage();

/**
 * 	\return Returns position of the screen according to the tempoCounter.
 */

uint8 PENTA_getTempoCounterPosition();

/**
 * 	\return Returns the counter of the time counted by the pit.
 */

uint8 PENTA_getTimeCounter();

/**
 *	\return Returns whether the pentagram is saving on top or bottom using a modulo.
 */

uint8 PENTA_getTopOrBottom();

/**
 * 	\return Returns the flag that indicates if the screen should be refreshed.
 */

uint8 PENTA_getClearPenta();

#endif /* PENTA_H_ */
