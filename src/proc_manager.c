#include "proc.h"
#include "proc_manager.h"

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct proc_queue_item items[MAX_PROCS] = {0};

struct proc_queue blocked_queue = {0};
struct proc_queue ready_queue = {0};
// struct proc running[N_RUNNING] = {0};
uint8_t running_i;

uint8_t n_procs;

void init_queue(struct proc_queue* q) {
  q->tail_i = UNDEFINED_PROC_IDX;
  q->head_i = UNDEFINED_PROC_IDX;
  q->items = items;
}

int bootstrap_proc_manager() {
  n_procs = 0;
  running_i = UNDEFINED_PROC_IDX;
  init_queue(&blocked_queue);
  init_queue(&ready_queue);

  return 0;
}

uint8_t allocate_item() {
  uint8_t i = 0;
  // an item is free if it isn't connected to anyone
  while (
    items[i].is_allocated && i < MAX_PROCS
  ) {
    i++;
  }

  if (i == MAX_PROCS) return UNDEFINED_PROC_IDX;

  items[i].is_allocated = 1;

  return i;
}

void free_item(uint8_t i) {
  assert(i < MAX_PROCS);
  assert(items[i].is_allocated == 1);

  items[i].is_allocated = 0;
}

void append_to_queue(struct proc_queue* const q, uint8_t i) {
  printf("i = %d\n", i);
  if (q->head_i == UNDEFINED_PROC_IDX) {
    q->head_i = i;
  } else {
    // either both head and tail should be undefined
    // or neither is undefined
    assert(q->tail_i != UNDEFINED_PROC_IDX);

    q->items[q->tail_i].next_i = i;
  }
  q->tail_i = i;
}

int dispatch_proc(struct proc p) {
  uint8_t proc_i = allocate_item();

  if (proc_i == UNDEFINED_PROC_IDX) {
    return 10;
  }

// struct proc {
//   enum PROC_STATE state;
//   uint8_t pid;
//   uint8_t preemptable;
//   uint8_t quantum;
//   uint8_t time_left;
//   uint32_t pc;
//   // struct regs regs;
//   uint8_t priority;
//   uint32_t mem_offset;
//   uint32_t mem_size;
//   // bool uses_scanner;
//   // bool uses_printer;
//   // bool uses_modem;
//   // bool uses_sata;
// };
  items[proc_i].proc.time_left = p.time_left;
  items[proc_i].proc.priority = p.priority;
  items[proc_i].proc.preemptable = p.priority > 0;
  items[proc_i].proc.mem_offset = p.mem_offset;
  items[proc_i].proc.mem_size = p.mem_size;

  printf("OPA\n");
  append_to_queue(&ready_queue, proc_i);

  return 0;
}

int save_running_proc(const struct proc* const proc) {
  if (running_i == UNDEFINED_PROC_IDX) return 10;

  if (proc->pid != items[running_i].proc.pid) return 20;

  memcpy(&items[running_i].proc, proc, sizeof(proc));
  items[running_i].proc.priority++;
  if (items[running_i].proc.priority > 5) {
    items[running_i].proc.priority = 5;
  }

  if (items[running_i].proc.time_left <= 0) {
    free_item(running_i);
  } else if (items[running_i].proc.state == PROC__BLOCKED) {
    append_to_queue(&blocked_queue, running_i);
  } else if (items[running_i].proc.state == PROC__READY) {
    append_to_queue(&ready_queue, running_i);
  } else {
    return 30;
  }

  running_i = UNDEFINED_PROC_IDX;

  return 0;
}

int get_running_proc(struct proc *const p) {
  assert(p != NULL);

  if (running_i == UNDEFINED_PROC_IDX) return 10;

  memcpy(p, &items[running_i].proc, sizeof(struct proc));

  return 0;
}

uint8_t priority_to_quantum[] = {
  0, // special case, unlimited time
  6,
  5,
  4,
  3,
  2,
};

int run_next_proc() {
  printf("run_next_proc 0\n");
  if (ready_queue.head_i == UNDEFINED_PROC_IDX) return 10;
  printf("run_next_proc 1\n");

  if (running_i != UNDEFINED_PROC_IDX) {
    printf("run_next_proc 2\n");
    if (items[running_i].proc.priority == 0) {
      items[running_i].proc.quantum = items[running_i].proc.time_left;
    } else {
      assert(items[running_i].proc.priority <= 5);

      items[running_i].proc.quantum = priority_to_quantum[items[running_i].proc.priority];
    }
  }
  printf("run_next_proc 3\n");

  running_i = ready_queue.head_i;
  ready_queue.head_i = ready_queue.items[ready_queue.head_i].next_i;

  return 0;
}
