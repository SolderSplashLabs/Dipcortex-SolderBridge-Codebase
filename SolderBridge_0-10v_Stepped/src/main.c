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
    @brief 	SetDimLevels - each input demands a set dim level from an output
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
    @brief 	SetDimLevels - each input transition, high to low demands a set dim level from an output
    		Changes are ramped to instead of jumped.

	Input 0 = 0v
	Input 1 = 2.5v
	Input 2 = 5v
	Input 3 = 7.5v
	Input 4 = 10v

*/
// ------------------------------------------------------------------------------------------------------------
void SetDimLevels2 ( void )
{
static uint16_t bottomOutput = MAX_DIM_DUTY;
static uint16_t topOutput = MAX_DIM_DUTY;
static uint16_t bottomOutputTarget = MAX_DIM_DUTY;
static uint16_t topOutputTarget = MAX_DIM_DUTY;
uint8_t buttons;

	// First we action any button presses
	// These update the taget output voltage

	Buttons_GetPressed( &buttons );
	Buttons_ActionPressed();

	if ( buttons & BIT0 )
	{
		bottomOutputTarget = DIM_LEVELS[0];
		topOutputTarget = DIM_LEVELS[0];
	}
	else if ( buttons & BIT1 )
	{
		bottomOutputTarget = DIM_LEVELS[1];
		topOutputTarget = DIM_LEVELS[1];
	}
	else if ( buttons & BIT2 )
	{
		bottomOutputTarget = DIM_LEVELS[2];
		topOutputTarget = DIM_LEVELS[2];
	}
	else if ( buttons & BIT3 )
	{
		bottomOutputTarget = DIM_LEVELS[3];
		topOutputTarget = DIM_LEVELS[3];
	}
	else if ( buttons & BIT4 )
	{
		bottomOutputTarget = DIM_LEVELS[4];
		topOutputTarget = DIM_LEVELS[4];
	}

	// The next Half will handle getting the outputs to the target
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
    @brief Dimmer - Each input controls a single output, press and hold the input to dim up and dim down
*/
// ------------------------------------------------------------------------------------------------------------
void Dimmer ( void )
{
uint8_t i = 0;
uint8_t mask = 0x01;
uint8_t buttonsHeld = 0;
uint8_t buttonsPressed = 0;

	Buttons_GetHeld(&buttonsHeld);
	Buttons_GetPressed(&buttonsPressed);
	Buttons_ActionPressed();

	for (i=0; i<BUTTON_NO; i++)
	{
		if (buttonsHeld & mask)
		{
			if (! DimmingOutputs[i].hasStarted )
			{
				// We have just had our first held button
				DimmingOutputs[i].hasStarted = true;

				if ( DimmingOutputs[i].goingUp )
				{
					DimmingOutputs[i].goingUp = false;
					DimmingOutputs[i].goingDown = true;
				}
				else
				{
					DimmingOutputs[i].goingUp = true;
					DimmingOutputs[i].goingDown = false;
				}
			}

			if ( DimmingOutputs[i].goingUp )
			{
				// Decreasing the pwm duty value increases the output voltage
				Pwm_DutyDecrease(mask, MIN_DIM_DUTY);
			}
			else
			{
				// Increasing the pwm duty value Decreases the output voltage
				Pwm_DutyIncrease(mask, MAX_DIM_DUTY);
			}
		}
		else if (buttonsPressed & mask)
		{
			// a press has occurred on this input
		}
		else
		{
			DimmingOutputs[i].hasStarted = false;
		}

		// Next button
		mask <<= 1;
	}

}

// ------------------------------------------------------------------------------------------------------------
/*!
    @brief UpDownStepper - use inputs in pairs first adds to the output voltage, next takes away
*/
// ------------------------------------------------------------------------------------------------------------
void UpDownStepper ( void )
{
uint8_t i = 0;
uint8_t output = 0;
uint8_t mask = 0x01;
uint8_t buttonsHeld = 0;
uint8_t buttonsPressed = 0;

	Buttons_GetHeld(&buttonsHeld);
	Buttons_GetPressed(&buttonsPressed);
	Buttons_ActionPressed();

	for (i=0; i<BUTTON_NO; i++)
	{
		if (buttonsHeld & mask)
		{
			// ignore
		}
		else if (buttonsPressed & mask)
		{
			// a press has occurred on this input, work out which output it affects
			// 2 inputs control each output
			output = i / 2;

			// if this is an odd input bit0 is set
			if ( i & 0x01 )
			{
				// we decrease the voltage by the step size if possible
				// increasing the PWM decreases the voltage
				DimmingOutputs[output].targetDuty += DIM_STEP;

				// limit to the max
				if ( DimmingOutputs[output].targetDuty > MAX_DIM_DUTY)
				{
					DimmingOutputs[output].targetDuty = MAX_DIM_DUTY;
				}
			}
			else
			{
				// increase the voltage by the step size if possible
				// decreasing the PWM increases the voltage

				if ( DimmingOutputs[output].targetDuty >= DIM_STEP)
				{
					DimmingOutputs[output].targetDuty -= DIM_STEP;
				}
				else
				{
					DimmingOutputs[output].targetDuty = MIN_DIM_DUTY;
				}
			}
		}
		else
		{
			// nothing
		}

		// Update the duty of this output to move it towards the target
		// It might already be there,


		// Next button
		mask <<= 1;
	}

	// Now loop around each out and step them closer to their targets
	mask = 0x01;
	for (i=0; i<PWM_NO_OF; i++)
	{
		Pwm_DutyStep(mask, DimmingOutputs[i].targetDuty);

		mask <<= 1;
	}
}


// ------------------------------------------------------------------------------------------------------------
/*!
    @brief main - C Entry point
*/
// ------------------------------------------------------------------------------------------------------------
int main(void)
{
uint8_t i = 0;

	// Set up the System Tick, 33.3ms tick gives us a 1v per second linear ramp
	SysTick_Config(SystemCoreClock / 30);

	LpcLowPowerIoInit();

	Buttons_Init();

	Pwm_Init();
	Pwm_On();

	// Led, PIO mode, no pull up/down, open drain
	LPC_IOCON->PIO0_23 = (1<<7 | 1<<10);
	LPC_GPIO->DIR[0] |= (1 << 23);

	// Initalise target duty, used in UpDownStepped Mode
	for (i=0; i<PWM_NO_OF; i++)
	{
		DimmingOutputs[i].targetDuty = MAX_DIM_DUTY;
	}

	while(1)
	{
		// The System tick set
		if ( actionButtons )
		{
			actionButtons = false;

			// Call the button module and tell it how often we are calling it
			Buttons_Task(33);

			// Use inputs as dip switches to select the voltage in large steps
			//SetDimLevels();

			// Use inputs as buttons, each press triggers a set voltage output
			SetDimLevels2();

			// Each button dims up and down it's associated output
			//Dimmer();

			// use inputs in pairs, first adds a step to output voltage, the next removes it
			//UpDownStepper();
		}
	}

	return 0;
}
