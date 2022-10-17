/*
    코드 옆에  " // 📌  " 를 표시한 부분이 이번 주차에서 추가 및 변경한 내용입니다.
*/

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

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
/////////////////////// TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'///////////////////////
	/* UART 1 TX/RX port clock enable, USART1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
    /* UART 2 enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  // 📌 실험 과정(1) UART2 ENABLE 하기 
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void) // stm32f10x_gpio.c,.h 
{
    GPIO_InitTypeDef GPIO_InitStructure;

/////////////////////// TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'///////
	
    /* UART pin setting */ 
    //TX, Output
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_9; // 📌 실험 과정(2) UART2 and UART1 for TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //RX, Input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;  // 📌 실험 과정(2) UART2 and UART1 for RX
    GPIO_InitStructure.GPIO_Speed = 0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void USART1_Init(void) 
{
	USART_InitTypeDef USART1_InitStructure;

	// Enable the USART1 peripheral
	USART_Cmd(USART1, ENABLE);
	
////// TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'//////////////////////////
    /*  BaudRate : 9600
        WordLength or Data : 8 bits
        Parity : None
        StopBits : 1 bit
        Hardware Flow Control of Flow Control : None
    ���� ������� ��� ����
     */
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART1_InitStructure.USART_BaudRate = 9600; // 📌 실험 과정(3) 보레이트 값 변경
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART1_InitStructure.USART_Parity = USART_Parity_No;
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART1_InitStructure);
    

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
////// TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
void USART2_Init(void) // 📌  실험과정(4) USART2_init(void) 함수 만들기
{
	USART_InitTypeDef USART2_InitStructure;
	USART_Cmd(USART2, ENABLE);
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART2_InitStructure.USART_BaudRate = 9600;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_Parity = USART_Parity_No;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART2_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void NVIC_Configure(void) { 
  
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /*  Priority Group : 0
        Preemtion Priority : Joystick down = up = User S1 = UART 1
        Sub Priority ���� : Joystick down -> up -> User S1 -> UART 1
     */
////// TODO: fill the arg Priority Group 0///////////////////////////////////////////////////////////////////////////////////////
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////// TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'/////////////////////////////
    
    // UART1
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_EnableIRQ(USART1_IRQn);
    
    // UART2
    // 📌  실험과정(5) UART2 에 대한 우선순위 추가하기 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_EnableIRQ(USART2_IRQn);
    
    // 'NVIC_EnableIRQ' is only required for USART setting

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void USART1_IRQHandler() { // 📌  실험과정(6) USART1_IRQHandler() 함수 수정 및
	uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);
        USART_SendData(USART2, word);
        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void USART2_IRQHandler() { // 📌  실험과정(6) USART2_IRQHandler() 함수 만들기
	uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART2); //���ڿ� �϶��� �۵��ϴ°�?
        USART_SendData(USART1, word);
        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}


void sendDataUART1(uint16_t data) { // 📌  실험과정(7) sendDataUART1(unit16_t data) 함수 수정 및
	USART_SendData(USART1, data);
}

void sendDataUART2(uint16_t data) { // 📌  실험과정(7) sendDataUART2(unit16_t data) 함수 만들기
	USART_SendData(USART2, data);
}

int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    USART1_Init();
    
    USART2_Init(); // 📌  실험과정(8) USART2_Init() 함수 호출하고, 

    NVIC_Configure();

    while (1) { // 📌  실험과정(8) while 안의 내용 삭제 
    }
    return 0;
}
