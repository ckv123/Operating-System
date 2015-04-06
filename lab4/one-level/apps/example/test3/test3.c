#include "usertraps.h"
#include "misc.h"

void main (int argc, char *argv[])
{
  sem_t s_procs_completed; // Semaphore to signal the original process that we're done
  // maximum virtual address 0xfffff
  // max virtual address + 1 to byte algin
  // that + 4 go beyond max address space for sure
  int* faultaddr = 0xfffff + 1 + 4;
  // Convert the command-line strings into integers for use as handles
  s_procs_completed = dstrtol(argv[1], NULL, 10);

  // Now print a message to show that everything worked
  Printf("Testcase 3: Segfault case\n");
  Printf("test3 (%d): First print about argc\n", getpid())
  Printf("test2 (%d):   &argc = 0x%x\n", getpid(), &argc);
  Printf("test2 (%d):   argc  = 0x%x\n", getpid(), argc);
  Printf("test2 (%d): Print about a memory location beyond max virtual address (0xfffff)\n");
  Printf("test2 (%d):   faultaddr = 0x%x\n", getpid(), faultaddr);
  Printf("test2 (%d):   *faultaddr= 0x%x\n", getpid(), *faultaddr);

  // Signal the semaphore to tell the original process that we're done
  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("test2 (%d): Bad semaphore s_procs_completed (%d)!\n", getpid(), s_procs_completed);
    Exit();
  }

  Printf("test2 (%d): Done!\n", getpid());
}
