#include "stm32f10x.h"
#include "dog_display_stm32.h"

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

	RCC->APB2ENR |=  0x8;                       //enable GPIOB (for RS and Reset signal)

    dog162_init();                              //init spi, gpios and the dog display
    
    dog_string("STM32 at HSA\0");               //write a string in the first line
    dog_set_cursor(0,1);                        //set the cursor on line 2
    dog_string("...have fun!\0");               //write another string in the second line
    

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

