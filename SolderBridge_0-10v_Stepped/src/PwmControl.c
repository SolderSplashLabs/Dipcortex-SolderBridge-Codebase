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

    PWM Control of 6 outputs
*/

#include "SolderSplashLpc.h"

#define _PWM_CON_
#include "PwmControl.h"

// -------------------------------------------------------------------------------------------
// Pwm_Init
// Initalise the PWM port
// -------------------------------------------------------------------------------------------
void Pwm_Init ( void )
{
	// Enable the CT32B0, CT32B1 & CT16B0 peripherals
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9) | (1<<10) | (1<<7);

	// Configure match pins - see header file
	PWM1_IOCON;
	PWM2_IOCON;
	PWM3_IOCON;
	PWM4_IOCON;
	PWM5_IOCON;
	PWM6_IOCON;

	// Toggle match GPIO on timer match
	LPC_CT32B0->EMR = (0xFF<<4);
	LPC_CT32B1->EMR = (0xFF<<4);

	// Interrupt and reset on MR3
	LPC_CT32B0->MCR = 1<<9 | 1<<10;

	// Interrupt and reset on MR3
	LPC_CT32B1->MCR = 1<<9 | 1<<10;

	Pwm_SetDuty( 0xFF, PWM_DEFAULT_PERIOD /2 );

	// Use MR3 as our Period register
	LPC_CT32B0->MR3 = PWM_DEFAULT_PERIOD;
	LPC_CT32B1->MR3 = PWM_DEFAULT_PERIOD;

	LPC_CT32B0->PR = PWM_PRESCALE;
	LPC_CT32B1->PR = PWM_PRESCALE;


#ifdef PWM_BIT_BANG_PWM5

	// For the prototype we need to bit bang this pwm channel
	// Using a free timer match we control the gpio from the interrupt

	LPC_GPIO->DIR[PWM5_PORT] |= PWM5_MASK;
	LPC_GPIO->CLR[PWM5_PORT] = PWM5_PIN;

	// Enable MR2 interrupt
	LPC_CT32B1->MCR = LPC_CT32B1->MCR | (1<<6);
	NVIC_EnableIRQ(TIMER_32_1_IRQn);
#endif

	LPC_CT32B0->PWMC = 0x0007;
	LPC_CT32B1->PWMC = 0x0007;
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Modify the duty of the masked PWM channels
*/
// -------------------------------------------------------------------------------------------
void Pwm_SetDuty ( uint8_t pwmMask, uint32_t duty )
{
	if ( pwmMask & BIT0 )
	{
		*PWM1_DUTY_REG = duty;
	}

	if ( pwmMask & BIT1 )
	{
		*PWM2_DUTY_REG = duty;
	}

	if ( pwmMask & BIT2 )
	{
		*PWM3_DUTY_REG = duty;
	}

	if ( pwmMask & BIT3 )
	{
		*PWM4_DUTY_REG = duty;
	}

	if ( pwmMask & BIT4 )
	{
		#ifdef PWM_BIT_BANG_PWM5
			*PWM5_DUTY_REG = duty;
		#else
			*PWM5_DUTY_REG = PWM_DEFAULT_PERIOD;
		#endif
	}

	if ( pwmMask & BIT5 )
	{
		*PWM6_DUTY_REG = duty;
	}
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Increase the duty of the masked PWM channels
*/
// -------------------------------------------------------------------------------------------
void Pwm_DutyIncrease ( uint8_t pwmMask, uint32_t maxDuty )
{
uint8_t i = 0;
uint8_t mask = 0x01;

	for (i=0; i<PWM_NO_OF; i++)
	{
		if ( pwmMask & mask )
		{
			if ( *(Pwm_Duty[i]) < maxDuty )
			{
				*Pwm_Duty[i] = *Pwm_Duty[i] + 1;
			}
		}
		mask <<= 1;
	}
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Decrease the duty of the masked PWM channels
*/
// -------------------------------------------------------------------------------------------
void Pwm_DutyDecrease ( uint8_t pwmMask, uint32_t minDuty )
{
uint8_t i = 0;
uint8_t mask = 0x01;

	for (i=0; i<PWM_NO_OF; i++)
	{
		if ( pwmMask & mask )
		{
			if ( *(Pwm_Duty[i]) > minDuty )
			{
				*Pwm_Duty[i] = *Pwm_Duty[i] - 1;
			}
		}
		mask <<= 1;
	}
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Pwm_DutyStep - Move towards the target duty 1 count at a time
    // TODO : Allow a varible step size later
*/
// -------------------------------------------------------------------------------------------
void Pwm_DutyStep ( uint8_t pwmMask, uint32_t targetDuty )
{
uint8_t i = 0;
uint8_t mask = 0x01;

	for (i=0; i<PWM_NO_OF; i++)
	{
		if ( pwmMask & mask )
		{
			if ( *(Pwm_Duty[i]) < targetDuty )
			{
				*(Pwm_Duty[i]) = *(Pwm_Duty[i]) + 1;
			}
			else if ( *(Pwm_Duty[i]) > targetDuty )
			{
				*(Pwm_Duty[i]) = *(Pwm_Duty[i]) - 1;
			}
			else
			{
				// it's where it needs to be
			}
		}
		mask <<= 1;
	}
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Return the duty from the selected channel
*/
// -------------------------------------------------------------------------------------------
uint32_t Pwm_GetDuty ( uint8_t channel )
{
uint32_t result = 0;

	if ( channel < PWM_NO_OF )
	{
		result = *Pwm_Duty[channel];
	}

	return result;
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Timer32-1 Interrupt
*/
// -------------------------------------------------------------------------------------------
void TIMER32_1_IRQHandler(void)
{
#ifdef PWM_BIT_BANG_PWM5

	if ( LPC_CT32B1->IR & (0x1<<3) )
	{
		// MR3 Interrupt - Clear it down
		LPC_GPIO->CLR[PWM5_PORT] = PWM5_MASK;
	}
	else if ( LPC_CT32B1->IR & (0x1<<2) )
	{
		// MR0 Interrupt - Set output high
		LPC_GPIO->SET[PWM5_PORT] = PWM5_MASK;
	}
#endif
	// Clear all interrupts
	LPC_CT32B1->IR  = 0xff;
}

// -------------------------------------------------------------------------------------------
/*!
    @brief Enable all PWMs
*/
// -------------------------------------------------------------------------------------------
void Pwm_On ( void )
{
	// GO!
	LPC_CT32B0->TCR = 1;
	LPC_CT32B1->TCR = 1;
#ifdef PWM_BIT_BANG_PWM5
	LPC_CT32B1->IR = 0xff;
#endif
}
