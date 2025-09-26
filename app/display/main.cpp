#include <stdint.h>
#include <string.h>
#include "main.h"
#include "stm32h7xx_hal.h"
#include "../../drivers/ledcontrol/ledcontrol.h" // TODO: fix path

// Define the SPI handle (declared as extern in ledcontrol.h)
SPI_HandleTypeDef hspi1;

int main()
{
	HAL_Init();

	// Create LED controller instance
	LEDControl led_controller(&hspi1);

	// Initialize the LED strip
	led_controller.initialize_strip();

	// Initialize LED states (all LEDs on for testing)
	memset(led_controller.led_states, 0xFF, sizeof(led_controller.led_states));

	while (true)
	{
		// Set LED states on (all bits to 1 for brightness)
		memset(led_controller.led_states, 0xFF, sizeof(led_controller.led_states));

		// Turn on first 5 LEDs
		for (int i = 0; i < 5; i++)
		{
			LEDControl::LED_State led = led_controller.create_led(i);
			led.on = true;
			led_controller.set_led_state(&led);
		}

		// Wait 1 second
		HAL_Delay(1000);

		// Turn off LEDs
		memset(led_controller.led_states, 0x00, sizeof(led_controller.led_states));

		for (int i = 0; i < 5; i++)
		{
			LEDControl::LED_State led = led_controller.create_led(i);
			led.on = false;
			led_controller.set_led_state(&led);
		}

		// Wait 1 second
		HAL_Delay(1000);

		// Rainbow pattern - alternate different LEDs
		for (int pattern = 0; pattern < 3; pattern++)
		{
			// Clear all LEDs first
			memset(led_controller.led_states, 0x00, sizeof(led_controller.led_states));

			// Turn on every 3rd LED starting from pattern offset
			for (int i = pattern; i < 15; i += 3)
			{
				LEDControl::LED_State led = led_controller.create_led(i);
				led.on = true;
				led_controller.set_led_state(&led);
			}

			// Wait 500ms
			HAL_Delay(500);
		}
	}
	return 0;
}

// SysTick interrupt handler - required for HAL_Delay()
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}