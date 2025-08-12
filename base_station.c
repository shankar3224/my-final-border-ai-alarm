
#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>

PROCESS(base_station_process, "Base Station Node");
AUTOSTART_PROCESSES(&base_station_process);

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  char *msg = (char *)packetbuf_dataptr();
  printf("Base Station: %s (from node %d)\n", msg, from->u8[0]);
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

PROCESS_THREAD(base_station_process, ev, data) {
  PROCESS_EXITHANDLER(unicast_close(&uc);)

  PROCESS_BEGIN();
  unicast_open(&uc, 146, &unicast_callbacks);
  printf("Base Station started...\n");
  PROCESS_END();
}
