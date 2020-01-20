#include "contiki.h"
#include "/home/user/contiki/core/dev/leds.h"
#include "/home/user/contiki/core/sys/etimer.h"
#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  while(1) {
    leds_on(LEDS_RED);
    printf("Red led ON! \n");
    etimer_set(&et, CLOCK_SECOND * 2);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_stop(&et);
    leds_off(LEDS_RED); 
    printf("Red led OFF! \n");
    leds_on(LEDS_BLUE);
    printf("Blue led ON! \n");
    etimer_set(&et, CLOCK_SECOND * 4);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_stop(&et);
    leds_on(LEDS_RED); 
    printf("Blue and Red leds ON \n");
    etimer_set(&et, CLOCK_SECOND * 1);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_stop(&et);
    leds_off(LEDS_RED);
    leds_off(LEDS_BLUE);    
    printf("All Leds OFF! \n");
}
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
