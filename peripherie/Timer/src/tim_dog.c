//  STM32 Discovery PWM sample
//
//  generates a pwm at Port B8 with Timer 4 (OC3)

#include "stm32f10x.h"

#define STACK_TOP 0x20002000-4										// init Stackpointer

void nmi_handler(void);												
void hardfault_handler(void);

int main(void);

/* vector table */

unsigned int * myvectors[4] 
__attribute__ ((section("vectors")))= {
    (unsigned int *)	STACK_TOP,         
    (unsigned int *) 	main,              
    (unsigned int *)	nmi_handler,       
    (unsigned int *)	hardfault_handler,

};

int main(void){	

    int temp;
    char buffer[5];

    buffer[4] = 0;              //terminate string

	RCC->APB2ENR |= 0x8;                       //enable GPIOB (for RS and Reset signal) and pwm output
    RCC->APB1ENR |= 0x4;                        //enable Timer4
    GPIOB->CRH &= 0xFFFFFFF0;                   //configure pwm output
    GPIOB->CRH |= 0x0000000B;                //PB8 AFIO push pull 50 MHz

    TIM4->CR1 = 0x81;             //auto reload preload enable, enable counter
    TIM4->CCMR2 = 0x68;          //output compare mode 3: pwm 1, output compare 3 preload enable
    TIM4->CCER = 0x100;           //Capture/Compare 3 ouput enable
    TIM4->ARR = 0xFFFE;          //auto reload value +1
    TIM4->EGR = 0x9;             //capture/compare 3 generation, set update generation

    TIM4->CCR3 = 0xF000;         //pwm value
    
    
    while(1){                                   //loop forever
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

