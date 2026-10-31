#include <stdio.h>
#include <list.h>

int main()
{
    list_t list = {};

    ListInit(&list, 10);
    AddToTail(&list, 7);
    PrintList(&list);
    for (int i = 0; i < 10; i++)
        AddToTail(&list, i);
    PrintList(&list);
    MakeGraph(&list);
    ListDestroy(&list);

    return 0;
}
