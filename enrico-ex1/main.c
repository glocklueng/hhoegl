/** 
   
  Date:        April 2011 
  Author:     Enrico Marinoni
  MCU:        STM32F100RBT6B 
  EvaBoard:    STM32FVL Discovery  
              http://www.emcu.it/STM32Discovery/STM32ValueLineDiscovery.html 
   
  STM Library Version:    V3.4.0 
  KEIL Version:            uVision4                        
                        IDE-Version: µVision V4.10 
                        Toolchain:        RealView MDK-ARM  Version: 4.12 
                        Toolchain Path:    BIN40\ 
                        C Compiler:         Armcc.Exe         V4.1.0.481 [Evaluation] 
                        Assembler:          Armasm.Exe        V4.1.0.481 [Evaluation] 
                        Linker/Locator:     ArmLink.Exe       V4.1.0.481 [Evaluation] 
                        Librarian:          ArmAr.Exe         V4.1.0.481 [Evaluation] 
                        Hex Converter:      FromElf.Exe       V4.1.0.481 [Evaluation] 
                        CPU DLL:            SARMCM3.DLL       V4.12 
                        Dialog DLL:         DARMSTM.DLL       V1.47 
                        Target DLL:         STLink\ST-LINKIII-KEIL.dll       V1.5.1 
                        Dialog DLL:         TARMSTM.DLL       V1.47 
 
 
  Software Description: 
                       Configure SYSCLK (System clock) to 24Mhz 
                       Send to the MCO (PA8) the SYSCLK 
                       Configure LD3 & LD4 (Led) 
                       LD4 flashes    according with ADC measure 
                       ADC (used on ADC1_IN10 pin PC0) in DMA mode that determines the speed of LD4 flashing 
                    Push Button-BLUE (B1 USER) if is press the LD3 go ON 
                    USART1 is configure to send the status of Push Button-BLUE (B1 USER - see the variable: RxChar) 
                            The USART1 SetUp is: 
                                USART1 used in UART1 mode 
                                BaudRate 9600 
                                  WordLength_8b 
                                  StopBits_1 
                                  Parity_No 
                                  FlowControl_None 
                    SPI1 (used on PA5 (CLK), PA6 (MISO), PA7 (MOSI). 
                            SPI is configure to send the status of Push Button-BLUE (B1 USER - see the variable: SPI1val) 

  * 
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS 
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE 
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY 
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING 
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS. 
  * 
  */  
 
/* Includes ------------------------------------------------------------------*/ 
#include "stm32f10x.h" 
#include "stdio.h" 
/* Private typedef -----------------------------------------------------------*/ 
/* Private define ------------------------------------------------------------*/ 
#define ADC1_DR_Address    ((uint32_t)0x4001244C)        // Used for ADC      
/* Private macro -------------------------------------------------------------*/ 
/* Private variables ---------------------------------------------------------*/ 
GPIO_InitTypeDef GPIO_InitStructure; 
RCC_ClocksTypeDef RCC_ClockFreq; 
ErrorStatus HSEStartUpStatus; 
ADC_InitTypeDef ADC_InitStructure;                      // Used for ADC 
DMA_InitTypeDef DMA_InitStructure;                     // Used for ADC 
__IO uint16_t ADCConvertedValue;                        // Used for ADC 
USART_InitTypeDef USART_InitStructure;           // Used for USART 
u8 RxChar=0x0;                                                          // Used for USART 
SPI_InitTypeDef  SPI_InitStructure;                        // Used for SPI 
u16 SPI1val;                                                               // Used for SPI 
/* Private function prototypes -----------------------------------------------*/ 
void Delay(__IO uint32_t nCount); 
void SetSysClockTo24(void);        // Configure System Clock to HSE at 24MHz (external 8MHz crystal)  
void EnableCLK(void);            // Enable CLK to PORTs and Peripherals 
void GPIO_Configuration(void);    // Configure the I/O 
void ConfigureDMAforADC(void);  // Configure DMA for ADC 
void NVIC_Configuration(void);    // Used for USART 
void ConfigureUART1(void);        // Used for USART 
void ConfigureSPI1(void);        // Used for SPI 
/* Private functions ---------------------------------------------------------*/ 
 
int main(void) 
{ 
 
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,   
     initialize the PLL and update the SystemFrequency variable. */ 
  SystemInit(); 
   
  // Sets System clock frequency to 24MHz 
  SetSysClockTo24(); 

  // Enable CLK to PORTs and Peripherals 
  EnableCLK(); 

  // Configure the I/O 
  GPIO_Configuration();  
 
  // Configure DMA for ADC 
  ConfigureDMAforADC(); 
 
  // Configure USART1 
  ConfigureUART1(); 
 
  // Configure SPI 
  ConfigureSPI1();  
 
     
  while (1) 
  { 
 
    /* Turn on LD4 - PC8 */ 
    GPIO_SetBits ( GPIOC,  GPIO_Pin_8 ); 
    /* Insert delay */ 
    Delay(ADCConvertedValue*100); 
    /* Turn off LD4 - PC8 */ 
    GPIO_ResetBits ( GPIOC,  GPIO_Pin_8 ); 
    /* Insert delay */ 
    Delay(ADCConvertedValue*100); 
 
 
    // Read BLUE Botton 
      if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) 
        { 
        GPIO_ResetBits ( GPIOC,  GPIO_Pin_9 ); 
        USART_SendData(USART1, '1'); 
        SPI_I2S_SendData  (SPI1, 0x1);  
        } 
    else 
        { 
        GPIO_SetBits ( GPIOC,  GPIO_Pin_9 ); 
        USART_SendData(USART1, '0'); 
        SPI_I2S_SendData  (SPI1, 0x0);  
        } 
 
    // Read SPI1 
    SPI1val=SPI_I2S_ReceiveData(SPI1);   
 
  } 
} 
 
 
 
 
// +++++++++ SetUp +++++++++++++++++++++++++++++++++++++++++++ 
 
void Delay(__IO uint32_t nCount) 
{ 
  for(; nCount != 0; nCount--); 
} 
 
 
// 
// Configure SPI 
// 
void ConfigureSPI1(void)         
{ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 
  SPI_Init(SPI1, &SPI_InitStructure); 
  SPI_Cmd(SPI1, ENABLE);           /* Enable the SPI  */   
} 
 
 
// 
// Configure UART1 
// 
void ConfigureUART1(void) 
{ 
  /* USART1 configured as follow: 
        - BaudRate = 9600 baud   
        - Word Length = 8 Bits 
        - ONE Stop Bit 
        - NO parity 
        - Hardware flow control disabled (RTS and CTS signals) 
        - Receive and transmit enabled 
  */ 
  USART_InitStructure.USART_BaudRate = 9600; 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
  USART_InitStructure.USART_StopBits = USART_StopBits_1; 
  USART_InitStructure.USART_Parity = USART_Parity_No; 
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_Init(USART1, &USART_InitStructure);       // Configure the USART1  
 
/* Enable the USART Transmit interrupt: this interrupt is generated when the  
   USART1 transmit data register is empty */   
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
 
/* Enable the USART Receive interrupt: this interrupt is generated when the  
   USART1 receive data register is not empty */ 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
 
  /* Enable USART1 */ 
  USART_Cmd(USART1, ENABLE); 
} 
 
 
// 
// Configure DMA for ADC 
//  
void ConfigureDMAforADC(void) 
{ 
  /* DMA1 channel1 configuration ----------------------------------------------*/ 
  DMA_DeInit(DMA1_Channel1); 
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; 
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue; 
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
  DMA_InitStructure.DMA_BufferSize = 1; 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
   
  /* Enable DMA1 channel1 */ 
  DMA_Cmd(DMA1_Channel1, ENABLE); 
   
  /* ADC1 configuration ------------------------------------------------------*/ 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
  ADC_InitStructure.ADC_NbrOfChannel = 1; 
  ADC_Init(ADC1, &ADC_InitStructure); 
 
  /* ADC1 regular channel 10 configuration */  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5); 
 
  /* Enable ADC1 DMA */ 
  ADC_DMACmd(ADC1, ENABLE); 
   
  /* Enable ADC1 */ 
  ADC_Cmd(ADC1, ENABLE); 
 
  /* Enable ADC1 reset calibaration register */    
  ADC_ResetCalibration(ADC1); 
  /* Check the end of ADC1 reset calibration register */ 
  while(ADC_GetResetCalibrationStatus(ADC1)); 
 
  /* Start ADC1 calibaration */ 
  ADC_StartCalibration(ADC1); 
  /* Check the end of ADC1 calibration */ 
  while(ADC_GetCalibrationStatus(ADC1)); 
      
  /* Start ADC1 Software Conversion */  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
} 
 
 
// 
// Configure the I/O 
// 
void GPIO_Configuration(void) 
{ 
  /* Configure Leds (PC8 & PC9) mounted on STM32 Discovery board - OutPut Push Pull*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
  // Configure BLUE Botton (B1 User - PA0) on STM32 Discovery board - Input Floatting 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  // Configure ADC on ADC1_IN10    pin PC0 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
  /* Configure USART1 Tx (PA9) as alternate function push-pull */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
  /* Configure USART1 Rx (PA10) as input floating */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
  // Configure SPI - CLK PA5, MISO PA6, MOSI PA7 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
} 
 
 
/** 
  * Sets System clock frequency to 24MHz and configure HCLK, PCLK2  
  *   and PCLK1 prescalers.  
  * param  None 
  * retval None 
  */ 
void SetSysClockTo24(void) 
{ 
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------*/    
  /* RCC system reset(for debug purpose) */ 
  RCC_DeInit(); 
 
  /* Enable HSE */ 
  RCC_HSEConfig(RCC_HSE_ON); 
 
  /* Wait till HSE is ready */ 
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 
 
  if (HSEStartUpStatus == SUCCESS) 
  { 
    /* Flash 0 wait state */ 
    FLASH_SetLatency(FLASH_Latency_0); 
    /* HCLK = SYSCLK */ 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  
   
    /* PCLK2 = HCLK */ 
    RCC_PCLK2Config(RCC_HCLK_Div1);  
 
    /* PCLK1 = HCLK */ 
    RCC_PCLK1Config(RCC_HCLK_Div1); 
     
    /* PLLCLK = (8MHz/2) * 6 = 24 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_HSE, RCC_PREDIV1_Div2); 
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6); 
 
    /* Enable PLL */  
    RCC_PLLCmd(ENABLE); 
 
    /* Wait till PLL is ready */ 
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) 
    { 
    } 
 
    /* Select PLL as system clock source */ 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
 
    /* Wait till PLL is used as system clock source */ 
    while (RCC_GetSYSCLKSource() != 0x08) 
    { 
    } 
  } 
  else 
  {
    /* If HSE fails to start-up, the application will have wrong clock configuration. 
       User can add here some code to deal with this error */     
    /* Go to infinite loop */ 
    while (1) 
    { 
    } 
  } 
} 
 
 
// 
// Enable CLK to PORTs and Peripherals 
// 
void EnableCLK(void) 
{ 
 
  // Enable CLK to port ADC1, GPIOC/A, USART1, SPI1  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1  
                          | RCC_APB2Periph_SPI1, ENABLE); 
 
  /* Enable DMA1 clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
 
  /* Fills the RCC_ClockFreq structure with the current 
     frequencies of different on chip clocks (for debug purpose) */ 
  RCC_GetClocksFreq(&RCC_ClockFreq); 
 
} 
 
 
 
#ifdef  USE_FULL_ASSERT 
void assert_failed(uint8_t* file, uint32_t line) 
{  
  /* User can add his own implementation to report the file name and line number, 
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */ 
 
  /* Infinite loop */ 
  while (1) 
  { 
  } 
} 
#endif 

/***********************END OF FILE****/
