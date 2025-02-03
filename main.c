/*
----------------------------------------------------
| File        : main.c
| Description : This file serves as the entry point
|               for testing the custom memory
|               allocation functions implemented
|               in custom_malloc.c.
|
| Features:
| - Allocates and frees memory using custom_malloc()
| - Displays the memory state after each operation
| - Demonstrates merging and splitting of memory chunks
----------------------------------------------------
*/

/*
----------------------------------------------------
|  Standard Header Files
----------------------------------------------------
*/

#include <stdio.h>

/*
----------------------------------------------------
|  User Defined Header Files
----------------------------------------------------
*/

#include "custom_malloc.h"

/*
----------------------------------------------------
|  Main Entry Point
----------------------------------------------------
*/

/*
 * Function:  main
 *           ------
 *           This function tests the custom memory allocation
 *           and deallocation functions by performing a series
 *           of memory allocations and freeing operations.
 *
 * Parameters:
 *   None (void).
 *
 * Returns:
 *   int: Returns 0 on successful execution.
 */
int main() {

  // Display program start message
  printf("\n===========================================");
  printf("\n        Custom Malloc Test Program         ");
  printf("\n===========================================\n");

  // ---------------------- STAGE 1 ---------------------- //
  // Allocating 4 bytes of memory
  printf("\n\n%s: ----------------------  STAGE 1: Allocating 4 bytes of memory...\n",
         __func__);
  int *ptr = (int *)custom_malloc(4);
  printf("\n %s : Allocated 4 bytes at address: %p\n", __func__, (void *)ptr);
  display(); // Display current memory state

  // ---------------------- STAGE 2 ---------------------- //
  // Allocating 8, 16, and 32 bytes of memory
  printf(
      "\n\n----------------------  STAGE 2: Allocating 8, 16, and 32 bytes of memory...\n");
  int *ptr1 = (int *)custom_malloc(8);
  int *ptr2 = (int *)custom_malloc(16);
  int *ptr3 = (int *)custom_malloc(32);

  printf("\n %s : Allocated 8 bytes  at address: %p", __func__, (void *)ptr1);
  printf("\n %s : Allocated 16 bytes at address: %p", __func__, (void *)ptr2);
  printf("\n %s : Allocated 32 bytes at address: %p\n", __func__, (void *)ptr3);
  display(); // Display updated memory state

  // ---------------------- STAGE 3 ---------------------- //
  // Freeing 4 & 8 bytes of memory
  printf("\n\n----------------------  STAGE 3: Freeing 4 & 8 bytes of memory...\n");
  custom_free(ptr);
  custom_free(ptr1);

  printf("\n %s : Freed memory at address: %p", __func__, (void *)ptr);
  printf("\n %s : Freed memory at address: %p\n", __func__, (void *)ptr1);
  display(); // Display memory state after freeing

  // ---------------------- STAGE 4 ---------------------- //
  // Allocating 16 bytes of memory
  printf("\n\n----------------------  STAGE 4: Allocating 16 bytes of memory...\n");
  int *ptr7 = (int *)custom_malloc(16);
  printf("\n %s : Allocated 16 bytes at address: %p", __func__, (void *)ptr7);
  display(); // Display memory state after allocation

  // Display program completion message
  printf("\n===================================================================");
  printf("\n        Program Execution Completed                                ");
  printf("\n===================================================================\n");

  return 0;
}
