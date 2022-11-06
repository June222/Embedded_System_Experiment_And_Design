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
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);

void Delay(void);

void sendDataUART1(uint16_t data);

//---------------------------------------------------------------------------------------------------

void RCC_Configure(void) 
{
	/* Port A, C, D, E clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
    /* ADC1 enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    /* ADC1 pin setting */
    //PC1, Input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PC1/ADC12_IN8/ETH_MDC
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //�Ƴ��α� �Է� ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void ADC_Configure(void) // stm32f10x_gpio.c, .h ����
{
    ADC_InitTypeDef ADC_InitStructure; //ADC_Configure
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void NVIC_Configure(void) { // stm32f10x.h, misc.c, .h ����
  
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;// ADC1_2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

uint16_t value = 0;

void ADC1_2_IRQHandler() {
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC)!=RESET){
      value = ADC_GetConversionValue(ADC1);
      ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}


int main(void)
{
    uint16_t pos_x = 0;
    uint16_t pos_y = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    SystemInit();

    RCC_Configure();

    GPIO_Configure();
    
    ADC_Configure();

    NVIC_Configure();
    
    LCD_Init();
    
    Touch_Configuration();
    
    Touch_Adjust();
    
    LCD_Clear(WHITE);
    
    while (1) {
      LCD_ShowString(1, 1, "MON_Team09", BLACK, WHITE);
      LCD_ShowNum(200, 1, value, 4, BLACK, WHITE); //��������
      
      Touch_GetXY(&pos_x, &pos_y, 1);
      Convert_Pos(pos_x, pos_y, &x, &y);
      LCD_DrawCircle(x, y, 5);
      LCD_ShowNum(50, 50, pos_x, 4, BLACK, WHITE); //x��ǥ
      LCD_ShowNum(30, 30, pos_y, 4, BLACK, WHITE); //y��ǥ
    }
    return 0;

}