#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

typedef int      data_t;
typedef uint16_t lst_err;
#define os       "%d"
#define POISON   -67

enum LST_ERR_MASK
{
    ALL_CORRECT   = 0x00,
    NULL_LIST_PTR  = 0x01,
    EMPTY_LIST    = 0x02,
    LIST_OVERFLOW = 0x04,
    LIST_OOM      = 0x08
};
struct node
{
    data_t data;
    ssize_t next;
    ssize_t prev;
};

struct list_t
{
    node* list_arr;
    size_t size;
    size_t capacity;
    size_t head;
    size_t tail;

};

lst_err ListInit       ( list_t* list, size_t init_capacity );
lst_err AddToTail      ( list_t* list, data_t value );
lst_err AddToHead      ( list_t* list, data_t value );
lst_err DeleteFromHead ( list_t* list );
lst_err DeleteFromTail ( list_t* list );
lst_err ListVerifier   ( list_t* list );
lst_err ListDestroy    ( list_t* list );
void PrintList         ( list_t* list );
#endif
