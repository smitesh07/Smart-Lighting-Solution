/**
 * @file controlLoop.c
 * @author Smitesh Modak and Ashish Tak
 * @brief
 * @version 0.1
 * @date 2019-04-27
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "controlLoop.h"
#include "queue.h"

//Global Control structure variable to be updated by the function getCurrentAction()
extern CONTROL_TX_t dataOut;

// bool controlHeartbeatFlag;


void getCurrentAction (CONTROL_RX_t rxData) {
  if (rxData.sensorStatus == SENSOR_NOT_WORKING) {
    printf("\nSensor on the Tiva board is disconnected!");
    enQueueForLog(PLAIN_MSG, ERROR, "Sensor on the Tiva board is disconnected!", NULL, NULL);
    printf("\nInto DEGRADED mode I of operation.");
    enQueueForLog(PLAIN_MSG, ERROR, "Into DEGRADED mode I of operation.", NULL, NULL);
    printf("\nCommand would be sent only to maintain default lighting condition.");
    enQueueForLog(PLAIN_MSG, ERROR, "Command would be sent only to maintain default lighting condition.", NULL, NULL);
    // TODO: Turn on the appropriate LED on BBG
    dataOut.light = LIGHT_MAINTAIN_DEFAULT;
    dataOut.motor = MOTOR_NO_CHANGE;
  } else if(rxData.proximity == PROXIMITY_DETECTED) {
    if(rxData.lux > LOW_LIGHT && rxData.lux < HIGH_LIGHT) {
      dataOut.light = LIGHT_NO_CHANGE;
      dataOut.motor = MOTOR_NO_CHANGE;
    } else if (rxData.lux < LOW_LIGHT && rxData.blindsStatus == MOTOR_CLOSE) {
      dataOut.light = LIGHT_NO_CHANGE;
      dataOut.motor = MOTOR_OPEN;
    } else if (rxData.lux < LOW_LIGHT && rxData.blindsStatus == MOTOR_OPEN) {
      dataOut.light = LIGHT_INCREASE;
      dataOut.motor = MOTOR_NO_CHANGE;
    } else if (rxData.lux > HIGH_LIGHT && rxData.blindsStatus == MOTOR_OPEN) {
      dataOut.light = LIGHT_NO_CHANGE;
      dataOut.motor = MOTOR_CLOSE;
    } else if (rxData.lux > HIGH_LIGHT && rxData.blindsStatus == MOTOR_CLOSE) {
      dataOut.light = LIGHT_DECREASE;
      dataOut.motor = MOTOR_NO_CHANGE;
    }
  } else {  //No proximity detected
      dataOut.light = LIGHT_NO_CHANGE;
      dataOut.motor = MOTOR_NO_CHANGE;
  }
}

// void *controlLoopHandler(void *arg) {
//     while (1) {
//       deQueueFromLog();
//       fflush(filePtr);
//       //Periodically set the heartbeat flag to be checked by main()
//       controlHeartbeatFlag=true;
//       //Main sets this global flag on receiving the SIGINT signal from user
//       if (terminateSignal) {
//         enQueueForLog(PLAIN_MSG, WARN, "Termination signal received to Control Loop thread.", NULL, NULL);
//         deQueueFromLog();
//         fflush(filePtr);
//         break;
//       }
//       sleep(1);
//     }
// }
