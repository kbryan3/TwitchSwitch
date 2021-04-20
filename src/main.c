/*
 *  main.c
 *
 *  This is the main file for the application.  The application blinks the LED
 *  at a chosen interval which can be entered in main.h.  The application also
 *  allows the user to set which mode to sleep in, also set in main.h
 *
 *  @author Kyle Bryan
 *  @date October 2020
 *
 */

#include <sleep.h>
#include "gecko_configuration.h"
#include "gpio.h"
#include "native_gecko.h"
#include "main.h"
#include "timer.h"
#include "oscillators.h"
#include "em_device.h"
#include "scheduler.h"
#include "log.h"
#include "i2c.h"
#include "ble.h"
#include "display.h"


//variables
uint8_t led_state;
uint8_t connection_Open_Flag;
uint8_t connection;


int appMain(gecko_configuration_t *config)
{
	//initialize globals
	connection = 0;
	connection_Open_Flag = 0;
	// Initialize stack
	gecko_init(config);
	gpioInit();
	initI2C();
	sensorEnable();
	displayInit();
	led_state = 0;
#if (LOWEST_ENERGY_MODE == 3)
	init_ULFRCO();
#else
	init_LFXO();
#endif
	  initLETIMER0();
	  /*Initialize logging */
	  logInit();
	  NVIC_ClearPendingIRQ(LETIMER0_IRQn);
	  NVIC_EnableIRQ(LETIMER0_IRQn);
	#if ENABLE_SLEEPING == 1
	  SLEEP_Init_t sleepConfig = {0};
	  SLEEP_InitEx(&sleepConfig);
	#if LOWEST_ENERGY_MODE == 0
	  SLEEP_SleepBlockBegin(sleepEM1);
	#elif LOWEST_ENERGY_MODE == 1
	  SLEEP_SleepBlockBegin(sleepEM2);
	#elif LOWEST_ENERGY_MODE == 2
	  SLEEP_SleepBlockBegin(sleepEM3);
	#else
	  //SLEEP_SleepBlockBegin(sleepEM4);
	#endif

	#endif

	  struct gecko_cmd_packet* evt;
	  /* Infinite loop */
	  while (1)
	  {
		  if(!gecko_event_pending())
		  {
			  logFlush();
		  }

		  evt = gecko_wait_event();
		  handle_ble_event(evt);
		  getTemperature(evt);
	  }
}
