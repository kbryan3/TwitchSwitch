/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();

//set Sensor Enable on and off(used for temp sensor)
//maybe try making these static inline functions per Roberto Baquerizo
void gpioSensorEnableSetOn();
void gpioSensorEnableSetOff();

//*****************************************************************************
/// Changes the state of the extcomin signal.  Signal must be setup already
///  before calling this function
///
///  @param bool extcomin_High : whether the signal is high(true) or low(false)
///  @return void
///
//*****************************************************************************
void gpioSetDisplayExtcomin(bool extcomin_High);
#endif /* SRC_GPIO_H_ */
