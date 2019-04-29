/*
 * prox_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef PROX_TASK_H_
#define PROX_TASK_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "uartComm.h"


typedef enum {
    NO_PROXIMITY,
    PROXIMITY_DETECTED
}PROXIMITY_STATUS;


//*****************************************************************************
//
// Prototypes for the proximity sensor task.
//
//*****************************************************************************
uint32_t proxTaskInit(void);

#endif /* PROX_TASK_H_ */
