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

float32 DSP_digToFloat(uint16 data);

uint8 DSP_checkAttack(uint16 data);

void DSP_saveNote(uint16 data, float32 * noteBuffer);

void DSP_autocor(float32 * noteBuffer, float32 * corrBuffer);

uint16 DSP_detectPeak(float32 * corrBuffer);

float32 DSP_findPitch(uint16 pitch_period);

uint8 DSP_getSavingFlag();

#endif /* DSP_H_ */
