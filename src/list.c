#include <list.h>

lst_err ListInit( list_t* list, ssize_t init_capacity )
{
    if (list == NULL)
        return NULL_LIST_PTR;

    if (init_capacity < 0)
        return INCORRECT_ARG;

    list->list_arr = (node_t*)calloc((size_t)(init_capacity + 1), sizeof(node_t));
    if (list->list_arr == NULL)
        return LIST_OOM;

    list->capacity = init_capacity;
    list->size = 0;
    list->first_empty_node = 1;

    for (ssize_t i = 1; i <= list->capacity - 1; i++)
    {
        list->list_arr[i].data = POISON;
        list->list_arr[i].next = i + 1;
        list->list_arr[i].prev = -1;
    }

    list->list_arr[list->capacity].data = POISON;
    list->list_arr[list->capacity].next = 0;
    list->list_arr[list->capacity].prev = -1;


    list->list_arr[0].data = POISON;
    list->list_arr[0].next = 0;
    list->list_arr[0].prev = 0;

    return ALL_CORRECT;
}

// This is not list interface function, you shouldn't use it alone
lst_err ReallocListUp( list_t* list )
{
    ssize_t new_capacity = (list->capacity == 0 ? INIT_CAPACITY : list->capacity * 2);

    node_t* new_list_arr = (node_t*) realloc(list->list_arr, ((size_t)(new_capacity + 1) * sizeof(node_t)));
    if (new_list_arr == NULL)
        return LIST_OOM;

    list->list_arr = new_list_arr;

    for (ssize_t i = list->capacity + 1; i <= new_capacity - 1; i++)
    {
        list->list_arr[i].data = POISON;
        list->list_arr[i].next = i + 1;
        list->list_arr[i].prev = -1;
    }

    list->list_arr[new_capacity].next = list->first_empty_node;
    list->list_arr[new_capacity].data = POISON;
    list->list_arr[new_capacity].prev = -1;

    list->first_empty_node = list->capacity + 1;

    list->capacity = new_capacity;

    return ALL_CORRECT;
}

lst_err AddAfterId( list_t* list, ssize_t prev_id, data_t value )
{
    if (prev_id > list->size || prev_id < 0) //FIXME - разобраться с идеологией индексации
        return INCORRECT_ARG;

    lst_err status = ListVerifier(list);
    if ((status & ~(ALL_CORRECT | EMPTY_LIST )) != 0)
        return status;

    if (list->size == list->capacity)
    {
        status = ReallocListUp(list);
        if (status != ALL_CORRECT)
            return status;
    }

    ssize_t prev_index = 0;
    for (ssize_t i = 0; i <= prev_id; i++)
        prev_index = list->list_arr[prev_index].next;
    //после цикла index равен индексу элемента с номером prev_id

    ssize_t next_index = list->list_arr[prev_index].next;

    ssize_t new_node_index = list->first_empty_node;
    list->first_empty_node = list->list_arr[new_node_index].next;

    list->list_arr[new_node_index].data = value;
    list->list_arr[new_node_index].next = next_index;
    list->list_arr[new_node_index].prev = prev_index;
    list->list_arr[prev_index].next = new_node_index;
    list->list_arr[next_index].prev = new_node_index;
    list->size++;

    return ALL_CORRECT;
}

lst_err DeleteById( list_t* list, ssize_t id )
{
    if (id >= list->size || id <= 0) //FIXME - разобраться с идеологией индексации
        return INCORRECT_ARG;

    lst_err status = ListVerifier(list);
    if ((status & ~(ALL_CORRECT | LIST_OVERFLOW)) != 0)
        return status;

    ssize_t index = 0;
    for (ssize_t i = 0; i <= id; i++)
        index = list->list_arr[index].next;
    //после цикла index равен индексу элемента с номером id

    list->list_arr[list->list_arr[index].next].prev = list->list_arr[index].prev;
    list->list_arr[list->list_arr[index].prev].next = list->list_arr[index].next;

    list->list_arr[index].next = list->first_empty_node;
    list->list_arr[index].data = POISON;
    list->list_arr[index].prev = -1;
    list->first_empty_node = index;
    list->size--;

    return ALL_CORRECT;
}

// lst_err AddBeforeId( list_t* list, ssize_t next_id, data_t value )
// {
//     if (next_id > list->size || next_id < 0) //FIXME - разобраться с идеологией индексации
//         return INCORRECT_ARG;
//
//     lst_err status = ListVerifier(list);
//     if ((status & ~(ALL_CORRECT | EMPTY_LIST)) != 0)
//         return status;
//
//     if (list->size == list->capacity)
//     {
//         status = ReallocListUp(list);
//         if (status != ALL_CORRECT)
//             return status;
//     }
//
//     ssize_t next_index = 0;
//     for (ssize_t i = 0; i <= next_id; i++)
//         next_index = list->list_arr[next_index].next;
//     //после цикла index равен индексу элемента с номером next_id
//
//     ssize_t prev_index = list->list_arr[next_index].prev;
//
//     ssize_t new_node_index = list->first_empty_node;
//     list->first_empty_node = list->list_arr[new_node_index].next;
//
//     list->list_arr[new_node_index].data = value;
//     list->list_arr[new_node_index].next = next_index;
//     list->list_arr[new_node_index].prev = prev_index;
//     list->list_arr[prev_index].next = new_node_index;
//     list->list_arr[next_index].prev = new_node_index;
//
//     return ALL_CORRECT;
// }


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
//  if (list->size == list->capacity)
//      status |= LIST_OVERFLOW;
    if (list->first_empty_node == 0)
        status |= NO_EMPTY_NODES;

    return status;
}

lst_err AddToHead( list_t* list, data_t value )
{
    lst_err status = ListVerifier(list);

    if ((status & ~(ALL_CORRECT | EMPTY_LIST)) != 0)
        return status;

    if (list->size == list->capacity)
    {
        status = ReallocListUp(list);
        if (status != ALL_CORRECT)
            return status;
    }

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
    printf("status: 0x%X\n", status);
    if ((status & ~(ALL_CORRECT | EMPTY_LIST | NO_EMPTY_NODES)) != 0)
        return status;

    if (list->size == list->capacity)
    {
        status = ReallocListUp(list);
        if (status != ALL_CORRECT)
            return status;
    }

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


    // for (ssize_t id = list->list_arr[0].next; id != 0; id = list->list_arr[id].next)
    // {
    //     printf("[%zd]\t\t| %d\t\t| %zd\t\t| %zd\t\t\n",
    //             id,
    //             list->list_arr[id].data,
    //             list->list_arr[id].next,
    //             list->list_arr[id].prev);
    // }

    for (ssize_t id = 0; id <= list->capacity; id++)
    {
        printf("[%zd]\t\t| %d\t\t| %zd\t\t| %zd\t\t\n",
                id,
                list->list_arr[id].data,
                list->list_arr[id].next,
                list->list_arr[id].prev);
    }

    printf("========================================================\n\n\n");
}

void MakeGraph( list_t* list )
{
    FILE* graph = fopen("scripts/graph.txt", "w");
    assert(graph);

    fprintf(graph, "digraph\n");
    fprintf(graph, "{\n");
    //fprintf(graph, "rankdir=LR\n");

    fprintf(graph,
        "first_empty [shape=Mrecord; style = filled; fillcolor = \"#ffc0ec\"; color = \"#e605e6f1\";\n"
        "      label = \" FIRST_EMPTY \"; ]\n\n");
    fprintf(graph,
        "head [shape=Mrecord; style = filled; fillcolor = \"#ffc0ec\"; color = \"#e605e6f1\";\n"
        "      label = \" HEAD \"; ]\n\n");
    fprintf(graph,
        "tail [shape=Mrecord; style = filled; fillcolor = \"#ffc0ec\"; color = \"#e605e6f1\";\n"
        "      label = \" TAIL \"; ]\n\n");


    for (ssize_t id = list->list_arr[0].next; id != 0; id = list->list_arr[id].next)
    {
        fprintf(graph,
        "node%zd [shape=Mrecord; style = filled; fillcolor = \"#C0FFC0\"; color = \"#008000\";\n"
        "         label = \"  <data> data = %d | <id> id = %zd | { <prev> prev = %zd | <next> next = %zd }\"; ]\n\n",
                  id, list->list_arr[id].data, id, list->list_arr[id].prev, list->list_arr[id].next);
    }

    for (ssize_t id = list->first_empty_node; id != 0; id = list->list_arr[id].next)
    {
        fprintf(graph,
        "empty_node%zd [shape=Mrecord; style = filled; fillcolor = \"#da6262\"; color = \"#c50404\";\n"
        "         label = \" { <data> EMPTY | <id> id = %zd | <next> next = %zd }\"; ]\n\n",
                  id, id, list->list_arr[id].next);
    }

    ssize_t first_node = list->list_arr[0].next;
    ssize_t last_node  = list->list_arr[0].prev;

    fprintf(graph, "first_empty  -> empty_node%zd:id\n"
                   "head         -> node%zd\n"
                   "node%zd:next -> tail\n"
                   "node%zd:next -> node%zd:id\n"
                   "node%zd:prev -> node%zd:id\n",
                    list->first_empty_node,
                    first_node,
                    last_node,
                    first_node, list->list_arr[first_node].next,
                    last_node , list->list_arr[last_node ].prev);


    for (ssize_t id = list->list_arr[first_node].next; id != last_node; id = list->list_arr[id].next)
    {
        fprintf(graph,
        "node%zd:next -> node%zd:id\n"
        "node%zd:prev -> node%zd:id\n",
        id, list->list_arr[id].next,
        id, list->list_arr[id].prev);
    }


    for (ssize_t id = list->first_empty_node; list->list_arr[id].next != 0; id = list->list_arr[id].next)
    {
        fprintf(graph,
        "empty_node%zd:next -> empty_node%zd:id\n",
        id, list->list_arr[id].next);
    }

    fprintf(graph, "}");
    fclose(graph);
}
