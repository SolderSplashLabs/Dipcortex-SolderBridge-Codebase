
#define PWM_DEFAULT_PERIOD			320			// 150KHz - before the prescaler
#define PWM_PRESCALE				30			// Clock Ticks are needed before incrementing the count register
												// 30 Gives us a 5Khz Timebase
												// 15 Gives us a 10Khz Timebase

#define PWM_NO_OF					6			// How many channels of pwm do we have control of
#define PWM_BIT_BANG_PWM5

// CT32B1_MAT0
#define PWM1_PORT		0
#define PWM1_PIN		13
#define PWM1_MASK		BIT13
#define PWM1_IOCON		{LPC_IOCON->TDO_PIO0_13 = 3 | 1<<7;}
#define PWM1_DUTY_REG	&(LPC_CT32B1->MR0)

// CT32B1_MAT1
#define PWM2_PORT		0
#define PWM2_PIN		14
#define PWM2_MASK		BIT14
#define PWM2_IOCON		{LPC_IOCON->TRST_PIO0_14 = 3 | 1<<7;}
#define PWM2_DUTY_REG	&(LPC_CT32B1->MR1)

// CT32B0_MAT1
#define PWM3_PORT		1
#define PWM3_PIN		25
#define PWM3_MASK		BIT25
#define PWM3_IOCON		{LPC_IOCON->PIO1_25 = 1;}
#define PWM3_DUTY_REG	&(LPC_CT32B0->MR1)

// CT32B0_MAT2
#define PWM4_PORT		1
#define PWM4_PIN		26
#define PWM4_MASK		BIT26
#define PWM4_IOCON		{LPC_IOCON->PIO1_26 = 1;}
#define PWM4_DUTY_REG	&(LPC_CT32B0->MR2)

#ifdef PWM_BIT_BANG_PWM5
	#define PWM5_PORT		1
	#define PWM5_PIN		27
	#define PWM5_MASK		BIT27
	#define PWM5_IOCON		{LPC_IOCON->PIO1_27 = 0;}
	// Prototype board has all four matches assigned, which you can't do
	// so use CT32B1 MR2 instead and toggle the IO in an interrupt
	// fixed on production board
	#define PWM5_DUTY_REG	&(LPC_CT32B1->MR2)
#else
	// CT32B0_MAT3
	#define PWM5_PORT		1
	#define PWM5_PIN		27
	#define PWM5_MASK		BIT27
	#define PWM5_IOCON		{LPC_IOCON->PIO1_27 = 1;}
	#define PWM5_DUTY_REG	&(LPC_CT32B0->MR3)
#endif

// CT32B0_MAT0
#define PWM6_PORT		1
#define PWM6_PIN		24
#define PWM6_MASK		BIT24
#define PWM6_IOCON		{LPC_IOCON->PIO1_24 = 1;}
#define PWM6_DUTY_REG	&(LPC_CT32B0->MR0)

void Pwm_Init ( void );
void Pwm_SetDuty ( uint8_t pwmMask, uint32_t duty );
void Pwm_On ( void );
void Pwm_DutyIncrease ( uint8_t pwmMask, uint32_t maxDuty );
void Pwm_DutyDecrease ( uint8_t pwmMask, uint32_t minDuty );
uint32_t Pwm_GetDuty ( uint8_t channel );
void Pwm_DutyStep ( uint8_t pwmMask, uint32_t targetDuty );

#ifdef _PWM_CON_

// PWM Duty control array
volatile uint32_t* Pwm_Duty[PWM_NO_OF] = { PWM1_DUTY_REG, PWM2_DUTY_REG, PWM3_DUTY_REG, PWM4_DUTY_REG, PWM5_DUTY_REG ,PWM6_DUTY_REG };

#endif
