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


void RCC_Configure(void) 
{
	/* Port A, D clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
  /* TIM2 enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

     /* TIM2 setting */
    //TIM2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // PA1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /* LED pin setting */
    //PD2, 3, Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // PD2, PD3
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void TIM_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    TIM_TimeBaseStructure.TIM_Period = 1200;
    TIM_TimeBaseStructure.TIM_Prescaler = 60000;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void NVIC_Configure(void) {
  
    NVIC_InitTypeDef NVIC_InitStructure;
    // TIM2
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

FunctionalState LEDTOGGLE = DISABLE;
FunctionalState LED1 = DISABLE;
FunctionalState LED2 = DISABLE;
int TIME = 0;

void TIM2_IRQHandler() {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){
      if(LEDTOGGLE == ENABLE){
        TIME++;
        if(TIME % 1 == 0){
          LED1 = !LED1;
          if(LED1 == ENABLE){
            GPIOD->ODR |= GPIO_Pin_2;
          }
          else{
            GPIOD->ODR &= ~(GPIO_Pin_2);
          }
        }
            
      if(TIME % 5 == 0){
        LED2 = !LED2;
        if(LED2 == ENABLE){
          GPIOD->ODR |= GPIO_Pin_3;
        }
        else{
          GPIOD->ODR &= ~(GPIO_Pin_3);
        }
      }
      if(TIME % 50000 == 0){TIME = 0;} 
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
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
    TIM_Configure();
    NVIC_Configure();
    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
    LCD_Clear(WHITE);
    
    while (1) 
    {
      LCD_ShowString(1, 1, "MON_Team09", BLACK, WHITE);
      LCD_ShowString(1, 20, LEDTOGGLE==ENABLE?"ON ":"OFF", BLACK, WHITE);
      LCD_ShowString(100, 100, "BUT", BLACK, WHITE);
      LCD_DrawRectangle(80, 80, 120, 120);
      
      Touch_GetXY(&pos_x, &pos_y, 1);
      Convert_Pos(pos_x, pos_y, &x, &y);
  
      if(x >= 80 && x <= 120 && y >=80 && y <= 120)
      {
        if(LEDTOGGLE == ENABLE){LEDTOGGLE = DISABLE;}
        else {LEDTOGGLE = ENABLE;}
      }
    }
    return 0;
}