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

//Prototypes


/**
 * @brief Handler function / Entry point for the Control Loop thread
 * 
 */
void *controlLoopHandler(void *arg);

#endif /* SRC_CONTROLLOOP_H_ */