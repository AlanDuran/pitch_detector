/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author Dario Hoyo y Alan Duran
	\date	7/09/2014
	\todo
	    Interrupts are not implemented in this API implementation.
 */

#include "MK64F12.h"
#include "GPIO.h"


#define MASK 1
#define SYSTEM_CLOCK 21000000
#define DELAY 0.03F

static GPIO_interruptFlags_t GPIO_intrStatusFlag;

void PORTC_IRQHandler()
{
	///Enables flag so interrupt can be detected elsewhere.
	GPIO_intrStatusFlag.flagPortC = TRUE;
	GPIO_clearInterrupt(GPIO_C);
}

uint8 GPIO_getIRQStatus(GPIO_portNameType gpio)
{
	///Returns the flag of the corresponding port.
	switch (gpio) {
		case GPIO_A:
			return(GPIO_intrStatusFlag.flagPortA);
			break;
		case GPIO_B:
			return(GPIO_intrStatusFlag.flagPortB);
			break;
		case GPIO_C:
			return(GPIO_intrStatusFlag.flagPortC);
			break;
		case GPIO_D:
			return(GPIO_intrStatusFlag.flagPortD);
			break;
		case GPIO_E:
			return(GPIO_intrStatusFlag.flagPortE);
			break;
		default:
			return(ERROR);
			break;
	}

}

uint8 GPIO_clearIRQStatus(GPIO_portNameType gpio)
{
	///Clears the flag of the corresponding port.
	switch (gpio) {
		case GPIO_A:
			GPIO_intrStatusFlag.flagPortA = FALSE;
			break;
		case GPIO_B:
			GPIO_intrStatusFlag.flagPortB = FALSE;
			break;
		case GPIO_C:
			GPIO_intrStatusFlag.flagPortC = FALSE;
			break;
		case GPIO_D:
			GPIO_intrStatusFlag.flagPortD = FALSE;
			break;
		case GPIO_E:
			GPIO_intrStatusFlag.flagPortE = FALSE;
			break;
		default:
			return(ERROR);
			break;
	}

	return(TRUE);

}


void GPIO_clearInterrupt(GPIO_portNameType portName)
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}
uint8 GPIO_clockGating(GPIO_portNameType portName)
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8 GPIO_pinControlRegister(GPIO_portNameType portName,uint8 pin,const GPIO_pinControlRegisterType*  pinControlRegister)
{

	switch(portName)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pinControlRegister;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_writePORT(GPIO_portNameType portName, uint32 Data )
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			GPIOA->PDOR = Data;
			break;
		case GPIO_B: /** GPIO B is selected*/
			GPIOB->PDOR = Data;
			break;
		case GPIO_C: /** GPIO C is selected*/
			GPIOC->PDOR = Data;
			break;
		case GPIO_D: /** GPIO D is selected*/
			GPIOD->PDOR = Data;
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDOR = Data;
			break;
		default:
			break;
	}// end switch
}
uint32 GPIO_readPORT(GPIO_portNameType portName)
{
	///Reads corresponding port through pdir.
	switch (portName) {
		case GPIO_A:
			return(GPIOA->PDIR);
			break;
		case GPIO_B:
			return(GPIOB->PDIR);
			break;
		case GPIO_C:
			return(GPIOC->PDIR);
			break;
		case GPIO_D:
			return(GPIOD->PDIR);
			break;
		case GPIO_E:
			return(GPIOE->PDIR);
			break;
		default:
			break;
	}
	return ERROR;
}
uint8 GPIO_readPIN(GPIO_portNameType portName, uint8 pin)
{
	///Reads corresponding port and shifts it pin times to the right (so it ends up in the first position)
	///and masks it so there is no more thrash in the return value.
	switch (portName) {
		case GPIO_A:
			return (GPIOA->PDIR >> pin) & MASK;
			break;
		case GPIO_B:
			return (GPIOB->PDIR >> pin) & MASK;
			break;
		case GPIO_C:
			return (GPIOC->PDIR >> pin) & MASK;
			break;
		case GPIO_D:
			return (GPIOD->PDIR >> pin) & MASK;
			break;
		case GPIO_E:
			return (GPIOE->PDIR >> pin) & MASK;
			break;
		default:
			break;
	}
	return ERROR;
}
void GPIO_setPIN(GPIO_portNameType portName, uint8 pin)
{
	///Sets pin by shifting a 1 pin times and or(ing) it in psor
	switch (portName) {
		case GPIO_A:
			GPIOA->PSOR |= TRUE << pin;
			break;
		case GPIO_B:
			GPIOB->PSOR |= TRUE << pin;
			break;
		case GPIO_C:
			GPIOC->PSOR |= TRUE << pin;
			break;
		case GPIO_D:
			GPIOD->PSOR |= TRUE << pin;
			break;
		case GPIO_E:
			GPIOE->PSOR |= TRUE << pin;
			break;
		default:
			break;
	}
}
void GPIO_clearPIN(GPIO_portNameType portName, uint8 pin)
{
	///clears pin shifting a one and or(ing) it to pcor
	switch (portName) {
		case GPIO_A:
			GPIOA->PCOR |= TRUE << pin;
			break;
		case GPIO_B:
			GPIOB->PCOR |= TRUE << pin;
			break;
		case GPIO_C:
			GPIOC->PCOR |= TRUE << pin;
			break;
		case GPIO_D:
			GPIOD->PCOR |= TRUE << pin;
			break;
		case GPIO_E:
			GPIOE->PCOR |= TRUE << pin;
			break;
		default:
			break;
	}
}
void GPIO_tooglePIN(GPIO_portNameType portName, uint8 pin)
{
	///Toogles pin by shifting a one pin times and or(ing) it to ptor.
	switch (portName) {
		case GPIO_A:
			GPIOA->PTOR |= TRUE << pin;
			break;
		case GPIO_B:
			GPIOB->PTOR |= TRUE << pin;
			break;
		case GPIO_C:
			GPIOC->PTOR |= TRUE << pin;
			break;
		case GPIO_D:
			GPIOD->PTOR |= TRUE << pin;
			break;
		case GPIO_E:
			GPIOE->PTOR |= TRUE << pin;
			break;
		default:
			break;
	}
}
void GPIO_dataDirectionPORT(GPIO_portNameType portName ,uint32 direction)
{
	///Sets direction, 1 -> output, 0 -> input to pddr.
	switch (portName) {
		case GPIO_A:
			GPIOA->PDDR = direction;
			break;
		case GPIO_B:
			GPIOB->PDDR = direction;
			break;
		case GPIO_C:
			GPIOC->PDDR = direction;
			break;
		case GPIO_D:
			GPIOD->PDDR = direction;
			break;
		case GPIO_E:
			GPIOE->PDDR = direction;
			break;
		default:
			break;
	}
}
void GPIO_dataDirectionPIN(GPIO_portNameType portName, uint8 State, uint8 pin)
{
	///Sets direction, 1 -> output, 0 -> input to pddr, by first shifting a 0 pin times and and(ing) it to pddr
	///and then shifting state (either 1 or 0) and oring it in pddr.
	switch (portName) {
		case GPIO_A:
			GPIOA->PDDR &= ~(MASK << pin);
			GPIOA->PDDR |= (State << pin);
			break;
		case GPIO_B:
			GPIOB->PDDR &= ~(MASK << pin);
			GPIOB->PDDR |= (State << pin);
			break;
		case GPIO_C:
			GPIOC->PDDR &= ~(MASK << pin);
			GPIOC->PDDR |= (State << pin);
			break;
		case GPIO_D:
			GPIOD->PDDR &= ~(MASK << pin);
			GPIOD->PDDR |= (State << pin);
			break;
		case GPIO_E:
			GPIOE->PDDR &= ~(MASK << pin);
			GPIOE->PDDR |= (State << pin);
			break;
		default:
			break;
	}
}


