# 🔧 4주차

## 🥇 실험 목표
### 1.  첫 번째 목표
스캐터 파일(Scatter File)의 이해 및 플래시 프로그래밍 <br>

### 2.  두 번째 목표
릴레이 모듈 및 모터 구동 이해 및 임베디드 펌웨어를 통한 동작 <br>

### 3 세 번째 목표
폴링 방식의 원리 및 문제점 이해 확인<br>

## 🥇 실험 이론 및 원리

### 1 플로팅(Floating)
- 주변의 노이즈에 의해 입력값이 일정하게 유지되지 못하고 High Low 신호를 빠르게 이동하며 잘못된 결과를 출력하는 현상을 뜻한다.
- 노이즈란, 전기가 통하며 생기는 자기장이나 회로에 작용하는 외력과 같이 회로에 정상적으로 전류가 흐르는 것을 방해하는 모든 것을 뜻한다.
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/4%EC%A3%BC%EC%B0%A8/image/1.png"  width="500">
- 버튼을 눌렀을 때 입력값을 출력하는 코드는 다음과 같이 출력하게 된다.
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/4%EC%A3%BC%EC%B0%A8/image/2.png"  width="500">
- 왼쪽이 Floating 현상이 발생하고 있는 모습이고 오른쪽이 이상적인 상황에서의 결과이다. 둘 모두 버튼을 일정시간 누르고 떼는 것을 반복하고 있는 상황이다. 

### 2 Pull Up, Pull Down
- Pull Up, Pull Down는 이렇게 노이즈에 의해 발생되는 Floating 현상을 막기 위한 방법이다.
- 두 방법 모두 장치와 입력 단자를 직접 연결하지 않고 저항과 스위치를 이용하여 회로를 구성, 노이즈로 인해 발생한 0과 1을 분리하고 원할 때만 입력을 받을 수 있게 하는 방법이다.
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/4%EC%A3%BC%EC%B0%A8/image/3.png"  width="500">
- 저항은 0과 1을 분리하고, Switch는 원할 때만 입력을 받을 수 있게 한다.

### 3 Scatter File
- Scatter Load는 분산 적재라는 뜻이고
- Scatter File은 자주 사용되거나 빠른 실행을 요구하는 코드 영역을 접근 시간이 빠른 메모리에 우선 배치하도록 할 수 있게 해주는 것을 뜻한다.

### 4 Relay Module 
- 릴레이를 제어하는 모듈이다.
- 전자기유도원리를 이용하여 스위치 역할을 하고,
- 릴레이에 신호를 가하면 출력 상태(ON/OFF)가 변경된다


## 🥇실험 과정 
### 1. Scatter File 작성하고 업로드하기
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/4%EC%A3%BC%EC%B0%A8/image/4.png"  width="500">
- 스케터 파일에서 ROM, RAM 주소값을 정의함으로써 원하는 만큼의 메모리 영역을 할당한다. 
- RAM 의 시작 주소는 0x20000000, ROM의 시작주소는 0x08000000 이고, 우리는 RAM 크기 0x8000 로 ROM 크기 0x80000 로 할당할 예정이므로, RAM의 끝 주소는 0x20008000로 ROM의 끝 주소는 0x08080000 이 된다.
- 이후 작성한 스케터 파일을 프로젝트에 업로드한다. 

### 2 STM32, 브레드보드, 릴레이모듈, 모터 연결하기
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/4%EC%A3%BC%EC%B0%A8/image/5.png"  width="500">
- PD11 핀과 PD 12핀을 두개의 릴레이 모듈에 각각 연결해주었다. 

### 3 코드 작성하기

- 조이스틱에 해당하는 GPIOC 가 up인지 down 인지 판단한다. ( 이 내용까지는 저번 주차와 같으므로 설명은 생략 하겠음. )

- PD 11과 PD 12는 각각 0x800, 0x1000에 해당하므로 set과 reset하는 코드를 작성한다. 

``` c
 if(!(GPIOC & UP)){
    GPIOD_BRR |= 0x1000
    GPIOD_BSRR |= 0x800
  }
 if(!(GPIOC & DOWN)){
    GPIOD_BRR |= 0x800
    GPIOD_BSRR |= 0x1000
  }
```

- 스위치로 모터 작동을 멈추게 하는 코드는 차후에 추가했다. 

## 🥇시행 착오
### 📌 Issue 1 : USB 연결 인식 오류
``` 
‘ Failed to get CPU status after 4 retries Retry? ‘ 
```
- idea1 : 재부팅 -> not solved
- idea2 : USB 재연결 -> not solved
- idea3 : 코드 reset -> solved

지난 주 프로젝트 파일을 다시 새로 다운 받아 세팅하니 오류가 사라졌다.

### 📌 Issue 2 : 릴레이 모듈로 모터의 방향 안바뀜.
릴레이 모듈로 모터의 방향을 바꾸려고 하였다.<br>
하지만 릴레이 모듈의 역할은 단순 "증폭" 역할임을 알게되었다. <br>
우리가 사용하는 보드는 모터를 구동할 만큼의 전류를 제공하지 못하기에 우리는 릴레이 모듈을 통해 전류를 증폭시킨다.<br>
차후에 브레드보드 연결 회로설계를 변경해서 모터의 방향을 바꾸었다.<br>

### 📌 Issue 3 : 회로 연결했지만, 릴레이 모듈에 신호 안가서 모터가 작동하지 않음. 

조이스틱에 따른 LED 상태변경은 원활하기 때문에, 코드적 문제는 아니라고 판단했다.<br>

하드웨어적 문제라고 판단했다.<br>
- idea1 : Pin 변경 -> not solved
- idea2 : 릴레이모듈 하나로 변경 -> not solved
- idea3 : 딜레이 시간 조절 -> not solved
- idea4 : 릴레이 모듈과 STM32 교체, 그리고 건전지로 연결 -> solved

하드웨어의 장치적인 문제 그리고, 전력이 부족한 것이 문제였다. <br>
릴레이 모듈의 초록색 LED가 망가져 있어서 변경을 했으며, 건전지로 회로를 만드니 해결되었다. <br>

### 📌  Issue 4 : PortD 초기화 Pin 설정의 오류 
Output Pin 설정과정에서 생긴 오류였다.<br>
LED를 제어(3주차 실험)할 때 사용했던 코드를 그대로 사용했다.<br>
그때, LED output을 읽는 과정에서 PortD 3, 4, 5, 7 를 사용했기 때문에 CRL(L = Low) 주소를 사용했지만 <br>
이번 실험에서는 PortD 11, 12를 사용하기 때문에, CRH(H = High)주소를 사용해야 한다. <br>
<br>
그래서 코드를 수정했다.<br>
0x40011400(포트D 주소값) + 0x04 (GPIOx_CRH 오프셋) = 0x40011404 이므로 아래와 같이 선언한다.<br>

``` c 
#define GPIOD_CRH (*(volatile unsigned int *) 0x40011404)
```

그리고 편하게 모든 핀에 대해  초기화를 하고 설정했다.

``` c
GPIOD_CRH &= ~0xFFFFFFFF;//0x000F0F00;
GPIOD_CRH |=  0x33333333;//0x00030300;
```

### 📌  Issue 5 : 핀을 제거하면 모터가 돌아가고, 핀을 꽂으면 정지함.

issue 3에서 PD 11 핀 에 장치적 결함이 있는 것으로 판단을 하여 LED 제어를 하는 PD2와 PD3에 꽂아 뒀었다. <br>
이 때, 해당 핀이 다른거랑 연결되어 있으면 GND로 걸리거나 다른 영향을 주기 때문에 원하는대로 작동이 안되는 것이었다.<br>
따라서 , PD13과 PD 14로 핀을 변경했다. <br>

``` c
GPIOD_BSRR |= 0x4000; //PD 14 set
GPIOD_BRR |=  0x2000; //PD 13 reset
``` 

### 📌  Issue 6 : PD11로 걸려 있는 S1 스위치가 작동을 안함.
코드 상으로 보면, GPIOD_IDR 선언해서 ```  if (!(GPIOD_IDR & PD11 ))```  했는데도 안되었다. <br>
그래서, PC 13 핀으로 입력받는 S5 TAMPER 스위치를 사용했다.<br>

``` c
if (!(GPIOC_IDR & 0x2000)) { // S5 TAMPER (PC13) 
        GPIOD_BRR |= 0x4000; //PD 14 끄기
        GPIOD_BRR |=  0x2000; //PD 13  끄기
 }
``` 

### 📌  Issue 7 : 모터가 반시계 방향으로 돌아갈 때 스위치를 누르면 멈추는데, 반대일 때는 눌러도 멈추지 않음.

변수를 사용해서 코드를 수정했다. <br>
먼저 변수를 선언하고, 모터가 반시계/시계 일때의 변수 값을 각각 다르게 둔다.<br>

``` c
int count = 0; 

     if(!(GPIOC_IDR & UP) ) { 
        count = 1;
       ….
       }
      if(!(GPIOC_IDR & DOWN) ) { 
        count =2;
       ….
      }
``` 
'스위치를 누른다'라는 조건문이 통과되면,모터가 어느 방향으로 구동하는지를 이 변수를 이용해 조건문을 걸고, 모터를 멈추는 코드를 짠다.<br>

``` c
if (!(GPIOC_IDR & 0x2000)) { // S5 TAMPER (PC13)  스위치를 누른다면 

      if (count==2) { // DOWN 눌렀을 때 
        GPIOD_BRR |= 0x4000; //PD 14
        GPIOD_BRR |=  0x2000; //PD 13 
      }
       else if (count==1) { // UP 눌렀을 때 
          GPIOD_BSRR |= 0x4000; 
          GPIOD_BSRR |=  0x2000; 
       }
    }
``` 

## 🥇 실험 결과
- 조이스틱이 UP 일 때 모터가 시계방향으로 돈다.
- 조이스틱이 DOWN 일 때 모터가 반시계 방향으로 돈다. 
- 스위치를 누르면 모터가 정지한다. 
<img src = "https://github.com/June222/Embedded_System_Experiment_And_Design/blob/main/Docs/4%EC%A3%BC%EC%B0%A8/image/6.png"  width="500">


