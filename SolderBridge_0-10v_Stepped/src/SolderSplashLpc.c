/*
  ____        _     _           ____        _           _		 _          _
 / ___|  ___ | | __| | ___ _ __/ ___| _ __ | | __ _ ___| |__	| |    __ _| |__  ___
 \___ \ / _ \| |/ _` |/ _ \ '__\___ \| '_ \| |/ _` / __| '_ \	| |   / _` | '_ \/ __|
  ___) | (_) | | (_| |  __/ |   ___) | |_) | | (_| \__ \ | | |	| |__| (_| | |_) \__ \
 |____/ \___/|_|\__,_|\___|_|  |____/| .__/|_|\__,_|___/_| |_|	|_____\__,_|_.__/|___/
                                     |_|
 (C)SolderSplash Labs 2013 - www.soldersplash.co.uk - C. Matthews - R. Steel


	@file     SolderSplashLpc.c
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

    A collection of helper functions for the LPC.

    LpcLowPowerIoInit, configures the chips IO to their lowest power state as a base to work on
*/

#include "SolderSplashLpc.h"

#define ADMODE_OFF	1<<7

// --------------------------------------------------------------------------------------
// LpcLowPowerIoInit
// Set up every GPIO pin to a GPIO and an input
// --------------------------------------------------------------------------------------
void LpcLowPowerIoInit ( void )
{
	// Leave the reset pin as a reset input
	LPC_IOCON->RESET_PIO0_0 = 0;

	// Initialise all Pins as gpio with inactive pull up/downs
	LPC_IOCON->PIO0_1 = 0;
	LPC_IOCON->PIO0_2 = 0;
	LPC_IOCON->PIO0_3 = 0;

	// PIO0_4 & PIO0_5 are I2C pins by default, changing them to GPIO
	LPC_IOCON->PIO0_4 = 1<<8;
	LPC_IOCON->PIO0_5 = 1<<8;

	// PIO0_6 is normally the USB Connect pin
	LPC_IOCON->PIO0_6 = 1;

	LPC_IOCON->PIO0_7 = 0;
	LPC_IOCON->PIO0_8 = 0;
	LPC_IOCON->PIO0_9 = 0;

	// Leave SWCLK alone, used for debugging
	LPC_IOCON->SWCLK_PIO0_10 = 0;

	LPC_IOCON->TDI_PIO0_11 = 1 | ADMODE_OFF;
	LPC_IOCON->TMS_PIO0_12 = 1 | ADMODE_OFF;
	LPC_IOCON->TDO_PIO0_13 = 1 | ADMODE_OFF;
	LPC_IOCON->TRST_PIO0_14 = 1 | ADMODE_OFF;

	// Leave SWDIO alone, used for debugging
	LPC_IOCON->SWDIO_PIO0_15 = ADMODE_OFF;

	LPC_IOCON->PIO0_16 = ADMODE_OFF;
	LPC_IOCON->PIO0_17 = 0;
	LPC_IOCON->PIO0_18 = 0;
	LPC_IOCON->PIO0_19 = 0;
	LPC_IOCON->PIO0_20 = 0;
	LPC_IOCON->PIO0_21 = 0;

	LPC_IOCON->PIO0_22 = ADMODE_OFF;
	LPC_IOCON->PIO0_23 = ADMODE_OFF;

	LPC_IOCON->PIO1_0 = 0;
	LPC_IOCON->PIO1_1 = 0;
	LPC_IOCON->PIO1_2 = 0;
	LPC_IOCON->PIO1_3 = 0;
	LPC_IOCON->PIO1_4 = 0;
	LPC_IOCON->PIO1_5 = 0;
	LPC_IOCON->PIO1_6 = 0;
	LPC_IOCON->PIO1_7 = 0;
	LPC_IOCON->PIO1_8 = 0;
	LPC_IOCON->PIO1_9 = 0;
	LPC_IOCON->PIO1_10 = 0;
	LPC_IOCON->PIO1_11 = 0;
	LPC_IOCON->PIO1_12 = 0;
	LPC_IOCON->PIO1_13 = 0;
	LPC_IOCON->PIO1_14 = 0;
	LPC_IOCON->PIO1_15 = 0;
	LPC_IOCON->PIO1_16 = 0;
	LPC_IOCON->PIO1_17 = 0;
	LPC_IOCON->PIO1_18 = 0;
	LPC_IOCON->PIO1_19 = 0;
	LPC_IOCON->PIO1_20 = 0;
	LPC_IOCON->PIO1_21 = 0;
	LPC_IOCON->PIO1_22 = 0;
	LPC_IOCON->PIO1_23 = 0;
	LPC_IOCON->PIO1_24 = 0;
	LPC_IOCON->PIO1_25 = 0;
	LPC_IOCON->PIO1_26 = 0;
	LPC_IOCON->PIO1_27 = 0;
	LPC_IOCON->PIO1_28 = 0;
	LPC_IOCON->PIO1_29 = 0;
	LPC_IOCON->PIO1_30 = 0;
	LPC_IOCON->PIO1_31 = 0;
}
