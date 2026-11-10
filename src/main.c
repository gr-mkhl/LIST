#define LOGGING

#include <main.h>

int main()
{
    #ifdef LOGGING
    system("rm -rf logs; mkdir -p logs/htm; mkdir -p logs/img; mkdir -p logs/txt" );
    STARTLOG()
    #endif

    list_t list = {};
    data_t value = 0;
    ssize_t capacity = 0;
    ssize_t list_index = 0;
    iterator_t iterator = 0;
    int ch = 0;


    printf(MENU);
    while ((ch = getchar()) != 'q')
    {
        switch (ch)
        {
            case '1':
                scanf("%zd", &capacity);
                printf("\n\n%s\n\n", list_errors[ListInit(&list, capacity)]);
                break;
            case '2':
                scanf(os "%zd", &value, &list_index);
                if (list_index > list.size || list_index < 0)
                {
                    printf("Incorrect index to insert\n");
                    break;
                }
                iterator = GetHead(&list);
                for (ssize_t i = 1; i < list_index; i++)
                    iterator = Next(&list, iterator);
                printf("\n\n%s\n\n", list_errors[InsertAfter(&list, iterator, value)]);
                break;
            case '3':
                scanf("%zd", &list_index);
                if (list_index > list.size || list_index <= 0)
                {
                    printf("Incorrect index to delete\n");
                    break;
                }
                iterator = GetHead(&list);
                for (ssize_t i = 1; i < list_index; i++)
                    iterator = Next(&list, iterator);
                printf("\n\n%s\n\n", list_errors[DeleteByIterator(&list, iterator)]);
                break;
            case '4':
                FPrintList(&list, LOG_FILE);
                GraphDump(&list);
                break;
            case '5':
                printf("\n\n%s\n\n", list_errors[ListDestroy(&list)]);
                break;
            case 't':
            {
                int result = StartTestSystem();
                if (result == -1)
                {
                    printf("Tests completed correctly\n");
                }
                else
                {
                    printf("Test %d failed\n", result);
                }
                break;
            }
            default:
                printf("Invalid input\n");
                break;
        }
        printf(MENU);
        CleanBuffer();
    }

    ON_LOG(ENDLOG())
    return 0;
}

void CleanBuffer()
{
    int n = 0;

    while ((n = getchar()) != '\n' && n != EOF)
        continue;
    return;
}
