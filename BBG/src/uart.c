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

#define UART_CONNECTION_TIMEOUT 20 //in seconds

//Heartbeat flag for the UART thread
bool uartHeartbeatFlag;

//Heartbeat flag to indicate UART connection 
bool uartConnectedHeartBeatFlag;

int fd;
CONTROL_RX_t dataIn;
CONTROL_TX_t dataOut;


/**
 * @brief Function to perform the UART Initialization routine
 * 
 */
void uartInit (void) {
    char buf[] = UART_DEV_FILE;
    struct termios uart1;
	  fd = open(buf, O_RDWR | O_NOCTTY );
	  if(fd < 0) {
        printf("port failed to open\n");
        enQueueForLog(PLAIN_MSG, ERROR, "Port failed to open", NULL, NULL);
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
}


/**
 * @brief Timer ISR to trigger UART transmission to the Tiva board
 * 
 */
void UARTTransmissionTrigger (void) {
    //Pass the latest UART data received to the control function 
    sem_wait(sem_uart_rx_data);
    getCurrentAction (dataIn);
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
    read(fd, &dataIn, sizeof(CONTROL_RX_t));
    uartConnectedHeartBeatFlag = true;
    sem_post(sem_uart_rx_data);
    printf("\nRecd: %f\t%d\t%d\t%d",dataIn.lux, dataIn.proximity, dataIn.sensorStatus, dataIn.blindsStatus);
    tcflush(fd,TCIFLUSH);
}


void *uartHandler(void *arg) {
    uartInit();

    //Create and Initialize the semaphore to synchronize access to the data structure received from UART
    sem_uart_rx_data = sem_open(SEM_UART_RX_DATA, O_RDWR | O_CREAT, 0666, 1);
    if (sem_uart_rx_data == SEM_FAILED || sem_uart_rx_data == SEM_FAILED) {
        perror("sem_open failed\n");
        enQueueForLog(PLAIN_MSG, ERROR, "sem_open failed open", NULL, NULL);
      }

    timer_t uartTxTimerid, uartRxTimerid;

    uartTxTimerid= initTimer(UART_TRANSMISSION_INTERVAL*(uint64_t)1000000000, UARTTransmissionTrigger);
    uartRxTimerid= initTimer(UART_RECEPTION_INTERVAL*(uint64_t)1000000000, UARTReceptionTrigger);

    uint8_t timoeoutCount=0;

    while (1) {
      deQueueFromLog();
      fflush(filePtr);
      //Periodically set the heartbeat flag to be checked by main()
      uartHeartbeatFlag=true;

      //Check the UART connection to the Tiva board
      ++timoeoutCount;
      if (timoeoutCount==UART_CONNECTION_TIMEOUT) {
        timoeoutCount=0;
        if (uartConnectedHeartBeatFlag) {
          uartConnectedHeartBeatFlag=false;
        }
        else{
          printf("\nUART Connection to the Tiva board is lost.");
          printf("\nInto DEGRADED mode II of operation.");
          printf("\nNo commands would be sent unless further sensor data is received.");
          //TODO: Turn on the appropriate LED On the BBG
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