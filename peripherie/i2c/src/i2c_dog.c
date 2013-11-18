//  STM32 Discovery I2C Sample
//
//  reads the temperature of a LM75 sensor on I2C1 bus and print it on a DOG display


#include "stm32f10x.h"
#include "dog_display_stm32.h"

#define STACK_TOP 0x20002000-4										// init Stackpointer

void get_temp(void);

void nmi_handler(void);												
void hardfault_handler(void);
    
    unsigned char temp = 0;
    unsigned char temp2 = 0;



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

    int foo;
    char buffer[5];

    buffer[4] = 0;              //terminate string

	RCC->APB2ENR |= 0x8;                       //enable GPIOB (for RS and Reset signal) and for i2c
    RCC->APB1ENR |= 0x200000;                    //enable i2c1

    GPIOB->CRL |= 0xFF000000;               //PB6 and PB7 as alternate function open drain

    //init i2c1

    I2C1->CR2 = 0x8;                    //Periphal clock frequency 8M

    I2C1->CCR = 0x28;                   //100kHz
    I2C1->TRISE = 0x8;                  //rise time
    I2C1->CR1 = 0x401;                    // Periphal enable
   
    dog162_init();                              //init spi, gpios and the dog display
    

    while(1){                                   //loop forever
        dog_command(CLEAR_DISPLAY);             //clear display
        get_temp();                             //get current temperature
        tiny_itoa(temp, buffer);                //convert to ascii
        dog_string("T: \0");                    //write on display
        dog_string(buffer); 
        dog_string(".\0");                      //seperator
        if(temp2 & 0x80)                        //if highest bit of the second byte is set
            dog_string("5\0");                  //write a 5
        else
            dog_string("0\0");                  //else a 0
        dog_data(0xdf);                         //°
        dog_data('C');
        
        delay_ms(500);                          //wait before next sequence
    }
}

void get_temp(void){

    int a;
   //read lm75
    I2C1->CR1 |= 0x500;              //send start, ACK enable

    while(!(I2C1->SR1 & 0x1)){}     //wait until start condition is set (start bit/master mode)

    I2C1->DR = 0x91;                //send address in read mode
    while(!(I2C1->SR1 & 0x2)){}     //wait until address was sent (addr bit)
    a = I2C1->SR2;                  //read SR2 to clear the addr bit

    while(!(I2C1->SR1 & 0x40)){}    //wait for the first byte

    temp = I2C1->DR;                //get temperature
    I2C1->CR1 &= ~0x400;            //remove ack

    while(!(I2C1->SR1 & 0x40)){}    //wait for the next byte

    I2C1->CR1 |= 0x200;             //stop
    temp2 = I2C1->DR;               //read the lower byte (.x)

}

void tiny_itoa(int number, char *buff){
    int i, temp;

    for(i=0;i<4;i++)                            //set all to ascii blank
        buff[i] = ' ';

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

