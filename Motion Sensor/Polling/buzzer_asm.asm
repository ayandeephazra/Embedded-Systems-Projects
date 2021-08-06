;
; Lab 4, ECE 266, spring 2021
;
; Assembly functions related to buzzer operations.
;
; Created by Zhao Zhang
;

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","buzzer.h"

                .text
                .global buzzerOn
                .global buzzerOff

; GPIO peripheral, port base and pin mask value for the buzzer
; The buzzer is assumed to be connected to J17, and then the buzzer's
; signal pin is connected to PC5.
BUZZER_PORT     .field  GPIO_PORTC_BASE
BUZZER_PIN      .equ    GPIO_PIN_5

;
; void buzzOn(): Turn on the buzzer. It calls GPIOPinWrite() to write 1 to the signal pin.
;
buzzerOn        PUSH    {LR}            ; save the return address

                ; Write 1 to the GPIO pin that the buzzer uses:
                ;   GPIOPinWrite(BUZZ_PORT, BUZZ_PIN, BUZZ_PIN)
                LDR     r0, BUZZER_PORT
                MOV     r1, #BUZZER_PIN
                MOV     r2, #BUZZER_PIN
                BL      GPIOPinWrite

                POP     {PC}            ; return

;
; void buzzOff(): Turn off the buzzer. It alls GPIOPinWrite() to write 0 to the pin
;
buzzerOff       PUSH    {LR}            ; save the return address

                ; Write 0 to the GPIO pin that the buzzer uses
                ;   GPIOPinWrite(BUZZ_PORT, BUZZ_PIN, 0)
                LDR     r0, BUZZER_PORT
                MOV     r1, #BUZZER_PIN
                MOV     r2, #0
                BL      GPIOPinWrite

                POP     {PC}            ; return
