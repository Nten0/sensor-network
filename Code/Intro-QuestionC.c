#include "contiki.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();

  printf("Kyriakou Andronikos-5806\nNtenezos Panagiotis-5853\n ");
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/