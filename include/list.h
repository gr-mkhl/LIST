#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <log.h>

typedef int      data_t;
typedef ssize_t  iterator_t;
typedef uint16_t lst_err;

#define os           "%d"
#define POISON       -67
#define INIT_CAPACITY 16


enum LST_ERR_MASK
{
    ALL_CORRECT    = 0x00,
    INCORRECT_ARG  = 0x01,
    NULL_LIST_PTR  = 0x02,
    EMPTY_LIST     = 0x04,
    LIST_OVERFLOW  = 0x08,
    LIST_OOM       = 0x10,
    CAPACITY_ERROR = 0x20,
    SIZE_ERROR     = 0x40
};

struct node_t
{
    data_t data;
    iterator_t next;
    iterator_t prev;
};

struct list_t
{
    node_t* list_arr;
    ssize_t size;
    ssize_t capacity;
    iterator_t first_empty_iterator;
};

lst_err ListInit         ( list_t* list, ssize_t init_capacity );
lst_err ListDestroy      ( list_t* list );
iterator_t GetHead       ( list_t* list );
iterator_t GetTail       ( list_t* list );
iterator_t Next          ( list_t* list, iterator_t iterator );
iterator_t Prev          ( list_t* list, iterator_t iterator );
lst_err InsertAfter      ( list_t* list, iterator_t prev_iterator, data_t value );
lst_err InsertBefore     ( list_t* list, iterator_t next_iterator, data_t value );
lst_err InsertToHead     ( list_t* list, data_t value );
lst_err InsertToTail     ( list_t* list, data_t value );
lst_err DeleteByIterator ( list_t* list, iterator_t iterator_to_delete );
lst_err DeleteFromHead   ( list_t* list );
lst_err DeleteFromTail   ( list_t* list );
lst_err ReallocListUp    ( list_t* list );
lst_err ListVerifier     ( list_t* list );
void PrintList           ( list_t* list );
void MakeGraph           ( list_t* list );

#endif
