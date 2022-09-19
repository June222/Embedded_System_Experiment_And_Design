#include "stm32f10x.h"
#include "stdio.h"

#define UP 0x20
#define DOWN 0x4
#define LEFT 0x8
#define RIGHT 0x10

// Enable Port D REGISTER
#define RCC_APB2ENR (*(volatile unsigned int*) 0x40021018)

// Port C AND D : configuration register low
#define GPIOD_CRL (*(volatile unsigned int *) 0x40011400)
#define GPIOC_CRL (*(volatile unsigned int *) 0x40011000)

// Port c : input data register
#define GPIOC_IDR (*(volatile unsigned int *) 0x40011008)

// Port D  : ouput -> BRR : reset, BSRR : reset or set
#define GPIOD_BRR (*(volatile unsigned int *) 0x40011414)
#define GPIOD_BSRR (*(volatile unsigned int *) 0x40011410)

int main(void)
{
  // PORT D EN
  RCC_APB2ENR |= 0x20;
  
  // PORT C EN
  RCC_APB2ENR |= 0x10;
  
  // PORT C - Input mode pull up, pull down
  GPIOC_CRL &= !0x00FFFF00;
  GPIOC_CRL |= 0x00888800;
  
  // PORT D - General purpose output push-pull down
  GPIOD_CRL &= ~0xF00FFF00;
  GPIOD_CRL |= 0x30033300;
  
  // Initialize Port C input 
  GPIOC_IDR &= 0x00000000;
  
  // Initialize Port D output
  GPIOD_BSRR &= 0x00000000;
  GPIOD_BRR &= 0x00000000;

  while(1){  
    // Port C 의 IDR 값 확인(UP, DOWN, LEFT, RIGHT)
       if(!(GPIOC_IDR & UP) ) {
        // LED ON 
        GPIOD_BSRR |= 0x04; // LED1 (PD2) ON
        GPIOD_BSRR |= 0x08; // LED2 (PD3) ON
       }
      else if(!(GPIOC_IDR & DOWN) ) {
        // LED OFF
        GPIOD_BRR |= 0x04; // LED1 (PD2) OFF
        GPIOD_BRR |= 0x08; // LED2 (PD3) OFF
      }
      else if(!(GPIOC_IDR & RIGHT) ) {
        // LED ON
        GPIOD_BSRR |= 0x10; // LED3 (PD4) ON
        GPIOD_BSRR |= 0x80; // LED4 (PD7) ON
      }
      else if(!(GPIOC_IDR & LEFT) ) {
        // LED OFF
        GPIOD_BRR |= 0x10; // LED3 (PD4) OFF
        GPIOD_BRR |= 0x80; // LED4 (PD7) OFF
      }
  }
}
