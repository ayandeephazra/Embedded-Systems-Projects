/*
 * pwdled.h: Starter code for Lab 7, Spring 2021
 *
 *  Created by Zhao Zhang
 */

#ifndef PWMLED_H_
#define PWMLED_H_

#include <stdint.h>

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void ledPwmInit();

/*
 * Set the LED parameters for the three sub-LEDs
 */
void ledPwmSet(int pulsePeriod, int pulseWidthRed, int pulseWidthBlue, int pulseWidthGreen);

#endif /* PWDLED_H_ */
