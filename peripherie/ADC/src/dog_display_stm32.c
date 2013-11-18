#include "dog_display_stm32.h"

//init spi interface and two pins for RS and RESET
void spi_init(void){
unsigned int i = 0;

//define RESET pin as an output (0b0011) -> push pull with 50 MHz
#if RESET_PIN<8
    i = RESET_PORT->CRL;
    i &= ~(0xF<<(RESET_PIN*4));
    i |= (0x3<<(RESET_PIN*4));
    RESET_PORT->CRL = i;
#else
    i = RESET_PORT->CRH;
    i &= ~(0xF<<(RESET_PIN-8)*4);
    i |= (0x3<<(RESET_PIN-8)*4);
    RESET_PORT->CRH = i;
#endif

//define RS Pin as an output (0b0011) -> push pull with 50 MHz
#if RS_PIN<8
    i = RS_PORT->CRL;
    i &= ~(0xF<<(RS_PIN*4));
    i |= (0x3 << (RS_PIN*4));
    RS_PORT->CRL = i;
#else
    i = RS_PORT->CRH;
    i &= ~(0xF << (RS_PIN-8)*4);
    i |= (0x3 << (RS_PIN-8)*4);
    RS_PORT->CRH = i;
#endif

//define SCK and MOSI as alternate function output (0b1011) -> Alternate Function push pull with 50 MHz
#if SPI == 1
    RCC->APB2ENR |= (1<<12);      //clock for SPI1
    i = GPIOA->CRL;
    i &= ~0x0F0FFFFF;
    i |= 0xB0B00000;
    GPIOA->CRL = i;
#else   //SPI2
    RCC->APB1ENR |= (1<<14);      //clock for SPI2
    i = GPIOB->CRH;
    i &= ~0xF0F00000;
    i |= 0xB0B00000;
    GPIOB->CRH = i;
#endif

    DOG_SPI->CR1 |= (0x300);    //software slave management (because no slave select is used)
    DOG_SPI->CR1 |= (0x40);     //spi enable
    DOG_SPI->CR1 |= (0x18);     //BRR = f(pclk)/16
    DOG_SPI->CR1 |= (0x07);     //set STM32 as master, clock is high when idle, data at rasising edge
//     | (0x40) | (0x18) | (0x7));//TODO: write plain
}

void dog_data(unsigned char data){
    RS_PORT->BSRR |= (1<<RS_PIN);    //set RS
    delay_us(10);
    spi_send(data);
    delay_us(200);
}

void dog_command(unsigned char command){
    RS_PORT->BSRR |= (1<<(RS_PIN+16));   //clear RS
    delay_us(10);
    spi_send(command);
    delay_ms(1);
}

void dog_string(unsigned char *data){
    while(*data){
        dog_data(*data);
        data++;
    }
}

// y=0: line 1, y=2: line 2
void dog_set_cursor(unsigned char x, unsigned char y){
    int address = 0x80;     //command for set DDRAM address
    
    if(y == 1)
        address+= 40;
    address += x;
    dog_command(address);
        
}


void dog162_init(void){
	//init on chip hardware
	spi_init();
	//generate a clear reset to bring the DOG display in init state
	RESET_PORT->BSRR |= (1<<(RESET_PIN+16));//clear reset
	delay_us(100);
	RESET_PORT->BSRR |= (1<<RESET_PIN);     //set reset
	
	RS_PORT->BSRR |= (1<<(RS_PIN+16));   //clear RS
    delay_ms(40);                   //wait after reset
    

    spi_send(0x38);                //obsolete?
    delay_us(40);

    spi_send(0x39);                //8 bit data length, 2 lines, instruction table 1

    delay_us(40);
    spi_send(0x14);                //BS: 1/5, 2 line LCD
    
    delay_us(40);
    spi_send(0x55);                //booster on, contrast C5, set C4
    
    delay_us(40);
    spi_send(0x6D);                //set voltage follower and gain
    
    delay_us(40);
    spi_send(0x78);                //set contrast C3, C2, C1
    
    delay_us(40);
    spi_send(0x38);                //switch back to instruction table 0
    
    delay_ms(200);                 //more delay for power stable
    spi_send(0xF);                 //display on, cursor on, cursor blink
    
    delay_us(40);
    spi_send(0x1);                 //delete display, cursor at home
    
    delay_us(40);
    spi_send(0x6);                 //cursor auto-increment
    
    delay_ms(1);                   //wait before the next instruction

}


void spi_send(unsigned char data){
    while(!(DOG_SPI->SR & 0x2));    //wait for empty data register
    DOG_SPI->DR = data;
}


void delay_us(unsigned int delay){
    delay *= 8000000UL/F_CPU;
    for(;delay;){
        --delay;
    }
}
 
void delay_ms(unsigned int delay){
    int i;
    for(i=0;i<delay;i++){
        delay_us(1000);
    }
}


