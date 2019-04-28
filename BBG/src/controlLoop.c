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

#define SENSOR_WORKING  1  
#define SENSOR_NOT_WORKING  0

#define PROXIMITY_DETECTED  1
#define NO_PROXIMITY  0

extern CONTROL_TX_t dataOut;

bool controlHeartbeatFlag;

// default state of the light task on Tiva
LIGHT_CONTROL lightControl = LIGHT_NO_CHANGE;

// default state of the motor task on Tiva
MOTOR_CONTROL motorControl = MOTOR_NO_CHANGE;


void getCurrentAction (CONTROL_RX_t rxData) {
  // CONTROL_TX_t *controlAction;
  // controlAction = (CONTROL_TX_t *) malloc (sizeof(CONTROL_TX_t));

  // float lum;
  // uint8_t proximity, sensorStatus, blindStatus;

  if (rxData.sensorStatus == SENSOR_NOT_WORKING) {
    // TODO: Log ERROR condition on BBG and change the present state to a degraded condition
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
    } else {
      dataOut.light = LIGHT_NO_CHANGE;
      dataOut.motor = MOTOR_NO_CHANGE;
    }
  }
}

void *controlLoopHandler(void *arg) {
    while (1) {
      deQueueFromLog();
      fflush(filePtr);
      //Periodically set the heartbeat flag to be checked by main()
      controlHeartbeatFlag=true;
      //Main sets this global flag on receiving the SIGINT signal from user
      if (terminateSignal) {
        enQueueForLog(WARN, "Termination signal received to Control Loop thread.", 0);
        deQueueFromLog();
        fflush(filePtr);
        break;
      }
      sleep(1);
    }
}