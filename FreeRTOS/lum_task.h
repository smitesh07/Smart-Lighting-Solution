/*
 * lum_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef LUM_TASK_H_
#define LUM_TASK_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "apds9301.h"
#include "uartComm.h"


typedef enum {
    SENSOR_NOT_WORKING,
    SENSOR_WORKING
}SENSOR_STATUS;


//*****************************************************************************
//
// Prototypes for the luminosity sensor task.
//
//*****************************************************************************
uint32_t lumTaskInit(void);

#endif /* LUM_TASK_H_ */
