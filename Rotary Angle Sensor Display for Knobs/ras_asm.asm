                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/adc.h","ras.h"

                .text
                .global RASreturn

; GPIO peripheral, port base and pin mask value for the SENSOR
; The SENSOR is assumed to be connected to J16, and then the SENSOR's
; signal pin is connected to PC4.
RAS_PORT     .field  ADC0_BASE
;RAS_PIN      .equ    ADC7_BASE


;
; void buzzOn(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
RASreturn       PUSH    {LR}            ; save the return address
                PUSH    {r0}
                LDR     r0, RAS_PORT
                MOV     r1, #0
                BL      ADCProcessorTrigger
while_loop      LDR     r0, RAS_PORT
                MOV     r1, #0
                MOV     r2, #0
                BL      ADCIntStatus
                CMP     r0, #0
                BEQ     while_loop
                BNE     continue
continue        POP     {r2}
                MOV     r1, #0
                LDR     r0, RAS_PORT
                BL      ADCSequenceDataGet
                PUSH    {r0}
                LDR     r0, RAS_PORT
                MOV     r1, #0
                BL      ADCIntClear
                POP     {r0}
                POP     {PC}            ; return

