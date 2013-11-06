;;; Dimm the LED on the STM32Discovery ARM Cortex M3 board.

;;; Directives
        .thumb                  ; (same as saying '.code 16')
        .syntax unified

;;; Equates
        ;;; GPIO registers
        .equ GPIOC_CRH,   0x40011004
        .equ GPIOC_ODR,   0x4001100C
        .equ GPIOA_CRL,   0x40010800
        .equ GPIOA_CRH,   0x40010804
        .equ GPIOA_IDR,   0x40010808
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

        ;;; USART1 registers
        .equ USART1BaseAdress, 0x40013800
        .equ USART1_SR, USART1BaseAdress + 0x00
        .equ USART1_DR, USART1BaseAdress + 0x04
        .equ USART1_BRR, USART1BaseAdress + 0x08
        .equ USART1_CR1, USART1BaseAdress + 0x0C
        .equ USART1_CR2, USART1BaseAdress + 0x10
        .equ USART1_CR3, USART1BaseAdress + 0x14
        .equ USART1_GTPR, USART1BaseAdress + 0x18

        .equ LEDDELAY, 200
        .equ Counter, 0x20000000
        .equ GreenValue, 0x20000004
        .equ BlueValue, 0x20000008
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
        ;; Enable the Port A peripheral clock by setting bit 2
        ;; Enable the USART1 peripheral clock by setting bit 14
        ldr r6, = RCC_APB2ENR
        ldr r0, = 0x4014
        str r0, [r6]

        ;; Set the config and mode bits for Port C bit 12 so it will
        ;; be a push-pull output (up to 50 MHz) by setting bits 19-16
        ;; to '0011'.

        ldr r6, = GPIOC_CRH
        ldr r0, = 0x44444433   ; PC8, PC9
        str r0, [r6]

        ;; on Port A is the Button
        ;; PA0: Set as floating input (0b0100)
        ldr r6, = GPIOA_CRL
        ldr r0, [r6]
        ldr r1, = 0xFFFFFFF4
        and r0, r1
        str r0, [r6]

        ;; On Port A are the RX and TX Pins of USART1
        ;; RX: A10, set as Floating input (0b0100) (reset state)
        ;; RX:  A9, set as Alternate Function output Push-pull (0b1011)
        ldr r6, = GPIOA_CRH
        ldr r0, [r6]
        ldr r1, = 0xFFFFFF0F
        and r0, r1
        ldr r1, = 0x000000B0
        orr r0, r1
        str r0, [r6]

        ;; set the load value for the systick timer
        ldr r5, = STK_LOAD
        ldr r0, = LEDDELAY
        str r0, [r5]

        ;; start the systick timer
        ldr r5, = STK_CTRL
        ldr r0, = 0x7
        str r0, [r5]

        ;; set the baudrate for USART1
        ldr r5, = USART1_BRR
        ldr r0, = 0x340     ; set baud to 9k6
        str r0, [r5]

        ;; enable USART 1 for rx and tx
        ldr r5, = USART1_CR1
        ldr r0, = 0x200C    ; usart enable, tx and rx enable
        str r0, [r5]

        ;; some pointers for polling USART1
        ldr r5, = USART1_DR
        ldr r4, = USART1_SR
        ldr r3, = GreenValue

loop:
        ldr r0, [r4]
        ldr r1, = 0x00000020
        and r0, r1      ; check if a byte is received
        cmp r0, 0x0
        beq loop
        ldr r0, [r5]    ; if then store it on both pwm values
        str r0, [r3]
        add r3, 0x4
        str r0, [r3]
        sub r3, 0x4
        b loop


_sys_tick_handler:
        push {r0-r5}            ; save all registers
        ldr r5, = Counter       ; load the pwm counter
        ldrb r0, [r5]
        add r5, 0x4             ; load the pwm values
        ldrb r1, [r5]
        add r5, 0x4
        ldrb r2, [r5]
        ldr r5, = GPIOC_ODR     ; load the current port state
        ldr r3, [r5]

        add r0, 0x1             ; increment the pwm counter
        cmp r0, 0xFF
        bne _compare            ; if 0xFF reset the counter
        mov r0, 0x0
        mov r4, 0x0
        and r3, r4              ; switch off both leds
_compare:
        cmp r1, r0              ; compare counter and pwm value
        bhs _compare_2          ; if equal or higher switch led on
        orr r3, 0x200
_compare_2:
        cmp r2, r0
        bhs _compare_3
        orr r3, 0x100
_compare_3:
        str r3, [r5]            ; set the output
        ldr r5, =Counter        ; save new counter state
        str r0, [r5]
        
        add r7, 0x1             ; check every 0x2FF SysTick calls the button
        ldr r0, = 0x2FF
        cmp r7, r0
        bne _end
        mov r7, 0x0
        ldr r5, = GPIOA_IDR
        ldr r0, [r5]
        and r0, 0x1
        cmp r0, 0x0             ; if pressed increment the pwm values
        beq _end
        add r1, 0x1
        add r1, 0x1
        ldr r5, = GreenValue
        str r1, [r5]
        add r5, 0x4
        str r1, [r5]

_end:
        pop {r0-r5}             ; get saved registers
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

