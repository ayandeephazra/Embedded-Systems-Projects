#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/adc.c>

// Pin usage: Grove base port J10, Tiva C PC5 (Port A, Pin 1)
//#define RAS_PERIPH   SYSCTL_PERIPH_ADC7
//#define RAS_PORT     GPIO_PORTA_BASE
//#define RAS_PIN      GPIO_PIN_1

void RASInit()
{
    // ADC0 is the port, ADC7 is the pin
    // Enable the port peripheral used by the buzzer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {
    }
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH6);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH7);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 0);

}
/*
void RASreturn(uint32_t data[])
{

     uint32_t ui32Value;

     while (!ADCIntStatus(ADC0_BASE, 0, false))
     {
     }

     ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
     //int32_t retval = 0;
     ADCIntClear(ADC0_BASE, 0);

     return ui32Value;

    // Trigger ADC sample sequence
    ADCProcessorTrigger(ADC0_BASE, 0);
    // Wait until the sample sequence has completed
    while (!ADCIntStatus(ADC0_BASE, 0, false))
    {
    }
    // Read ADC Sequence Data, may have multiple steps
    ADCSequenceDataGet(ADC0_BASE, 0, &data);
    // Clear interrupt status
    ADCIntClear(ADC0_BASE, 0);
}
*/
