/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>


/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port 5
#define LED0_pin  4
#define LED1_port 5
#define LED1_pin  5
#define SENSOR_ENABLE_port 3
#define SENSOR_ENABLE_pin  15
#define EXTCOMIN_port 3
#define EXTCOMIN_pin 13


void gpioInit()
{
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	//GPIO_DriveStrengthSet(SENSOR_ENABLE_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(SENSOR_ENABLE_port, SENSOR_ENABLE_pin, gpioModePushPull , false);

	GPIO_PinModeSet(EXTCOMIN_port, EXTCOMIN_pin, gpioModePushPull , false);



}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}
void gpioSensorEnableSetOn()
{
	GPIO_PinOutSet(SENSOR_ENABLE_port,SENSOR_ENABLE_pin);
}
void gpioSensorEnableSetOff()
{
	GPIO_PinOutClear(SENSOR_ENABLE_port,SENSOR_ENABLE_pin);
}

void gpioSetDisplayExtcomin(bool extcomin_High)
{
	if(extcomin_High == true)
	{
		GPIO_PinOutClear(EXTCOMIN_port,EXTCOMIN_pin);
	}
	else
	{
		GPIO_PinOutSet(EXTCOMIN_port,EXTCOMIN_pin);
	}
}

