/*
 * ECE 266, Spring 2021, Lab 4 starter code
 *
 * lab4_main.c
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>

// ON/Off state type
typedef enum
{
    On, Off
} OnOff_t;

// System running state
static OnOff_t sysState = Off;

// Alarm state
static OnOff_t alarmState = Off;

// Buzzer state
static OnOff_t buzzerState = Off;

/*
 * Task 1: Play the buzzer
 */

// Callback function for playing the buzzer
void callbackBuzzerPlay(uint32_t time)                    // the scheduled time
{
    uint32_t delay = 10;

    if (alarmState == On)
    {
        assert(sysState == On);

        // Turn the buzzer on and off alternatively
        // Adjust the time values to control the sound intensity
        switch (buzzerState)
        {
        case On:
            buzzerOff();
            buzzerState = Off;
            delay = 2988;                           // off for 2988 ms
            break;

        case Off:
            buzzerOn();
            buzzerState = On;
            delay = 12;                            // on for 12 ms
            break;
        }
    }

    // schedule the next callback
    schdCallback(callbackBuzzerPlay, time + delay);
}

/*
 * Task 2: Check push buttons
 */

// Callback function for checking the push buttons
void callbackCheckPushButton(uint32_t time)
{
    uint32_t delay = 10;        // the default delay for the next checking

    int code = pbRead();        // read the pushbutton
    switch (code)
    {
    case 1:                     // SW1: Turn on the system and the alarm
        sysState = On;
        delay = 250;
        ledTurnOnOff(false /* red */, false /* blue */, true /* green */);
        break;

    case 2:                     // SW2: Turn off the system and the alarm
        sysState = Off;
        alarmState = Off;
        ledTurnOnOff(false /* red */, false /* blue */, false/* green */);
        buzzerOff();
        delay = 250;
        break;
    }

    // schedule the next callback
    schdCallback(callbackCheckPushButton, time + delay);
}

/*
 * Task 3: YOUR CODE, check the PIR motion sensor
 */

// Callback function for playing the buzzer
void callbackMotion(uint32_t time)                    // the scheduled time
{
    uint32_t delay = 5;

    if(sysState==On){
    if (motionOnOff() == 16)
    {
        ledTurnOnOff(true /* red */, false /* blue */, false /* green */);
        alarmState = On;
        uprintf("%s\n\r", "motion detected");
    }
    else
    {
        ledTurnOnOff(false /* red */, false /* blue */, true /* green */);
        alarmState = Off;
        uprintf("%s\n\r", "motion not detected");
    }
    }
    // schedule the next callback
    schdCallback(callbackMotion, time + delay);
}

/*
 * The main function
 */
int main(void)
{

    lpInit();
    buzzerInit();
    motionInit();

    // Print out a start message
    uprintf("%s\n\r", "Lab 4 starts");

    // Schedule the first callback events

    schdCallback(callbackBuzzerPlay, 1000);
    schdCallback(callbackCheckPushButton, 1002);
    schdCallback(callbackMotion, 2000);

    // Turn on the green sub-LED
    ledTurnOnOff(false /* red */, false /* blue */, false/* green */);
    sysState = Off;

    // Run the callback scheduler
    while (true)
    {
        schdExecute();
    }
}
