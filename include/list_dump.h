#ifndef _LIST_DUMP_H_
#define _LIST_DUMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <log.h>
#include <list_struct.h>

#define color_lightgray  "\"#999999\""
#define color_gray       "\"#747474\""
#define color_lightblue  "\"#00abbb\""
#define color_blue       "\"#006b75\""
#define color_darkblue   "\"#000dff\""
#define color_lightred   "\"#ff0000\""
#define color_red        "\"#a80000\""
#define color_lightgreen "\"#C0FFC0\""
#define color_green      "\"#008000\""
#define color_lightpink  "\"#ff2edc\""
#define color_pink       "\"#b70098\""

void FPrintList ( list_t* list, FILE* file );
void GraphDump  ( list_t* list );

#endif
