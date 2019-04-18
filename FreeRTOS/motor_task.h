/*
 * motor_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef MOTOR_TASK_H_
#define MOTOR_TASK_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
//*****************************************************************************
//
// Prototypes for the motor task.
//
//*****************************************************************************
uint32_t motorTaskInit(void);

#endif /* MOTOR_TASK_H_ */
