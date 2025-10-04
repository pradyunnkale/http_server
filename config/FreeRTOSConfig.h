#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "stm32h7xx_hal.h"

#include <stdint.h>

/* Scheduler settings */
#define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0
#define configCPU_CLOCK_HZ (SystemCoreClock)  // set in HAL
#define configTICK_RATE_HZ ((TickType_t)1000) // 1 ms tick
#define configMAX_PRIORITIES 5
#define configMINIMAL_STACK_SIZE 128
#define configTOTAL_HEAP_SIZE (64 * 1024)
#define configMAX_TASK_NAME_LEN 16
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 1
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configKERNEL_INTERRUPT_PRIORITY 255

/* Mutexes and semaphores */
#define configUSE_MUTEXES 1
#define configUSE_COUNTING_SEMAPHORES 1
#define configUSE_BINARY_SEMAPHORES 1
#define configUSE_RECURSIVE_MUTEXES 1

#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* API functions */
#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_xSemaphoreTake 1
#define INCLUDE_xSemaphoreGive 1
#define INCLUDE_xSemaphoreGiveFromISR 1
#define INCLUDE_taskYIELD 1

#endif /* FREERTOS_CONFIG_H */
