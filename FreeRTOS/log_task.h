/*
 * lum_task.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#ifndef LOG_TASK_H_
#define LOG_TASK_H_

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//// Identifier to determine which task has updated the QUEUE_t structure
//typedef enum {
//    LED_TASK,
//    TEMP_TASK
//}LOG_t;
//
//// Queue structure to be utilized for Inter task communication
//typedef struct {
//    LOG_t taskID;
//    union task_str{
//    LED_t led_task_t;
//    TEMP_t temp_task_t;
//    }TASK_t;
//}QUEUE_t;

//*****************************************************************************
//
// Prototypes for the luminosity sensor task.
//
//*****************************************************************************
uint32_t logTaskInit(void);
//void enqueueForLog(QUEUE_t *);

#endif /* LOG_TASK_H_ */
