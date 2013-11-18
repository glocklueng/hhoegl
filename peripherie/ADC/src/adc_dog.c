//  STM32 Discovery ADC Sample
//
//  reads the value on PA0 (ADC 0) and print a decimal number on the screen


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

    int temp;
    char buffer[5];

    buffer[4] = 0;              //terminate string

	RCC->APB2ENR |= 0x8;                       //enable GPIOB (for RS and Reset signal)
    RCC->APB2ENR |= 0x4;                        //enable GPIOA (analog input)
    RCC->APB2ENR |= 0x200;                      //enable ADC1


    GPIOA->CRL = 0x44444440;             //PA0 analog mode
    
    ADC1->CR2 = 0x800003;            //temp sensor enable, continuous conversion, adc on
    ADC1->SMPR1 = 0x1C0000;          //Channel 16 sample time 252 cycles
    ADC1->SMPR2 = 0x1;               //Channel 0 sample time 14 cycles


    dog162_init();                              //init spi, gpios and the dog display
    

    while(1){                                   //loop forever
        ADC1->CR2 |= 0x1;                       //start adc
        while(!(ADC1->SR & 0x2)){}              //wait for conversion
        temp = ADC1->DR;                        //read result
        tiny_itoa(temp, buffer);                //generate a ascii string
        dog_command(CLEAR_DISPLAY);             //clear display
        dog_string("ADC0: \0");                 //write a string
        dog_string(buffer);                     //write the result
        delay_ms(500);                          //wait before next sequence
    }
}

void tiny_itoa(int number, char *buff){
    int i, temp;

    for(i=0;i<4;i++)                            //set all to ascii 0
        buff[i] = '0';

    i = 3;                                      //calculate four digits
    while(number > 0 && i >= 0){
        temp = number%10;                       //take the last digit
        buff[i] = temp+'0';                     //place it in the string (right to left)
        number = number/10;                     //delete this digit from the number
        i--;                                    //address next digit
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

