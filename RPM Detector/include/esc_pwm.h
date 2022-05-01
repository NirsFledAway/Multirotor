#ifndef ESC_PWM
#define ESC_PWM

#include <mbed.h>


const PinName ESCPin = PinName::PA_1;

const unsigned int PWM_FREQ = 50; // Hz
const unsigned int MIN_THROTTLE = 1000; // us
const unsigned int MAX_THROTTLE =  2000; // us

void esc_init();

void arm(Serial*);

void esc_write(int val);

void setupLimits(Serial* pc);

#endif