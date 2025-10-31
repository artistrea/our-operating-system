#ifndef PROC_H
#define PROC_H

#include <stdint.h>
#include <stdbool.h>

enum PROC_STATE {
  PROC__RUNNING,
  PROC__BLOCKED,
  PROC__READY,
};

// struct regs {
  
// };

struct proc {
  enum PROC_STATE state;
  int pid;
  bool preemptable;
  int quantum;
  int time_left;
  int priority;
  int pc;
  // struct regs regs;
  int mem_offset;
  int mem_size;
  // bool uses_scanner;
  // bool uses_printer;
  // bool uses_modem;
  // bool uses_sata;
};

// int save_prc_regs();

#endif // PROC_H
