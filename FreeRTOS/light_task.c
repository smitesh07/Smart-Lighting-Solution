/*
 * light_task.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#include "light_task.h"

//*****************************************************************************
//
// The stack size for the light task.
//
//*****************************************************************************
#define LIGHTTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_LIGHT_TASK       3

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// Light task.
//
//*****************************************************************************
void lightTask( void *pvParameters ) {

}

//*****************************************************************************
//
// Initializes the Light task.
//
//*****************************************************************************
uint32_t lightTaskInit(void) {
    //
    // Create the light task.
    //
    if(xTaskCreate(lightTask, (const portCHAR *)"QUEUE", LIGHTTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LIGHT_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
