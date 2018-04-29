/*
 * DSP.h
 *
 *  Created on: Apr 28, 2018
 *      Author: kanta
 */

#ifndef DSP_H_
#define DSP_H_

#include "DataTypeDefinitions.h"

#define ATTACK_TRESH 2.8f

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

uint8 DSP_getSavingFlag();

#endif /* DSP_H_ */
