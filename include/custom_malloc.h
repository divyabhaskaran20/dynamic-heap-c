/*
----------------------------------------------------
| File        : custom_malloc.h
| Discription : This file defines the API for a
|               custom memory allocation function
|               using sbrk().
----------------------------------------------------
*/

/*
----------------------------------------------------
| Header Guards
----------------------------------------------------
*/

#ifndef __CUSTOM_MALLOC__
#define __CUSTOM_MALLOC__

/*
----------------------------------------------------
| Function Declaration
----------------------------------------------------
*/

void *custom_malloc(int size); // Allocates memory

void custom_free(void *ptr); // Frees allocated memory

void display(void); // Displays memory state

#endif /* end of __CUSTOM__MALLOC__*/
