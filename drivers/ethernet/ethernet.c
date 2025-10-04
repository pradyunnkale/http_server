#include "ethernet.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "projdefs.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_eth.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_uart.h"
#include <stdio.h>
#include <string.h>

__attribute__((aligned(4))) ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT];
__attribute__((aligned(4))) ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT];

// Temp before implementing lwip
uint8_t frame_data[1524] = {
    // Destination MACAddr
    0xC8, 0x53, 0x09, 0xED, 0xDE, 0x0E,
    // Source MACAddr
    0x00, 0x80, 0xE1, 0x00, 0x00, 0x00,
    // EtherType: Custom (easier to spot)
    0xAB, 0xCD,
    // Payload: Very distinctive pattern
    0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE, 0x12, 0x34, 0x56, 0x78,
    0x9A, 0xBC, 0xDE, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};

uint16_t frame_len = 64;

SemaphoreHandle_t ethRxSemaphore;
extern UART_HandleTypeDef huart3;

// TODO: Initiate GPIO pins for the PHY
void ethernet_init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    ETH_MACConfigTypeDef mac_config;
    uint8_t MACAddr[6] = {0x00, 0x80, 0xE1, 0x00, 0x00, 0x00};

    heth1.Instance = ETH;
    heth1.Init.MACAddr = &MACAddr[0];
    heth1.Init.MediaInterface = HAL_ETH_RMII_MODE;
    heth1.Init.RxBuffLen = 1524;

    // Assign DMA descriptors - use correct field names
    heth1.Init.TxDesc = DMATxDscrTab;
    heth1.Init.RxDesc = DMARxDscrTab;

    if (HAL_ETH_Init(&heth1) != HAL_OK) {
        return;
    }

    HAL_ETH_GetMACConfig(&heth1, &mac_config);
    mac_config.DuplexMode = ETH_FULLDUPLEX_MODE;
    mac_config.Speed = ETH_SPEED_100M;
    HAL_ETH_SetMACConfig(&heth1, &mac_config);

    if (HAL_ETH_Start(&heth1) != HAL_OK) {
        return;
    }

    ethRxSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(ethRxSemaphore);

    HAL_NVIC_SetPriority(ETH_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

// Transmit a frame
void ethernet_transfer(void) {
    ETH_BufferTypeDef txBuf;
    ETH_TxPacketConfigTypeDef txCfg;

    txBuf.buffer = frame_data;
    txBuf.len = frame_len;
    txBuf.next = NULL;

    txCfg.TxBuffer = &txBuf;
    txCfg.Length = txBuf.len;
    txCfg.Attributes =
        ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;

    // Send the packet with very short timeout
    HAL_StatusTypeDef result =
        HAL_ETH_Transmit(&heth1, &txCfg, 1000); // 1000ms timeout

    // If it fails, don't hang - just continue
    (void)result;
}

void ETH_IRQHandler(void) {
    HAL_ETH_IRQHandler(&heth1);
}

void eth_rx_task(void* argument) {
    for (;;) {
        if (xSemaphoreTake(ethRxSemaphore, portMAX_DELAY) == pdTRUE) {
            ethernetif_input(&gnetif); // Feed packet to LwIP
        }
    }
}

void eth_tx_task(void* argument) {
    vTaskDelay(
        pdMS_TO_TICKS(5000)); // Initial delay to allow system to stabilize
    for (;;) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = lwip_htons(5000);
        inet_aton("100.69.108.106", &server_addr.sin_addr);

        if (connect(sock, (struct sockaddr*)&server_addr,
                    sizeof(server_addr)) == 0) {
            char http_request[] = "POST / HTTP/1.1\r\n"
                                  "Host: 100.69.108.106:5000\r\n"
                                  "Content-Type: application/json\r\n"
                                  "Content-Length: 19\r\n"
                                  "\r\n"
                                  "{\"sensor_data\": 42}";
            send(sock, http_request, sizeof(http_request) - 1, 0);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
            vTaskDelay(pdMS_TO_TICKS(100));
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        }
        close(sock);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef* heth) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(ethRxSemaphore, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        taskYIELD();
    }
}