/**
	\file 
		SPI.h
	\brief 
		This is a starter file to use the Nokia 5510 LCD. 
		The LCD is connected as follows:
		reset-PDT0
		CE-GND
		DC-PTD3
		DIN-PTD2
		CLK-PTD1
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	1/08/2015
	\todo
	    The SPI device driver needs to be completed.
 */

#ifndef SPI_H_
#define SPI_H_
#include <MK64F12.h>
#include "DataTypeDefinitions.h"
#include "GPIO.h"


/** Constant that represent the clock enable for the SPI modules */
#define SPI0_CLOCK_GATING 0x1000
#define SPI1_CLOCK_GATING 0x2000
#define SPI2_CLOCK_GATING 0x1000

/** Constant to define the frame size in the SPI transmission */
#define SPI_FSIZE_4 SPI_CTAR_FMSZ(3)
#define SPI_FSIZE_5 SPI_CTAR_FMSZ(4)
#define SPI_FSIZE_6 SPI_CTAR_FMSZ(5)
#define SPI_FSIZE_7 SPI_CTAR_FMSZ(6)
#define SPI_FSIZE_8 SPI_CTAR_FMSZ(7)
#define SPI_FSIZE_9 SPI_CTAR_FMSZ(8)
#define SPI_FSIZE_10 SPI_CTAR_FMSZ(9)
#define SPI_FSIZE_11 SPI_CTAR_FMSZ(10) 
#define SPI_FSIZE_12 SPI_CTAR_FMSZ(11)
#define SPI_FSIZE_13 SPI_CTAR_FMSZ(12)
#define SPI_FSIZE_14 SPI_CTAR_FMSZ(13)
#define SPI_FSIZE_15 SPI_CTAR_FMSZ(14)
#define SPI_FSIZE_16 SPI_CTAR_FMSZ(15)

/** Constants to double the clock rate */
#define SPI_DOUBLE_BAUD_RATE 1
#define SPI_NON_DOUBLE_BAUD_RATE 0

/** Constant to configure the baud rate */
#define SPI_BAUD_RATE_2     0x00
#define SPI_BAUD_RATE_4     0x01
#define SPI_BAUD_RATE_6     0x02
#define SPI_BAUD_RATE_8     0x03
#define SPI_BAUD_RATE_16    0x04
#define SPI_BAUD_RATE_32    0x05
#define SPI_BAUD_RATE_64    0x06
#define SPI_BAUD_RATE_128   0x07
#define SPI_BAUD_RATE_256   0x08
#define SPI_BAUD_RATE_512   0x09
#define SPI_BAUD_RATE_1024  0x0A
#define SPI_BAUD_RATE_2048  0x0B
#define SPI_BAUD_RATE_4096  0x0C
#define SPI_BAUD_RATE_8192  0x0D

/** Constant that used to select a chip select */
#define SPI_PCSIS_0 0x100
#define SPI_PCSIS_1 0x200
#define SPI_PCSIS_2 0x400
#define SPI_PCSIS_3 0x800
#define SPI_PCSIS_4 0x1000
#define SPI_PCSIS_5 0x2000


/**Type for FIFO configuration*/
typedef enum {SPI_DISABLE_FIFO, SPI_ENABLE_FIFO} SPI_EnableFIFOType;

typedef enum {CTAR0, CTAR1, CTAR2} CTAR_channelType;
/**Type for polarity configuration*/
typedef enum {SPI_LOW_POLARITY,SPI_HIGH_POLARITY} SPI_PolarityType;
/**Type for phase configuration*/
typedef enum {SPI_LOW_PHASE,SPI_HIGH_PHASE} SPI_PhaseType;
/**Type for the star of the frame, either MSB o LSM */
typedef enum {SPI_MSB, SPI_LSM} SPI_LSMorMSBType;
/**Type for SPI modules*/
typedef enum {SPI_0,
			  SPI_1,
			  SPI_2
			  } SPI_ChannelType;
/**Type for master or slave configuration*/
typedef enum{SPI_SLAVE,SPI_MASTER} SPI_MasterType;			  
/**Type for GPIO configuration that activates the GPIO for SPI*/
typedef struct{
	GPIO_portNameType GPIO_portName;
	uint32 SPI_clk;
	uint32 SPI_Sout;
}GPIOForSPIType;
/**Type that is used for SPI configuration, It contains all the information needed for a SPI module*/
typedef struct
{
	SPI_EnableFIFOType SPI_EnableFIFO;
	SPI_PolarityType SPI_Polarity;
	SPI_PhaseType SPI_Phase;
	SPI_LSMorMSBType SPI_LSMorMSB;
	SPI_ChannelType SPI_Channel;
	SPI_MasterType SPI_Master;		
	GPIO_pinControlRegisterType pinConttrolRegisterPORTD;
	uint8 baudrate;
	uint32 frameSize;
	GPIOForSPIType GPIOForSPI;
} SPI_ConfigType;


/**it enable the clock module of the SPI by modifying the MDIS bits*/
/**
 * 	\brief Enables the clock module by modifying the MDIS bits.
 *
 * 	\param[in] channel Number of SPI to select.
 */
static void SPI_enable(SPI_ChannelType channel);
/**It activate the clock gating*/

/**
 * 	\brief Activates clock gating for given channel
 *
 *	\param[in] channel Number of SPI to select.
 */
static void SPI_clk(SPI_ChannelType channel);
/*It configure the SPI as a master or slave depending on the value of masterOrslave*/

/**
 * 	\brief Selects whether given SPI works on master mode or slave mode
 *
 * 	\param[in] channel Number of SPI to select
 * 	\param[in] masterOrSlave True or False indicating master mode or slave mode. 0 enables Slave mode, 1 enables Master mode.
 */
static void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrSlave);
/*It activate the TX and RX FIFOs of the SPI depending on the value of enableOrdisable*/
/**
 * 	\brief Activates RX and TX FIFO on given channel of the SPI.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] enableOrDisable 0 to enable FIFO, 1 to disable FIFO.
 */
static void SPI_FIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable);
/*It selects the clock polarity depending on the value of cpol*/

/**
 * 	\brief Determines the polarity of inactive value of the transfer.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] cpol 0 Inactive state is low, 1 Inactive state is high.
 */
static void SPI_clockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol);
/*It selects the frame size depending on the value of frameSize and the macros that are defined above*/

/**
 * 	\brief Changes the number of bits transfered per frame.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] frameSize Number of bits transfered per fame, macro already shifted.
 */
static void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize);
/*It selects the clock phase depending on the value of cpha*/

/**
 * 	\brief Selects the clock phase.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] cpha 0 -> Data is captured on the leading edge of SCK and changed on the following edge., 1 -> Data is changed on the leading edge of SCK and captured on the following edge.
 */

static void SPI_clockPhase(SPI_ChannelType channel, SPI_PhaseType cpha);
/*It selects the baud rate depending on the value of baudRate and the macros that are defined above*/

/**
 * 	\brief Selects the baud rate and macros.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] baudRate Macro that selects the baud rate and shifts it, so you only have to assign it.
 */
static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate);
/*It selects if MSB or LSM bits is first transmitted*/

/**
 * 	\brief Selects whether the MSB or the LSB bit is transmitted first.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] msb 0 -> Data is transferred MSB first. 1 -> Data is transferred LSB first.
 */
static void SPI_MSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb);
/*It stars the SPI transmission by modifying the value of HALT bit*/

/**
 * 	\brief Starts SPI transmission modifying the value of HALT.
 *
 * 	\param[in] channel NUMBMER of SPI to select.
 */

void SPI_startTranference(SPI_ChannelType channel);
/*It stops the SPI transmission by modifying the value of HALT bit*/

/**
 * 	\brief Stops transmission modifying the value of HALT.
 *
 * 	\param[in] channel Number of SPI to select.
 */
void SPI_stopTranference(SPI_ChannelType channel);
/*It transmits the information contained in data*/

/**
 * 	\brief Transmits one byte of information.
 *
 * 	\param[in] channel Number of SPI to select.
 * 	\param[in] Data One byte of information.
 */
void SPI_sendOneByte(SPI_ChannelType channel,uint8 Data);
/*It configures the SPI for transmission, this function as arguments receives a pointer to a constant structure where are all 
 * the configuration parameters*/

/**
 * 	\brief Initializes necessary values for SPI communication
 *
 * 	\param[in] SPI_Config Structure with configurations.
 */
void SPI_init(const SPI_ConfigType* SPI_Config);



#endif /* SPI_H_ */
