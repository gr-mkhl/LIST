#define LOGGING
#include <main.h>

int main()
{

    list_t list = {};
    data_t value = 0;
    ssize_t capacity = 0;
    ssize_t list_index = 0;
    iterator_t iterator = 0;
    int ch = 0;

    ON_LOG(system("rm -rf graph_dump; mkdir -p graph_dump");)

    printf(MENU);
    while ((ch = getchar()) != 'q')
    {
        switch (ch)
        {
            case '1':
                scanf("%zd", &capacity);
                ListInit(&list, capacity);
                break;
            case '2':
                scanf(os "%zd", &value, &list_index);
                if (list_index > list.size || list_index < 0)
                {
                    printf("Incorrect index to insert\n");
                    break;
                }
                iterator = GetHead(&list);
                for (ssize_t i = 0; i < list_index; i++)
                    iterator = Next(&list, iterator);
                InsertAfter(&list, iterator, value);
                break;
            case '3':
                scanf("%zd", &list_index);
                iterator = GetHead(&list);
                for (ssize_t i = 0; i < list_index; i++)
                    iterator = Next(&list, iterator);
                DeleteByIterator(&list, iterator);
                break;
            case '4':
                PrintList(&list);
                MakeGraph(&list);
                break;
            case '5':
                ListDestroy(&list);
                break;
            default:
                printf("Invalid input\n");
                break;
        }
        printf(MENU);
        CleanBuffer();
    }

    ON_LOG(system("rmdir --ignore-fail-on-non-empty graph_dump");)

    return 0;
}

void CleanBuffer()
{
    int n = 0;

    while ((n = getchar()) != '\n' && n != EOF)
        continue;
    return;
}
