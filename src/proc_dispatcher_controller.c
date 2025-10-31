#include "proc_dispatcher_controller.h"

#include <stdint.h>
#include <stdio.h>
#include <assert.h>


void dispatch_procs_upto_time(
  struct proc_dispatch_queue* queue,
  int (*dispatcher)(struct proc),
  uint32_t t
) {
  while (
    queue->start < queue->end &&
    queue->arrival_time[queue->start] <= t
  ) {
    printf("DISPATCHING\n");
    dispatcher(queue->items[queue->start]);
    printf("DISPATCHED\n");
    queue->start++;
  }
}

void init_proc_dispatch_queue_from_fname(
  struct proc_dispatch_queue* queue,
  char *fname
) {
  FILE *file_ptr = fopen(fname, "r");
  assert(file_ptr != NULL);

  int MAX_LINE_LENGTH = 200;
  char line_buffer[MAX_LINE_LENGTH];

  queue->start = 0;
  queue->end = 0;

  while (fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL) {
    struct proc* p = &queue->items[queue->end];
    uint32_t *t = &queue->arrival_time[queue->end];
    int todo; // TODO: change this

// <tempo de inicialização>, <prioridade>, <tempo de processador>, <blocos em memória>,
// <número-código da impressora requisitada>, <requisição do scanner>,
// <requisição do modem>, <número-código do disco>
    if (sscanf(
      line_buffer,
      "%d, %d, %d, %d, %d, %d, %d, %d",
      t, &p->priority, &p->time_left, &p->mem_size,
      &todo, &todo, &todo, &todo
    ) != 8) {
      fprintf(stderr, "Malformed line...: \n'%s'", line_buffer);
    }

    queue->end++;
  }
  fclose(file_ptr);
}
