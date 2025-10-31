#ifndef PROC_MANAGER_H
#define PROC_MANAGER_H

#include "proc.h"

#include <stdint.h>

#define MAX_PROCS 100
#define UNDEFINED_PROC_IDX 255

struct proc_queue_item {
  struct proc proc;
  // since max procs is less than 256, uint8_t is enough
  // to index all procs
  uint8_t next_i;
  uint8_t previous_i;
  uint8_t is_allocated;
};

struct proc_queue {
  struct proc_queue_item *items;
  uint8_t head_i;
  uint8_t tail_i;
};

int bootstrap_proc_manager();

int save_running_proc(const struct proc* const p);

int get_running_proc(struct proc *const p);

int dispatch_proc(struct proc);

int run_next_proc();

#endif // PROC_MANAGER_H
