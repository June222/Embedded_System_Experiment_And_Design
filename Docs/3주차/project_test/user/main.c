#include "stm32f10x.h"
#include "stdio.h"

#define UP 0x20
#define DOWN 0x4
#define LEFT 0x8
#define RIGHT 0x10

// Enable Port D REGISTER
#define RCC_APB2ENR (*(volatile unsigned int*) 0x40021018)

// PORT D: I/O MODE REGISTER
#define GPIOD_CRL (*(volatile unsigned int *) 0x40011400)

// LED ON REGISTER
#define GPIOD_BSRR (*(volatile unsigned int *) 0x40011410)

// INPUT PORT C
#define GPIOC_CRL (*(volatile unsigned int *) 0x40011000)
#define GPIOC_IDR (*(volatile unsigned int *) 0x40011008)

// OUTPUT PORT D
#define GPIOD_ODR (*(volatile unsigned int *) 0x4001140C)
#define GPIOD_BRR (*(volatile unsigned int *) 0x40011414)

int main(void)
{
  // PORT D EN
  RCC_APB2ENR |= 0x20;
  
  // PORT C EN
  RCC_APB2ENR |= 0x10;
  
  // PORT C Pull Up-Down: 초기화, Pin 설정
  GPIOC_CRL &= !0x00FFFF00;
  GPIOC_CRL |= 0x00888800;
  
  // PORT D Pull Up-Down: 초기화, Pin 설정
  GPIOD_CRL &= ~0xF00FFF00;
  GPIOD_CRL |= 0x30033300;
  
  // Port C 의 IDR 초기화
  GPIOC_IDR &= 0x00000000;
  // Port D 의 ON/OFF 초기화
  GPIOD_BSRR &= 0x00000000;

  while(1){  
    // Port C 의 IDR 값 확인(UP, DOWN, LEFT, RIGHT)
       if(!(GPIOC_IDR & UP) ) {
        // LED ON 
        GPIOD_BSRR |= 0x04; // 2
        GPIOD_BSRR |= 0x08; // 3
       }
      else if(!(GPIOC_IDR & DOWN) ) {
        // LED OFF
        GPIOD_BRR |= 0x04; // 2
        GPIOD_BRR |= 0x08; // 3
      }
      else if(!(GPIOC_IDR & LEFT) ) {
        // LED ON
        GPIOD_BSRR |= 0x10; // 4
        GPIOD_BSRR |= 0x80; // 7
      }
      else if(!(GPIOC_IDR & RIGHT) ) {
        // LED OFF
        GPIOD_BRR |= 0x10; // 4 
        GPIOD_BRR |= 0x80; // 7
      }
  }
}
