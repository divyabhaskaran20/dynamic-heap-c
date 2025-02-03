/*
----------------------------------------------------
| File        : custom_sbrk.c
| Description : This file implements the custom_sbrk function 
|               for managing memory allocation by simulating the 
|               behavior of the standard sbrk system call.
----------------------------------------------------
*/

/*
----------------------------------------------------
| Standard Header Files
----------------------------------------------------
*/
#include <inttypes.h>

/*
----------------------------------------------------
| User Defined Header Files
----------------------------------------------------
*/
#include "custom_sbrk.h"

/*
----------------------------------------------------
| Global Variables Declaration
----------------------------------------------------
*/
static char *heap_mem[SBRK_HEAP_SIZE]; // Static array to simulate heap memory

void *current_ptr = heap_mem;          // Pointer to the current position in the heap memory
void *end_ptr = heap_mem + SBRK_HEAP_SIZE; // Pointer to the end of the heap memory

/*
----------------------------------------------------
| Custom Function Definitions
----------------------------------------------------
*/

/**
 * Function:  custom_sbrk
 *            ------------
 *            Simulates the behavior of the `sbrk` system call by allocating 
 *            memory in a static heap. 
 *
 * Parameters:
 *   increments:  The number of bytes to increment the program's 
 *                break (the end of the heap memory).
 *
 * Returns:
 *   void*:       On success - returns a pointer to the allocated memory block.
 *                On failure - returns `(void*)-1` to indicate memory exhaustion.
 */
void *custom_sbrk(intptr_t increments) {

  void *prev_ptr = current_ptr; // Store the current heap pointer (before increment)
  current_ptr  = (char*)current_ptr + increments;    // Increment the heap pointer to allocate memory

  // If the new pointer exceeds the end of the heap, rollback to previous position and return failure
  if (current_ptr > end_ptr) {
    current_ptr = prev_ptr; // Restore the previous pointer
    return (void *)-1;       // Indicate failure (out of memory)
  }

  return prev_ptr; // Return the previous heap pointer (start of the allocated block)
}

