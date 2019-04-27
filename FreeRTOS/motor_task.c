/*
 * motor_task.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#include "motor_task.h"

//*****************************************************************************
//
// The stack size for the motor task.
//
//*****************************************************************************
#define MOTORTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_MOTOR_TASK       2

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// Motor task.
//
//*****************************************************************************
void motorTask( void *pvParameters ) {

}

//*****************************************************************************
//
// Initializes the Motor task.
//
//*****************************************************************************
uint32_t motorTaskInit(void) {
    //
    // Create the motor task.
    //
    if(xTaskCreate(motorTask, (const portCHAR *)"QUEUE", MOTORTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_MOTOR_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
