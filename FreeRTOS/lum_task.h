/*
 * lum_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef LUM_TASK_H_
#define LUM_TASK_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//*****************************************************************************
//
// Prototypes for the luminosity sensor task.
//
//*****************************************************************************
uint32_t lumTaskInit(void);

#endif /* LUM_TASK_H_ */
