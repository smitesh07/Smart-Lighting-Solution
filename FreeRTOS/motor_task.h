/*
 * motor_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef MOTOR_TASK_H_
#define MOTOR_TASK_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "uartComm.h"


// different states defining action to be perfomed by the Motor Task on Tiva
typedef enum {
    MOTOR_OPEN,
    MOTOR_CLOSE,
    MOTOR_NO_CHANGE
}MOTOR_CONTROL;

extern xTaskHandle motorTaskHandle;

//*****************************************************************************
//
// Prototypes for the motor task.
//
//*****************************************************************************
uint32_t motorTaskInit(void);

#endif /* MOTOR_TASK_H_ */
