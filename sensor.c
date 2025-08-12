
#include "contiki.h"
#include "net/rime/rime.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include <stdio.h>
#include <string.h>

// Each sensor node will have a location name
static const char *location_name;

PROCESS(sensor_process, "Border Sensor Node");
AUTOSTART_PROCESSES(&sensor_process);

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  // Sensor doesn't expect to receive anything in this example
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

PROCESS_THREAD(sensor_process, ev, data) {
  static struct etimer et;
  PROCESS_EXITHANDLER(unicast_close(&uc);)

  PROCESS_BEGIN();

  unicast_open(&uc, 146, &unicast_callbacks);

  // Assign location name based on node_id
  if (linkaddr_node_addr.u8[0] == 2) location_name = "East Forest Area";
  else if (linkaddr_node_addr.u8[0] == 3) location_name = "West Forest Area";
  else if (linkaddr_node_addr.u8[0] == 4) location_name = "Hill Top Area";
  else if (linkaddr_node_addr.u8[0] == 5) location_name = "Left Base Station Area";
  else location_name = "Unknown Location";

  while (1) {
    // Random delay between 5 and 15 seconds
    etimer_set(&et, CLOCK_SECOND * (5 + (random_rand() % 10)));
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Prepare alert message
    char buf[50];
    sprintf(buf, "ALERT: Intrusion at %s", location_name);

    // Send to Base Station (node 1)
    linkaddr_t recv;
    recv.u8[0] = 1;
    recv.u8[1] = 0;
    unicast_send(&uc, &recv);
    printf("Sensor(%s) -> ALERT sent to Base Station\n", location_name);
  }

  PROCESS_END();
}
