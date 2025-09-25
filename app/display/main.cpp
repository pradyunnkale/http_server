#include <stdint.h>
#include "stm32h7xx_hal.h"

// Function declarations
void SystemClock_Config(void);
void GPIO_Init(void);

int main()
{
	HAL_Init();
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// Enable GPIOB clock
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure PB0 (Green LED) as output
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Start with LED off
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

	while (true)
	{
		// Toggle Green LED on PB0
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_Delay(500); // 500ms delay
	}
	return 0;
}

// SysTick interrupt handler - required for HAL_Delay()
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}