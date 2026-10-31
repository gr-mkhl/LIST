#include <stdio.h>
#include "list.h"

int main()
{
    list_t list = {};

    ListInit(&list, 10);

    AddToTail(&list, 1);
    PrintList(&list);
    AddToTail(&list, 2);
    PrintList(&list);
    AddToHead(&list, 3);
    PrintList(&list);
    AddToTail(&list, 4);
    PrintList(&list);
    AddToTail(&list, 5);
    PrintList(&list);
    DeleteFromHead(&list);
    PrintList(&list);
    DeleteFromHead(&list);
    PrintList(&list);
    DeleteFromHead(&list);
    PrintList(&list);
    DeleteFromHead(&list);
    PrintList(&list);
    DeleteFromHead(&list);
    PrintList(&list);

    ListDestroy(&list);

    return 0;
}
