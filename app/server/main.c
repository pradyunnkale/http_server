#include "ethernet.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "ethernetif.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "stm32h753xx.h"
#include "stm32h7xx_hal.h"
#include "task.h"
#include <stdint.h>
#include <string.h>

ETH_HandleTypeDef heth1;
UART_HandleTypeDef huart3;
struct netif gnetif;

int main() {
    HAL_Init();

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    ethernet_init();

    // Make sure system clock is properly configured for FreeRTOS
    SystemCoreClockUpdate();

    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0xFF);

    // Create LwIP init task
    BaseType_t taskResult3 =
        xTaskCreate(lwip_init_task, "lwip", 2048, NULL, 1, NULL);

    // Create networking tasks
    BaseType_t taskResult =
        xTaskCreate(eth_tx_task, "eth_tx", 1024, NULL, 2, NULL);
    BaseType_t taskResult2 =
        xTaskCreate(eth_rx_task, "eth_rx", 1024, NULL, 2, NULL);

    vTaskStartScheduler();

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    while (1) {

    }
}
