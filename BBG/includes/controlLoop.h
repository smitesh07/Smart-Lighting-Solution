/**
 * @file controlLoop.h
 * @author Smitesh Modak and Ashish Tak
 * @brief 
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SRC_CONTROLLOOP_H_
#define SRC_CONTROLLOOP_H_

#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <signal.h>
#include "timer.h"
#include "SimpleGPIO.h"

extern FILE *filePtr;
extern bool controlHeartbeatFlag;

// Macros defining the boundary conditions for light
#define LOW_LIGHT   20.00
#define HIGH_LIGHT  100.00

// different states defining action to be perfomed by the Light Task on Tiva
typedef enum {
    LIGHT_INCREASE,
    LIGHT_DECREASE,
    LIGHT_NO_CHANGE,
    LIGHT_MAINTAIN_DEFAULT
}LIGHT_CONTROL;

// different states defining action to be perfomed by the Motor Task on Tiva
typedef enum {
    MOTOR_OPEN,
    MOTOR_CLOSE,
    MOTOR_NO_CHANGE
}MOTOR_CONTROL;

typedef enum {
    SENSOR_NOT_WORKING,
    SENSOR_WORKING
}SENSOR_STATUS;

typedef enum {
    NO_PROXIMITY,
    PROXIMITY_DETECTED
}PROXIMITY_STATUS;

// structure to be sent via UART to the Tiva board to perform desired action
typedef struct __attribute__((__packed__)){
    uint8_t light;
    uint8_t motor;
}CONTROL_TX_t;

//structure to be received via UART from the Tiva board regarding status of peripherals
typedef struct __attribute__((__packed__)){
    float lux;
    uint8_t proximity;
    uint8_t sensorStatus;
    uint8_t blindsStatus;
} CONTROL_RX_t;

//Prototypes

/**
 * @brief Get the Action to be performed for the light and motor task
 * 
 * @param rxData    Data structure received via UART from the sensor node (Tiva)
 */
void getCurrentAction (CONTROL_RX_t rxData);

/**
 * @brief Handler function / Entry point for the Control Loop thread
 * 
 */
// void *controlLoopHandler(void *arg);

#endif /* SRC_CONTROLLOOP_H_ */