#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "procon.h"

void main (int argc, char *argv[])
{
  circular_buffer *cb;      // Used to access missile codes in shared memory page
  uint32 h_mem;             // Handle to the shared memory page
  sem_t h_procs_sem;  // Semaphore to signal the original process that we're done
  lock_t h_cb_lock;
  sem_t s_fullslots;
  sem_t s_emptyslots;
  char msg[] = THE_MSG;
  int i;

  // if (argc != 4) { 
  //   Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore> <lock handle>\n"); 
  //   Exit();
  // } 

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  h_procs_sem = dstrtol(argv[2], NULL, 10);
  h_cb_lock = dstrtol(argv[3], NULL, 10);
  s_emptyslots = dstrtol(argv[4], NULL, 10);
  s_fullslots = dstrtol(argv[5], NULL, 10);

  // Map shared memory page into this process's memory space
  if ((cb = (circular_buffer *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  for(i = 0; i < THE_MSG_LENGTH; i++) {
    if(sem_wait(s_emptyslots) != SYNC_SUCCESS) {
      Printf("Bad semaphore s_emptyslots (%d) in ", s_emptyslots); Printf(argv[0]); Printf(", exiting...\n");
      Exit();
    }
    while(lock_acquire(h_cb_lock) != SYNC_SUCCESS);
    // CRITICAL SECTION
    cb->buffer[cb->tail] = msg[i];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    Printf("Producer %d ", getpid()); Printf("inserted: %c\n", msg[i]);
    if(sem_signal(s_fullslots) != SYNC_SUCCESS) {
      Printf("Bad semaphore s_fullslots (%d) in ", s_fullslots); Printf(argv[0]); Printf(", exiting...\n");
      Exit();
    }
    while(lock_release(h_cb_lock) != SYNC_SUCCESS);
    // END CRITICAL SECTION
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("producer: PID %d is complete.\n", getpid());
  if(sem_signal(h_procs_sem) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", h_procs_sem); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}
