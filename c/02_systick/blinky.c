/* 02_systick/blinky.c */

#include "stm32f10x.h"

#define STACK_TOP 0x20002000-4	

void nmi_handler(void);
void hardfault_handler(void);
void SysTickISR(void);
int main(void);

/* vector table */
unsigned int * myvectors[16]
__attribute__ ((section("vectors")))= {
    (unsigned int *) STACK_TOP,
    (unsigned int *) main,
    (unsigned int *) nmi_handler,
    (unsigned int *) hardfault_handler,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) 0,
    (unsigned int *) SysTickISR
};



int main(void) {
    int n = 0;
    int button;

    /* Enable the GPIOA (bit 2) and GPIOC (bit 8) */
    RCC->APB2ENR |= 0x10 | 0x04;

    /* Set GPIOC Pin 8 and Pin 9 to outputs */
    GPIOC->CRH = 0x11;

    /* Set GPIOA Pin 0 to input floating */
    GPIOA->CRL = 0x04;

    /* reload value of the systick timer */
    SysTick->LOAD = 0x200;

    /* enable systick timer */
    SysTick->CTRL = 0x7;

    while(1)
    {
        /* empty */
    }
}


void SysTickISR(void) {

    static int n = 0;
    int button;

    button = ((GPIOA->IDR & 0x1) == 0);
    /* Read the button - the button pulls down PA0 to logic 0 */
    n++; /* Count the delays */
    if (n & 1) {
        /* Copy bit 0 of counter into GPIOC:Pin 8 */
        GPIOC->BSRR = 1<<8 ;
    } else {
        GPIOC->BSRR = 1<<24;
    }
    if ((n & 4) && button) {
        /* Copy bit 4 of counter into GPIOC:Pin 9 if button pressed */
        GPIOC->BSRR = 1<<9 ;
    } else {
        GPIOC->BSRR = 1<<25;
    }
}


void nmi_handler(void)
{
    return ;
}


void hardfault_handler(void)
{
    return ;
}


/*
vim: et sw=4 ts=4
*/
