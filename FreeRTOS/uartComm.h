/*
 * uartComm.h
 *
 *  Created on: Apr 27, 2019
 *      Authors: Smitesh Modak and Ashish Tak
 */

#ifndef UARTCOMM_H_
#define UARTCOMM_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "light_task.h"
#include "motor_task.h"


//Structure to be sent via UART to the BBG for reporting status of the peripherals
typedef struct __attribute__((__packed__)){
    float lux;
    uint8_t proximity;
    uint8_t sensorStatus;
    uint8_t blindsStatus;
} sensorTx;

//Structure to be received via UART from the BBG for performing the actuation tasks
typedef struct __attribute__((__packed__)){
    uint8_t lightControl;
    uint8_t motorControl;
} sensorRx;


extern xSemaphoreHandle UARTTxDataSem;
extern sensorTx dataOut;
extern sensorRx dataIn;


/**
 * Prototypes
 */

/**
 * Initialize the UART task
 */
int8_t uartTaskInit (void);

#endif /* UARTCOMM_H_ */
