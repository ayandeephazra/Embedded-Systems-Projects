/*
 * ECE 266 Lab 2 starter code, Spring 2021
 *
 * lab2_main.c
 *
 * Created by Zhao Zhang
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "launchpad.h"
#include "seg7.h"

// 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
// are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
// Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
// segment, and '0' turns OFF a segment.
static uint8_t seg7Coding[10] = { 0b00111111, 		// digit 0
        0b00000110, 		// digit 1
        0b01011011,			// digit 2
        0b01001111,			// digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01101111,         // digit 9
        // MORE CODINGS
        };

// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.
static uint8_t colon = 0;
uint8_t second1 = 0;
uint8_t second2 = 0;
uint8_t minute1 = 0;
uint8_t minute2 = 0;

void checkPushButton(uint32_t time)
{
    // Read the pushbutton state; see pbRead() in launchpad.h
    int code = pbRead();
    uint32_t delay = 0;

    switch (code)
    {
    case 1:
        minute1 = (minute1 + 1) % 10;
        if (minute1 == 0)
        {
            minute2 = (minute2 + 1) % 6;
        }

        // Use an inertia for soft de-bouncing
        delay = 500;
        break;

    case 2:

        second1 = (second1 + 1) % 10;
        if (second1 == 0)
        {
            second2 = (second2 + 1) % 6;

            if (second2 == 0)
            {
                minute1 = (minute1 + 1) % 10;
                if (minute1 == 0)
                {
                    minute2 = (minute2 + 1) % 6;
                }
            }
        }

        // Use an inertia for soft de-bouncing
        delay = 500;
        break;
    }

}

// Update the clock display
void clockUpdate(uint32_t time)						// pointer to a 4-byte array
{

    checkPushButton(time);
    uint8_t code[4];			// The 7-segment code for the four clock digits

    // Display 01:23 on the 7-segment displays
    // The colon ':' will flash on and off every 0.5 seconds
    code[0] = seg7Coding[second1] + colon;
    code[1] = seg7Coding[second2] + colon;
    code[2] = seg7Coding[minute1] + colon;
    code[3] = seg7Coding[minute2] + colon;

    //code[0] = colon;
    //code[1] = colon;
    //code[2] = colon;
    //code[3] = colon;
    seg7Update(code);

    // Calculate the display digits and colon setting for the next update
    if (colon == 0b00000000)
    {
        colon = 0b10000000;
        if (second1 == 9)
        {
            second1 = 0;

            if (second2 == 5)
            {
                second2 = 0;

                if (minute1 == 9)
                {
                    minute1 = 0;

                    if (minute2 == 5)
                    {
                        minute2 = 0;
                    }
                    else
                    {
                        minute2++;
                    }

                }
                else
                {
                    minute1++;
                }

            }
            else
            {
                second2++;
            }

        }
        else
        {
            second1++;
        }

    }
    else
    {
        colon = 0b00000000;
    }

    // Call back after 1 second
    schdCallback(clockUpdate, time + 500);
}

int main(void)
{
    lpInit();
    seg7Init();

    uprintf("%s\n\r", "Lab 2: Wall clock");

    // Schedule the first callback events for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.

    schdCallback(clockUpdate, 1000);

    // Loop forever
    while (true)
    {

        schdExecute();
    }
}
