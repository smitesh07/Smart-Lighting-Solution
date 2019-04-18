/*
 * alert_task.h
 *
 *  Created on: Apr 10, 2019
 *      Author: modak
 */

#ifndef ALERT_TASK_H_
#define ALERT_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "utils/uartstdio.h"

/* Handles for the tasks create by main(). */
extern TaskHandle_t xAlertTask;
uint32_t AlertTaskInit(void);

#endif /* ALERT_TASK_H_ */
