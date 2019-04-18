/*
 * temp_task.c
 *
 *  Created on: Apr 9, 2019
 *      Author: modak
 */

#include "temp_task.h"
#include "log_task.h"
#include "my_i2c.h"
#include "alert_task.h"

//*****************************************************************************
//
// The stack size for the Temperature sensing task.
//
//*****************************************************************************
#define TEMPTASKSTACKSIZE        128         // Stack size in words

#define PRIORITY_TEMP_TASK       1

/* Converts a time in ticks to a time in milliseconds. */
#define pdTICKS_TO_MS( xTimeInTicks ) ( ( TickType_t ) ( ( ( TickType_t ) ( xTimeInTicks ) * ( TickType_t ) 1000 ) / ( TickType_t ) configTICK_RATE_HZ ) )

#define TEMP_SENSOR_ADDR 0x48      // TMP102 address from datasheet

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

/* Semaphore to be used to wake up Temp task at 1 Hz */
xSemaphoreHandle xSemaphoreTemp;

uint8_t tempReg = 0x00;             // Temperature register from datasheet
uint8_t configReg = 0x01;           // Config register from datasheet
uint8_t tlowReg = 0x02;             // Temperature Low register from datasheet
uint8_t thighReg= 0x03;             // Temperature High register from datasheet

/**
 * @brief Read temperature register
 *
 * @return float returns temperature in degree celsius
 */
static float readTemp(void) {
    uint8_t buf[2] = {0};
    int16_t data;

    I2CGet2Bytes(buf, TEMP_SENSOR_ADDR, tempReg);

    // Bit 0 of second byte will always be 0 in 12-bit readings and 1 in 13-bit
    if(buf[1]&0x01) // 13 bit mode
    {
        // printf("Entered 13 bit mode");
        // Combine bytes to create a signed int
        data = ((buf[0]) << 5) | (buf[1] >> 3);
        // Temperature data can be + or -, if it should be negative,
        // convert 13 bit to 16 bit and use the 2s compliment.
        if(data > 0xFFF)
        {
            data |= 0xE000;
        }
    }
    else    // 12 bit mode
    {
        // Combine bytes to create a signed int
        data = ((buf[0]) << 4) | (buf[1] >> 4);
        // Temperature data can be + or -, if it should be negative,
        // convert 12 bit to 16 bit and use the 2s compliment.
        if(data > 0x7FF)
        {
            data |= 0xF000;
        }
    }


    float tempData = (float)data;
    return tempData * 0.0625;
}

/* Timer handler */
void Timer1IntHandler(void) {
    // Clear the timer interrupt.
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//    unsigned int currTime = xTaskGetTickCount();
//    UARTprintf("Current handler: %d\n", pdTICKS_TO_MS(currTime));
//    UARTprintf("Interrupt Giving LED Semaphore\n");
    xSemaphoreGive(xSemaphoreTemp);
}

//*****************************************************************************
//
// This task reads temperature sensor values and notifies the alert task
// whenever the temperature value crosses user defined threshold.
//
//*****************************************************************************
static void
TempTask(void *pvParameters)
{
        uint32_t currTime;              // Timestamp
        TEMP_t tempUpdate;
        QUEUE_t queueUpdate;
        float tempVal;

        //
        // Loop forever.
        //
        while(1)
        {
//            UARTprintf("Waiting for Temp Semaphore\n");

            xSemaphoreTake(xSemaphoreTemp, portMAX_DELAY);

            currTime = xTaskGetTickCount();
//            UARTprintf("Current time: %d\n", pdTICKS_TO_MS(currTime));


            //
            // Update the temp structure
            //
            tempUpdate.timeNow = pdTICKS_TO_MS(currTime);
            tempVal = readTemp();
            tempUpdate.tempVal = tempVal;

            if(tempVal > ALERT_TEMP) {
                /* Send a notification to AlertTask(), bringing it out of the Blocked
                        state. */
                xTaskNotifyGive( xAlertTask );
            }

            queueUpdate.taskID = TEMP_TASK;
            queueUpdate.TASK_t.temp_task_t = tempUpdate;

            enqueueForLog(&queueUpdate);
        }
}

//*****************************************************************************
//
// Initializes the Temperature task.
//
//*****************************************************************************
uint32_t
TempTaskInit(void)
{
    //
    // Enable the I2C2 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);

    //
    // Enable the GPIO port that is used for I2C.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Configure the I2CSDA and I2CSCL pins
    //
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinConfigure(GPIO_PN5_I2C2SCL);

    //
    // Select the I2C function for these pins.
    //
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);

    //
    // Enable the Timer peripheral.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //
    // Enable and initialize the I2C2 master module.
    //
    I2CMasterInitExpClk(I2C2_BASE, g_ui32SysClock, false);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Configure the two 32-bit periodic timer.
    //
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A,  g_ui32SysClock - 1);

    //
    // Setup the interrupts for the timer timeout.
    //
    ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timer.
    //
    ROM_TimerEnable(TIMER1_BASE, TIMER_A);

    // Create a Semaphore to signal the task
    xSemaphoreTemp = xSemaphoreCreateBinary();

    //
    // Create the Temperature task.
    //
    if(xTaskCreate(TempTask, (const portCHAR *)"LED", TEMPTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_TEMP_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}


