#include <stdint.h>
#include <string.h>
#include "main.h"
#include "stm32h7xx_hal.h"

int main()
{
	HAL_Init();

	// Enable GPIOA clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configure PA5 (CLK) and PA7 (DATA) as output
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Set both pins low initially
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DATA low

	while (true)
	{
		// Send start frame (32 zeros)
		for (int i = 0; i < 32; i++)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // DATA = 0
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	  // CLK high
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
		}

		// Send 3 LED frames (red LEDs)
		for (int led = 0; led < 3; led++)
		{
			// Brightness byte (0xE0 | brightness)
			uint8_t brightness = 0xE0 | 0x1F;
			for (int bit = 7; bit >= 0; bit--)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, (brightness & (1 << bit)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	  // CLK high
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
			}

			// Blue byte (0x00)
			for (int bit = 7; bit >= 0; bit--)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Blue = 0
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	  // CLK high
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
			}

			// Green byte (0x00)
			for (int bit = 7; bit >= 0; bit--)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // Green = 0
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	  // CLK high
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
			}

			// Red byte (0xFF)
			for (int bit = 7; bit >= 0; bit--)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);	  // Red = 255
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	  // CLK high
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
			}
		}

		// Send end frame (32 ones)
		for (int i = 0; i < 32; i++)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);	  // DATA = 1
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);	  // CLK high
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // CLK low
		}

		HAL_Delay(1000); // Wait 1 second
	}

	return 0;
}

// SysTick interrupt handler - required for HAL_Delay()
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}