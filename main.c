#include "hardware.h"
#include "systick.h"
#include "timer.h"

int main()
{
  
  SysTick_Init();
  SPI_Init(&spi2);

  TIM1_Init(2000000);
  TIM1_Config_PWM(TIM_CHANNEL_1, &test_pin);
  TIM1_Set_Duty_Cycle(TIM_CHANNEL_1, 50);

  while (1) {
    uint8_t data = 0xFF;
    SPI_Write(&spi2, &data, 1);
    SysTick_Delay_Milliseconds(1000);
  }
  return 0;
}
