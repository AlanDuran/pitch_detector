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

#define MAX_SAMPLES 2500

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

void DSP_saveNote(uint16 data);

void DSP_autocor();

uint16 DSP_detectPeak(void);

float32 DSP_findPitch(uint16 pitch_period);

void DSP_clearBuffer(void);

uint8 DSP_getSavingFlag();

uint8 DSP_getAutoCorFlag();

void DSP_setStatus(uint8 status_no);

void DSP_clearStatus(uint8 status_no);

uint16 DSP_getGeneralStatus();

uint8 DSP_getBuffNo();

uint8 DSP_getCorBuffNo();

void DSP_setNewAttackThresh(uint16 data);

#endif /* DSP_H_ */
