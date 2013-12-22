/*
  ____        _     _           ____        _           _		 _          _
 / ___|  ___ | | __| | ___ _ __/ ___| _ __ | | __ _ ___| |__	| |    __ _| |__  ___
 \___ \ / _ \| |/ _` |/ _ \ '__\___ \| '_ \| |/ _` / __| '_ \	| |   / _` | '_ \/ __|
  ___) | (_) | | (_| |  __/ |   ___) | |_) | | (_| \__ \ | | |	| |__| (_| | |_) \__ \
 |____/ \___/|_|\__,_|\___|_|  |____/| .__/|_|\__,_|___/_| |_|	|_____\__,_|_.__/|___/
                                     |_|
 (C)SolderSplash Labs 2013 - www.soldersplash.co.uk - C. Matthews - R. Steel


	@file     ButtonCon.h
	@author   C. Matthews (soldersplash.co.uk)
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

	Please Refer to user manual for IOCON register descriptions

*/

#define BUTTON_NO			6			// Number of buttons
#define BUTTON_HELD_MS		500			// Button Held Milliseconds
#define BUTTON_PRESSED_MS	5			// Button Pressed Milliseconds

typedef struct BUTTONS
{
	uint8_t	Port;
	uint8_t	Pin;
	uint32_t Mask;
} BUTTONS;


#define BUTTON1_PORT		1
#define BUTTON1_PIN			13
#define BUTTON1_MASK		BIT13

// Select : PIO, Pull Up, Hysteresis
#define BUTTON1_IOCON		{LPC_IOCON->PIO1_13 = (0 | 1<<5 | 1<<4);}

#define BUTTON2_PORT		1
#define BUTTON2_PIN			14
#define BUTTON2_MASK		BIT14

// Select : PIO, Pull Up, Hysteresis
#define BUTTON2_IOCON		{LPC_IOCON->PIO1_14 = (0 | 1<<5 | 1<<4);}

#define BUTTON3_PORT		1
#define BUTTON3_PIN			15
#define BUTTON3_MASK		BIT15

// Select : PIO, Pull Up, Hysteresis
#define BUTTON3_IOCON		{LPC_IOCON->PIO1_15 = (0 | 1<<5 | 1<<4);}

#define BUTTON4_PORT		1
#define BUTTON4_PIN			16
#define BUTTON4_MASK		BIT16

// Select : PIO, No Pull, Hysteresis
#define BUTTON4_IOCON 		{LPC_IOCON->PIO1_16 = (0 | 1<<5);}

#define BUTTON5_PORT		1
#define BUTTON5_PIN			21
#define BUTTON5_MASK		BIT21

// Select : PIO, No Pull, Hysteresis
#define BUTTON5_IOCON		{LPC_IOCON->PIO1_21 = (0 | 1<<5);}

#define BUTTON6_PORT		0
#define BUTTON6_PIN			12
#define BUTTON6_MASK		BIT12

// Select : PIO, No Pull, Hysteresis, Digital mode, Glitch filtered
#define BUTTON6_IOCON		{LPC_IOCON->TMS_PIO0_12 = (1 | 1<<5 | 1<<7 | 1<<8);}

// Mask used on pressed return byte
#define BUTTON1				BIT0
#define BUTTON2				BIT1
#define BUTTON3				BIT2
#define BUTTON4				BIT3
#define BUTTON5				BIT4
#define BUTTON6				BIT5

void Buttons_ActionPressed ( void );
void Buttons_GetPressed ( uint8_t *pressed );
void Buttons_GetHeld ( uint8_t *pressed );
void Buttons_GetPrevState ( uint8_t *buttonsState );
void Buttons_Init ( void );
void Buttons_Task ( uint32_t msCallRate );

#ifdef _BUTTONS_

BUTTONS Buttons[BUTTON_NO] =
{
		{BUTTON1_PORT, BUTTON1_PIN, BUTTON1_MASK},
		{BUTTON2_PORT, BUTTON2_PIN, BUTTON2_MASK},
		{BUTTON3_PORT, BUTTON3_PIN, BUTTON3_MASK},
		{BUTTON4_PORT, BUTTON4_PIN, BUTTON4_MASK},
		{BUTTON5_PORT, BUTTON5_PIN, BUTTON5_MASK},
		{BUTTON6_PORT, BUTTON6_PIN, BUTTON6_MASK}
};


volatile uint8_t ButtonsPressed;
volatile uint8_t ButtonsHeld;
volatile uint8_t ButtonsPrevState;
uint16_t ButtonHeldCnt[BUTTON_NO] = {0, 0, 0, 0, 0, 0};


#endif
