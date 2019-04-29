/*
 * uartComm.c
 *
 *  Created on: Apr 27, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#include "uartComm.h"


//Task stack size specified as the number of variables
#define UARTTASKSTACKSIZE   128

#define PRIORITY_UART_TASK  1

#define UART_TX_CYCLE   5000    //in milliseconds

//Defined as the number of times a TX cycle is attempted
#define UART_CONNECTION_TIMEOUT 10

uint8_t UARTConnectionStatus=1, UARTTimeoutCount=0;

//UART transmission semaphore (to be periodically released through a timer)
xSemaphoreHandle UARTTxSempahore;

//Binary semaphores to guard the UART Tx and Rx Data structures
xSemaphoreHandle UARTTxDataSem, UARTRxDataSem;

sensorRx dataIn;
sensorTx dataOut;

extern xTaskHandle lightTaskHandle, motorTaskHandle;

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
extern uint32_t g_ui32SysClock;


/**
 * ISR to handle UART reception
 */
void UART6_IntHandler (void) {
    UARTConnectionStatus=1;
    static uint8_t *ptr = (uint8_t *)&dataIn;
    xSemaphoreTake(UARTRxDataSem, portMAX_DELAY);
    while (UARTCharsAvail(UART6_BASE)) {
        //Write data byte-by-byte to the sensorRx structure
        //Roll over and notify the actuation tasks if the structure is filled up
        if (ptr== (uint8_t *)&dataIn + sizeof(sensorRx)) {
            ptr = (uint8_t *)&dataIn;
            UARTprintf("\nReceived Light Control: %d\tMotor Control: %d", dataIn.lightControl, dataIn.motorControl);
            if (dataIn.lightControl!=LIGHT_NO_CHANGE) {
                xTaskNotifyGive(lightTaskHandle);
            }
            if (dataIn.motorControl!=MOTOR_NO_CHANGE) {
                xTaskNotifyGive(motorTaskHandle);
            }
        }
        *ptr= (uint8_t) UARTCharGet(UART6_BASE);
        ++ptr;
    }
    xSemaphoreGive(UARTRxDataSem);

    UARTIntClear(UART6_BASE, UART_INT_RX);
//    UARTIntEnable(UART6_BASE, UART_INT_RX);
}


/**
 * Perform the configuration for UART Tx and Rx
 * (currently harcoded to UART6)
 */
void configureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);

    // Enable UART6.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PP0_U6RX);
    ROM_GPIOPinConfigure(GPIO_PP1_U6TX);
    ROM_GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Set UART parameters
    UARTConfigSetExpClk(UART6_BASE, g_ui32SysClock, 115200, \
                        (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8) );

    UARTEnable(UART6_BASE);

    //Enable the Receive interrupt and register its Interrupt Handler
    UARTIntEnable(UART6_BASE, UART_INT_RX);
    UARTIntRegister(UART6_BASE, UART6_IntHandler);

    //Setting the FIFO level as per the size of the structure to be received
    uint32_t pui32TxLevel, pui32RxLevel;
    UARTFIFOLevelGet(UART6_BASE, &pui32TxLevel, &pui32RxLevel);
    UARTFIFOLevelSet(UART6_BASE, pui32TxLevel, sizeof(sensorRx));
}


/**
 * Timer callback to trigger the UART transmission task
 */
void vUARTTimerCallBack( TimerHandle_t xUARTTimer ) {
    //Signal the light task to maintain a default lighting intensity
    //in case of a connection loss with the control node
    ++UARTTimeoutCount;
    if (UARTTimeoutCount == UART_CONNECTION_TIMEOUT) {
        UARTTimeoutCount=0;
        if (UARTConnectionStatus) {
            UARTConnectionStatus=0;
        }
        else {
            xTaskNotifyGive(lightTaskHandle);
        }
    }

    //Release the semaphore to begin UART transmission
    xSemaphoreGive(UARTTxSempahore);
}


/**
 * Entry point for the UART transmission task
 */
void uartTxRoutine (void * pvParameters) {
    uint8_t *ptr, i;

    while (1) {
        if( xSemaphoreTake( UARTTxSempahore, portMAX_DELAY ) == pdTRUE ) {
            xSemaphoreTake(UARTTxDataSem, portMAX_DELAY);
            ptr=(uint8_t *) &dataOut;
            for (i=0; i<sizeof(dataOut);i++) {
                UARTCharPut(UART6_BASE, (uint8_t)*((uint8_t *)ptr));
                ++ptr;
            }
            xSemaphoreGive(UARTTxDataSem);
        }
    }
}


int8_t uartTaskInit (void) {
    if (xTaskCreate(uartTxRoutine, (const portCHAR *)"UART_TRANSMIT_TASK", UARTTASKSTACKSIZE, NULL, \
                    tskIDLE_PRIORITY + PRIORITY_UART_TASK, NULL) != pdTRUE) {
        UARTprintf("\n\nError in creating the UART Transmission task!\n");
        return -1;
    }

    configureUART();

    vSemaphoreCreateBinary(UARTTxSempahore);
    if (UARTTxSempahore == NULL) {
        UARTprintf("\n\nError in creating the semaphore for the UART Transmission task!\n");
        return -1;
    }

    vSemaphoreCreateBinary(UARTTxDataSem);
    if (UARTTxDataSem == NULL) {
        UARTprintf("\n\nError in creating the semaphore for the UART Transmission data!\n");
        return -1;
    }

    vSemaphoreCreateBinary(UARTRxDataSem);
    if (UARTRxDataSem == NULL) {
        UARTprintf("\n\nError in creating the semaphore for the UART Reception data!\n");
        return -1;
    }

    //Create and start the timer for triggering UART transmission
    TimerHandle_t xUARTTimer = xTimerCreate("UARTTimer", pdMS_TO_TICKS(UART_TX_CYCLE), pdTRUE, (void *) 0, vUARTTimerCallBack);
    if ( xTimerStart(xUARTTimer, 0)!= pdPASS ) {
        UARTprintf("\n\nError in starting UART timer!\n");
        return -1;
    }

    //
    // Return Success.
    //
    return(0);
}
