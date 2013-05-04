
/*
  ____        _     _           ____        _           _		 _          _
 / ___|  ___ | | __| | ___ _ __/ ___| _ __ | | __ _ ___| |__	| |    __ _| |__  ___
 \___ \ / _ \| |/ _` |/ _ \ '__\___ \| '_ \| |/ _` / __| '_ \	| |   / _` | '_ \/ __|
  ___) | (_) | | (_| |  __/ |   ___) | |_) | | (_| \__ \ | | |	| |__| (_| | |_) \__ \
 |____/ \___/|_|\__,_|\___|_|  |____/| .__/|_|\__,_|___/_| |_|	|_____\__,_|_.__/|___/
                                     |_|
 (C)SolderSplash Labs 2013 - www.soldersplash.co.uk - C. Matthews

	@file     main.c
	@author   Carl Matthews (soldersplash.co.uk)
	@date     01 May 2013

	@section DESCRIPTION

	Simple example that shows you how to : toggle an IO using a timed interrupt
	and showing semihosting, letting you printf to the console.

	To enable Semihosting when using a Jtag
	- goto project properties
	- C/C++ Build > Settings
	- Select Target under MCU Linker in the tool settings tree
	- Change C library to Redlib (semihost)
	NOTE : 	Semihosting will cause code to stop when not under jtag control
			(It can not run standalone)

	Alternative method : Quickstart Panel -> Quick Settings -> Set library type Redlib (semihost)
*/

#ifdef __USE_CMSIS
#include "LPC13Uxx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include <stdio.h>


/**************************************************************************/
/*!
    @brief SysTick_Handler - interrupt
*/
/**************************************************************************/
void SysTick_Handler(void)
{
static uint32_t msCounter = 0;

	msCounter ++;
	if ( msCounter > 1000 )
	{
		// Toggle the output
		LPC_GPIO->NOT[0] = 1<<13;

		printf("Pin Toggled\n");

		// Clear counter
		msCounter = 0;
	}
}


/**************************************************************************/
/*!
    @brief main - C Entry point
*/
/**************************************************************************/
int main(void)
{
	// Set up the System Tick for a 1ms interrupt
	SysTick_Config(SystemCoreClock / 1000);
	
	// Enable GPIO Clock ( powers the GPIO periperal )
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	// Select GPIO Mode and disable analog mode, refer to User Manual - UM10524
	LPC_IOCON->TDO_PIO0_13 = (1 | 1<<7);

	// Set the pin direction, set high for an output
	LPC_GPIO->DIR[0] |= 1<<13;

	while(1)
	{
		// do nothing, Wait for an interrupt
		__WFI();
	}

	return 0;
}
