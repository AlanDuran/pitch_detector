/*
 * DSP.h
 *
 *  Created on: Apr 28, 2018
 *      Author: kanta
 */

#ifndef DSP_H_
#define DSP_H_

#include "DataTypeDefinitions.h"
#include "FLEX.h"

#define MAX_SAMPLES 1000

/** Defines for the corresponding notes fundamental frequencies*/
#define C4 261.63f
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
#define C5 523.25f
#define CS5 554.37f
#define D5 587.33f
#define DS5 622.25f
#define E5 659.25f
#define F5 698.46f
#define FS5 739.99f
#define G5 783.99f
#define GS5 830.61f
#define A5 880

/**
 * ----------FUNCIONES-------------
 * Checar ganancia de attack(boolean)
 * Convertir a float
 * Guardar datos
 * FFT
 *
 * ----------VARIABLES-------------
 * Bandera para cuando esta guardando valores
 * Buffer para guardar nota
 *
 */

/** Type of structure to identify the note with the fundamental frequency */

typedef struct Notes
{
	float32 f0;
	uint8 sharp;
} DSP_note_type;

float32 DSP_digToFloat(uint16 data);

uint8 DSP_checkAttack(uint16 data);

void DSP_saveNote(uint16 data, float32 * noteBuffer);

void DSP_autocor(float32 * noteBuffer, float32 * corrBuffer);

uint16 DSP_detectPeak(float32 * corrBuffer);

float32 DSP_findPitch(uint16 pitch_period);

void DSP_findNote();

void DSP_clearBuffer(float32 * buffer);

uint8 DSP_getSavingFlag();

uint8 DSP_getAutoCorFlag();

#endif /* DSP_H_ */
