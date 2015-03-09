#include "usertraps.h"
#include "misc.h"

#include "reactions.h"

void main (int argc, char *argv[])
{
  sem_t h_procs_sem;  // Semaphore to signal the original process that we're done
  mbox_t h_n2_mbox;
  // Convert the command-line strings into integers for use as handles
  h_procs_sem = dstrtol(argv[1], NULL, 10);
  h_n2_mbox = dstrtol(argv[2], NULL, 10);
  
  if (mbox_open(h_n2_mbox) == MBOX_FAIL) {
    Printf("inject_n2 (%d): Could not open the mailbox!\n", getpid());
    Exit();
  }
  Printf("--PID %d: INJECT N2 molecule\n", getpid());
  if (mbox_send(h_n2_mbox, 0, NULL) == MBOX_FAIL) {
    Printf("inject_n2 (%d): Could not map the virtual address to the memory!\n", getpid());
    Exit();
  }
    
  // Signal the semaphore to tell the original process that we're done
  if(sem_signal(h_procs_sem) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", h_procs_sem); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
  // Printf("*****%s ended\n", argv[0]);
}
