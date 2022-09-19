# 🔧 3주차 : GPIO 제어

- 담당자 : 지수, 준우

## 🥇 실험 목표
### 1. 첫번째 목표
Datasheet 및 Reference Manual을 참고하여 해당 레지스터 및 주소에 대한 설정을 이해한다. <br>

### 2. 두번째 목표
GPIO(general-purpose input/output)를 사용하여 조이스틱과 LED를 제어한다. <br>

조이스틱이 DOWN 되면 LED1과 LED2가 꺼지도록 한다. <br>
조이스틱이 LEFT 되면 LED3과 LED4가 꺼지도록 한다. <br>
조이스틱이 RIGHT 되면 LED3과 LED4가 켜지도록 한다. <br>
조이스틱이 UP 되면 LED1과 LED2가 켜지도록 한다.<br>

## 🥇실험 이론 및 원리
### 1. GPIO, RCC, Clock, PORT

- 입출력 장치는 GPIO(General Purpose Input Output)가 관리하며, 각 장치 마다 사용하는 PORT가 있고, <br>
- 이 PORT 들은 APB2 버스를 통해 데이터가 전달되며 <br>
- RCC(Reset Clock Control)는 이 버스의 활성화(Clock)를 관리한다.<br>
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/3%EC%A3%BC%EC%B0%A8/image/1.png"  width="500">
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/3%EC%A3%BC%EC%B0%A8/image/2.png"  width="500">
### 2. Reference Manual을 이용한 I/O 장치의 주소 접근 && 포인터를 이용하여 주어진 address의 value 제어

- GPIO Port E의 BSRR 레지스터 주소 계산: <br>
(Port E base) + (GPIOx_BSRR address offset) => 0x40011800 + 0x10 = 0x40011810 <br>
해당 bit에 대입: *((volatile unsigned int *)0x40011810) |= 0x00200000; <br>

- 포인터를 이용한 레지스터 주소를 이용하는 변수 선언 <br>
volatile unsigned int* var_name = “address1” <br>
*var_name = “value1” <br>
(*(volatile unsigned int*) var_name) = “address2” <br>
var_name = “value2” <br>

### 3. 알고리즘
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/3%EC%A3%BC%EC%B0%A8/image/3.png"  width="500">
<br>

## 🥇실험 과정

### 1. RCC 를 사용하여 GPIO 에 clock 인가하기
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/3%EC%A3%BC%EC%B0%A8/image/4.png"  width="200">
LED 1,2,3,4 ➡️ PD 2,3,4,7 <br>
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/3%EC%A3%BC%EC%B0%A8/image/5.png"  width="300">
DOWN, LEFT, RIGHT, UP ➡️ PC 2,3,4,5 <br>

PD 가 의미하는 것은 GPIO Port D, PC 가 의미하는 것은 GPIO Port C로 우리는 포트 C와 D를 사용할 것이다. <br>
GPIO는 APB2 버스를 통해 접근 되므로 APB2 버스를 활성화 시켜야한다.  <br>
“Reference Manual.pdf”를 보고 RCC 레지스터에서 APB2 버스에 관련된 clock을 찾는다. <br>





port C와 port D를 사용하기 때문에 IOPCEN(Bit 4)와 IOPDEN(Bit 5)를 제어해주어야한다. <br>
포트 C는 0000000000010000 (16진법으로 0x10), 포트 D는  0000000000100000 (16진법으로 0x20) 이다. <br>

그러면 RCC_APB2ENR 레지스터 주소에 접근을 해야한다. <br>
“Datasheet.pdf”를 통해 찾은 RCC 레지스터 시작 주소는 0x40021000 이였다. <br>

RCC_APB2ENR 레지스터의 address offset이 0x18 이였기에 RCC_APB2ENR 레지스터의 주소는 0x40021000 + 0x18 = 0x40021018 이 된다. <br>

코드로 작성해보면 아래와 같다. <br>
`#define RCC_APB2ENR (*(volatile unsigned int*) 0x40021018)`<br>
이제 여기서 GPIO C와 GPIO D의 clock을 활성화하는 코드를 작성한다. <br>
  `// PORT D EN` <br>
  `RCC_APB2ENR |= 0x20;` <br>
  `// PORT C EN` <br>
  `RCC_APB2ENR |= 0x10;` <br>

### 2. GPIO 포트 초기화 및 Pin 설정
clock이 공급되었다면 이제 GPIO port에 Data를 전달해주어야한다. <br>
CRL은 0~7 핀에 관여하는데, 우리는 port C 2,3,4,5 와 port D 2,3,4,7 을 사용하므로 CRL을 사용한다.<br>
 Datasheet 33 page. <br>
GPIOD(0x40011400), GPIOx_CRL address offset(0x00) 이므로 0x40011400 가 주소, <br>
GPIOC(0x40011000), GPIOx_CRL address offset(0x00) 이므로 0x40011000 가 주소다.  <br>

`// PORT D: I/O MODE REGISTER`<br>
`#define GPIOD_CRL (*(volatile unsigned int *) 0x40011400)`<br>
`// INPUT PORT C`<br>
`#define GPIOC_CRL (*(volatile unsigned int *) 0x40011000)`<br>

port D는 2,3,4,7 번째 비트에 해당하는 부분을 초기화 하고, pin을 설정한다. <br>
`// PORT D Pull Up-Down: 초기화, Pin 설정`<br>
`GPIOD_CRL &= !0xF00FFF00;`<br>
`GPIOD_CRL |= 0x30033300;`<br>

portC는 2,3,4,5 번째 비트에 해당하는 부분을 초기화 하고, pin을 설정한다. <br>
`// PORT C Pull Up-Down: 초기화, Pin 설정`<br>
`GPIOC_CRL &= !0x00FFFF00;`<br>
`GPIOC_CRL |= 0x00888800;`<br>

### 3. GPIO  포트에 input 설정하기
조이스틱은 PortC를 이용해서 탐지가 가능하다. <br>
GPIOC_IDR 인 input data register 를 이용해서 탐지한다. <br>

GPIOC(0x40011000), GPIOx_IDR address offset(0x08) 이므로 0x40011008 로 주소 지정한다.<br>
`#define GPIOC_IDR (*(volatile unsigned int *) 0x40011008)` <br>
 
port C의 IDR을 초기화 한다. <br>
`// Port C 의 IDR 초기화` <br>
`GPIOC_IDR &= 0x00000000;` <br>

### 4.GPIO  포트에 output 설정하기
BRR은 reset을 관여하는 레지스터고, BSRR은 reset과 set 둘다 관여하는 레지스터이다. <br>
연산량을 줄이기 위해는 reset은 BRR 레지스터로만, set은 BSRR 레지스터로만 제어할 것이다. <br>


GPIOD(0x40011400), GPIOx_BSRR address offset(0x10) 이므로 0x40011410 가 주소.<br>
GPIOD(0x40011400), GPIOx_BRR address offset(0x14) 이므로 0x40011414 가 주소.<br>

`// OUTPUT PORT D` <br>
`#define GPIOD_BSRR (*(volatile unsigned int *) 0x40011410)`<br>
`#define GPIOD_BRR (*(volatile unsigned int *) 0x40011414)`<br>

port D의 BSRR 과 BRR을 초기화 한다. <br>
`GPIOD_BSRR &= 0x00000000;`<br>
`GPIOD_BRR &= 0x00000000;`<br>

### 5. 조이스틱 제어로 LED ON/OFF
조이스틱을 상하좌우로 컨트롤 하면 bit 가 다음과 같이 바뀌게 된다. <br>
0b00x0 0000 =>  UP <br>
0b000x 0000 => RIGHT <br>
0b0000 x000 => LEFT <br>
0b0000 0x00 => DOWN <br>

`#define UP 0x20`<br>
`#define DOWN 0x4`<br>
`#define LEFT 0x8`<br>
`#define RIGHT 0x10`<br>

조이스틱 입력이 들어오면 x 가 0으로 바뀐다. <br>
그렇기에 조이스틱 입력 여부를 살펴보려면 GPIOC_IDR 과 AND 연산을 하여 값이 0이 되는지 살핀다. <br>
또, LED를 ON 하려면 BSRR 을 이용해 set을 하고, LED를 OFF 하려면 BRR 을 이용해 reset을 한다. <br>

예를 들어 조이스틱을 UP 했을 때  LED 2가 켜지는 것을 코드로 쓰면 아래와 같다. <br>
 `if(!(GPIOC_IDR & UP) )  GPIOD_BSRR |= 0x04;`  <br>

반대로 LED2를 끄려면, GPIOD_BRR에 접근한다.  <br>
 `if(!(GPIOC_IDR & UP) )  GPIOD_BRR |= 0x04;` <br>

## 🥇실험 결과

while 문을 통해 무한 반복을 시킨다.  <br>

JoyStick 입력 상황별 결과: <br>
- UP -> LED1, LED2 on <br>
      ```
      if(!(GPIOC_IDR & UP) ) {
        GPIOD_BSRR |= 0x04; // LED1 (PD2) ON
        GPIOD_BSRR |= 0x08; // LED2 (PD3) ON
      }
      ```


- DOWN -> LED1, LED2 off <br>
      ```
      else if(!(GPIOC_IDR & DOWN) ) {
        GPIOD_BRR |= 0x04; //  LED1 (PD2) OFF
        GPIOD_BRR |= 0x08; // LED2 (PD3) OFF
      }
      ```

- RIGHT -> LED3, LED4 on <br>
       ```
       else if(!(GPIOC_IDR & RIGHT) ) {
        GPIOD_BSRR |= 0x10; // LED3 (PD4) ON
        GPIOD_BSRR |= 0x80; // LED4 (PD7) ON
      }
      ```

- LEFT -> LED3, LED4 off <br>
      ```
      else if(!(GPIOC_IDR & LEFT) ) {
        GPIOD_BRR |= 0x10; // LED3 (PD4) OFF
        GPIOD_BRR |= 0x80; // LED4 (PD7) OFF
      }
      ```
 

## 🥇기타 논의

### 실험 과정 중 발생한 문제: 
조이스틱의 입력 값이 확인이 되지않음

### 발생한 문제의 해결 방법:
GPIOC_IDR의 주소를 잘못 입력된 것을 수정 <br>
GPIOC_CRL Pull Up-Down 설정 <br>
IF문에서 입력값을 확인하는 방법을 수정( (IDR == Direction) => (!(IDR & Direction) ) <br>



