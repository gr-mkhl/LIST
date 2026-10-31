#include <list.h>

lst_err ListInit( list_t* list, size_t init_capacity )
{
    if (list == NULL)
        return NULL_LIST_PTR;

    list->list_arr = (node*)calloc(init_capacity, sizeof(node));
    if (list->list_arr == NULL)
        return LIST_OOM;

    list->capacity = init_capacity;
    list->size = 0;

    for (ssize_t i = 1; i < list->capacity; i++)
    {
        list->list_arr[i].next = -1;
        list->list_arr[i].prev = -1;
    }

    list->list_arr[0].next = 0;
    list->list_arr[0].prev = 0;
    list->list_arr[0].data = POISON;

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

    list->capacity = 0;
    list->size = 0;
    free(list->list_arr);

    return ALL_CORRECT;
}

lst_err ListVerifier( list_t* list )
{
    lst_err status = ALL_CORRECT;

    if (list == NULL)
        status |= NULL_LIST_PTR;
    if (list->size == 0)
        status |= EMPTY_LIST;
    if (list->size == list->capacity)
        status |= LIST_OVERFLOW;

    return status;
}

lst_err AddToHead( list_t* list, data_t value )
{
    lst_err status = ListVerifier(list);

    if ((status & ~(ALL_CORRECT | EMPTY_LIST)) != 0)
        return status;

    for (ssize_t i = 0; i < list->capacity; i++)
    {
        if (list->list_arr[i].next == -1)
        {
            list->list_arr[i].next = list->list_arr[0].next;
            list->list_arr[i].data = value;
            list->list_arr[i].prev = 0;
            list->list_arr[list->list_arr[0].next].prev = i;
            list->list_arr[0].next = i;
            list->size++;
            break;
        }
    }

    return ALL_CORRECT;
}

lst_err AddToTail( list_t* list, data_t value )
{
    lst_err status = ListVerifier(list);
    if ((status & ~(ALL_CORRECT | EMPTY_LIST)) != 0)
        return status;

    for (ssize_t i = 0; i < list->capacity; i++)
    {
        if (list->list_arr[i].next == -1)
        {
            list->list_arr[i].prev = list->list_arr[0].prev;
            list->list_arr[i].data = value;
            list->list_arr[i].next = 0;
            list->list_arr[list->list_arr[0].prev].next = i;
            list->list_arr[0].prev = i;
            list->size++;
            break;
        }
    }

    return ALL_CORRECT;
}

lst_err DeleteFromHead( list_t* list )
{
    assert(list);

    if (list->size == 0)
        return EMPTY_LIST;

    size_t new_head = list->list_arr[list->list_arr[0].next].next;

    list->list_arr[list->list_arr[0].next].next = -1;
    list->list_arr[list->list_arr[0].next].prev = -1;
    list->list_arr[list->list_arr[0].next].data = POISON;

    list->list_arr[new_head].prev = 0;
    list->list_arr[0].next = new_head;
    list->size--;

    return ALL_CORRECT;
}

lst_err DeleteFromTail( list_t* list )
{
    assert(list);

    if (list->size == 0)
        return EMPTY_LIST;

    size_t new_tail = list->list_arr[list->list_arr[0].prev].prev;

    list->list_arr[list->list_arr[0].prev].data = POISON;
    list->list_arr[list->list_arr[0].prev].next = -1;
    list->list_arr[list->list_arr[0].prev].prev = -1;

    list->list_arr[new_tail].next = 0;
    list->list_arr[0].prev = new_tail;
    list->size--;

    return ALL_CORRECT;
}


void PrintList( list_t* list )
{
    printf("__list__\n");
    printf("========================================================\n");
    printf("size: %zu\ncapacity: %zu\nhead: %zu\ntail: %zu\n",
            list->size,
            list->capacity,
            list->list_arr[0].next,
            list->list_arr[0].prev);
    printf("--------------------------------------------------------\n");

    printf("index\t\t| data\t\t| next\t\t| prev\t\t\n");
    printf("--------------------------------------------------------\n");


    for (ssize_t id = list->list_arr[0].next; id != 0; id = list->list_arr[id].next)
    {
        printf("[%zu]\t\t| %d\t\t| %zd\t\t| %zd\t\t\n",
                id,
                list->list_arr[id].data,
                list->list_arr[id].next,
                list->list_arr[id].prev);
    }
    printf("========================================================\n\n\n");
}


