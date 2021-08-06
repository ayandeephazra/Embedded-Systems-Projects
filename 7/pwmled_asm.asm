; pwmled.asm: Starter code, assembly PWM-LED functions
; Lab 7, ECE 266, Spring 2021
; Created by Zhao Zhang

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","pwmled.h"

                .text

; Field and any other declarations
TIMER0          .field  TIMER0_BASE
TIMER1          .field  TIMER1_BASE
PORT            .field  GPIO_PORTF_BASE
PIN_ROUTE_1     .field  GPIO_PF1_T0CCP1
PIN_ROUTE_2     .field  GPIO_PF2_T1CCP0
PIN_ROUTE_3     .field  GPIO_PF3_T1CCP1

;
; void ledPwmSet(int pulsePeriod, int pulseWidthRed, int pulseWidthGreen, int pulseWidthBlue)
;
; Set color of the on-board LED by setting the intensity of the primary colors
;
                .asmfunc
ledPwmSet       PUSH    {lr, r3, r2, r1, r0}        ; Save r1-r3 and lr

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER0, TIMER_B, pulsePeriod)
                ;   call TimerMatchSet(TIMER0, TIMER_B, pulsePeriodRed)
                ;   Note that r0, r1, r2 each holds a pwm_t type, with
                LDR     r0, TIMER0                  ; r0 = TIMER0
                MOV     r1, #TIMER_B                ; r1 = TIMER_B
                LDR     r2, [sp, #0]                ; r2 = pulsePeriod (pushed r0 in stack)
                BL      TimerLoadSet                ; call TimerLoadSet

                LDR     r0, TIMER0                  ; r0 = TIMER0
                MOV     r1, #TIMER_B                ; r1 = TIMER_B
                LDR     r2, [sp, #4]                ; r2 = pulseWidthRed (pushed r1 in stack)
                BL      TimerMatchSet

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER1, TIMER_A, pulsePeriod)
                ;   call TimerMatchSet(TIMER1, TIMER_A, pulsePeriodBlue)
                LDR     r0, TIMER1                  ; r0 = TIMER1
                MOV     r1, #TIMER_A                ; r1 = TIMER_A
                LDR     r2, [sp, #0]                ; r2 = pulsePeriod (pushed r0 in stack)
                BL      TimerLoadSet                ; call TimerLoadSet

                LDR     r0, TIMER0                  ; r0 = TIMER0
                MOV     r1, #TIMER_A                ; r1 = TIMER_A
                LDR     r2, [sp, #8]                ; r2 = pulseWidthRed (pushed r2 in stack)
                BL      TimerMatchSet               ; call TimerMatchSet

                ; Set the period and duty cycle length for the read sub-LED
                ;   call TimerLoadSet(TIMER1, TIMER_B, pulsePeriod)
                ;   call TimerMatchSet(TIMER1, TIMER_B, pulsePeriodGreen)
                LDR     r0, TIMER1                  ; r0 = TIMER1
                MOV     r1, #TIMER_B                ; r1 = TIMER_B
                LDR     r2, [sp, #0]                ; r2 = pulsePeriod (pushed r0 in stack)
                BL      TimerLoadSet                ; call TimerLoadSet

                LDR     r0, TIMER1                  ; r0 = TIMER1
                MOV     r1, #TIMER_B                ; r1 = TIMER_B
                LDR     r2, [sp, #12]               ; r2 = pulseWidthGreen (pushed r3 in stack)
                BL      TimerMatchSet               ; call TimerMatchSet

                ADD     sp, #16                     ; release the stack space for r3, r2, r1, r0
                POP     {pc}
                .endasmfunc

