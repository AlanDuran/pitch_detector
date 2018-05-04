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

#define CONSTAT_SUB 3


/** Type of structure to identify the note with the fundamental frequency */

typedef struct Notes
{
	float32 f0;
	uint8 sharp;
	sint8 id;
	float32 diff;
} PENTA_note_type;

/**
 * 	\brief Structure referencing notes
 *
 * 	This indicates the id of the note to be played next and it's duration.
 * 	The duration indicated here goes from 1 to 16, with a sensibility of a sixteenth note.
 * 	So a duration of 2 would equal an eight, 4 would equal a quarter note, 8 would equal a half note
 * 	and 16 would equal a whole note.
 */

typedef struct s
{
	sint8 id;
	uint8 sharp;
	uint8 duration;
}ToBePlayedNote_type;

void PENTA_findNote();

void PENTA_setId(sint8 id);

void PENTA_graph(uint8 duration);

void PENTA_startTimeMeassure();

void PENTA_stopTimeMeassure();

#endif /* PENTA_H_ */
