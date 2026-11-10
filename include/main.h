#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <log.h>
#include <list.h>
#include <list_dump.h>
#include <tests.h>

void CleanBuffer();

#define MENU "menu:\n"                                \
                "1 - init list sizeof\n"              \
                "2 - insert to list after\n"          \
                "3 - delete from list\n"              \
                "4 - test list dump\n"                \
                "5 - destroy list\n"                  \
                "t - run tests\n"                     \
                "q - exit program\n"

#endif
