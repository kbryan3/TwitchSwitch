/*
 *  main.h
 *
 *  This is the main header file for the application.  It contains the settings
 *  that can be selected for the application.  The Period(1-7secons) and the
 *  on time(0-1second) indicated how long the led is on and the duty cycle of
 *  the on/off flash.  The ENABLE_SLEEPING can be set to 1 or 0 depending if
 *  the user wants to leverage the Blue Gecko's sleep modes and then the
 *  LOWEST_ENERGY_MODE can be set to say what mode the MCU should
 *  sleep in(EM0,1,2,3)
 *
 *  @author Kyle Bryan
 *  @date September 2020
 *
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_


#define LETIMER_PERIOD_MS 3000
#define LETIMER_ON_TIME_MS 175

#define ENABLE_SLEEPING  1
#define LOWEST_ENERGY_MODE 2

#define INCLUDE_LOGGING 1


#endif /* SRC_MAIN_H_ */
