#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "core_cm3.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"

int color[12] =
{WHITE, CYAN, BLUE, RED, MAGENTA, LGRAY, GREEN, YELLOW, BROWN, BRRED, GRAY, BLACK};

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void DMA_Configure(void);

volatile uint32_t ADC_Value[1];


//---------------------------------------------------------------------------------------------------

void RCC_Configure(void) // stm32f10x_rcc.c, .h 참고, 아래에 주석이 묶여있는 기능끼리 합쳐서 함수에 매개변수 전달 하시오!!
{
	/* DMA1 Clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* ADC1, Port B Clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE); 
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void) // stm32f10x_gpio.c, .h 참고
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    /* ADC1 pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PB0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //아날로그 입력 모드
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void ADC_Configure(void) // stm32f10x_gpio.c, .h 참고
{
    ADC_InitTypeDef ADC_InitStructure; //ADC_Configure
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);

    ADC_DMACmd(ADC1, ENABLE); // 일반 Interrupt 방식 대신 DMA 방식으로 !
    
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void DMA_Configure(void) { // stm32f10x.h, misc.c, .h 참고
  
    DMA_InitTypeDef DMA_InitStructure;
    /*DMA_PriorityGroupConfig(DMA_PriorityGroup_0);*/
    
    // DMA
    DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR; //ADC1의 Data Register
    DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)&ADC_Value[0]; //ADC1의 값을 어디에 쓸지
    
    DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC; //peripheral to CPU
    DMA_InitStructure.DMA_BufferSize=2; //peripheral과 memory word를 받으므로 2
    DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable; //DMA_peripheral_incremented_mode Disable
    DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable; //DMA_memory_incremented_mode Enable
    DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word; //word만큼 할당
    DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode=DMA_Mode_Circular; //Circular Mode
    DMA_InitStructure.DMA_Priority=DMA_Priority_High; //우선순위 High
    DMA_InitStructure.DMA_M2M=DMA_M2M_Disable; //Memory to Memory Disable
    
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1,  ENABLE);
}



int main(void)
{
    u16 bgColor = WHITE; //background Color default WHITE
    
    SystemInit();

    RCC_Configure();

    GPIO_Configure();
    
    ADC_Configure();

    DMA_Configure();
    
    LCD_Init();
    
    Touch_Configuration();
    
    Touch_Adjust();
    
    LCD_Clear(WHITE);
    
    while (1) {
      if(ADC_Value[0] >= 2400 && bgColor == WHITE){ //조도센서값이 2400이상이고 배경이 WHITE면  GRAY로 바꾼다.
        bgColor = GRAY;
        LCD_Clear(bgColor);
      }
      else if(ADC_Value[0] < 2400 && bgColor == GRAY){ //조도센서값이 2400미만이고 배경이 GRAY면  WHITE로 바꾼다.
        bgColor = WHITE;
        LCD_Clear(bgColor);
      }
      LCD_ShowString(1, 1, "MON_Team09", BLACK, bgColor);
      LCD_ShowNum(200, 1,ADC_Value[0], 4, BLACK, bgColor); //조도센서 값 출력
    }
    return 0;

}