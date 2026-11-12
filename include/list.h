#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <log.h>
#include <list_struct.h>
#include <list_dump.h>


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

uint64_t Hash_djb2   ( const void* beg, size_t len );
uint64_t CalcListHash( list_t* list );

#endif
