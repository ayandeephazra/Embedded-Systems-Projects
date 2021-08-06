;
; Lab 4, ECE 266, spring 2021
;
; Assembly functions related to buzzer operations.
;
; Created by Zhao Zhang
;

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","motion.h"

                .text
                .global motionOnOff

; GPIO peripheral, port base and pin mask value for the SENSOR
; The SENSOR is assumed to be connected to J16, and then the SENSOR's
; signal pin is connected to PC4.
MOTION_PORT     .field  GPIO_PORTC_BASE
MOTION_PIN      .equ    GPIO_PIN_4


;
; void buzzOn(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
motionOnOff     PUSH    {LR}            ; save the return address

                ; Read from the GPIO pin that the SENSOR uses
                ;   GPIOPinRead(MOTION_PORT, MOTION_PIN)
                LDR     r0, MOTION_PORT
                MOV     r1, #MOTION_PIN
                BL      GPIOPinRead

                POP     {PC}            ; return

