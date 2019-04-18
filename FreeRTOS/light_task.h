/*
 * light_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef LIGHT_TASK_H_
#define LIGHT_TASK_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//*****************************************************************************
//
// Prototypes for the light task.
//
//*****************************************************************************
uint32_t lightTaskInit(void);

#endif /* LIGHT_TASK_H_ */
