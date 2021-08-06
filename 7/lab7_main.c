/*
 * main.c: Starter code for Lab 7, Spring 2021
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
//#include "ras.h"
#include "pwmled.h"
//#include "pwmbuzzer.h"

// LED-related constant
#define LED_PWM_PERIOD          5000
#define LED_MAX_PULSE_WIDTH     100

/*
 * Task 1: Play a sine pattern on LED
 */

typedef struct
{
    int pwmPeriod;              // PWM period for LED
    int maxPulseWidth;          // maximum pulse width
} LedState_t;

static LedState_t led =
        { LED_PWM_PERIOD /* PWM Period */, LED_MAX_PULSE_WIDTH * 0.4 /* Multiply factor */};

// A sine function that uses degree as input. It converts degree to radian and then
// calls the sin() function of the C library.
static inline double sine(uint16_t degree)
{
    double radian = 2 * M_PI * ((double) (degree % 360) / 360);
    return sin(radian);
}

// LED playing callback function
void callbackLedPlay(uint32_t time)
{
    static uint16_t angle = 0;      // the degree of angle, used for calculating sine value
    int delay = 5;                  // the callback delay in ms

    // Calculate PWM parameters for red, blue, and green sub-LEDs using sine function.
    // Use phase shift of 60, 30, and 0 degrees for red, blue, and green
    int pulseWidthRed = sine(angle + 72) * led.maxPulseWidth;
    int pulseWidthBlue = sine(angle + 36) * led.maxPulseWidth;
    int pulseWidthGreen = sine(angle) * led.maxPulseWidth;

    // Set the PWM parameters for the three sub-LEDs
    ledPwmSet(led.pwmPeriod, pulseWidthRed, pulseWidthBlue, pulseWidthGreen);

    // Advance the angle by one degree, so a play period is 360 * 5 = 1800 ms
    angle = (angle + 1) % 360;

    // Schedule the next callback
    schdCallback(callbackLedPlay, time + delay);
}

void main(void)
{
    lpInit();
    ledPwmInit();

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(callbackLedPlay, 1002);

    // Loop forever
    while (true)
    {
        schdExecute();
    }
}
