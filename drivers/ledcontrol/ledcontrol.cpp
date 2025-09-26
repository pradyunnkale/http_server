#include "ledcontrol.h"

LEDControl::LEDControl(SPI_HandleTypeDef *spi)
{
    hspi = spi;
    memset(led_states, 0xFF, sizeof(led_states)); // Set all bits to 1 for testing
}

void LEDControl::initialize_strip(void)
{

    // Enable GPIOA clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure PA5 and PA7 for SPI CLK AND DIN
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1; // SPI1 alternate function
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Enable SPI1 clock
    __HAL_RCC_SPI1_CLK_ENABLE();

    // Configure SPI1 - minimal config for APA102C
    hspi->Instance = SPI1;
    hspi->Init.Mode = SPI_MODE_MASTER;
    hspi->Init.Direction = SPI_DIRECTION_2LINES;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi->Init.NSS = SPI_NSS_SOFT;
    hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64; // Slower for debugging
    hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    if (HAL_SPI_Init(hspi) != HAL_OK)
    {
        // SPI init failed - you could add error handling here
        while (1)
            ; // Hang if SPI init fails
    }

    for (int i = 0; i < 144; i++)
    {
        all_leds[i] = create_led(i);
    }

    // Clear all LED states and buffer
    memset(led_states, 0x00, sizeof(led_states)); // Clear the bit-packed state array
    memset(led_buffer, 0, LED_BUFFER_SIZE);

    uint8_t start_frame[4] = {0x00, 0x00, 0x00, 0x00};
    HAL_SPI_Transmit(hspi, start_frame, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(hspi, led_buffer, LED_BUFFER_SIZE, HAL_MAX_DELAY);
    uint8_t end_frame[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    HAL_SPI_Transmit(hspi, end_frame, 4, HAL_MAX_DELAY);
}

LEDControl::LED_State LEDControl::create_led(uint8_t led_number)
{
    LED_State led;
    led.degrees = led_number * (MAX_DEGREES / LEDS_PER_STRIP); // Calculate degrees based on height (helix)
    led.led_number = led_number;
    return led;
}

void LEDControl::set_led_state(LED_State *led)
{
    uint16_t buffer_start = led->led_number * 4;

    // TEMPORARY DEBUG: Force LED ON regardless of bit state
    // LED is ON - set to red color with medium brightness
    led_buffer[buffer_start + 0] = 0xE0 | 0x1F; // Brightness byte (0xE0 | brightness)
    led_buffer[buffer_start + 1] = 0x00;        // Blue = 0
    led_buffer[buffer_start + 2] = 0x00;        // Green = 0
    led_buffer[buffer_start + 3] = 0xFF;        // Red = max (bright red)

    // Physically update the LED strip immediately
    uint8_t start_frame[4] = {0x00, 0x00, 0x00, 0x00};
    HAL_SPI_Transmit(hspi, start_frame, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(hspi, led_buffer, LED_BUFFER_SIZE, HAL_MAX_DELAY);
    uint8_t end_frame[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    HAL_SPI_Transmit(hspi, end_frame, 4, HAL_MAX_DELAY);
}
