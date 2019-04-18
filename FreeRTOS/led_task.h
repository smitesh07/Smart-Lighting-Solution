/*
 * led_task.h
 *
 *  Created on: Apr 5, 2019
 *      Author: modak
 */

#ifndef LED_TASK_H_
#define LED_TASK_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef struct {
    unsigned int timeNow;       // timestamp
    unsigned int tCount;        // toggle count
    char name[10];              // name
}LED_t;

//*****************************************************************************
//
// Prototypes for the LED task.
//
//*****************************************************************************
extern uint32_t LEDTaskInit(void);

#endif /* LED_TASK_H_ */
