#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"

#include "simple-udp.h"

#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"

#include <stdio.h>
#include <string.h>

#include "sys/ctimer.h"
#define UDP_PORT 1234

static struct ctimer timer;

static struct simple_udp_connection broadcast_connection;

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_example_process, "UDP broadcast example process");

AUTOSTART_PROCESSES(&broadcast_example_process);
/*---------------------------------------------------------------------------*/

static void turn_off(void *ptr)
{
	 		leds_off(LEDS_GREEN);
}
int i = 0;
int flag = 0; // 0 = OK , 1 = ERROR

static struct etimer et2;
static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
uint16_t receiver_port, const uint8_t *data, uint16_t datalen)
{
	printf("Light sensor value received on port %d from port %d with length %d %s\n",receiver_port, sender_port, datalen,data);
	if ( strcmp((char*)data,"ALARM") == 0 )
	{  
		leds_off(LEDS_ALL);
		printf("Received ALARM \n");
	   	for (i=0;i<10;i++)
	   	{
	   		leds_on(LEDS_BLUE);
	   		printf("i = %d: Blue Led ON\n",i);
            clock_wait (0.5*CLOCK_SECOND);
	   		printf("i = %d: Blue Led OFF\n",i);
	        leds_off(LEDS_BLUE);
	   		leds_on(LEDS_RED);
	   		printf("i = %d: Red Led ON\n",i);
	   		clock_wait (0.5*CLOCK_SECOND);
	        leds_off(LEDS_RED);
	   		printf("i = %d: Red Led OFF\n",i);
	    }
	}
	if (strcmp ((char*)data,"Alert Low Value") == 0)
	{
		printf("Reeived Low Value Alert! Green Led ON \n!");
		leds_on(LEDS_GREEN);
 		ctimer_set(&timer,5*CLOCK_SECOND,turn_off,NULL);
	}
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_example_process, ev, data)
{
  static struct etimer et,et2;
  uip_ipaddr_t addr;

  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);
  simple_udp_register(&broadcast_connection, UDP_PORT,
                      NULL, UDP_PORT,
                      receiver);
 
  etimer_set(&et, 10*CLOCK_SECOND);
  int value;
  while(1) {

  PROCESS_WAIT_EVENT_UNTIL((ev == sensors_event &&
			     data == &button_sensor)||(etimer_expired(&et)));

	if(data == &button_sensor)
	{
		printf("Sending broadcast ALARM\n");
		uip_create_linklocal_allnodes_mcast(&addr);
		simple_udp_sendto(&broadcast_connection, "ALARM", sizeof("ALARM"), &addr);
		etimer_set(&et, 10*CLOCK_SECOND);
		flag = 0;
	}
	else if(flag == 0)
	{
		SENSORS_ACTIVATE(light_sensor);
		printf("Normal Execution - Light: %d\n",value=light_sensor.value(0));
    	SENSORS_DEACTIVATE(light_sensor);
  		if (value < 80)
		{
			printf("Found Value < 80 \n");
 			flag = 1;
 			etimer_set(&et,3*CLOCK_SECOND);
		}
        else
        {
  			etimer_set(&et, 10*CLOCK_SECOND);
        }
	}	
	else if (flag == 1)
	{
	    	SENSORS_ACTIVATE(light_sensor);
	    	printf("Low Light Mode - Light: %d\n",value=light_sensor.value(0));
	    	SENSORS_DEACTIVATE(light_sensor);
	  		if (value < 80)
			{
	 			printf ("Value < 80 - Alert Low Value - Broadcasting \n");
	  			uip_create_linklocal_allnodes_mcast(&addr);
	  			simple_udp_sendto(&broadcast_connection, "Alert Low Value", sizeof("Alert Low Value"), &addr);
			}
			flag = 0;
	 		etimer_set(&et, 10*CLOCK_SECOND);
   	}
   }
   PROCESS_END();
}
/*---------------------------------------------------------------------------*/
