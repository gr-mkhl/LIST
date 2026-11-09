#include <stdio.h>
#include <stdlib.h>
#include <log.h>
#include <list.h>
#include <list_dump.h>

void CleanBuffer();

#define MENU "menu:\n"                                \
                "1 - init list sizeof\n"              \
                "2 - insert to list after\n"          \
                "3 - delete from list\n"              \
                "4 - test list dump\n"                \
                "5 - destroy list\n"                  \
                "q - exit program\n"
