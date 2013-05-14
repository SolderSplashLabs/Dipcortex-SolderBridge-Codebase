/*
  ____        _     _           ____        _           _		 _          _
 / ___|  ___ | | __| | ___ _ __/ ___| _ __ | | __ _ ___| |__	| |    __ _| |__  ___
 \___ \ / _ \| |/ _` |/ _ \ '__\___ \| '_ \| |/ _` / __| '_ \	| |   / _` | '_ \/ __|
  ___) | (_) | | (_| |  __/ |   ___) | |_) | | (_| \__ \ | | |	| |__| (_| | |_) \__ \
 |____/ \___/|_|\__,_|\___|_|  |____/| .__/|_|\__,_|___/_| |_|	|_____\__,_|_.__/|___/
                                     |_|
 (C)SolderSplash Labs 2013 - www.soldersplash.co.uk - C. Matthews - R. Steel

	@file     ButtonCon.c
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
*/

#include "SolderSplashLpc.h"

#define _BUTTONS_
#include "ButtonCon.h"

// -------------------------------------------------------------------------------------------
/*!
    @brief 	Buttons_Task - Call this at a rate you would like to detect a button press,
			will record a press on a high to low transition
*/
// -------------------------------------------------------------------------------------------
void Buttons_Task ( uint32_t msCallRate )
{
	uint8_t i = 0;
	uint8_t mask = 1;
	uint8_t buttonState = 0;

	buttonState = ButtonsPrevState;

	for (i=0; i<BUTTON_NO; i++)
	{
		if ( LPC_GPIO->PIN[Buttons[i].Port] & Buttons[i].Mask )
		{
			// High - the default state of the input

			if (! (buttonState & mask))
			{
				// Just transitioned to high from low
				if (ButtonsHeld & mask)
				{
					// Button was held
					ButtonsHeld &= ~mask;
				}
				else
				{
					// Wasn't held, but was it long enough for a press
					if (ButtonHeldCnt[i] >= BUTTON_PRESSED_MS)
					{
						ButtonsPressed |= mask;
					}
				}
			}

			// Update Prev State
			buttonState |= mask;
		}
		else
		{
			if (buttonState & mask)
			{
				// Was high now low
				ButtonHeldCnt[i] = 0;
			}
			else
			{
				if (ButtonHeldCnt[i] < BUTTON_HELD_MS)
				{
					ButtonHeldCnt[i] += msCallRate;
					/*
					if (ButtonHeldCnt[i] == BUTTON_PRESSED_MS)
					{
						newButtonsPressed |= mask;
					}
					*/
				}
				else
				{
					// Declare the button as held
					ButtonsHeld |= mask;
				}
			}

			// Update Prev State
			buttonState &= ~mask;
		}

		mask <<= 1;
	}

	ButtonsPrevState = buttonState;
}

// -------------------------------------------------------------------------------------------
/*!
    @brief 	Buttons_ActionPressed - clear the pressed button mask
*/
// -------------------------------------------------------------------------------------------
void Buttons_ActionPressed ( void )
{
    ButtonsPressed = 0;
}

// -------------------------------------------------------------------------------------------
/*!
	@brief 	Buttons_GetPressed - returns a bit mask of pressed buttons
*/
// -------------------------------------------------------------------------------------------
void Buttons_GetPressed ( uint8_t *pressed )
{
    *pressed = ButtonsPressed;
}

// -------------------------------------------------------------------------------------------
/*!
	@brief 	Buttons_GetPrevState - returns the last state of the buttons
*/
// -------------------------------------------------------------------------------------------
void Buttons_GetPrevState ( uint8_t *buttonsState )
{
    *buttonsState = ButtonsPrevState;
}

// -------------------------------------------------------------------------------------------
/*!
	@brief 	Buttons_GetHeld - returns a bit mask of held buttons
*/
// -------------------------------------------------------------------------------------------
void Buttons_GetHeld ( uint8_t *pressed )
{
    *pressed = ButtonsHeld;
}

// -------------------------------------------------------------------------------------------
/*!
	@brief 	Buttons_Init - Set up GPIO
*/
// -------------------------------------------------------------------------------------------
void Buttons_Init( void )
{
uint8_t i = 0;

	// Configure IOCON registers for the 6 buttons
	BUTTON1_IOCON;
	BUTTON2_IOCON;
	BUTTON3_IOCON;
	BUTTON4_IOCON;
	BUTTON5_IOCON;
	BUTTON6_IOCON;

	for (i=0; i<BUTTON_NO; i++)
	{
		// Set buttons as GP inputs
		LPC_GPIO->DIR[Buttons[i].Port] &= ~(1<<Buttons[i].Pin);
	}
}
