#include <tests.h>

lst_err CapacityErrorVerifier()
{
    list_t list = {};
    ListInit(&list, 10);
    InsertAfter(&list, GetHead(&list), 3131);
    list.capacity = -3;
    lst_err status = ListVerifier(&list);
    ListDestroy(&list);
    return status;
}

lst_err SizeErrorVerifier()
{
    list_t list = {};
    ListInit(&list, 10);
    InsertAfter(&list, GetHead(&list), 13);
    list.size = -1;
    lst_err status = ListVerifier(&list);
    ListDestroy(&list);
    return status;
}

lst_err IncorrectArgumentVerifier()
{
    list_t list = {};
    ListInit(&list, 10);
    lst_err status = InsertAfter(&list, 123, 2);
    ListDestroy(&list);
    return status;
}

lst_err NullListPtrVerifier()
{
    return ListVerifier(NULL);
}

lst_err EmptyListVerifier()
{
    list_t list = {};
    ListInit(&list, 10);
    lst_err status = DeleteByIterator(&list, 1);
    ListDestroy(&list);
    return status;
}

lst_err ListOverflowVerifier()
{
    list_t list = {};
    ListInit(&list, 10);
    list.size = list.capacity + 1;
    lst_err status = ListVerifier(&list);
    ListDestroy(&list);
    return status;
}

lst_err ListOOMVerifier()
{
    list_t list = {};
    return ListInit(&list, 100000000000000);
}

lst_err LinkErrorVerifier()
{
    list_t list = {};
    ListInit(&list, 10);
    InsertAfter(&list, GetHead(&list), 3);
    InsertAfter(&list, GetHead(&list), 134);
    InsertAfter(&list, GetHead(&list), 14);
    list.list_arr[1].next = 1;
    lst_err status = ListVerifier(&list);
    ListDestroy(&list);
    return status;
}

lst_err HashCorruptionError()
{
    list_t list = {};
    ListInit(&list, 10);
    InsertAfter(&list, GetHead(&list), 13);
    InsertAfter(&list, Next(&list, GetHead(&list)), 14);
    list.first_empty_iterator ^= 0x3324205;

    lst_err status = ListVerifier(&list);
    ListDestroy(&list);
    return status;
}

lst_err ListDumpTest()
{
    list_t list = {};
    ListInit(&list, 10);
    for (ssize_t i = 0; i < 10; i++)
    {
        InsertAfter(&list, i, 1);
        InsertAfter(&list, i + 1, 2);
        DeleteByIterator(&list, i);
    }
    ListDestroy(&list);
    return (lst_err)1;
}
typedef lst_err (*test_f)(void);

test_f array_of_tests[] =
{
    IncorrectArgumentVerifier,
    NullListPtrVerifier,
    EmptyListVerifier,
    ListOverflowVerifier,
    ListOOMVerifier,
    CapacityErrorVerifier,
    SizeErrorVerifier,
    LinkErrorVerifier,
    HashCorruptionError,
    ListDumpTest
};

int StartTestSystem()
{
    ON_LOG(LOG("Начало работы системы тестирования"))
    for (int i = 0; (unsigned)i < sizeof(array_of_tests) / sizeof(array_of_tests[0]); i++)
    {
        lst_err result = array_of_tests[i]();
        ON_LOG(LOG("Тест №%d. Результат: %s. Тест %s", i, list_errors[result], result == ALL_CORRECT ? "не пройден" : "пройден");)
        if (result == ALL_CORRECT)
            return i;
    }

    ON_LOG(LOG("Система тестирования завершила работу"))
    return -1;
}
