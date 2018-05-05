/**
 * \file
 * 		ADC.h
 *
 * \brief
 * 		This is a driver for the internal ADC of a K64F board
 *
 * \authors
 *  	Dario Hoyo
 *     	Alan Duran
 *
 * \date
 * 		20/03/2018
 *
 *      */

#ifndef ADC_H_
#define ADC_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"

/**Type for interrupt enable or disable configuration*/
typedef enum {INTERRUPT_DISABLE,INTERRUPT_ENABLE} ADC_Interrupt;
/**Type for differential or single mode configuration*/
typedef enum {DIFFERENTIAL_DISABLE,DIFFERENTIAL_ENABLE} ADC_Differential;
/**Type for divide ratio configuration*/
typedef enum {DR_0,DR_1,DR_2,DR_3} ADC_divideRatio;
/**Type for the resolution of ADC conversion*/
typedef enum {MODE_8_BIT,MODE_10_BIT,MODE_12_BIT,MODE_16_BIT} ADC_mode;
/**Type for select the input clock source to generate the internal clock*/
typedef enum {BUS,ALTCLK2,ALTCLK,ADACK} ADC_clockSelect;
/**Type for determine how many ADC conversions will be averaged to create the ADC average result*/
typedef enum {SAMPLE_4,SAMPLE_8,SAMPLE_16,SAMPLE_32} ADC_averageSelect;
/**Type for Enable continuous conversions. */
typedef enum {CC_DISABLE,CC_ENABLE} ADC_continousConversion;
/**Type for enable the hardware average function of the ADC. */
typedef enum {AVERAGE_DISABLE,AVERAGE_ENABLE} ADC_averageEnable;

/**Type that is used for ADC configuration, It contains all the information needed for our ADC module*/
typedef struct
{
	ADC_Interrupt interrupt;
	ADC_Differential differential;
	ADC_mode mode;
	ADC_clockSelect clockSelect;
	uint16 channel;
	ADC_averageSelect nSamples;
	ADC_continousConversion cc;
	ADC_averageEnable averageEnable;
} ADC_ConfigType;

void ADC0_IRQHandler();

/**
 * 	 \brief	 This function configures the ADC0 for reading, this function as arguments
 * 	 	 	 receives a pointer to a constant structure where are all the configuration
 *	 	 	 parameters.
 *
 *	 @param[in]  ADC_ConfigType*  ADC_Config constant structure which contains all the
 *	 	 	 	 	 	 	 	 	 	 	 configurations for the ADC
 */
void ADC0_init(const ADC_ConfigType* ADC_Config);

/**
 * 	\brief Enables clock gating for ADC0.
 */

void ADC0_clockGating(void);

/**
 * 	\brief This function writes the values directly into the SC1, CFG1 and S3 registers
 * 			that are specified in the structure for a desired configuration mode.
 *
 * 	\param[in] ADC_Config Constant structure which contains all the
 *	 	 	 	 	 	 	 	 	 	 	 configurations for the SPI
 */

void setRegisters(const ADC_ConfigType* ADC_Config);

/**
 * 	\brief Once a conversion is finished, this functions returns the read value
 *
 * 	\return Mask of the first 16 bits of the R register
 */
uint16 ADC0_readValue();

/**
 * 	\brief Returns true when the ADC is ready to be read
 *
 * 	\return Mask of the COCO flag in SC1
 */
uint8 ADC0_conversionComplete();

/**
 * 	\brief Starts a conversion by modifying the SC1 register
 *
 * 	\param[in] channel Indicates where to read
 */
void startConversion(uint8 channel);

void DSP_clearSC1();

#endif /* PIT_H_ */
