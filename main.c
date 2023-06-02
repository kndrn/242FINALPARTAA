#include "main.h"
#include "stdio.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

#define SIGNAL_GPIO_PIN    GPIO_PIN_7
#define SIGNAL_GPIO_PORT   GPIOD
#define SIGNAL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()


TIM_HandleTypeDef htim3;
uint8_t signalStatus = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM_Init();
  MX_USB_DEVICE_Init();

  HAL_TIM_Base_Start(&htim3);

  while (1)
  {

    HAL_GPIO_WritePin(SIGNAL_GPIO_PORT, SIGNAL_GPIO_PIN, GPIO_PIN_SET);
    signalStatus = 1;
    CDC_Transmit_FS(&signalStatus, sizeof(signalStatus));
    HAL_Delay(2000);
    HAL_GPIO_WritePin(SIGNAL_GPIO_PORT, SIGNAL_GPIO_PIN, GPIO_PIN_RESET);
    signalStatus = 0;
    CDC_Transmit_FS(&signalStatus, sizeof(signalStatus));
    HAL_Delay(2000);
  }
}

void SystemClock_Config(void)
{

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  SIGNAL_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = SIGNAL_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SIGNAL_GPIO_PORT, &GPIO_InitStruct);
}

static void MX_TIM_Init(void)
{
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 42000 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2000 - 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);
}

void Error_Handler(void)
{

}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)
  {

  }
}
