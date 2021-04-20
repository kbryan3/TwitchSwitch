/*
 * i2c.h
 *
 *  This file contains the function prototypes to access the
 *      Si7021 temp/humidity sensor via i2c
 *
 *  @author Kyle Bryan
 *  @date September 2020
 *
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include <sleep.h>
#include "em_i2c.h"
#include "i2cspm.h"
#include "oscillators.h"
#include "main.h"
#include "log.h"
#include "gpio.h"
#include "timer.h"
#include "scheduler.h"
#include "ble.h"

I2C_TransferSeq_TypeDef si7013_seq;

I2CSPM_Init_TypeDef i2c;

uint8_t i2c_Write_Buffer[2];

uint8_t connection_Open_Flag;

enum temp_sensor_state
{
	TEMP_SENSOR_POWER_ON,
	TEMP_SENSOR_WAIT_FOR_POWER_UP,
	//TEMP_SENSOR_START_WRITE_TRANSFER,
	TEMP_SENSOR_WAIT_FOR_WRITE_COMPLETE,
	TEMP_SENSOR_WAIT_FOR_TEMP_MEASUREMENT,
	//TEMP_SENSOR_START_READ_TRANSFER,
	TEMP_SENSOR_WAIT_FOR_READ_COMPLETE
	//TEMP_SENSOR_POWER_OFF,
	//TEMP_SENSOR_NUM_STATES
};

//*****************************************************************************
/// Initializes the I2C controller
///
///  @param void
///  @return void
///
//*****************************************************************************
void initI2C();

//*****************************************************************************
/// Goes through the process of commanding routines to return a temperature
///
///  @param gecko_cmd_packet containing info on most recent event
///  @return void
///
//*****************************************************************************
void getTemperature(struct gecko_cmd_packet* evt);

//*****************************************************************************
/// Initiates a write transaction to begin a temperature measurement on Si7013
///
///  @param void
///  @return the status of the transfer
///
//*****************************************************************************
I2C_TransferReturn_TypeDef Si7013_InitiateWrite();

//*****************************************************************************
/// Initiates a read transaction to read latest temp measurement of Si7013
///
///  @param void
///  @return the status of the transaction
///
//*****************************************************************************
I2C_TransferReturn_TypeDef Si7013_InitiateRead();

//*****************************************************************************
/// Prints out temperature Value(can only be called after a MeasureTemp
///
///  @param void
///  @return void
///
//*****************************************************************************
void Si7013_getTemp();

//*****************************************************************************
/// Tells the si7013 to do a temperature measurement
///
///  @param void
///  @return void
///
//*****************************************************************************
void Si7013_MeasureTemp();

//*****************************************************************************
/// returns a pointer to the seq data structure
///
///  @param void
///  @return pointer to the I2C sequence(defined in em_i2c.h libary)
///
//*****************************************************************************
I2C_TransferSeq_TypeDef *getPtrToSI7013Sequence();

//*****************************************************************************
/// returns a pointer to the i2c_Write_Buffer
///
///  @param void
///  @return pointer to the i2c_Write_Buffer
///
//*****************************************************************************
uint8_t *getPtrToI2CWriteBuffer();

//*****************************************************************************
/// returns a pointer to the i2c I2CSPM_Init_TypeDef Structure
///
///  @param void
///  @return pointer to the i2c structure
///
//*****************************************************************************
I2CSPM_Init_TypeDef *getI2CStructure();

//*****************************************************************************
/// Enables or Disables the I2C0's SDA, SCL and VCC connections
///  Use to minimize power when not utilizing the I2C peripherals
///  @param void
///  @return void
///
//*****************************************************************************
void sensorEnable();
void sensorDisable();
#endif /* SRC_I2C_H_ */
