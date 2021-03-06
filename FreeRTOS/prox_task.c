/*
 * prox_task.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Smitesh Modak and Ashish Tak
 */

#include "prox_task.h"

//*****************************************************************************
//
// The stack size for the Proximity Sensor task.
//
//*****************************************************************************
#define PROXTASKSTACKSIZE   128         // Stack size in words

#define PRIORITY_PROX_TASK  3

#define PROXIMITY_DETECTION_LIMIT   40      //Dist in cm for determining proximity

#define PROX_TASK_PERIOD    5000        //in ms

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

/* Semaphore to be used to wake up Temp task at 1 Hz */
xSemaphoreHandle xSemaphoreProx;

/* Handle to the created timer. */
TimerHandle_t xProxTimer;

extern xSemaphoreHandle UARTTxDataSem;
extern sensorTx dataOut;


/* Define a callback function that will be used by Lum timer
 instance.  The callback function does nothing but pass on the semaphore to the waiting task */
void vProxTimerCallback( TimerHandle_t xTimer )
{
//    UARTprintf("Interrupt Giving Prox Semaphore\n");
    xSemaphoreGive(xSemaphoreProx);
}

//*****************************************************************************
//
// Initialize and enable timer to signal task after timeout.
//
//*****************************************************************************
static void enableTaskTimer(void) {
    xProxTimer = xTimerCreate
                       ( /* Just a text name, not used by the RTOS
                         kernel. */
                         "Timer",
                         /* The timer period in ticks, must be
                         greater than 0. */
                         pdMS_TO_TICKS(PROX_TASK_PERIOD),
                         /* The timers will auto-reload themselves
                         when they expire. */
                         pdTRUE,
                         /* The ID is used to store a count of the
                         number of times the timer has expired, which
                         is initialised to 0. */
                         ( void * ) 0,
                         /* Each timer calls the same callback when
                         it expires. */
                         vProxTimerCallback
                       );

    if( xProxTimer == NULL )
    {
        /* The timer was not created. */
        UARTprintf("Prox Timer not created \n");
    }
    else
    {
        UARTprintf("Prox Timer created \n");
        /* Start the timer.  No block time is specified, and
                 even if one was it would be ignored because the RTOS
                 scheduler has not yet been started. */
        if( xTimerStart( xProxTimer, 0 ) != pdPASS )
        {
            /* The timer could not be set into the Active
                     state. */
        }
    }

}

//*****************************************************************************
//
// Initialize proximity sensor
//
//*****************************************************************************
static void initProxSensor(void) {
    //
    // Enable the GPIO port that is used for the Proximity sensor.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM))
    {
    }

    //
    // Enable the GPIO pins for the Echo and Trigger pin.  Set the direction as input for Echo pin and
    // output for trigger pin, and enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_1);     //Trigger pin
    GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_0);     //Echo pin

    // Configure Echo pin was weak pull down
    GPIOPadConfigSet(GPIO_PORTM_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPD);
}

//*****************************************************************************
//
// Initialize timer to capture the echo pulse
//
//*****************************************************************************
static void captureInit(void){
    //
    // Set the timer to be periodic.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

    //
    // Configure the two 32-bit periodic timer.
    //
    ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);

    //
    // Enable the timer.
    //
    ROM_TimerEnable(TIMER2_BASE, TIMER_A);
}

//*****************************************************************************
//
// Proximity sensor task.
//
//*****************************************************************************
void proxTask( void *pvParameters ) {
    //This is to avoid doing the math everytime you do a reading
    const double temp = 1.0/120.0;

    //Stores the pulse length
    volatile uint32_t pulse=0;          // rising edge time
    volatile uint32_t pulse1 = 0;       // falling edge time

//    UARTprintf("Prox Task Created\n");

    while(1) {
        xSemaphoreTake(xSemaphoreProx, portMAX_DELAY);

//        UARTprintf("Semaphore received\n");

        //Does the required pulse of 10uS
        GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(SysCtlClockGet() / 100000);
        //    SysCtlDelay(266);
        GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_1, ~GPIO_PIN_1);

        while( GPIOPinRead(GPIO_PORTM_BASE, GPIO_PIN_0) == 0);

        pulse1 = TimerValueGet(TIMER2_BASE,TIMER_A); //record value

        while( GPIOPinRead(GPIO_PORTM_BASE, GPIO_PIN_0) == 1);

        pulse = TimerValueGet(TIMER2_BASE,TIMER_A); //record value

        //Converts the counter value to cm.
        pulse = pulse - pulse1;
        pulse = (uint32_t)(temp * pulse);
        pulse = pulse / 58;

        //Prints out the distance measured.
        UARTprintf("Distance = %2dcm \n" , pulse);

        xSemaphoreTake(UARTTxDataSem, portMAX_DELAY);
        if (pulse<PROXIMITY_DETECTION_LIMIT) {
            dataOut.proximity= PROXIMITY_DETECTED;
        }
        else {
            dataOut.proximity= NO_PROXIMITY;
        }
        xSemaphoreGive(UARTTxDataSem);
    }
}

//*****************************************************************************
//
// Initializes the Proximity sensor task.
//
//*****************************************************************************
uint32_t proxTaskInit(void) {

    // Initialize Proximity sensor
    initProxSensor();

    // Enable task timer
    enableTaskTimer();

    // Enable the timer to capture echo pulse
    captureInit();

    // Create a Semaphore to signal the task
    xSemaphoreProx = xSemaphoreCreateBinary();

    //
    // Create the prox task.
    //
    if(xTaskCreate(proxTask, (const portCHAR *)"QUEUE", PROXTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_PROX_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
