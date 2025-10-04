#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#include <stdint.h>

#define MEM_ALIGNMENT 4
#define MEMSIZE (16 * 1024)

#define TCPIP_THREAD_NAME "TCPIP"
#define TCPIP_THREAD_STACKSIZE 1024
#define TCPIP_THREAD_PRIO 5

#define LWIP_TCP 1
#define LWIP_UDP 1
#define LWIP_DHCP 1
#define LWIP_NETIF_HOSTNAME 1
#define LWIP_SOCKET 1
#define LWIP_NETCONN 1

/* Define byte order conversion functions for embedded system */
#define LWIP_DONT_PROVIDE_BYTEORDER_FUNCTIONS 0

/* Define the byte order conversion functions */
#define lwip_htons(x)                                                          \
  ((uint16_t)((((x) & 0x00ffU) << 8) | (((x) & 0xff00U) >> 8)))
#define lwip_ntohs(x) lwip_htons(x)
#define lwip_htonl(x)                                                          \
  ((uint32_t)((((x) & 0x000000ffUL) << 24) | (((x) & 0x0000ff00UL) << 8) |     \
              (((x) & 0x00ff0000UL) >> 8) | (((x) & 0xff000000UL) >> 24)))
#define lwip_ntohl(x) lwip_htonl(x)

#endif
