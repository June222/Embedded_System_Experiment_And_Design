9조의 6주차 실험 보고서

1. 실험 목표
📌 첫 번째 목표
	Datasheet 및 Reference Manual을 참고, 해당 레지스터 및 주소에 대한 설정 이해
📌 두 번째 목표
	NVIC, EXTI를 이용 GPIO에 Interrupt Handling 세팅(ISR은 최대한 빨리 끝나야함)
📌 세 번째 목표
	보드를 켜면 LED 물결 기능 유지(LED 1 -> 2 -> 3 -> 4 -> 1 -> … 반복)
	다른 버튼은 물결을 반대로 반복(LED 4 -> 3 -> 2 -> 1 -> 4 -> … 반복)


2. 실험 이론 및 원리
📌 이론1 : Polling 과 Interrupt
Polling은 특정 시간을 가지고 서버와 응답을 주고 받는 방식인데, 이는 특정한 이벤트를 처리하기 위해 다른 이벤트가 발생하는지 감시하는 것을 말한다. 

	
Intrerrupt란 특정 이벤트가 발생 했을 때, 현재 작업을 멈추고 해당 인터럽트의 서비스 루틴을 수행 한 후에 다시 이전 작업으로 돌아가는 방식을 말한다. 



📌 이론2 : Hardware Interrupt와 Software Interrupt
Hardware Interrupt는 비동기식 이벤트 처리로 주변 장치의 요청에 의해 발생하는 인터럽트이다. 높은 우선 순위를 가지며, 그 예로는 하드 디스크 읽기 요청, 디스크 읽기 끝남, 키보드 입력에 의한 발생 등이 있다.
Software Interrupt는 동기식 이벤트 처리로 사용자가 프로그램 내에서 인터럽트가 발생하도록 설정하는 인터럽트이다. 낮은 우선 순위를 가지며, 그 예로는 Trap, Exception 등이 있다.
📌 이론3 : EXTI(External Interrupt)
	모든 GPIO Pin들은 EXTI Line을 통해 연결되어 있음.
EXTICR1 레지스터에 통해 입력 받을 포트를 설정하고, 같은 번호의 핀 들은 같은 라인을 공유한다. 
-> GPIO_Config(Port, GPIO_Line_Number), Line(EXTI_Line_Number: GPIO_Line과 번호가 같음), Mode(Interrupt_Mode), Trigger(Up,Down), LineCmd(EN, DisEN)를를 설정해야함.

Input 부터 Interrupt처리 까지 절차: 
GPIO -> EXTI Line(LineCmd == EN 시에) -> NVIC (를 통한 우선순위 설정) -> EXTI_Handler(ISR) -> Interrupt 처리(원하는 기능)
📌 이론4 : NVIC(Nested Vectored Interrupt Controller)
	NVIC는 인터럽트간의 우선순위를 정하는 컨트롤러이다. Pre-emption priority로 선점 우선 순위를 결정한 후, sub priority로 아직 대기 중인 ISR들의 순서가 결정된다. 

3. 실험 과정 
1. week6_template.c 의 TODO 부분 채워넣기
 TODO 1   
➡️ Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'

RCC_APB2PeriphClockCmd( 첫번째 인자, 두번째  인자) 함수를 이용하여 두번째 인자에는 항상 “ENABLE”을 넣어주고 첫번째 인자 값만 각각 바꾸어주면 된다. 

< UART TX/RX 포트 클럭 가능하게 하기 >
첫번째 인자: RCC_APB2Periph_GPIOA

< 조이스틱 up/down 클럭 가능하게 하기 >
첫번째 인자 : RCC_APB2Periph_GPIOC

<조이스틱 selection 클럭 가능하게 하기>
첫번째 인자 : RCC_APB2Periph_GPIOB

< LED 포트 클럭 가능하게 하기 >
첫번째 인자 : RCC_APB2Periph_GPIOD

< USART1 클럭 가능하게 하기 >
첫번째 인자 : RCC_APB2Periph_USART1

< AFIO 클럭 가능하게 하기 > 
첫번째 인자 : RCC_APB2Periph_AFIO 

TODO 2 
➡️ Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'

핀을 설정해야하는 부분이다. 
“ GPIO_InitTypeDef ” 구조체 GPIO_InitStructure을 선언한 후, 구조체 내의 원소들에 값을 넣음으로써 설정한다. 

구조체 내 각 원소들에 대해 살펴보면
첫째, GPIO_Pin는 Pin 활성화 하는 부분이다. 
둘째, GPIO_Speed는 핀의 Output speed 설정하는 부분이다.

셋째, GPIO_Mode는 모드를 설정하는 부분이다.



따라서 아래와 같은 형식으로 각각에 대해 작성을 했다. 
GPIO_InitStructure.GPIO_Pin  =  핀값;
GPIO_InitStructure.GPIO_Speed = 속도;
GPIO_InitStructure.GPIO_Mode = 모드;
GPIO_Init( 포트 , &GPIO_InitStructure);

< 조이스틱 up/down 핀 세팅 > 
핀값 = GPIO_Pin_5 | GPIO_Pin_2
속도 = 0
모드 = GPIO_Mode_IPD;
포트 = GPIOC 
	
< 조이스틱 selection 핀 세팅 >
핀값 = GPIO_Pin_8
속도 = 0
모드 = GPIO_Mode_IPD;
포트 = GPIOB

<버튼 핀 세팅>
핀값 = GPIO_Pin_11
속도 = 0
모드 = GPIO_Mode_IPD;
포트 = GPIOD

<LED 핀 세팅>
핀값 = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7
속도 = GPIO_Speed_50MHz
모드 = GPIO_Mode_Out_PP
포트 = GPIOD

<UART 핀 세팅>
// TX 
핀값 = GPIO_Pin_8   📌 이 부분은 차후 수정하였다.
속도 = GPIO_Speed_50MHz
모드 = GPIO_Mode_TX    📌 이 부분은 차후 수정하였다.
포트 = GPIOA

// RX
핀값 =GPIO_Pin_10  
속도 = 0
모드 = GPIO_Mode_RX    📌 이 부분은 차후 수정하였다.
포트 = GPIOA

TODO 3
➡️ TODO: Select the GPIO pin (Joystick, button) used as EXTI Line using function 

➡️ TODO: Initialize the EXTI using the structure 'EXTI_InitTypeDef' and the function 'EXTI_Init'

/* Joystick Down */

// 어떤 Port의 어떤 Pin 번호가 입력되면 Interrupt로 인식할 것인가?
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);

	// 모든 Port의 Pin 2번으로 들어오면 Interrupt로 인식
    	EXTI_InitStructure.EXTI_Line = EXTI_Line2;


	// Mode는 Interrupt와 EventMode가 있음
    	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;



	// 버튼을 누를 때, 작동
    	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;


    	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    	EXTI_Init(&EXTI_InitStructure);

  


TODO 4
➡️ TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'

	// Enable Receive Interrtupt
USART_ITConfig(USART1, USART_IT_RXNE ,ENABLE);



TODO 5
➡️ TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

// Joystick Down
	
	// EXTI{num}으로 온 채널의 Interrupt
NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
// 선점 우선순위는
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
// 추가 우선순위는
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
// 사용 허가. 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

// UART1
NVIC_EnableIRQ(USART1_IRQn);

TODO 6
➡️ 

{NAME}_IRQHandler():

USART1_IRQHandler(){
uint16_t word;


// 비트에 0이 아닐 경우 데이터를 수신.
    	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    		// the most recent received data by the USART1 peripheral
        		word = USART_ReceiveData(USART1);
	}

	void EXTI2_IRQHandler(void) { // when the button is pressed
	
	// EXTI_Line2의 Interrupt가 확인되고고
		if (EXTI_GetITStatus(EXTI_Line2) != RESET) {

			// 우리가 원하는 Port C의 Interrupt 임을 확인한 후에
			if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == Bit_RESET) {
                 			isUp = DISABLE; // TODO implement
			
			}
		// Interrupt 처리 이후 폐기
        		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	
	EXTI9_5_IRQHandler()는 위와 형식 동일.

	void EXTI15_10_IRQHandler(void) {
 
		// when the button is pressed
		if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
			
			// Port D의 Pin 11에서 발생한 Interrupt인지 확인.
          			if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_RESET) {
           				printFlag = ENABLE; // TODO implement	
			}
        			EXTI_ClearITPendingBit(EXTI_Line11);
		}
}

	
TODO 6
// Print Words when press S1 Button.
	// Button이 
if(printFlag == ENABLE){
        		for(int i =0; i < 9; i++) {
        			sendDataUART1(printWords[i]);
        	}
        		printFlag = DISABLE;
}


// LED ON

   





4. 시행 착오
📌 입력을 하기도 전에 SerialPortMon에 이상값이 출력된다.  


시도 1: UART 핀을 세팅하는 부분에서 TX는 8번 핀 또는 9번 핀 에 대해 세팅해준다.
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
무의미한 이상값이 줄어들기는 했지만, 완전히 해결 되지는 않았다.

📌 여전히 입력 전에 SerialPortMon에 값이 출력되며, 버튼을 눌렀을 때 값이 출력되지 않는다. 

시도 1: main 문에서 “S1 버튼이 들어왔을 때 “ 에 해당하는 제어문에 break를 걸어 if 문을 통과하는 지 확인한다. 
➡️ if 문에 들어오는 것이 확인 되었고, sendDataUART1 함수도 잘 통과했다. 
 그렇다면, 데이터를 보내는 것은 문제가 없다.

시도 2:  혹시 레지스터 명을 잘못 적었는지 다시 검토를 해보았다.
➡️ 알고보니  UART 핀을 세팅하는 부분에서 MODE를 잘못 적었던 것이였다! 
TX는 GPIO mode가 AF_PP 이고, RX는 GPIO mode 가 IPD 이다.
// TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// RX
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;



4. 실험 결과
조이스틱을 UP하면 LED가 1->2->3->4->1->2->3->4 … 순으로  변경된다. 

조이스틱을 DOWN 하면 LED가 4->3->2->1->4->3->2->1 … 순으로 변경된다.
	
PC의 SerialPortMon에서 a,b 입력하여 보드 제어하기
        
➡️ a를 입력하면 led가 1->2->3->4->1->2->3->4 … 순으로  변경되고,
b를 입력하면 led가  4->3->2->1->4->3->2->1 … 순으로 변경된다.
S1버튼을 누르면 SerialPortMon으로 “TEAM09” 출력하기
	  ➡️  



