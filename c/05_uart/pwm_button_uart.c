
#include "stm32f10x.h"

#define STACK_TOP 0x20002000-4										// init Stackpointer

void nmi_handler(void);												
void hardfault_handler(void);
void SysTickISR(void);
int main(void);

/* vector table */

unsigned int * myvectors[16] 
__attribute__ ((section("vectors")))= {
    (unsigned int *)	STACK_TOP,         
    (unsigned int *) 	main,              
    (unsigned int *)	nmi_handler,       
    (unsigned int *)	hardfault_handler,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    0,
    (unsigned int *)    SysTickISR
};

unsigned char green,blue;		//values for brightness

int main(void){	
	int n = 0;
	int button;

	RCC->APB2ENR |= 0x10 | 0x04 | 0x4000;               //enable GPIOA, GPIOC and USART1
	GPIOC->CRH = 0x11;									/* Set GPIOC Pin 8 and Pin 9 to outputs */
	GPIOA->CRL = 0x04;									/* Set GPIOA Pin 0 to input floating */
    
    green = 0xF0;
    blue = 0xF0;

    USART1->BRR = 0x340;                                //set baud to 9k6
    USART1->CR1 = 0x200C;                               //enable uart, rx and tx

    SysTick->LOAD = 0x200;                              //reload value of the systick timer
    SysTick->CTRL = 0x7;                                //enable systick timer

    while(1)
    {
        if(USART1->SR & 0x20){                          //if a new byte is received
            green = USART1->DR;                         //update the brightness values
            blue = green;
        }
    }
}


void SysTickISR(void){
    static unsigned int counter = 0;                    //counter for pwm
    static unsigned int button_count = 0;               //counter for button
    
    counter++;
    
    if(counter > 0xFF){                                 //reset the counter at 0xFF
        counter = 0;
        GPIOC->BSRR = ((1<<25) | (1<<24));              //switch off both LEDs
    }
    if(counter > blue)
        GPIOC->BSRR = (1<<8);                           //if brightness is reached switch on the leds
    if(counter > green)
        GPIOC->BSRR = (1<<9);
    
    button_count++;
    
    if(button_count > 0x1FF){                          //reset the counter at 0x1ff
        button_count = 0;
        if((GPIOA->IDR & 0x1)){                        //increment the brightness if the button is pressed
            green++;
            blue++;
        }
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

