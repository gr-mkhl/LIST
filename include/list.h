#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

typedef int      data_t;
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
    NO_EMPTY_NODES = 0x20,
    CAPACITY_ERROR = 0x40,
    SIZE_ERROR     = 0x80

};

struct node_t
{
    data_t data;
    ssize_t next;
    ssize_t prev;
};

struct list_t
{
    node_t* list_arr;
    ssize_t size;
    ssize_t capacity;
    ssize_t first_empty_node;

};

lst_err ListInit       ( list_t* list, ssize_t init_capacity );
lst_err AddAfterId     ( list_t* list, ssize_t prev_id, data_t value );
lst_err DeleteById     ( list_t* list, ssize_t id );
//lst_err AddBeforeId  ( list_t* list, ssize_t next_id, data_t value );
lst_err AddToTail      ( list_t* list, data_t value );
lst_err AddToHead      ( list_t* list, data_t value );
lst_err DeleteFromHead ( list_t* list );
lst_err DeleteFromTail ( list_t* list );
lst_err ListVerifier   ( list_t* list );
lst_err ReallocListUp  ( list_t* list );
lst_err ListDestroy    ( list_t* list );
void PrintList         ( list_t* list );
void MakeGraph         ( list_t* list );

#endif
