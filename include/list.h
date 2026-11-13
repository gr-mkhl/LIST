#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <log.h>
#include <list_struct.h>
#include <list_dump.h>


enum lst_err ListInit         ( struct list_t* list, ssize_t init_capacity );
enum lst_err ListDestroy      ( struct list_t* list );
iterator_t GetHead       ( struct list_t* list );
iterator_t GetTail       ( struct list_t* list );
iterator_t Next          ( struct list_t* list, iterator_t iterator );
iterator_t Prev          ( struct list_t* list, iterator_t iterator );
enum lst_err InsertAfter      ( struct list_t* list, iterator_t prev_iterator, data_t value );
enum lst_err InsertBefore     ( struct list_t* list, iterator_t next_iterator, data_t value );
enum lst_err InsertToHead     ( struct list_t* list, data_t value );
enum lst_err InsertToTail     ( struct list_t* list, data_t value );
enum lst_err DeleteByIterator ( struct list_t* list, iterator_t iterator_to_delete );
enum lst_err DeleteFromHead   ( struct list_t* list );
enum lst_err DeleteFromTail   ( struct list_t* list );
enum lst_err ReallocListUp    ( struct list_t* list );
enum lst_err ListVerifier     ( struct list_t* list );

uint64_t Hash_djb2   ( const void* beg, size_t len );
uint64_t CalcListHash( struct list_t* list );

#endif
