#ifndef _DOG_DISPLAY_STM32_H
#define _DOG_DISPLAY_STM32_H

/*

STM32 VL Discovery and DOGM162 Display

Connection:

    DOGM162             STM32 VL discovery
    VDD                 +3.3V
    VSS                 GND
    SI                  PB15 (SPI MOSI)
    CLK                 PB13 (SPI CLK)
    RS                  PB10 (or any other GPIO)
    RESET               PB11 (or any other GPIO)
    A1                  +5V (with a resistor!!!)
    C1                  GND
    
*/

//defines for interface
//if you want to use other pins or interfaces modify the defines below
#define DOG_SPI SPI2        //which spi interface is used
#define SPI 2               //number of the spi interface
#define RS_PORT GPIOB       //Important: clock for this port must be enabled manually before any init function is called
#define RS_PIN 10           //pin for the rs signal
#define RESET_PORT GPIOB    //Important: clock for this port must be enabled manually before any init function is called
#define RESET_PIN 11        //pin for the reset signal of the display
#define F_CPU 8000000UL     //clock frequency for delay functions


//includes
#include "stm32f10x.h"

//defines
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define DISPLAY_ON 0x0F
#define DISPLAY_OFF 0x08



//functions

void dog162_init(void);                                 //init a 2x16 dog display
void spi_init(void);                                    //init spi and rs pin
void dog_data(unsigned char data);                      //send a data byte to the dog
void dog_command(unsigned char command);                //send a command to the dog
void dog_string(unsigned char *data);                    //send a string to the dog
void dog_set_cursor(unsigned char x, unsigned char y);       //set the cursor to a given position
void spi_send(unsigned char);                           //blocking send over spi
void delay_us(unsigned int);                            //delay for display
void delay_ms(unsigned int);                            //delay for reset


#endif
