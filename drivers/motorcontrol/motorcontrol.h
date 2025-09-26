#include <stdint.h>
#include "stm32h7xx_hal.h"

class MotorControl
{
private:
    TIM_HandleTypeDef *htim;
    unsigned int channel;
public:
    MotorControl(TIM_HandleTypeDef *htim, uint32_t channel);
    void initialize_motor(void);
    void set_motor_duty(uint16_t duty_cycle);
};