/*
 * PENTA.h
 *
 *  Created on: May 3, 2018
 *      Author: kanta
 */

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

#define CONSTAT_SUB 1

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

#define BOTTOM_OFF 134

#define AVG_MAX_SAMPLES 250

#define MAX_NOTES 22


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

static const PENTA_note_type Notes[MAX_NOTES] =
		{
				{C_4, 0, 115, DIFF1/2 - CONSTAT_SUB},
				{CS4, 1, 115, DIFF2/2 - CONSTAT_SUB},
				{D4, 0, 108, DIFF3/2 - CONSTAT_SUB},
				{DS4, 1, 108, DIFF4/2 - CONSTAT_SUB},
				{E4, 0, 100, DIFF5/2 - CONSTAT_SUB},
				{F4, 0, 93, DIFF6/2 - CONSTAT_SUB},
				{FS4, 1, 93, DIFF7/2 - CONSTAT_SUB},
				{G4, 0, 85, DIFF8/2 - CONSTAT_SUB},
				{GS4, 1, 85, DIFF9/2 - CONSTAT_SUB},
				{A4, 0, 78, DIFF10/2 - CONSTAT_SUB},
				{AS4, 1, 78, DIFF11/2 - CONSTAT_SUB},
				{B4, 0, 70, DIFF12/2 - CONSTAT_SUB},
				{C_5, 0, 63, DIFF13/2 - CONSTAT_SUB},
				{CS5, 1, 63, DIFF14/2 - CONSTAT_SUB},
				{D5, 0, 55, DIFF15/2 - CONSTAT_SUB},
				{DS5, 1, 55, DIFF16/2 - CONSTAT_SUB},
				{E5, 0, 48, DIFF17/2 - CONSTAT_SUB},
				{F5, 0, 40, DIFF18/2 - CONSTAT_SUB},
				{FS5, 1, 40, DIFF19/2 - CONSTAT_SUB},
				{G5, 0, 33, DIFF20/2 - CONSTAT_SUB},
				{GS5, 1, 33, DIFF21/2 - CONSTAT_SUB},
				{A5, 0, 25, DIFF22/2 - CONSTAT_SUB},

		};


/**
 * 	\brief Structure referencing notes
 *
 * 	This indicates the id of the note to be played next and it's duration.
 * 	The duration indicated here goes from 1 to 16, with a sensibility of a sixteenth note.
 * 	So a duration of 2 would equal an eight, 4 would equal a quarter note, 8 would equal a half note
 * 	and 16 would equal a whole note.
 */

typedef struct
{
	sint8 id;
	uint8 sharp;
	uint8 duration;
}ToBePlayedNote_type;

void PENTA_stall();

void PENTA_saveYPos(uint8 y_pos);

void PENTA_saveTopBottom(uint8 top_bottom);

void PENTA_saveStartingTime(void);

void PENTA_saveDuration(void);

void PENTA_graph();

void PENTA_findNote(float32 freq);

void PENTA_startTimeMeassure();

void PENTA_graphTempo();

void PENTA_timeCount();

void PENTA_stopTimeMeassure();

uint8 PENTA_getTempoCounterPosition();

uint8 PENTA_getTimeCounter();

uint8 PENTA_getTopOrBottom();

uint8 PENTA_getClearPenta();

void PENTA_clearClearPenta();

void PENTA_increaseTempo();

void PENTA_decreaseTempo();

void PENTA_restartPit();

void PENTA_clearSaves();

void PENTA_setMaxSaves(uint8 new_max);

#endif /* PENTA_H_ */
