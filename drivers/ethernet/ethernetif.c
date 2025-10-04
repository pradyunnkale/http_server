#include "ethernetif.h"
#include "FreeRTOS.h"
#include "ethernet.h"
#include "lwip/dhcp.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_eth.h"
#include "stm32h7xx_hal_gpio.h"
#include "task.h"
#include <string.h>

extern struct netif gnetif;

err_t low_level_init(struct netif* netif) {
    ethernet_init();
    return ERR_OK;
}

err_t low_level_output(struct netif* netif, struct pbuf* p) {
    ethernet_transfer();
    return ERR_OK;
}

struct pbuf* low_level_input(struct netif* netif) {
    ETH_BufferTypeDef rxBuf;
    HAL_StatusTypeDef status;
    struct pbuf* p = NULL;
    status = HAL_ETH_ReadData(&heth1, (void**)&rxBuf);
    if (status == HAL_OK) {
        struct pbuf* p;
        p = pbuf_alloc(PBUF_RAW, rxBuf.len, PBUF_POOL);
        if (p != NULL) {
            struct pbuf* q;
            uint8_t* buffer = (uint8_t*)rxBuf.buffer;
            uint16_t len = rxBuf.len;
            for (q = p; q != NULL; q = q->next) {
                memcpy(q->payload, buffer, q->len);
                buffer += q->len;
                len -= q->len;
            }
        }
    }
    return p;
}

err_t ethernetif_init(struct netif* netif) {
    netif->state = NULL;
    netif->name[0] = 'e';
    netif->name[1] = 'n';
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;
    low_level_init(netif);
    return ERR_OK;
}

void ethernetif_input(struct netif* netif) {
    struct pbuf* p;
    p = low_level_input(netif);
    if (p != NULL) {
        if (netif->input(p, netif) != ERR_OK) {
            pbuf_free(p);
            p = NULL;
        }
    }
}

void lwip_init_task(void* argument) {
    tcpip_init(NULL, NULL);
    netif_add(&gnetif, NULL, NULL, NULL, NULL, ethernetif_init, tcpip_input);
    netif_set_default(&gnetif);
    netif_set_up(&gnetif);

    dhcp_start(&gnetif);

    vTaskDelay(pdMS_TO_TICKS(1000));

    for (;;) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // LED OFF
        vTaskDelay(pdMS_TO_TICKS(100));                        // Short ON
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);   // LED
        vTaskDelay(pdMS_TO_TICKS(1000));                       // Long OFF
    }
}