#define LOGGING
#include <list.h>

//----------------------------правила работы со списком--------------------------------------------------
/*
элементы в списке нумеруются с 1, т.е. 1, 2, 3, ...; 0 - "технический" элемент
capacity включает в себя 0-элемент
size не  включает в себя 0-элемент
max(size) = capacity - 1
добавить перед 0-элементом <=> добавить в хвост
добавить после 0-элемента <=> добавить в голову
удалять 0-элемент нельзя

пользователю предлагается пользоваться следующим интерфейсом:
ListInit, InsertAfter, InsertBefore, DeleteByIterator, ListDestroy, Next, Prev, GetHead, GetTail

InsertToHead, InsertToTail, DeleteFromHead, DeleteFromTail ReallocListUp не являются интерфейсом списка и
не содержат в себе проверок на корректность построения списка для исключения повторных проверок
и вызываются функциями InsertAfter и InsertBefore
*/

const char* list_errors[] =
{
    [ALL_CORRECT]         = "List is correct",
    [INCORRECT_ARG]       = "Incorrect argument",
    [NULL_LIST_PTR]       = "Pointer to list is NULL",
    [EMPTY_LIST]          = "List is empty: size = 0",
    [LIST_OVERFLOW]       = "List isoverflow: size > capacity",
    [LIST_OOM]            = "List is out of memory: reallocation failed",
    [CAPACITY_ERROR]      = "Incorrect list capacity: capacity < 0",
    [SIZE_ERROR]          = "Incorrect list size: size < 0",
    [LINK_ERROR]          = "List double-link error: next(prev(A)) != A",
    [CYCLE_NEXT_ERROR]    = "Next-link cycle error",
    [CYCLE_PREV_ERROR]    = "Prev-link cycle error",
    [HASH_CORRUPTED]      = "Hash has been corrupted"
};

lst_err ListInit( list_t* list, ssize_t init_capacity )
{
    if (list == NULL)
        return NULL_LIST_PTR;

    if (init_capacity < 0)
        return INCORRECT_ARG;

    list->list_arr = (node_t*)calloc((size_t)(init_capacity), sizeof(node_t));
    if (list->list_arr == NULL)
        return LIST_OOM;

    list->capacity = init_capacity;
    list->size = 0;
    list->first_empty_iterator = 1;
// все, кроме последнего элемента инициализируются "свободными"
    for (ssize_t i = 1; i < list->capacity - 1; i++)
    {
        list->list_arr[i].data = POISON;
        list->list_arr[i].next = i + 1;
        list->list_arr[i].prev = -1;
    }

// (list->capacity - 1) - последний элемент списка
    list->list_arr[list->capacity - 1].data = POISON;
    list->list_arr[list->capacity - 1].next = 0;
    list->list_arr[list->capacity - 1].prev = -1;

// (0) - технический элемент списка
    list->list_arr[0].data = POISON;
    list->list_arr[0].next = 0;
    list->list_arr[0].prev = 0;

    CalcListHash(list);

    ON_LOG(LOG("Список проинициализирован с параметром init_capacity = {%zd}", init_capacity))
    ON_LOG(GraphDump(list);)

    return ALL_CORRECT;
}

uint64_t CalcListHash( list_t* list )
{
    uint64_t struct_hash = Hash_djb2(list, sizeof(list_t) - sizeof(uint64_t));
    uint64_t array_hash  = Hash_djb2(&list->list_arr[0], (size_t)list->capacity * sizeof(node_t));
    list->list_hash = struct_hash ^ array_hash;

    return struct_hash ^ array_hash;
}

iterator_t GetHead( list_t* list )
{
    ON_LOG(LOG("На голову списка указывает итератор {%zd}", list->list_arr[0].next))
    return list->list_arr[0].next;
}


iterator_t GetTail( list_t* list )
{
    ON_LOG(LOG("На хвост списка указывает итератор {%zd}", list->list_arr[0].prev))
    return list->list_arr[0].prev;
}


iterator_t Next( list_t* list, iterator_t iterator )
{
    ON_LOG(LOG("На следующий элемент списка указывает итератор {%zd}", list->list_arr[iterator].next))
    return list->list_arr[iterator].next;
}


iterator_t Prev( list_t* list, iterator_t iterator )
{
    ON_LOG(LOG("На предыдущий элемент списка указывает итератор {%zd}", list->list_arr[iterator].prev))
    return list->list_arr[iterator].prev;
}

// This is not list interface function, you shouldn't use it alone
lst_err ReallocListUp( list_t* list )
{
    ssize_t new_capacity = (list->capacity == 0 ? INIT_CAPACITY : list->capacity * 2);

    node_t* new_list_arr = (node_t*) realloc(list->list_arr, ((size_t)(new_capacity) * sizeof(node_t)));
    if (new_list_arr == NULL)
        return LIST_OOM;

    list->list_arr = new_list_arr;

// все элементы с (capacity) до (new_capacity - 2) инициализируются "свободными"
    for (ssize_t i = list->capacity; i < new_capacity - 1; i++)
    {
        list->list_arr[i].data = POISON;
        list->list_arr[i].next = i + 1;
        list->list_arr[i].prev = -1;
    }
// (new_capacity - 1) инициализируются как последний "свободный"
    list->list_arr[new_capacity - 1].next = list->first_empty_iterator;
    list->list_arr[new_capacity - 1].data = POISON;
    list->list_arr[new_capacity - 1].prev = -1;
// (capacity) инициализируются как первый "свободный"
    list->first_empty_iterator = list->capacity;

    list->capacity = new_capacity;

    CalcListHash(list);

    ON_LOG(LOG("Список расширен с capacity = {%zd} до new_capacity = {%zd}", list->capacity / 2, new_capacity))
    ON_LOG(GraphDump(list);)

    return ALL_CORRECT;
}

lst_err DeleteByIterator( list_t* list, iterator_t iterator_to_delete )
{
    if (list->size == 0)
        return EMPTY_LIST;

    if (!(0 < iterator_to_delete && iterator_to_delete <= list->size))
        return INCORRECT_ARG;

    lst_err status = ListVerifier(list);
    if (status != ALL_CORRECT)
        return status;

// достаем элемент из списка
    list->list_arr[list->list_arr[iterator_to_delete].next].prev = list->list_arr[iterator_to_delete].prev;
    list->list_arr[list->list_arr[iterator_to_delete].prev].next = list->list_arr[iterator_to_delete].next;

// инициализируем удаленный элемент "свободным" и добавляем первым в список свободных
    list->list_arr[iterator_to_delete].next = list->first_empty_iterator;
    list->list_arr[iterator_to_delete].data = POISON;
    list->list_arr[iterator_to_delete].prev = -1;
    list->first_empty_iterator = iterator_to_delete;

    list->size--;

    CalcListHash(list);

    ON_LOG(LOG("Из списка удален элемент, на который указывал итератор {%zd}", iterator_to_delete))
    ON_LOG(GraphDump(list);)

    return ALL_CORRECT;
}

lst_err InsertAfter( list_t* list, iterator_t prev_iterator, data_t value )
{
    if (!(0 <= prev_iterator && prev_iterator <= list->size))
        return INCORRECT_ARG;

    lst_err status = ListVerifier(list);
    if (status != ALL_CORRECT)
        return status;

    if (list->size == list->capacity - 1)
    {
        status = ReallocListUp(list);
        if (status != ALL_CORRECT)
            return status;
    }

    iterator_t next_iterator = list->list_arr[prev_iterator].next;

// ищем место в массиве под новый элемент
    iterator_t new_node_iterator = list->first_empty_iterator;
    list->first_empty_iterator = list->list_arr[list->first_empty_iterator].next;

// заполняем данные и связи для нового элемента
    list->list_arr[new_node_iterator].data = value;
    list->list_arr[new_node_iterator].next = next_iterator;
    list->list_arr[new_node_iterator].prev = prev_iterator;

// подключаем новый элемент в список
    list->list_arr[prev_iterator].next = new_node_iterator;
    list->list_arr[next_iterator].prev = new_node_iterator;

    list->size++;

    CalcListHash(list);

    ON_LOG(LOG("В список после элемента, на который указывает итератор {%zd}, добавлен элемент co значением {" os "}", prev_iterator, value))
    ON_LOG(GraphDump(list);)

    return ALL_CORRECT;
}

lst_err InsertBefore( list_t* list, iterator_t next_iterator, data_t value )
{
    if (!(0 <= next_iterator && next_iterator <= list->size))
        return INCORRECT_ARG;

    lst_err status = ListVerifier(list);
    if (status != ALL_CORRECT)
        return status;

    if (list->size == list->capacity - 1)
    {
        status = ReallocListUp(list);
        if (status != ALL_CORRECT)
            return status;
    }

    iterator_t prev_iterator = list->list_arr[next_iterator].prev;

// ищем место в массиве под новый элемент
    iterator_t new_node_iterator = list->first_empty_iterator;
    list->first_empty_iterator = list->list_arr[list->first_empty_iterator].next;

// заполняем данные и связи для нового элемента
    list->list_arr[new_node_iterator].data = value;
    list->list_arr[new_node_iterator].next = next_iterator;
    list->list_arr[new_node_iterator].prev = prev_iterator;

// подключаем новый элемент в список
    list->list_arr[prev_iterator].next = new_node_iterator;
    list->list_arr[next_iterator].prev = new_node_iterator;

    list->size++;

    CalcListHash(list);

    ON_LOG(LOG("В список перед элементом, на который указывает итератор {%zd}, добавлен элемент co значением {" os "}", next_iterator, value))
    ON_LOG(GraphDump(list);)

    return ALL_CORRECT;
}


lst_err ListDestroy( list_t* list )
{
    if (list == NULL)
        return NULL_LIST_PTR;

    for (ssize_t i = 0; i < list->capacity; i++)
    {
        list->list_arr[i].data = POISON;
        list->list_arr[i].next = POISON;
        list->list_arr[i].prev = POISON;
    }

    list->first_empty_iterator = POISON;
    list->capacity = POISON;
    list->size = POISON;

    list->list_hash = 0;

    free(list->list_arr);

    ON_LOG(LOG("Список удален"))

    return ALL_CORRECT;
}

lst_err ListVerifier( list_t* list )
{
    if (list == NULL)
        return NULL_LIST_PTR;
    else if (list->capacity <= 0)
        return CAPACITY_ERROR;
    else if (list->size < 0)
        return SIZE_ERROR;
    else if (list->size >= list->capacity)
        return LIST_OVERFLOW;
    else if (list->list_hash != CalcListHash(list))
        return HASH_CORRUPTED;

    for (ssize_t j = 0, i = 0; j < list->size + 1; j++)
    {
        if (list->list_arr[list->list_arr[i].next].prev != i)
            return LINK_ERROR;
        i = list->list_arr[i].next;
    }

    ssize_t start   = 0;
    ssize_t current = 0;

    for (ssize_t i = 0; i < list->size + 1; i++)
        current = list->list_arr[current].next;
    if (current != start)
        return CYCLE_NEXT_ERROR;

    current = 0;
    for (ssize_t i = 0; i < list->size + 1; i++)
        current = list->list_arr[current].prev;
    if (current != start)
        return CYCLE_PREV_ERROR;

    return ALL_CORRECT;
}


lst_err InsertToHead( list_t* list, data_t value )
{
    return InsertAfter(list, 0, value);
}

lst_err InsertToTail( list_t* list, data_t value )
{
    return InsertBefore(list, 0, value);
}

lst_err DeleteFromHead( list_t* list )
{
    return DeleteByIterator(list, list->list_arr[0].next);
}

lst_err DeleteFromTail( list_t* list )
{
   return DeleteByIterator(list, list->list_arr[0].prev);
}

//used hash algorithm djb2: https://dev.to/doogal/djb2-hash-function-string-to-integer-algorithm-explained-4bii?ysclid=mur1ep12vp401464212
uint64_t Hash_djb2( const void* beg, size_t len )
{
    uint64_t hash = 5381;

    for (const uint8_t* i = (const uint8_t*)beg; i < (const uint8_t*)beg + len; i++)
        hash = (hash << 5) + hash + *i;
    return hash;
}

uint64_t Hash_djb2_Except( const void* beg, const void* end, const void* except, size_t except_el_size )
{
    uint64_t hash = 5381;

    for (const uint8_t* i = (const uint8_t*)beg; i <= end; i++)
    {
        if (i == except)
            i += except_el_size;
        hash =  (hash << 5) + hash + *i;
    }
    return hash;
}
