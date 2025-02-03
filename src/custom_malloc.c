/*
----------------------------------------------------
| File        : custom_malloc.c
| Description : This file defines custom memory 
|               allocation functions similar to 
|               malloc(), free(), using a custom 
|               implementation of sbrk().
|               
|               Features:
|               - Allocates memory dynamically
|               - Merges two consecutive free chunks
|               - Splits larger chunks when needed
|               - Implements a basic heap management system
----------------------------------------------------
*/

/*
----------------------------------------------------
|  Standarad Header Files
----------------------------------------------------
*/

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
----------------------------------------------------
|  User Defined Header Files
----------------------------------------------------
*/

#include "custom_sbrk.h"

/*
----------------------------------------------------
|  Preprocessor Directive - Macros
----------------------------------------------------
*/

/* Define the metadata size for each memory chunk */
#define META_DATA_SIZE_BYTES sizeof(MEM_CHUNK)

/* Macro to align memory size based on pointer size (32-bit or 64-bit
 * architecture) */
#define CUSTOM_ALIGN(size) (size + sizeof(void *) - 1) & ~(sizeof(void *) - 1)

/*
----------------------------------------------------
|  Structure Definition
----------------------------------------------------
*/

/* Memory Chunk Structure */
typedef struct mem_chunk {
  bool free;  // Free flag (1 = free, 0 = allocated)
  uint size;   // Size of the memory chunk
  struct mem_chunk*next; // Pointer to the next memory chunk
} MEM_CHUNK;

/*
----------------------------------------------------
|  Global Variables Declaration
----------------------------------------------------
*/

MEM_CHUNK *heap_head = NULL; // Initialize the heap

/*
---------------------------------------------------
|  Inline Function Declaration
----------------------------------------------------
*/

static inline bool custom_memory_init(void); // initializes heap memory

static inline bool split(int size, MEM_CHUNK *chunk); // Splits a large chunk if possible

/*
---------------------------------------------------
|  Inline Function Definition
----------------------------------------------------
*/

/*
 * Function:  custom_memory_init
 *            ------------------
 *            Requests and initializes system memory for the heap,
 *            which is used for dynamic memory allocation (e.g., malloc).
 *
 * Parameters:
 *   None (void).
 *
 * Returns:
 *   bool:        on Success : True
 *                on Failure:  False
 */
static inline bool custom_memory_init(void) {

  heap_head = custom_sbrk(SBRK_HEAP_SIZE); // Request 4KB of system memory
  if (heap_head == (void *)-1) {
    printf("\n%s: Error: custom sbrk failed to initialise system memory",
           __func__);
    return false;
  }
  heap_head->size = SBRK_HEAP_SIZE - META_DATA_SIZE_BYTES; // calcualte remaining memory excluding metadata
  heap_head->free = 1;      // Mark the entire memory block as free
  heap_head->next = NULL;   // No next block initially

  printf("\n%s : heap_head_ptr = %p, total size = %d, free = %d next = %p, "
         "metadata size = %ld ---- >"
         "custom sbrk initialization successfull ",
         __func__, (void*)(heap_head+META_DATA_SIZE_BYTES), heap_head->size, heap_head->free, (void*)heap_head->next,
         META_DATA_SIZE_BYTES);
  return true;
}

/**
 * Function: split
 *           ------
 *           Splits a memory chunk into two parts: one part with the requested size
 *           and the other part with the remaining size. The current chunk is updated
 *           to the requested size, and a new chunk is created for the remaining memory.
 *
 * Parameters:
 *   size:      The requested size for memory allocation, adjusted for
 *               alignment and metadata.
 *   chunk:     A pointer to the chunk to be split.
 *
 * Returns:
 *   bool:    On success: true ( if the chunk was successfully split).
 *            On failure: false
 */
static inline bool split(int size, MEM_CHUNK *chunk) {

  // Check if the current chunk is large enough to be split (including metadata
  // size)
  if (chunk->size >= (size + META_DATA_SIZE_BYTES)) {

    // Create a new chunk after the current one. We use the `chunk + size +
    // META_DATA_SIZE_BYTES` to place it properly.
    MEM_CHUNK *new_chunk = (MEM_CHUNK *)((char *)chunk + size + META_DATA_SIZE_BYTES);

    // Set the size of the new chunk to be the remaining size of the current
    // chunk, minus the requested size and metadata
    new_chunk->size = chunk->size - size - META_DATA_SIZE_BYTES;
  
    new_chunk->free = 1; // Mark the new chunk as free
    // Set the `next` pointer of the new chunk to point to the next chunk of memory
    new_chunk->next = chunk->next;
    chunk->next = new_chunk; // Link the current chunk to the new chunk
    chunk->free = 0; // Mark the current chunk as allocated (no longer free)
    chunk->size = size;    // Set the size of the current chunk to the requested size
    return true; // split was successful

  }

  // Return false if the chunk was too small to split
  return false;
}


/*---------------------------------------------------
|  Utility Function Definition
----------------------------------------------------
*/

/*
 * Function:  display
 *            --------
 *            Displays the current state of the heap memory, including:
 *             - The address of each memory chunk.
 *             - The size of each chunk.
 *             - Whether the chunk is free or allocated.
 *             - The pointer to the next chunk in the memory list.
 *
 * Parameters:
 *   None (void).
 *
 * Returns:
 *   None (void). The function prints heap details to the console.
 */
void display(void) {

  MEM_CHUNK *is_free_chunk =
      heap_head; // Start from the first chunk in the heap
    
  printf("\n\t------------------------------------");
  printf("\n\t%s : Heap Memory Layout             ", __func__);
  printf("\n\t------------------------------------");
  // Traverse the heap and print details of each memory chunk
  while (is_free_chunk != NULL) {
    printf("\n\tChunk Address: %p | Size: %d | Free: %d | Next: %p",
           (void*)(is_free_chunk+META_DATA_SIZE_BYTES), is_free_chunk->size, is_free_chunk->free,
           (void*)(is_free_chunk->next));

    is_free_chunk = is_free_chunk->next; // Move to the next chunk
  }
}


/*
----------------------------------------------------
| Custom Function Definition
----------------------------------------------------
*/

/*
 * Function:  custom_malloc
 *            --------------
 *            Allocates uninitialized heap memory as per the size requested by
 * the application. To avoid fragmentation, this function merges unused
 * consecutive chunks.
 *
 * Parameters:
 *   size:    Memory size requested by the user (in bytes).
 *
 * Returns:
 *   void*:   On success: Pointer to the allocated memory chunk.
 *            On failure: NULL.
 */
void *custom_malloc(size_t size) {

  // Check if heap memory is allocated
  if (heap_head == NULL) {
    // Request system memory if heap is not initialized
    if (custom_memory_init() != true) {
      return NULL; // Failed to initialize heap memory
    }
  }

  MEM_CHUNK *cur_chunk =
      heap_head; // Traverse the memory list to find a free chunk
  uint requested_size =
      CUSTOM_ALIGN(size); // Align size based on architecture (32-bit or 64-bit)

  while (cur_chunk != NULL) {
    if (cur_chunk->free == 1) { // If the chunk is free

      // Case 1: Exact fit - allocate the chunk without splitting
      if ((cur_chunk->size + META_DATA_SIZE_BYTES) ==
          (requested_size + META_DATA_SIZE_BYTES)) {
        cur_chunk->free = 0;
        
        printf("\n%s : Exact fit chunk - Allocated address = %p", __func__,
               (void *)(cur_chunk + META_DATA_SIZE_BYTES));
        return (void *)(cur_chunk + META_DATA_SIZE_BYTES);
      }

      // Case 2: Attempt to split the chunk
      else if (split(requested_size, cur_chunk)) {
        printf("\n%s : Split the chunk - Allocated address = %p", __func__,
               (void *)(cur_chunk + META_DATA_SIZE_BYTES));
        return (void *)(cur_chunk + META_DATA_SIZE_BYTES);
      }

      // Case 3: Merge adjacent free chunks if possible
      else {
        if (((MEM_CHUNK *)(cur_chunk->next))->free == 1) {
          cur_chunk->size += ((MEM_CHUNK *)(cur_chunk->next))->size;
          cur_chunk->next = ((MEM_CHUNK *)(cur_chunk->next))->next;
          cur_chunk->free = 1;

          // If the merged chunk now matches the requested size, allocate it
          if (cur_chunk->size == requested_size) {
            cur_chunk->free = 0;
            printf("\n%s : Merged chunk - Allocated address = %p", __func__,
               (void *)(cur_chunk + META_DATA_SIZE_BYTES));
            return (void *)(cur_chunk + META_DATA_SIZE_BYTES);
          }
        }
      }
    }
    // Move to the next chunk if the current one is too small
    cur_chunk = cur_chunk->next;
  }

  // No suitable memory block found, request more system memory
  return NULL;
}


/*
 * Function:  custom_free
 *            -----------
 *            Frees allocated heap memory, marking the chunk as available for
 * future allocations.
 *
 * Parameters:
 *   ptr: Pointer to the memory block to be freed.
 *
 * Returns:
 *   None (void).
 */
void custom_free(void *ptr) {

  MEM_CHUNK *find_chunk = heap_head;

  // Traverse the heap to find the chunk corresponding to the given pointer
  while (find_chunk != NULL) {
    if (ptr == (void *)(find_chunk + META_DATA_SIZE_BYTES)) {

      // Mark the chunk as free
      find_chunk->free = 1;

      printf("\n %s :Found:  ptr = %p, size = %d, free = %d, next = %p", __func__,
             (void*)(find_chunk + META_DATA_SIZE_BYTES), find_chunk->size,
             find_chunk->free, (void*)find_chunk->next);
      return;
    }

    // Move to the next chunk
    find_chunk = find_chunk->next;
  }

  // If the chunk was not found
  printf("\n%s: Chunk not found, Failed to free the request memory\n", __func__);
  return;
}
