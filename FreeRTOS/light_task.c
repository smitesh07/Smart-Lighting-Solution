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

//about 2ms at 80Mhz
#define time 56666

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
    int i;
    while (1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

        //TODO: Change the duty cycle as per value of the commands received via UART from BBG
        xSemaphoreTake(UARTRxDataSem, portMAX_DELAY);
        if (dataIn.lightControl==LIGHT_INCREASE) {

        }
        else if (dataIn.lightControl==LIGHT_DECREASE) {

        }
        else if (dataIn.lightControl==LIGHT_MAINTAIN_DEFAULT) {

        }
        xSemaphoreGive(UARTRxDataSem);

        TimerMatchSet(TIMER1_BASE, TIMER_A, dutyCycle);
        TimerMatchSet(TIMER1_BASE, TIMER_B, dutyCycle);
        SysCtlDelay(time);

//        //Blue brightness goes up - PF2
//        for(i=Period-2; i > 0;i--){
//            TimerMatchSet(TIMER1_BASE, TIMER_A, i);
//            TimerMatchSet(TIMER1_BASE, TIMER_B, i);
//            SysCtlDelay(time);
//        }
//        //Red brightness goes down - PF1
//        for(i=1; i < Period-1; i++){
//            TimerMatchSet(TIMER0_BASE, TIMER_B, i);
//            SysCtlDelay(time);
//        }
//        //Green brightness goes up - PF3
//        for(i=Period-2; i >  0;i--){
//            TimerMatchSet(TIMER1_BASE, TIMER_B, i);
//            SysCtlDelay(time);
//        }
//        //Blue brightness goes down - PF2
//        for(i=1; i < Period-1; i++){
//            TimerMatchSet(TIMER1_BASE, TIMER_A, i);
//            TimerMatchSet(TIMER1_BASE, TIMER_B, i);
//            SysCtlDelay(time);
//        }
        //Red brightness goes up - PF1
        //        for(i=Period-2; i > 0;i--){
        //            TimerMatchSet(TIMER0_BASE, TIMER_B, i);
        //            SysCtlDelay(time);
        //        }
        //        //Green brightness goes down - PF3
        //        for(i=1; i < Period-1; i++){
        //            TimerMatchSet(TIMER1_BASE, TIMER_B, i);
        //            SysCtlDelay(time);
        //        }

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
    dutyCycle = Period-3;

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
    //

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

    //Start by rising Red LED
    for(i=Period-2; i >  0;i--){
        TimerMatchSet(TIMER0_BASE, TIMER_B, i);
        SysCtlDelay(time);
    }

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
