/*
 * lab3_main.c: starter code for ECE 266 Lab 3, stopwatch
 *
 * Created by Zhao Zhang for ECE 266, spring 2021
 *
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"

/*
 * System states
 */

// The running state of the stopwatch system
enum
{
    Reset, Run, Pause
} sysState = Pause; //Run;

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = { 0, 0, 0, 0, 1 };

/*
 * Callback function for updating clock watch
 */

extern stopWatchIncrement(seg7Display_t *);

void stopWatchUpdate(uint32_t time)                        // The scheduled time
{
    // Update clock and display, only if the stopwatch is running
    if (sysState == Run)
    {
        stopWatchIncrement(&seg7Display);
    }
    else if(sysState == Reset){
        seg7Display.d1 = 0;
        seg7Display.d2 = 0;
        seg7Display.d3 = 0;
        seg7Display.d4 = 0;
    }

    seg7DigitUpdate(&seg7Display);

    // Call back after 10 milliseconds
    schdCallback(stopWatchUpdate, time + 10);
}

/*
 * Callback function for checking push button. SW1 is the RESET button, and
 * SW2 is the START/PAUSE/RESUME button.
 */
void checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay;

    switch (code)
    {
    case 1:        // SW1 is the Reset button, only when the stopwatch is paused

        //
        // YOUR CODE
        //

        if (sysState == Pause)
        {

            sysState = Reset;

        }

        delay = 250;                // software debouncing
        break;

    case 2:                         // SW2 is the Start/Pause/Resume button

        //
        // YOUR CODE
        //
        if (sysState == Run)
        {

            sysState = Pause;

        }
        else
        {

            sysState = Run;

        }
        delay = 250;                // software debouncing
        break;

    default:
        delay = 10;
    }

    schdCallback(checkPushButton, time + delay);
}

/*
 * The main function
 */
int main(void)
{
    lpInit();
    seg7Init();

    uprintf("%s\n\r", "Lab 3: Stopwatch");

    // Update the clock display
    seg7DigitUpdate(&seg7Display);

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(stopWatchUpdate, 1000);
    schdCallback(checkPushButton, 1005);

    // Run the event scheduler to process callback events
    while (true)
    {
        schdExecute();
    }
}
