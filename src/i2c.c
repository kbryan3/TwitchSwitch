/*
 * i2c.c
 *
 *  This file contains the function to access the
 *      Si7021 temp/humidity sensor via i2c
 *
 *  @author Kyle Bryan
 *  @date September 2020
 *  @resources emlib functions were leveraged to set register values
 */
#include "i2c.h"


void initI2C()
{
	I2CSPM_Init_TypeDef *i2c;
	i2c = getI2CStructure();
	I2CSPM_Init_TypeDef i2C_init = I2CSPM_INIT_DEFAULT;
	*i2c = i2C_init;

	//LOG_INFO("I2C0 Clock Frequency: %u",CMU_ClockFreqGet(cmuClock_I2C0));

	//enable I2C Pins
	I2CSPM_Init(i2c);
	//LOG_INFO("I2C0 Bus Frequency: %u",I2C_BusFreqGet(I2C0));

}

void getTemperature(struct gecko_cmd_packet* evt)
{

	enum temp_sensor_state state;
	static enum temp_sensor_state next_state = TEMP_SENSOR_POWER_ON;
	uint8_t* write_buffer;

	state = next_state;
	/*if(BGLIB_MSG_ID(evt->header) != gecko_evt_system_boot_id ||
	   BGLIB_MSG_ID(evt->header) != gecko_evt_gatt_server_characteristic_status_id ||
	   BGLIB_MSG_ID(evt->header) != gecko_evt_le_connection_closed_id ||
	   BGLIB_MSG_ID(evt->header) != gecko_cmd_le_connection_set_parameters_id ||
	   BGLIB_MSG_ID(evt->header) != gecko_cmd_le_connection_disable_slave_latency_id ||
	   BGLIB_MSG_ID(evt->header) != gecko_cmd_le_connection_close_id)*/
	if(BGLIB_MSG_ID(evt->header) == gecko_evt_system_external_signal_id && connection_Open_Flag == 1)
	{
		switch(state)
		{
			case TEMP_SENSOR_POWER_ON: //enable sensor
				//LOG_INFO("External Signal: %d", evt->data.evt_system_external_signal.extsignals);
				if(evt->data.evt_system_external_signal.extsignals & LETIMER0_UF_EVENT)
				{
					//sensorEnable();
					timerWaitUs(80000);
					next_state = TEMP_SENSOR_WAIT_FOR_POWER_UP;
				}
				break;
			case TEMP_SENSOR_WAIT_FOR_POWER_UP:
				if(evt->data.evt_system_external_signal.extsignals
						& LETIMER0_WAIT_EXPIRED_EVENT)
				{
					Si7013_InitiateWrite();
					SLEEP_SleepBlockBegin(sleepEM2);
					next_state = TEMP_SENSOR_WAIT_FOR_WRITE_COMPLETE;
				}
				break;
			case TEMP_SENSOR_WAIT_FOR_WRITE_COMPLETE:
				if(evt->data.evt_system_external_signal.extsignals
						& I2C_TRANSFER_DONE_EVENT)
				{
					SLEEP_SleepBlockEnd(sleepEM2);
					NVIC_DisableIRQ(I2C0_IRQn);
					timerWaitUs(10000);
					next_state = TEMP_SENSOR_WAIT_FOR_TEMP_MEASUREMENT;

				}
				break;
			case TEMP_SENSOR_WAIT_FOR_TEMP_MEASUREMENT:
				if(evt->data.evt_system_external_signal.extsignals
						& LETIMER0_WAIT_EXPIRED_EVENT)
				{
					Si7013_InitiateRead(); //Start Read
					SLEEP_SleepBlockBegin(sleepEM2);
					next_state = TEMP_SENSOR_WAIT_FOR_READ_COMPLETE;
				}
				break;
			case TEMP_SENSOR_WAIT_FOR_READ_COMPLETE:
				if(evt->data.evt_system_external_signal.extsignals
						& I2C_TRANSFER_DONE_EVENT)
				{
					SLEEP_SleepBlockEnd(sleepEM2);
					NVIC_DisableIRQ(I2C0_IRQn);
					//sensorDisable(); //PowerOFF Sensor
					write_buffer = getPtrToI2CWriteBuffer();
					uint16_t raw_temp = (write_buffer[0] << 8) | write_buffer[1];
					float temp = (175.72 *(float)raw_temp)/65536.0 - 46.85;
					update_temperature(temp);
					LOG_INFO("Temperature is: %lu degrees C\n", (uint32_t)temp);
					next_state = TEMP_SENSOR_POWER_ON;
				}
				break;
		}
	}
	else if(connection_Open_Flag == 0)
	{
		//sensorDisable();
		next_state = TEMP_SENSOR_POWER_ON;
	}

}

I2C_TransferSeq_TypeDef *getPtrToSI7013Sequence()
{
	return (&si7013_seq);
}

uint8_t *getPtrToI2CWriteBuffer()
{
	return (&i2c_Write_Buffer[0]);
}

I2CSPM_Init_TypeDef *getI2CStructure()
{
	return(&i2c);
}

I2C_TransferReturn_TypeDef Si7013_InitiateWrite()
{


	I2C_TransferSeq_TypeDef *ptrToSeq;
	uint8_t *write_buffer;
	I2C_TransferReturn_TypeDef transferStatus;
	write_buffer = getPtrToI2CWriteBuffer();
	write_buffer[0] = 0xF3;
	ptrToSeq = getPtrToSI7013Sequence();
	ptrToSeq->addr = 0x80;
	ptrToSeq->flags = I2C_FLAG_WRITE;
	ptrToSeq->buf[0].data = write_buffer;
	ptrToSeq->buf[0].len = 1;
	NVIC_EnableIRQ(I2C0_IRQn);
	transferStatus = I2C_TransferInit(I2C0, ptrToSeq);
	return transferStatus;

}

I2C_TransferReturn_TypeDef Si7013_InitiateRead()
{


	I2C_TransferSeq_TypeDef *ptrToSeq;
	uint8_t *write_buffer;
	I2C_TransferReturn_TypeDef transferStatus;
	write_buffer = getPtrToI2CWriteBuffer();
	ptrToSeq = getPtrToSI7013Sequence();
	ptrToSeq->addr = 0x80;
	ptrToSeq->flags = I2C_FLAG_READ;
	ptrToSeq->buf[0].data = write_buffer;
	ptrToSeq->buf[0].len = 2;
	NVIC_EnableIRQ(I2C0_IRQn);
	transferStatus = I2C_TransferInit(I2C0, ptrToSeq);
	return transferStatus;

}

void Si7013_MeasureTemp()
{

	I2C_TransferSeq_TypeDef measureCMD;
	int32_t result;
	uint8_t cmd[1];
	//uint8_t read_data[2];


	cmd[0] = 0xF3; //take temp measurement
	measureCMD.addr = 0x80;
	measureCMD.flags = I2C_FLAG_WRITE;
	measureCMD.buf[0].data = cmd;
	measureCMD.buf[0].len = 1;
	//measureCMD.buf[1].data = read_data;
	//measureCMD.buf[1].len = 1;

	result = I2CSPM_Transfer(I2C0, &measureCMD);
	if(result != i2cTransferDone)
	{
		LOG_INFO("Transfer Unsuccessful");
	}
	else
	{
		//LOG_INFO("Transfer Successful");
	}
}

void Si7013_getTemp()
{

	uint16_t raw_temp;
	float temp;
	I2C_TransferSeq_TypeDef measureCMD;
	int32_t result;
	//uint8_t reg_read[1]; //take temp measurement
	uint8_t read_data[2];


	//reg_read[0] = 0xE0;
	measureCMD.addr = 0x80;
	measureCMD.flags = I2C_FLAG_READ;
	measureCMD.buf[0].data = read_data;
	measureCMD.buf[0].len = 2;
	//measureCMD.buf[1].data = read_data;
	//measureCMD.buf[1].len = 2;

	result = I2CSPM_Transfer(I2C0, &measureCMD);
	if(result != i2cTransferDone)
	{
		LOG_INFO("Transfer Unsuccessful");
	}
	else
	{
		//LOG_INFO("Transfer Successful");
	}
	raw_temp = (read_data[0] << 8) | read_data[1];
	temp = (175.72 *(float)raw_temp)/65536.0 - 46.85;
	LOG_INFO("Temperature is: %lu degrees C\n", (uint32_t)temp);
}



void sensorEnable()
{
	gpioSensorEnableSetOn();
}

void sensorDisable()
{
	gpioSensorEnableSetOff();
}
