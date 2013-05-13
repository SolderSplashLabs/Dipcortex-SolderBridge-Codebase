/*
  ____        _     _           ____        _           _		 _          _
 / ___|  ___ | | __| | ___ _ __/ ___| _ __ | | __ _ ___| |__	| |    __ _| |__  ___
 \___ \ / _ \| |/ _` |/ _ \ '__\___ \| '_ \| |/ _` / __| '_ \	| |   / _` | '_ \/ __|
  ___) | (_) | | (_| |  __/ |   ___) | |_) | | (_| \__ \ | | |	| |__| (_| | |_) \__ \
 |____/ \___/|_|\__,_|\___|_|  |____/| .__/|_|\__,_|___/_| |_|	|_____\__,_|_.__/|___/
                                     |_|
 (C)SolderSplash Labs 2013 - www.soldersplash.co.uk - C. Matthews - R. Steel


	@file     main.c
	@author   Carl Matthews (soldersplash.co.uk)
	@date     01 May 2013

    @section LICENSE

	Software License Agreement (BSD License)

    Copyright (c) 2013, C. Matthews - R. Steel (soldersplash.co.uk)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


	@section DESCRIPTION

	Uses 6(2x3) buttons to give 5 levels demand on two 0-10 channels, voltage between set points is ramped at 1v per second
	2 additional channels are used to provide a stepped output (not ramped)

*/

#include "SolderSplashLpc.h"
#include "PwmControl.h"
#include "ButtonCon.h"

#define _MAIN_
#include "main.h"

// ------------------------------------------------------------------------------------------------------------
/*!
    @brief SysTick_Handler - Timer interrupt
*/
// ------------------------------------------------------------------------------------------------------------
void SysTick_Handler(void)
{
	actionButtons = true;
}

// ------------------------------------------------------------------------------------------------------------
/*!
    @brief 	SetDimLevels - each push button demands a set dim level from an output
    		Changes are ramped to instead of jumped.

Setting | Input 0 | Input 1 | 	Input 2 | 	Output 0
   1		0			0			0			0V
   2		1			0			0			2.5V
   3		0			1			0			5V
   4		1			1			0			7.5V
   5		0			0			1			10V
   6		1			0			1			10V
   7		0			1			1			10V
   8		1			1			1			10V

*/
// ------------------------------------------------------------------------------------------------------------
void SetDimLevels ( void )
{
static uint16_t bottomOutput = 320;
static uint16_t topOutput = 320;
uint8_t buttons;
uint8_t bottomButtons;
uint8_t topButtons;
uint16_t bottomOutputTarget = 0;
uint16_t topOutputTarget = 0;

	Buttons_GetPrevState( &buttons );

	// first we only want the bottom 3 bits/buttons
	bottomButtons = buttons & (BIT0 | BIT1 | BIT2);

	// next we only want the top 3 bits/buttons
	topButtons = buttons & (BIT3 | BIT4 | BIT5);
	topButtons >>= 3;

	// range check & limit
	if ( bottomButtons > MAX_DIM_LEVELS-1) bottomButtons = MAX_DIM_LEVELS-1;
	if ( topButtons > MAX_DIM_LEVELS-1) topButtons = MAX_DIM_LEVELS-1;

	// Grab the pwm level from the array for this setting
	bottomOutputTarget = DIM_LEVELS[bottomButtons];
	topOutputTarget = DIM_LEVELS[topButtons];

	// Outputs 0 & 1 ramp towards target, linearly

	if ( bottomOutput > bottomOutputTarget )
	{
		bottomOutput --;
	}
	else if ( bottomOutput < bottomOutputTarget )
	{
		bottomOutput ++;
	}

	Pwm_SetDuty(BIT0, bottomOutput);

	if ( topOutput > topOutputTarget )
	{
		topOutput --;
	}
	else if ( topOutput < topOutputTarget )
	{
		topOutput ++;
	}

	Pwm_SetDuty(BIT1, topOutput);

	// Outputs 2 & 3 don't ramp. They just jump to the target

	Pwm_SetDuty(BIT2, bottomOutputTarget);
	Pwm_SetDuty(BIT3, topOutputTarget);

	Pwm_SetDuty(BIT4, bottomOutputTarget);
	Pwm_SetDuty(BIT5, topOutputTarget);
}


// ------------------------------------------------------------------------------------------------------------
/*!
    @brief main - C Entry point
*/
// ------------------------------------------------------------------------------------------------------------
int main(void)
{
	// Set up the System Tick, 33.3ms tick gives us a 1v per second linear ramp
	SysTick_Config(SystemCoreClock / 30);

	LpcLowPowerIoInit();

	Buttons_Init();

	Pwm_Init();
	Pwm_On();

	// Led, PIO mode, no pull up/down, open drain
	LPC_IOCON->PIO0_23 = (1<<7 | 1<<10);
	LPC_GPIO->DIR[0] |= (1 << 23);

	while(1)
	{
		// The System tick set
		if ( actionButtons )
		{
			Buttons_Task();
			//DimmingControl();
			SetDimLevels();
			actionButtons = false;
		}
	}

	return 0;
}
