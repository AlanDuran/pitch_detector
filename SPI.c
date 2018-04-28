/**
 * \file
 * 			SPI.c
 *
 * \authors
 * 			Dario Hoyo
 *      	Alan Duran
 * \date
 * 			22/03/18
 *
 *      */

#include "SPI.h"

void SPI_init(const SPI_ConfigType* SPI_Config)
{
	/**
	 * 	Functions with parameters from structure with configurations.
	 */
	SPI_clk(SPI_Config->SPI_Channel);
	GPIO_clockGating(SPI_Config->GPIOForSPI.GPIO_portName);
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName, SPI_Config->GPIOForSPI.SPI_clk, &(SPI_Config->pinConttrolRegisterPORTD));
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName, SPI_Config->GPIOForSPI.SPI_Sout, &(SPI_Config->pinConttrolRegisterPORTD));
	SPI_setMaster(SPI_Config->SPI_Channel, SPI_Config->SPI_Master);
	SPI_enable(SPI_Config->SPI_Channel);
	SPI_FIFO(SPI_Config->SPI_Channel, SPI_Config->SPI_EnableFIFO);
	SPI_clockPolarity(SPI_Config->SPI_Channel, SPI_Config->SPI_Polarity);
	SPI_frameSize(SPI_Config->SPI_Channel, SPI_Config->frameSize);
	SPI_clockPhase(SPI_Config->SPI_Channel, SPI_Config->SPI_Phase);
	SPI_baudRate(SPI_Config->SPI_Channel, SPI_Config->baudrate);
	SPI_MSBFirst(SPI_Config->SPI_Channel, SPI_MSB);
}

/*It activate the clock gating*/
static void SPI_clk(SPI_ChannelType channel)
{
	/**
	 * 	Activates clock gating for its respective number of SPI with masks.
	 */
	switch(channel)
	{
	case SPI_0:
		SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;
		break;
	case SPI_1:
		SIM->SCGC6 |= SIM_SCGC6_SPI1_MASK;
		break;
	case SPI_2:
		SIM->SCGC6 |= SIM_SCGC3_SPI2_MASK;
		break;
	default:
		break;
	}
}

/*it enable the clock module of the SPI by modifying the MDIS bits*/
static void SPI_enable(SPI_ChannelType channel)
{
	/**
	 * 	Clears the bit on MDIS to enable the clock modules with a switch with the variable channel
	 * 	that indicates the number of SPI
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR &= ~(SPI_MCR_MDIS_MASK);
		SPI0->MCR &= ~(SPI_MCR_FRZ_MASK);
		break;
	case SPI_1:
		SPI1->MCR &= ~(SPI_MCR_MDIS_MASK);
		SPI0->MCR &= ~(SPI_MCR_FRZ_MASK);
		break;
	case SPI_2:
		SPI2->MCR &= ~(SPI_MCR_MDIS_MASK);
		SPI0->MCR &= ~(SPI_MCR_FRZ_MASK);
		break;
	default:
		break;
	}
}

/*It configure the SPI as a master or slave depending on the value of masterOrslave*/
static void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrSlave)
{
	switch(channel)
	{
	/**
	 * Clears the position of the mask (in case var is 0) and then asigns whatever is on the variable.
	 */
	case SPI_0:
		SPI0->MCR &= ~(SPI_MCR_MSTR_MASK);
		SPI0->MCR |= masterOrSlave << SPI_MCR_MSTR_SHIFT;
		break;
	case SPI_1:
		SPI1->MCR &= ~(SPI_MCR_MSTR_MASK);
		SPI1->MCR |= masterOrSlave << SPI_MCR_MSTR_SHIFT;
		break;
	case SPI_2:
		SPI2->MCR &= ~(SPI_MCR_MSTR_MASK);
		SPI2->MCR |= masterOrSlave << SPI_MCR_MSTR_SHIFT;
		break;
	default:
		break;
	}
}
/*It activate the TX and RX FIFOs of the SPI depending on the value of enableOrdisable*/
static void SPI_FIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable)
{
	/**
	 * 	With a switch with the SPI channel we enable or disable TX and RX FIFOs with a macro which chooses the value and shifts the position
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR |= SPI_MCR_DIS_RXF(enableOrDisable);
		SPI0->MCR |= SPI_MCR_DIS_TXF(enableOrDisable);
		break;
	case SPI_1:
		SPI1->MCR |= SPI_MCR_DIS_RXF(enableOrDisable);
		SPI1->MCR |= SPI_MCR_DIS_TXF(enableOrDisable);
		break;
	case SPI_2:
		SPI2->MCR |= SPI_MCR_DIS_RXF(enableOrDisable);
		SPI2->MCR |= SPI_MCR_DIS_TXF(enableOrDisable);
		break;
	default:
		break;
	}
}
/*It selects the clock polarity depending on the value of cpol*/
static void SPI_clockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol)
{

	/**
	 * Clears the position of the mask (in case var is 0) and then asigns whatever is on the variable.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[CTAR0] &= ~(SPI_CTAR_CPOL_MASK);
		SPI0->CTAR[CTAR0] |= cpol << SPI_CTAR_CPOL_SHIFT;
		break;
	case SPI_1:
		SPI1->CTAR[CTAR0] &= ~(SPI_CTAR_CPOL_MASK);
		SPI1->CTAR[CTAR0] |= cpol << SPI_CTAR_CPOL_SHIFT;
		break;
	case SPI_2:
		SPI2->CTAR[CTAR0] &= ~(SPI_CTAR_CPOL_MASK);
		SPI2->CTAR[CTAR0] |= cpol << SPI_CTAR_CPOL_SHIFT;
		break;
	default:
		break;
	}
}
/*It selects the frame size depending on the value of frameSize and the macros that are defined above*/
static void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize)
{
	/**
	 * 	We clear the position of frame size in case it is 0 and then we assign the value in the parameter, since it was assigned by a macro before
	 * 	so it's already shifted.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[CTAR0] &= ~(SPI_CTAR_FMSZ_MASK);
		SPI0->CTAR[CTAR0] |= frameSize;
		break;
	case SPI_1:
		SPI1->CTAR[CTAR0] &= ~(SPI_CTAR_FMSZ_MASK);
		SPI1->CTAR[CTAR0] |= frameSize;
		break;
	case SPI_2:
		SPI2->CTAR[CTAR0] &= ~(SPI_CTAR_FMSZ_MASK);
		SPI2->CTAR[CTAR0] |= frameSize;
		break;
	default:
		break;
	}
}
/*It selects the clock phase depending on the value of cpha*/
static void SPI_clockPhase(SPI_ChannelType channel, SPI_PhaseType cpha)
{
	/**
	 * Clears the position of the mask (in case var is 0) and then asigns whatever is on the variable.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[CTAR0] &= ~(SPI_CTAR_CPHA_MASK);
		SPI0->CTAR[CTAR0] |= cpha << SPI_CTAR_CPHA_SHIFT;
		break;
	case SPI_1:
		SPI1->CTAR[CTAR0] &= ~(SPI_CTAR_CPHA_MASK);
		SPI1->CTAR[CTAR0] |= cpha << SPI_CTAR_CPHA_SHIFT;
		break;
	case SPI_2:
		SPI2->CTAR[CTAR0] &= ~(SPI_CTAR_CPHA_MASK);
		SPI2->CTAR[CTAR0] |= cpha << SPI_CTAR_CPHA_SHIFT;
		break;
	default:
		break;
	}
}
/*It selects the baud rate depending on the value of baudRate and the macros that are defined above*/
static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate)
{
	/**
	 * 	We clear the position of frame size in case it is 0 and then we assign the value in the parameter, since it was assigned by a macro before
	 * 	so it's already shifted.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[CTAR0] &= ~(SPI_CTAR_BR_MASK);
		SPI0->CTAR[CTAR0] |= baudRate;
		break;
	case SPI_1:
		SPI1->CTAR[CTAR0] &= ~(SPI_CTAR_BR_MASK);
		SPI1->CTAR[CTAR0] |= baudRate;
		break;
	case SPI_2:
		SPI2->CTAR[CTAR0] &= ~(SPI_CTAR_BR_MASK);
		SPI2->CTAR[CTAR0] |= baudRate;
		break;
	default:
		break;
	}
}
/*It selects if MSB or LSM bits is first transmitted*/
static void SPI_MSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb)
{
	/**
	 * Clears the position of the mask (in case var is 0) and then asigns whatever is on the variable.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->CTAR[CTAR0] &= ~(SPI_CTAR_LSBFE_MASK);
		SPI0->CTAR[CTAR0] |= msb << SPI_CTAR_LSBFE_SHIFT;
		break;
	case SPI_1:
		SPI1->CTAR[CTAR0] &= ~(SPI_CTAR_LSBFE_MASK);
		SPI1->CTAR[CTAR0] |= msb << SPI_CTAR_LSBFE_SHIFT;
		break;
	case SPI_2:
		SPI2->CTAR[CTAR0] &= ~(SPI_CTAR_LSBFE_MASK);
		SPI2->CTAR[CTAR0] |= msb << SPI_CTAR_LSBFE_SHIFT;
		break;
	default:
		break;
	}
}
/*It stars the SPI transmission by modifying the value of HALT bit*/
void SPI_startTranference(SPI_ChannelType channel)
{
	/**
	 * Clears the the bit in HALT to start the transference.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR &= ~(SPI_MCR_HALT_MASK);
		break;
	case SPI_1:
		SPI1->MCR &= ~(SPI_MCR_HALT_MASK);
		break;
	case SPI_2:
		SPI2->MCR &= ~(SPI_MCR_HALT_MASK);
		break;
	default:
		break;
	}
}
/*It stops the SPI transmission by modifying the value of HALT bit*/
void SPI_stopTranference(SPI_ChannelType channel)
{
	/**
	 * Asigns the bit in HALT to stop the transference.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->MCR |= SPI_MCR_HALT_MASK;
		break;
	case SPI_1:
		SPI1->MCR |= SPI_MCR_HALT_MASK;
		break;
	case SPI_2:
		SPI2->MCR |= SPI_MCR_HALT_MASK;
		break;
	default:
		break;
	}
}
/*It transmits the information contained in data*/
void SPI_sendOneByte(SPI_ChannelType channel, uint8 Data)
{
	/**
	 * Assings The Data in PUSHR and then waits for the transference flag to activate, then clears it by asigning it again.
	 */
	switch(channel)
	{
	case SPI_0:
		SPI0->PUSHR = Data;
		while (FALSE == (SPI0->SR & SPI_SR_TCF_MASK));
		SPI0->SR |= SPI_SR_TCF_MASK;
		break;
	case SPI_1:
		SPI1->PUSHR = Data;
		while (FALSE == (SPI1->SR & SPI_SR_TCF_MASK));
		SPI1->SR |= SPI_SR_TCF_MASK;
		break;
	case SPI_2:
		SPI2->PUSHR = Data;
		while (FALSE == (SPI2->SR & SPI_SR_TCF_MASK));
		SPI2->SR |= SPI_SR_TCF_MASK;
		break;
	default:
		break;
	}
}
/*It configures the SPI for transmission, this function as arguments receives a pointer to a constant structure where are all
 * the configuration parameters*/

