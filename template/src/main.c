/*************************************************************************************
* Test-program for STM32VLDiscovery, evaluation board for “STM32F100RBT6”.
* After program start green LED and blue LED will blink.
*
* for details see http://www.seng.de/downloads/HowTo_ToolChain_STM32_Ubuntu.pdf
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
* Setup compiler optimizer setting to "-O0" in file "Makefile.common"
*************************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


int main(int argc, char *argv[])
{
	GPIO_InitTypeDef GPIO_InitStructure;
	u32 delay;

	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PC8 to mode: fast rise-time, pushpull output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // GPIO No. 8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // fast rise time
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOC, &GPIO_InitStructure); // GPIOC init

	/* Configure PC9 the same way */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	while(1)
	{
		/* make some float calculations */
		/*float x = 42, y = 23, z = 7;
		int i = 0;
		for ( i = 0; i < 6; i++ )
		{
			z = (x*y)/z;
		};*/

		/* GPIO PC8 set, pin=high, GPIO PC9 reset, pin=low */
		GPIOC->BSRR = GPIO_BSRR_BS8;
        GPIOC->BSRR = GPIO_BSRR_BR9;

		/* delay --> compiler optimizer settings must be "-O0" */
		delay=500000;
		while(delay)
			delay--;

		/* GPIO PC8 reset, pin=low, GPIO PC9 set, pin=high */
		GPIOC->BSRR = GPIO_BSRR_BR8;
        GPIOC->BSRR = GPIO_BSRR_BS9;
	
		/* delay --> compiler optimizer settings must be "-O0" */
		delay=500000;
		while(delay)
			delay--;
	}
}


/* 
vim: tags=tags,../tags
*/
