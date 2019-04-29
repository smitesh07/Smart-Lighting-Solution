/*
 * light_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef LIGHT_TASK_H_
#define LIGHT_TASK_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "uartComm.h"


// different states defining action to be perfomed by the Light Task
typedef enum {
    LIGHT_INCREASE,
    LIGHT_DECREASE,
    LIGHT_NO_CHANGE,
    LIGHT_MAINTAIN_DEFAULT
}LIGHT_CONTROL;

extern xTaskHandle lightTaskHandle;


//*****************************************************************************
//
// Prototypes for the light task.
//
//*****************************************************************************
uint32_t lightTaskInit(void);

#endif /* LIGHT_TASK_H_ */
