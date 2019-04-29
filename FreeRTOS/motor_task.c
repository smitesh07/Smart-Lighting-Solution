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

xTaskHandle motorTaskHandle;

extern xSemaphoreHandle UARTRxDataSem, UARTTxDataSem;
extern sensorRx dataIn;
extern sensorTx dataOut;

//*****************************************************************************
//
// Initialize GPIO for motor control
//
//*****************************************************************************
static void gpioInit(void) {
    //
    // Enable the GPIO pin for the Motor (PL2).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL))
    {
    }

    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTL_BASE,GPIO_PIN_2,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
}

//*****************************************************************************
//
// Motor task.
//
//*****************************************************************************
void motorTask( void *pvParameters ) {
    while (1) {
        //Wait for a notification signal from the UART task that some actuation is required
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        xSemaphoreTake(UARTRxDataSem, portMAX_DELAY);
        xSemaphoreTake(UARTTxDataSem, portMAX_DELAY);
        if (dataIn.motorControl==MOTOR_CLOSE) {
            dataOut.blindsStatus= MOTOR_CLOSE;
        }
        else if (dataIn.motorControl==MOTOR_OPEN) {
            dataOut.blindsStatus= MOTOR_OPEN;
        }
        xSemaphoreGive(UARTTxDataSem);
        xSemaphoreGive(UARTRxDataSem);

        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_PIN_2);
        /* Calculated as 5 * SysCtlClockGet() / 3 for 5 seconds of delay */
        SysCtlDelay(400000000);
        GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
    }

}

//*****************************************************************************
//
// Initializes the Motor task.
//
//*****************************************************************************
uint32_t motorTaskInit(void) {

    // Initialize GPIO
    gpioInit();

    //
    // Create the motor task.
    //
    if(xTaskCreate(motorTask, (const portCHAR *)"MOTOR_TASK", MOTORTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_MOTOR_TASK, &motorTaskHandle) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
