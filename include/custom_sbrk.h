/*
----------------------------------------------------
| File        : custom_sbrk.h
| Discription : This file defines the API for a 
|               custom implementation of the sbrk() 
|               function for memory management.
----------------------------------------------------
*/


/*
----------------------------------------------------
| Header Guards
----------------------------------------------------
*/

#ifndef __CUSTOM_SBRK__
#define __CUSTOM_SBRK__


/*
----------------------------------------------------
| Standard Includes
----------------------------------------------------
*/

#include <inttypes.h>


/*
----------------------------------------------------
|  Global Variables Declaration
----------------------------------------------------
*/
#define SBRK_HEAP_SIZE 4096

/*
----------------------------------------------------
| Function Declaration
----------------------------------------------------
*/

void *custom_sbrk(intptr_t increments);


#endif  /* end of __CUSTOM__SBRK__*/
