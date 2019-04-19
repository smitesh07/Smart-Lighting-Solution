/**
 * @file apds9301.c
 * @author Smitesh Modak and Ashish Tak
 * @brief : Implementation of the library/HAL of the APDS-9301 Luminosity sensor
 * @version 0.1
 * @date 2019-03-30
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "apds9301.h"
#include "i2c.h"
#include "log.h"

#define I2CPORT_USED I2C0

//Command values to read/write byte and word respectively from the sensor
#define CMD_RW_BYTE 0x80
#define CMD_RW_WORD 0xA0

#define BYTE_SIZE   1
#define WORD_SIZE   2

//Low and High thresholds for interrupt generation (corresponds to channel 0)
#define LOW_THRESHOLD   0x20
#define HIGH_THRESHOLD  0Xb07

#define ID_REGISTER_VALUE 0x50

#define LUM_SLAVE_ADDRESS 0x39

//Register addresses for the APDS-9301 sensor
enum regAddress {
    CONTROL = 0x0,
    TIMING,
    THRESHLOWLOW,
    THRESHLOWHIGH,
    THRESHHIGHLOW,
    THRESHHIGHHIGH,
    INTERRUPT,
    ID = 0xA,
    DATA0LOW = 0xC,
    DATA0HIGH,
    DATA1LOW,
    DATA1HIGH
};

//char readBuffer[WORD_SIZE] = {0};
uint8_t commandReg;


int initLumSensor() {
	uint8_t writeBuffer[2];
	uint8_t readBuffer[2];
	int ret;
    LOG_INFO("Starting Built-in Startup tests for the Luminosity Sensor");

    //Write 00 to the control register to power down the device
    //Power it up again using 0x03 and read the value back
    writeBuffer[0]= CMD_RW_BYTE | CONTROL;
    writeBuffer[1]= 0x00;
    ret = i2c_write_word(I2CPORT_USED, LUM_SLAVE_ADDRESS, writeBuffer[0], writeBuffer[1]);
    if (ret != 0) {
    	LOG_ERROR("Powering down the device failed with error: %d", ret);
    	return -1;
    }

    writeBuffer[1]= 0x03;
    ret = i2c_write_word_read_byte(I2CPORT_USED, LUM_SLAVE_ADDRESS, writeBuffer[0], writeBuffer[1], readBuffer);
    if (ret != 1) {
    	LOG_ERROR("Powering it again using 0x03 failed with error: %d", ret);
    	return -1;
    } else {
    	LOG_INFO("APDS9301 Control register: 0x%x", readBuffer[0]);
    }


    //Read the identification register
    writeBuffer[0]= CMD_RW_BYTE | ID;
    ret = i2c_write_byte_read_byte(I2CPORT_USED, LUM_SLAVE_ADDRESS, readBuffer, writeBuffer[0]);
    if (ret != 1) {
    	LOG_ERROR("Reading Identification register failed with error: %d", ret);
    	return -1;
    } else {
    	LOG_INFO("Identification register: 0x%x", readBuffer[0]);
    }

    //Read the timing register
    writeBuffer[0]= CMD_RW_BYTE | TIMING;
    ret = i2c_write_byte_read_byte(I2CPORT_USED, LUM_SLAVE_ADDRESS, readBuffer, writeBuffer[0]);
    if (ret != 1) {
    	LOG_ERROR("Reading Timing register failed with error: %d", ret);
    	return -1;
    } else {
    	LOG_INFO("Timing Register at startup: 0x%x", readBuffer[0]);
    }

    //Set high gain and scale the integration time by 0.252 (101ms)
    writeBuffer[1]=0x11;
    ret = i2c_write_word(I2CPORT_USED, LUM_SLAVE_ADDRESS, writeBuffer[0], writeBuffer[1]);
    if (ret != 0) {
    	LOG_ERROR("Setting high gain and scaling the integration time failed with error: %d", ret);
    	return -1;
    }

    ret = i2c_write_byte_read_byte(I2CPORT_USED, LUM_SLAVE_ADDRESS, readBuffer, writeBuffer[0]);
    if (ret != 1) {
    	LOG_ERROR("Reading high gain and scaling the integration time failed with error: %d", ret);
    	return -1;
    } else {
    	LOG_INFO("Timing Register after setting gain and integration scale: 0x%x", readBuffer[0]);
    }

//    //Set the Interrupt Threshold Registers
//    writeBuffer[0]= CMD_RW_WORD | THRESHLOWLOW;
//    writeBuffer[1]= LOW_THRESHOLD & 0xFF;
//    writeBuffer[2]= (LOW_THRESHOLD & 0xFF00) >> 8;
//    i2cWrite(writeBuffer, WORD_SIZE+1);
//    i2cWrite(writeBuffer, BYTE_SIZE);
//    i2cRead(readBuffer, WORD_SIZE);
//    uint16_t threshold= readBuffer[0]+ (((uint16_t)readBuffer[1])<<8);
//    if (threshold!=LOW_THRESHOLD) {
//        return -1;
//    }
//    LOG_INFO("Low Threshold: 0x%x",threshold);
//
//    writeBuffer[0]= CMD_RW_WORD | THRESHHIGHLOW;
//    writeBuffer[1]= HIGH_THRESHOLD & 0xFF;
//    writeBuffer[2]= (HIGH_THRESHOLD & 0xFF00) >> 8;
//    i2cWrite(writeBuffer, WORD_SIZE+1);
//    i2cWrite(writeBuffer, BYTE_SIZE);
//    i2cRead(readBuffer, WORD_SIZE);
//    threshold= readBuffer[0]+ (((uint16_t)readBuffer[1])<<8);
//    if (threshold!=HIGH_THRESHOLD) {
//        return -1;
//    }
//    LOG_INFO("High Threshold: 0x%x",threshold);
//
//    //Enable the level interrupt in the Interrupt Control Register
//    //And set persist for 5 cycles of integration
//    writeBuffer[0]= CMD_RW_BYTE | INTERRUPT;
//    writeBuffer[1]= 0x15;
//    i2cWrite(writeBuffer, WORD_SIZE);
//    i2cWrite(writeBuffer, BYTE_SIZE);
//    i2cRead(readBuffer, BYTE_SIZE);
//    if (readBuffer[0]!=0x15) {
//        return -1;
//    }
//    LOG_INFO("Interrupt Control Reg (Level Interrupts enabled): 0x%x",readBuffer[0]);
//
//    LOG_INFO("Luminosity Sensor Startup Tests successful! ");
//
    return 0;
}


float getLum() {
    uint16_t ch0, ch1;
    uint8_t writeBuffer[2];
    uint8_t readBuffer[2];
    int ret;

    //Clear pending interrupt
//    writeBuffer[0]= CMD_RW_BYTE | 0x40;
//    ret = i2c_write(I2CPORT_USED, LUM_SLAVE_ADDRESS, writeBuffer[0]);
//    if (ret != 0) {
//    	LOG_ERROR("Clearing pending interrupt failed with error: %d", ret);
//    	return -1;
//    }

    //Read values from the 2 channels
    writeBuffer[0]= CMD_RW_WORD | DATA0LOW;
    ret = i2c_write_byte_read_word(I2CPORT_USED, LUM_SLAVE_ADDRESS, writeBuffer[0], readBuffer);
    if (ret != 2) {
    	LOG_ERROR("Reading Channel 0 failed with error: %d", ret);
    	return -1;
    } else {
    	ch0= readBuffer[0] + (uint16_t)(readBuffer[1]<<8); //Low byte is read from the sensor first
    }

    writeBuffer[0]= CMD_RW_WORD | DATA1LOW;
    ret = i2c_write_byte_read_word(I2CPORT_USED, LUM_SLAVE_ADDRESS, writeBuffer[0], readBuffer);
    if (ret != 2) {
    	LOG_ERROR("Reading Channel 1 failed with error: %d", ret);
    	return -1;
    } else {
    	ch1= readBuffer[0] + (uint16_t)(readBuffer[1]<<8); //Low byte is read from the sensor first
    }

    float lux, ratio;
    if (ch0 && ch1) {
        ratio= ch1/ch0;
    }

    //Calculate luminostiy based on the ratio
    if (ratio<=0.5) {
        lux=(0.0304*ch0)-(0.062*ch0*pow(ratio,1.4));
    }
    else if (ratio<=0.61) {
        lux=(0.0224*ch0)-(0.031*ch1);
    }
    else if (ratio<=0.8) {
        lux=(0.0128*ch0)-(0.0153*ch1);
    }
    else if (ratio<=1.3) {
        lux=(0.00146*ch0)-(0.00112*ch1);
    }
    else lux=0;

    return lux;
}

//float readTemp(void) {
////    uint8_t buf[2] = {0};
//    int16_t data;
//	uint8_t buf[2] = {0};
////    I2CGet2Bytes(buf, TEMP_SENSOR_ADDR, tempReg);
////    i2c_write_read(I2CPORT_USED, 0x48, &buf, 0x00);
//    i2c_write_byte_read_word(I2CPORT_USED, 0x48, buf, 0x00);
////    uint8_t buffer[2] = {(buf && 0xff00), (buf && 0x00ff)};
//
//    // Bit 0 of second byte will always be 0 in 12-bit readings and 1 in 13-bit
//    if(buf[1]&0x01) // 13 bit mode
//    {
//        // printf("Entered 13 bit mode");
//        // Combine bytes to create a signed int
//        data = ((buf[0]) << 5) | (buf[1] >> 3);
//        // Temperature data can be + or -, if it should be negative,
//        // convert 13 bit to 16 bit and use the 2s compliment.
//        if(data > 0xFFF)
//        {
//            data |= 0xE000;
//        }
//    }
//    else    // 12 bit mode
//    {
//        // Combine bytes to create a signed int
//        data = ((buf[0]) << 4) | (buf[1] >> 4);
//        // Temperature data can be + or -, if it should be negative,
//        // convert 12 bit to 16 bit and use the 2s compliment.
//        if(data > 0x7FF)
//        {
//            data |= 0xF000;
//        }
//    }
//
//
//    float tempData = (float)data;
//    return tempData * 0.0625;
//}

