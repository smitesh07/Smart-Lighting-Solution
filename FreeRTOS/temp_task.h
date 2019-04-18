/*
 * temp_task.h
 *
 *  Created on: Apr 9, 2019
 *      Author: modak
 */

#ifndef TEMP_TASK_H_
#define TEMP_TASK_H_

#include "led_task.h"

#define ALERT_TEMP 28

typedef struct {
    unsigned int timeNow;
    float tempVal;
}TEMP_t;

uint32_t TempTaskInit(void);

#endif /* TEMP_TASK_H_ */
