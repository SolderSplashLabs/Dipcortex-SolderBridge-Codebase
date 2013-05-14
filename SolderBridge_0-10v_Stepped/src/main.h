


#ifdef _MAIN_

#define MIN_DIM_DUTY			16	// This is equal to about 10v
#define MAX_DIM_DUTY			321 // 0v
#define DIM_STEP				76	// Equals roughly 2.5v

#define MAX_DIM_LEVELS 			8
// These values have been manually calculated, measured max output voltage and scaled that across the PWM resolution.
const uint16_t DIM_LEVELS[ MAX_DIM_LEVELS] = {321, 244, 169, 93, 16, 16, 16, 16};

typedef struct DIMMING_STRUCT
{
	uint8_t goingUp:1;
	uint8_t goingDown:1;
	uint8_t hasStarted:1;
	uint32_t targetDuty;
}DIMMING_STRUCT;

DIMMING_STRUCT DimmingOutputs[6];
bool actionButtons = false;

#endif
