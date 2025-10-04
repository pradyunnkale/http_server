// drivers/ethernet

#ifndef ETHERNETIF_H
#define ETHERNETIF_H

#include "FreeRTOS.h"
#include "ethernet.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "stm32h7xx_hal_gpio.h"
#include "task.h"

extern struct netif gnetif;

err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);
void lwip_init_task(void *argument);

#endif // ETHERNETIF_H