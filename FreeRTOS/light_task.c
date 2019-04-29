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

#define PRIORITY_LIGHT_TASK       1

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t g_ui32SysClock;

// Calulated as ((2ms/1000) * SysCtlClockGet())/ 3 for 2ms of delay
#define time 80000

//PWM frequency in hz
uint32_t freq = 100000;

uint32_t Period, dutyCycle;

xTaskHandle lightTaskHandle;

extern xSemaphoreHandle UARTRxDataSem;
extern sensorRx dataIn;

//*****************************************************************************
//
// Light task.
//
//*****************************************************************************
void lightTask( void *pvParameters ) {

    while (1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        xSemaphoreTake(UARTRxDataSem, portMAX_DELAY);
        if (dataIn.lightControl==LIGHT_INCREASE) {
            /* As timer counts down, to increase luminosity, decrease the match count to increase the on time */
            dutyCycle = dutyCycle - 2;
        }
        else if (dataIn.lightControl==LIGHT_DECREASE) {
            /* As timer counts down, to decrease luminosity, increase the match count to decrease the on time */
            dutyCycle = dutyCycle + 2;
        }
        else if (dataIn.lightControl==LIGHT_MAINTAIN_DEFAULT) {
            /* Default dutyCycle */
            dutyCycle = Period - 2;
        }
        xSemaphoreGive(UARTRxDataSem);

        TimerMatchSet(TIMER0_BASE, TIMER_B, dutyCycle);
        TimerMatchSet(TIMER1_BASE, TIMER_A, dutyCycle);
        TimerMatchSet(TIMER1_BASE, TIMER_B, dutyCycle);
        SysCtlDelay(time);
    }

}

//*****************************************************************************
//
// Initializes the Light task.
//
//*****************************************************************************
uint32_t lightTaskInit(void) {
    int i;

    Period = SysCtlClockGet()/freq ;

    /* Default dutyCycle */
    dutyCycle = Period-2;

    /*
        Configure PF1 as T0CCP1
        Configure PF2 as T1CCP0
        Configure PF3 as T1CCP1
     */

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlDelay(3);
    GPIOPinConfigure(GPIO_PD1_T0CCP1);
    GPIOPinConfigure(GPIO_PD2_T1CCP0);
    GPIOPinConfigure(GPIO_PD3_T1CCP1);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    /*
        Configure timer 0 to split pair and timer B in PWM mode
        Set period and starting duty cycle.
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlDelay(3);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_B, Period -1);
    TimerMatchSet(TIMER0_BASE, TIMER_B, dutyCycle); // PWM


    /*
        Configure timer 1 to split pair and timer A and B in PWM mode
        Set period and starting duty cycle.
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    SysCtlDelay(3);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER1_BASE, TIMER_A, Period -1);
    TimerLoadSet(TIMER1_BASE, TIMER_B, Period -1);
    TimerMatchSet(TIMER1_BASE, TIMER_A, dutyCycle);
    TimerMatchSet(TIMER1_BASE, TIMER_B, dutyCycle);

    //Turn on both timers
    TimerEnable(TIMER0_BASE, TIMER_B);
    TimerEnable(TIMER1_BASE, TIMER_A|TIMER_B);

    //
    // Create the light task.
    //
    if(xTaskCreate(lightTask, (const portCHAR *)"QUEUE", LIGHTTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LIGHT_TASK, &lightTaskHandle) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
