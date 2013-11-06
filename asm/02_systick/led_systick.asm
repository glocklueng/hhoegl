;;; Blink the LED on the STMDiscovery ARM Cortex M3 board.

;;; Directives
        .thumb                  ; (same as saying '.code 16')
        .syntax unified

;;; Equates
        ;;; GPIO registers
        .equ GPIOC_CRH,   0x40011004
        .equ GPIOC_ODR,   0x4001100C
        ;;; Clock enable register
        .equ RCC_APB2ENR, 0x40021018
        ;;; load value stack pointer
        .equ STACKINIT,   0x20002000    ; End of RAM (0x20000000 + 8kB)
        ;;; SysTick registers
        .equ STKBaseAdress, 0xE000E010
        .equ STK_CTRL, STKBaseAdress + 0x00
        .equ STK_LOAD, STKBaseAdress + 0x04
        .equ STK_VAL, STKBaseAdress + 0x08
        .equ STK_CALIB, STKBaseAdress + 0x0C
         
        .equ LEDDELAY, 800000
        .equ LEDON, 0x0
        .equ LEDOFF, 0x0300
        .equ LEDState, 0x20000000
.section .text
        .org 0

;;; Vectors
vectors:
        .word STACKINIT         ; stack pointer value when stack is empty
        .word _start + 1        ; reset vector (manually adjust to odd for thumb)
        .word _nmi_handler + 1  ; exception handlers
        .word _hard_fault  + 1  ;
        .word _memory_fault + 1 ;
        .word _bus_fault + 1    ;
        .word _usage_fault + 1  ;
        .org 0x3C
        .word _sys_tick_handler +1  ; systick handler

        .org 0x140
_start:

        ;; Enable the Port C peripheral clock by setting bit 4
        ldr r6, = RCC_APB2ENR
        mov r0, 0x10
        str r0, [r6]

        ;; Set the config and mode bits for Port C bit 12 so it will
        ;; be a push-pull output (up to 50 MHz) by setting bits 19-16
        ;; to '0011'.

        ldr r6, = GPIOC_CRH
        ;ldr r0, = 0x44443444
        ldr r0, = 0x44444433   ; PC8, PC9
        str r0, [r6]

        ;; set the load value for the systick timer
        ldr r5, = STK_LOAD
        ldr r0, = LEDDELAY
        str r0, [r5]

        ;; start the systick timer
        ldr r5, = STK_CTRL
        ldr r0, = 0x7
        str r0, [r5]

        ;; some pointers
        ldr r5, = LEDState
        ldr r6, = GPIOC_ODR


loop:
        b loop


_sys_tick_handler:
        ldr r0, [r5]    ; get current state
        cmp r0, 0x0     ; compare if off or on
        beq _led_off    
        ldr r0, = LEDON ; if off set on
        str r0, [r5]
        str r0, [r6]
        bx lr
_led_off:
        ldr r0, = LEDOFF; if on set off
        str r0, [r5]
        str r0, [r6]
        bx lr


_dummy:                        ; if any int gets triggered, just hang in a loop
        b _dummy

_nmi_handler:
        b _nmi_handler

_hard_fault:
        b _hard_fault

_memory_fault:
        b _memory_fault

_bus_fault:
        b _bus_fault

_usage_fault:
        b _usage_fault

