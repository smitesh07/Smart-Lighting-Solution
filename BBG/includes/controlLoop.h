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
#include "queue.h"

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

// structure to be sent via UART to the tiva to perform desired action
typedef struct {
    LIGHT_CONTROL light;
    MOTOR_CONTROL motor;
}CONTROL_TX_t;

//Prototypes

/**
 * @brief Get the Action to be performed of the light and motor task
 * 
 * @param lum       Luminosity value received 
 * @return CONTROL_TX_t     Action structure to be passed via UART
 */
void getCurrentAction (void);

/**
 * @brief Handler function / Entry point for the Control Loop thread
 * 
 */
void *controlLoopHandler(void *arg);

#endif /* SRC_CONTROLLOOP_H_ */