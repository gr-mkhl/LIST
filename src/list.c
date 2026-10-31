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
    list->first_empty_node = 1;

    for (ssize_t i = 1; i < list->capacity - 1; i++)
    {
        list->list_arr[i].data = POISON;
        list->list_arr[i].next = i + 1;
        list->list_arr[i].prev = -1;
    }

    list->list_arr[list->capacity - 1].data = POISON;
    list->list_arr[list->capacity - 1].next = 0;
    list->list_arr[list->capacity - 1].prev = -1;


    list->list_arr[0].data = POISON;
    list->list_arr[0].next = 0;
    list->list_arr[0].prev = 0;

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

    list->first_empty_node = POISON;
    list->capacity = POISON;
    list->size = POISON;
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
    if (list->first_empty_node == 0)
        status |= NO_EMPTY_NODES;

    return status;
}

lst_err AddToHead( list_t* list, data_t value )
{
    lst_err status = ListVerifier(list);

    if ((status & ~(ALL_CORRECT | EMPTY_LIST)) != 0)
        return status;

    ssize_t new_head = list->first_empty_node;
    list->first_empty_node = list->list_arr[new_head].next;

    ssize_t old_head = list->list_arr[0].next;
    list->list_arr[new_head].next = old_head;
    list->list_arr[new_head].data = value;
    list->list_arr[new_head].prev = 0;

    list->list_arr[old_head].prev = new_head;
    list->list_arr[0].next = new_head;
    list->size++;

    return ALL_CORRECT;
}

lst_err AddToTail( list_t* list, data_t value )
{
    lst_err status = ListVerifier(list);

    if ((status & ~(ALL_CORRECT | EMPTY_LIST)) != 0)
        return status;

    ssize_t new_tail = list->first_empty_node;
    list->first_empty_node = list->list_arr[new_tail].next;

    ssize_t old_tail = list->list_arr[0].prev;
    list->list_arr[new_tail].prev = old_tail;
    list->list_arr[new_tail].data = value;
    list->list_arr[new_tail].next = 0;

    list->list_arr[old_tail].next = new_tail;
    list->list_arr[0].prev = new_tail;
    list->size++;

    return ALL_CORRECT;
}

lst_err DeleteFromHead( list_t* list )
{
    lst_err status = ListVerifier(list);

    if ((status & ~(ALL_CORRECT | LIST_OVERFLOW)) != 0)
        return status;

    ssize_t old_head = list->list_arr[0].next;
    ssize_t new_head = list->list_arr[old_head].next;

    list->list_arr[old_head].next = list->first_empty_node;
    list->list_arr[old_head].data = POISON;
    list->list_arr[old_head].prev = -1;
    list->first_empty_node = old_head;

    list->list_arr[new_head].prev = 0;
    list->list_arr[0].next = new_head;
    list->size--;

    return ALL_CORRECT;
}

lst_err DeleteFromTail( list_t* list )
{
    lst_err status = ListVerifier(list);

    if ((status & ~(ALL_CORRECT | LIST_OVERFLOW)) != 0)
        return status;

    ssize_t old_tail = list->list_arr[0].prev;
    ssize_t new_tail = list->list_arr[old_tail].prev;

    list->list_arr[old_tail].next = list->first_empty_node;
    list->list_arr[old_tail].data = POISON;
    list->list_arr[old_tail].prev = -1;
    list->first_empty_node = old_tail;

    list->list_arr[new_tail].next = 0;
    list->list_arr[0].prev = new_tail;
    list->size--;

    return ALL_CORRECT;
}


void PrintList( list_t* list )
{
    printf("__list__\n");
    printf("========================================================\n");
    printf("size: %zd\ncapacity: %zd\nhead: %zd\ntail: %zd\nfirst_empty: %zd\n",
            list->size,
            list->capacity,
            list->list_arr[0].next,
            list->list_arr[0].prev,
            list->first_empty_node);
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


