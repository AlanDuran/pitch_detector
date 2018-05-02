/**
 * \file
 * 			ADC.c
 *
 * \authors
 * 			Dario Hoyo
 *      	Alan Duran
 * \date
 * 			19/03/18
 *
 *      */

#ifndef FLEX_H_
#define FLEX_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"

#define SAMPLE_FS 96359.9985f /** Sample frequency, change every time you change mod or ps*/
#define FLEX_TIMER_0_CLOCK_GATING 0x01000000

#define FLEX_TIMER_FAULTIE  0x80
#define FLEX_TIMER_FAULTM_0   0x00
#define FLEX_TIMER_FAULTM_1   0x20
#define FLEX_TIMER_FAULTM_2   0x40
#define FLEX_TIMER_FAULTM_3   0x60
#define FLEX_TIMER_CAPTEST  0x10
#define FLEX_TIMER_PWMSYNC  0x08
#define FLEX_TIMER_WPDIS    0x04
#define FLEX_TIMER_INIT     0x02
#define FLEX_TIMER_FTMEN    0x01

#define FLEX_TIMER_TOF     0x80
#define FLEX_TIMER_TOIE    0x40
#define FLEX_TIMER_CPWMS   0x20
#define FLEX_TIMER_CLKS_0  0x00
#define FLEX_TIMER_CLKS_1  0x08
#define FLEX_TIMER_CLKS_2  0x10
#define FLEX_TIMER_CLKS_3  0x18
#define FLEX_TIMER_PS_1    0x00
#define FLEX_TIMER_PS_2    0x01
#define FLEX_TIMER_PS_4    0x02
#define FLEX_TIMER_PS_8    0x03
#define FLEX_TIMER_PS_16    0x04
#define FLEX_TIMER_PS_32    0x05
#define FLEX_TIMER_PS_64    0x06
#define FLEX_TIMER_PS_128    0x07

#define FLEX_TIMER_PWMLOAD_CH0 0x01
#define FLEX_TIMER_PWMLOAD_CH1 0x02
#define FLEX_TIMER_PWMLOAD_CH2 0x04
#define FLEX_TIMER_PWMLOAD_CH3 0x08
#define FLEX_TIMER_PWMLOAD_CH4 0x10
#define FLEX_TIMER_PWMLOAD_CH5 0x20
#define FLEX_TIMER_PWMLOAD_CH6 0x40
#define FLEX_TIMER_PWMLOAD_CH7 0x80
#define FLEX_TIMER_LDOK        0x200


#define  FLEX_TIMER_DMA   0x01
#define  FLEX_TIMER_ELSA  0x04
#define  FLEX_TIMER_ELSB  0x08
#define  FLEX_TIMER_MSA   0x10
#define  FLEX_TIMER_MSB   0x20
#define  FLEX_TIMER_CHIE  0x40
#define  FLEX_TIMER_CHF   0x80

typedef enum {WPDIS_ENABLED, WPDIS_DISABLED}FTM_WPDIS;
typedef enum {FTMEN_DISABLED, FTMEN_ENABLED}FTM_FTMEN;

typedef struct
{
	uint16 ADC_mod;
	uint16 GRAD_mod;
	uint16 CAP_mod;
	uint16 PWM_mod;
	GPIO_pinControlRegisterType ALTPIN_PWM;
	GPIO_pinControlRegisterType ALTPIN_CAP;
	FTM_WPDIS PWM_WPDIS;
	FTM_FTMEN PWM_FTMEN;
	FTM_WPDIS CAP_WPDIS;
	FTM_FTMEN CAP_FTMEN;
	uint8 PWM_CSC;
	uint8 CAP_CSC;
	uint16 PWM_CV;
	uint8 PWM_initial_val;
	uint8 PWM_SC;
	uint8 GRAD_SC;
	uint8 ADC_SC;
	uint8 CAP_SC;
	uint16 CAP_CONF;
} FLEX_ConfigType;

void FLEX_init();


/**
 * 	\brief Configures FTM3 into overflow interrupts to indicate when to take measurements from the ADC.
 *
 */
void init_adcTimer();



#endif /* FLEX_H_ */
