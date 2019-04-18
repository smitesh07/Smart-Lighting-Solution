/*
 * prox_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef PROX_TASK_H_
#define PROX_TASK_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//*****************************************************************************
//
// Prototypes for the proximity sensor task.
//
//*****************************************************************************
uint32_t proxTaskInit(void);

#endif /* PROX_TASK_H_ */
