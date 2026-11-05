#include <stdio.h>
#include <list.h>

int main()
{
    list_t list = {};

    ListInit(&list, 10);
    iterator_t this_it = GetHead(&list);
    for (int i = 1; i < 15; i++)
    {
        lst_err status = InsertAfter(&list, this_it, i);
        printf("0x%X\n", status);
        PrintList(&list);
        this_it = Next(&list, this_it);
    }
    MakeGraph(&list);
    ListDestroy(&list);

    return 0;
}
