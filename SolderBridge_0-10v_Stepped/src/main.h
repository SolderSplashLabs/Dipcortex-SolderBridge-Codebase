
#define MAX_DIM_LEVELS 			8


#ifdef _MAIN_

bool actionButtons = false;

// These values have been manually calculated, measured max output voltage and scaled that across the PWM resolution.
const uint16_t DIM_LEVELS[ MAX_DIM_LEVELS] = {321, 244, 169, 93, 18, 18, 18, 18};

#endif
