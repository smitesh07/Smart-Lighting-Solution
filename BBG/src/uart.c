/**
 * @file uart.c
 * @author Smitesh Modak and Ashish Tak
 * @brief  
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "uart.h"

//Path to the file on the device for using UART1
#define UART_DEV_FILE "/dev/ttyO1"

//Semaphore to synchronize access to the data structure received from UART
#define SEM_UART_RX_DATA "/sem_uart_rx_data"

sem_t * sem_uart_rx_data;

//Transmission and reception intervals in seconds
#define UART_TRANSMISSION_INTERVAL 5
#define UART_RECEPTION_INTERVAL 5

#define UART_CONNECTION_TIMEOUT 10 //in seconds

//Heartbeat flag for the UART thread
bool uartHeartbeatFlag;

//Heartbeat flag to indicate UART connection 
bool uartConnectedHeartBeatFlag=false;

//Flag to detect every time the remote node goes in and out of service
bool newConnection=false;

int fd;
CONTROL_RX_t dataIn;
CONTROL_TX_t dataOut;


/**
 * @brief Function to perform the UART Initialization routine
 * 
 */
int uartInit (void) {
    char buf[] = UART_DEV_FILE;
    struct termios uart1;
	  fd = open(buf, O_RDWR | O_NOCTTY );
	  if(fd < 0) {
        printf("Failed to open the port for UART\n");
        enQueueForLog(PLAIN_MSG, ERROR, "Failed to open the port for UART", NULL, NULL);
        return -1;
    }
    bzero(&uart1,sizeof(uart1));

    //Set the UART parameters: Baud 115200, 8-bit mode, Ignore parity
    uart1.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    uart1.c_iflag = IGNPAR | ICRNL;
    uart1.c_oflag = 0;
    uart1.c_lflag = 0;

    uart1.c_cc[VTIME] = 0;
    //Wait for the specified number of bytes for read() to complete
    uart1.c_cc[VMIN] = sizeof(CONTROL_RX_t);

    //clean the line and set the attributes
    tcflush(fd,TCIOFLUSH);
    tcsetattr(fd,TCSANOW,&uart1);

    return 0;
}


/**
 * @brief Timer ISR to trigger UART transmission to the Tiva board
 * 
 */
void UARTTransmissionTrigger (void) {
    //Pass the latest UART data received to the control function 
    sem_wait(sem_uart_rx_data);
    getCurrentAction (dataIn);
    enQueueForLog(CONTROL_TX, INFO, "Sending Data to BBG", NULL, &dataIn);
    sem_post(sem_uart_rx_data);
    tcflush(fd,TCOFLUSH);
    //getCurrentAction() updates the value of the global var dataOut
    //NOTE: No need to guard dataOut with mutex in the present scenario,
    //since it is used by write() only after a return from getCurrentAction()
    write(fd, &dataOut, sizeof(CONTROL_TX_t));
}


/**
 * @brief Timer ISR to trigger UART reception from the Tiva board
 * 
 */
void UARTReceptionTrigger (void) {
    sem_wait(sem_uart_rx_data);
    if (read(fd, &dataIn, sizeof(CONTROL_RX_t)) ==sizeof(CONTROL_RX_t)) {
      gpio_set_value(USR_LED1, 0);
      if (newConnection==false) {
        newConnection=true;
        printf("\nUART Connection to the remote node is now established");
        enQueueForLog(PLAIN_MSG, INFO, "UART Connection to the remote node is now established", NULL, NULL);
      }
      uartConnectedHeartBeatFlag = true;
      if (dataIn.sensorStatus) {
        printf("\n\nReceived data from the remote node.");
        printf("\nLuminosity: %f", dataIn.lux);
        printf("\nProximity: %s", dataIn.proximity ? "Detected" : "Not detected");
        printf("\nBlinds: %s", dataIn.blindsStatus ? "Closed" : "Open" );
        enQueueForLog(CONTROL_RX, INFO, "Received data from the remote node.", &dataIn, NULL);
      }
    }
    sem_post(sem_uart_rx_data);
    tcflush(fd,TCIFLUSH);
}


void *uartHandler(void *arg) {
    printf("\nUART thread spawned..");

    if (uartInit()!=0) {
      printf("\nUART thread terminating..");
      enQueueForLog(PLAIN_MSG, ERROR, "UART thread terminating..", NULL, NULL);
      return;
    }

    //Create and Initialize the semaphore to synchronize access to the data structure received from UART
    sem_uart_rx_data = sem_open(SEM_UART_RX_DATA, O_RDWR | O_CREAT, 0666, 1);
    if (sem_uart_rx_data == SEM_FAILED) {
        perror("sem_open failed\n");
        enQueueForLog(PLAIN_MSG, ERROR, "sem_open failed.", NULL, NULL);
        printf("\nUART thread terminating..");
        enQueueForLog(PLAIN_MSG, ERROR, "UART thread terminating..", NULL, NULL);
        return;
    }

    timer_t uartTxTimerid, uartRxTimerid;

    uartTxTimerid= initTimer(UART_TRANSMISSION_INTERVAL*(uint64_t)1000000000, UARTTransmissionTrigger);
    uartRxTimerid= initTimer(UART_RECEPTION_INTERVAL*(uint64_t)1000000000, UARTReceptionTrigger);

    uint8_t timoeoutCount=0;

    while (1) {
      deQueueFromLog();
      fflush(filePtr);
      //Periodically set the thread's heartbeat flag to be checked by main()
      uartHeartbeatFlag=true;

      //Check the UART connection to the Tiva board
      ++timoeoutCount;
      if (timoeoutCount==UART_CONNECTION_TIMEOUT) {
        timoeoutCount=0;
        if (uartConnectedHeartBeatFlag) {
          uartConnectedHeartBeatFlag=false;
        }
        else{
          newConnection=false;
          printf("\nUART Connection to the remote node is lost.");
          printf("\nInto DEGRADED mode II of operation.");
          printf("\nNo commands would be sent unless further sensor data is received.");
          gpio_set_value(USR_LED1, 1);
          enQueueForLog(PLAIN_MSG, ERROR, "UART Connection to the remote node is lost.", NULL, NULL);
          enQueueForLog(PLAIN_MSG, ERROR, "Into DEGRADED mode II of operation.", NULL, NULL);
          enQueueForLog(PLAIN_MSG, ERROR, "No commands would be sent unless further sensor data is received.", NULL, NULL);
        }
      }  

      //Main sets this global flag on receiving the SIGINT signal from user
      if (terminateSignal) {
        enQueueForLog(PLAIN_MSG, WARN, "Termination signal received to UART thread.", NULL, NULL);
        deQueueFromLog();
        fflush(filePtr);
        break;
      }
      sleep(1);
    }
}