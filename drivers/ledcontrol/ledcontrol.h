#include <stdint.h>
#include <string.h>
#include "main.h"
#include "stm32h7xx_hal.h"

#define MAX_DEGREES 360
#define MAX_RADIUS 1
#define LEDS_PER_STRIP 144

#define LED_BUFFER_SIZE (MAX_RADIUS * LEDS_PER_STRIP * 4) // 4 bytes per LED for SK8922

extern SPI_HandleTypeDef hspi1;

class LEDControl
{
private:
    SPI_HandleTypeDef *hspi;

public:
    uint8_t led_buffer[LED_BUFFER_SIZE]; // Buffer to store LED data (public for testing)
    unsigned int led_states[MAX_DEGREES][LEDS_PER_STRIP / 32 + 1]; // Bit-packed LED states
    struct LED_State
    {
        uint16_t degrees;
        uint8_t led_number;
        bool on;
    };
    LEDControl(SPI_HandleTypeDef *spi);
    void initialize_strip(void);
    LED_State create_led(uint8_t led_number);
    void set_led_state(LED_State *led);
};