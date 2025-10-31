#include <stdio.h>

#include "proc_manager.h"
#include "proc.h"
#include "proc_dispatcher_controller.h"

#include <assert.h>

void run_proc_tick(struct proc *const p) {
  printf("[%d] PC at %d\n", p->pid, p->pc);

  p->pc++;
}

uint32_t global_time;

int main(int argc, char **argv) {
  global_time = 0;
  bootstrap_proc_manager();

  struct proc_dispatch_queue dispatch_queue;

  init_proc_dispatch_queue_from_fname(&dispatch_queue, "procs.txt");
  printf("dispatch_queue.start = %d\n", dispatch_queue.start);
  printf("dispatch_queue.end = %d\n", dispatch_queue.end);

  struct proc running_proc;

  int max_loop = 2;

  while (max_loop--) {
    printf("dispatch_queue.start = %d\n", dispatch_queue.start);
    printf("dispatch_queue.end = %d\n", dispatch_queue.end);

    printf("-1\n");
    dispatch_procs_upto_time(
      &dispatch_queue,
      dispatch_proc,
      global_time
    );

    printf("0\n");

    int err = run_next_proc();
    if (err == 10) {
      printf("HERE\n");
      global_time++;
      if (dispatch_queue.end == dispatch_queue.start) {
        // no more processes to consider
        break;
      }
      continue;
    } else {
      // FIXME: actual error handling instead of assertion
      printf("%d\n", err);
      assert(err == 0);
    }
    printf("1\n");

    assert(
      get_running_proc(&running_proc) == 0
    );
    printf("2\n");

    int count;
    if (running_proc.preemptable) {
      count = running_proc.quantum;
    } else {
      count = running_proc.time_left;
    }

    while (count--) {
      run_proc_tick(&running_proc);
      global_time++;
      // dispatch_procs_upto_time(global_time);
    }
    printf("3\n");
  }

  return 0;
}
