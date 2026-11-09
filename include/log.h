#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#ifdef LOGGING
    #define ON_LOG(...) __VA_ARGS__
#else
    #define ON_LOG(...)
#endif

#define LOG_FILENAME "logs/htm/LOG.htm"
extern FILE* LOG_FILE;

void StartLog( const char* filename, const char* func, int line );
#define STARTLOG() StartLog(__FILE__, __func__, __LINE__);

char* FillTime();
char* FillTimeForNames();
void Log( const char* str );

void EndLog( const char* filename, const char* func, int line );
#define ENDLOG() EndLog(__FILE__, __func__, __LINE__);

#define LOG(...) do                                                      \
{                                                                        \
    LOG_FILE = fopen(LOG_FILENAME, "a");                                 \
    assert(LOG_FILE);                                                    \
    fprintf(LOG_FILE, "[%s] %s(): <", FillTime(), __func__);             \
    fprintf(LOG_FILE, __VA_ARGS__);                                      \
    fprintf(LOG_FILE, "> in %s:%d\n", __FILE__, __LINE__);               \
    fclose(LOG_FILE);                                                    \
} while (0);

#endif
