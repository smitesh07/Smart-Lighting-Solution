/**
 * @file uart.h
 * @author Smitesh Modak and Ashish Tak
 * @brief 
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

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
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include "timer.h"
#include "queue.h"
#include "controlLoop.h"

extern FILE *filePtr;
extern bool uartHeartbeatFlag;

extern CONTROL_TX_t dataOut;

//Prototypes

/**
 * @brief Handler function / Entry point for the UART communication thread
 * 
 */
void *uartHandler(void *arg);

#endif /* SRC_UART_H_ */