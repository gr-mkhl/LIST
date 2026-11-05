#include <list.h>

//----------------------------правила работы со списком--------------------------------------------------
/*
элементы в списке нумеруются с 1, т.е. 1, 2, 3, ...; 0 - технический элемент
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

    return ALL_CORRECT;
}


iterator_t GetHead( list_t* list )
{
    return list->list_arr[0].next;
}


iterator_t GetTail( list_t* list )
{
    return list->list_arr[0].prev;
}


iterator_t Next( list_t* list, iterator_t iterator )
{
    return list->list_arr[iterator].next;
}


iterator_t Prev( list_t* list, iterator_t iterator )
{
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

    return ALL_CORRECT;
}

lst_err DeleteByIterator( list_t* list, iterator_t iterator_to_delete )
{
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

    free(list->list_arr);

    return ALL_CORRECT;
}

lst_err ListVerifier( list_t* list )
{
    lst_err status = ALL_CORRECT;

    if (list == NULL)
        status |= NULL_LIST_PTR;
    if (list->capacity <= 0)
        status |= CAPACITY_ERROR;
    if (list->size < 0)
        status |= SIZE_ERROR;
    if (list->size >= list->capacity)
        status |= LIST_OVERFLOW;

    return status;
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


void PrintList( list_t* list )
{
    printf("__list__\n");
    printf("========================================================\n");
    printf("size: %zd\ncapacity: %zd\nhead: %zd\ntail: %zd\nfirst_empty: %zd\n",
            list->size,
            list->capacity,
            list->list_arr[0].next,
            list->list_arr[0].prev,
            list->first_empty_iterator);
    printf("--------------------------------------------------------\n");

    printf("index\t\t| data\t\t| next\t\t| prev\t\t\n");
    printf("--------------------------------------------------------\n");


    // for (ssize_t iterator = list->list_arr[0].next; iterator != 0; iterator = list->list_arr[iterator].next)
    // {
    //     printf("[%zd]\t\t| %d\t\t| %zd\t\t| %zd\t\t\n",
    //             iterator,
    //             list->list_arr[iterator].data,
    //             list->list_arr[iterator].next,
    //             list->list_arr[iterator].prev);
    // }

    for (ssize_t id = 0; id < list->capacity; id++)
    {
        printf("[%zd]\t\t| %d\t\t| %zd\t\t| %zd\t\t\n",
                id,
                list->list_arr[id].data,
                list->list_arr[id].next,
                list->list_arr[id].prev);
    }

    printf("========================================================\n\n\n");
}

void MakeGraph( list_t* list ) //FIXME - переделать, чтоб элементы были в порядке возрастания индекса в массиве
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

    for (ssize_t id = list->first_empty_iterator; id != 0; id = list->list_arr[id].next)
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
                    list->first_empty_iterator,
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


    for (ssize_t id = list->first_empty_iterator; list->list_arr[id].next != 0; id = list->list_arr[id].next)
    {
        fprintf(graph,
        "empty_node%zd:next -> empty_node%zd:id\n",
        id, list->list_arr[id].next);
    }

    fprintf(graph, "}");
    fclose(graph);
}
