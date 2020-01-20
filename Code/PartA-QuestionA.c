#include "contiki.h"
#include "/home/user/contiki/core/dev/leds.h"
#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();
  printf("Kyriakou Andronikos-5806\nNtenezos Panagiotis-5853\n ");
  leds_on(LEDS_GREEN);
  leds_on(LEDS_RED);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
