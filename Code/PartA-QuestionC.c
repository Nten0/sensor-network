#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"


/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test button");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(test_button_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();
  active = 0;
  SENSORS_ACTIVATE(button_sensor);
  
  while(1)
  {
  	etimer_set(&et, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL((ev == sensors_event &&
			     data == &button_sensor)||(etimer_expired(&et)));
    leds_toggle(LEDS_ALL);
    etimer_stop(&et);
    SENSORS_ACTIVATE(light_sensor);
    printf("Light: %d\n", light_sensor.value(0));
   	SENSORS_DEACTIVATE(light_sensor);
  }
  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
