#ifndef _LIST_STRUCT_H_
#define _LIST_STRUCT_H_

#include <stddef.h>
#include <stdint.h>

typedef int      data_t;
#define format_specifier  "%d"


typedef ssize_t  iterator_t;


#define POISON            -67
#define INIT_CAPACITY      16

#ifdef DEBUGGING
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif

enum lst_err
{
    ALL_CORRECT      = 0,
    INCORRECT_ARG    = 1,
    NULL_LIST_PTR    = 2,
    EMPTY_LIST       = 3,
    LIST_OVERFLOW    = 4,
    LIST_OOM         = 5,
    CAPACITY_ERROR   = 6,
    SIZE_ERROR       = 7,
    LINK_ERROR       = 8,
    CYCLE_NEXT_ERROR = 9,
    CYCLE_PREV_ERROR = 10,
    HASH_CORRUPTED   = 11
};

extern const char* list_errors[];

struct node_t
{
    data_t     data;
    iterator_t next;
    iterator_t prev;
};

struct list_t
{
    node_t* list_arr;
    ssize_t size;
    ssize_t capacity;
    iterator_t first_empty_iterator;
    uint64_t list_hash;
};

#endif
