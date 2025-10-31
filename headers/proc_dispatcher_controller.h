#ifndef PROC_DISPATCHER_CONTROLLER_H
#define PROC_DISPATCHER_CONTROLLER_H

#include "proc.h"

#include <stdint.h>


struct proc_dispatch_queue {
  struct proc items[200];
  uint32_t arrival_time[200];
  uint8_t start, end;
};

void dispatch_procs_upto_time(
  struct proc_dispatch_queue* queue,
  int (*)(struct proc),
  uint32_t t
);

void init_proc_dispatch_queue_from_fname(
  struct proc_dispatch_queue* queue,
  char *fname
);

#endif // PROC_DISPATCHER_CONTROLLER_H
