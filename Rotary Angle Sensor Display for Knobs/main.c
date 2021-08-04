#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include <driverlib/adc.h>
#include "ras.h"
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include <math.h>
typedef enum
{
    On, Off
} OnOff_t;

// System running state

seg7Display_t seg7Display = { 0, 0, 0, 0 };
static OnOff_t sysState = On;

void callbackRAS(uint32_t time)                    // the scheduled time
{
    uint32_t data[2];
    uint32_t delay = 5;
    if (sysState == On)
    {
        RASreturn(data);
        float temp = (float) (4095 - data[0]);
        float temp2 = (float) temp / 4095;
        //uprintf("%i\n\r", );

        float temp3 = (float) (4095 - data[1]);
        float temp4 = (float) temp3 / 4095;
        uprintf("%i\t%i\n\r", (int) (temp2 * 99), (int) (temp4 * 99));
        int val1 = (int) (temp2 * 99);
        int val2 = (int) (temp4 * 99);

        seg7Display.d1 = (uint8_t) val1 % 10;
        seg7Display.d2 = (uint8_t) val1 / 10;
        seg7Display.d3 = (uint8_t) val2 % 10;
        seg7Display.d4 = (uint8_t) val2 / 10;

        seg7DigitUpdate(&seg7Display);
    }

// schedule the next callback
    schdCallback(callbackRAS, time + delay);
}

/**
 * main.c
 */
int main(void)
{
    lpInit();
    seg7Init();
    RASInit();

// Print out a start message
// uprintf("%s\n\r", "Lab 4 starts");

// Schedule the first callback events

    sysState = On;
    seg7DigitUpdate(&seg7Display);
    schdCallback(callbackRAS, 1000);

//    sysState = Off;

// Run the callback scheduler
    while (true)
    {
        schdExecute();
    }

}
