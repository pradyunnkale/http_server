#ifndef ETHERNET_DRIVER_H
#define ETHERNET_DRIVER_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_def.h"
#include "stm32h7xx_hal_eth.h"
#include "stm32h7xx_hal_eth_ex.h"
#include "task.h"
#include <stdint.h>

// LwIP includes
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"

// DMA descriptor counts
#ifndef ETH_RX_DESC_CNT
#define ETH_RX_DESC_CNT 4
#endif
#ifndef ETH_TX_DESC_CNT
#define ETH_TX_DESC_CNT 4
#endif

// Note: Defined in main.c
extern ETH_HandleTypeDef heth1;

// LwIP network interface - will be defined when you create ethernetif.c
extern struct netif gnetif;

// Initialize ethernet + phy
void ethernet_init(void);

// Transmit a frame
void ethernet_transfer(void);

void ETH_IRQHandler(void);
// Handle received Ethernet Frames (gonna do ISR because im using HAL, gonna do
// DMA as well, RTOS as well)
void ethernet_rx_handler(void);

void eth_rx_task(void *argument);

void eth_tx_task(void *argument);

// LwIP integration functions (will be implemented in ethernetif.c)
err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);

#endif // ETHERNET_DRIVER_H
