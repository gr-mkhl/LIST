#include <log.h>

FILE* LOG_FILE = NULL;
char time_buffer[50] = {};


void StartLog( const char* filename, const char* func, int line )
{
    LOG_FILE = fopen(LOG_FILENAME, "w");
    assert(LOG_FILE);
    fprintf(LOG_FILE, "<pre>\n");
    fprintf(LOG_FILE, "[%s] LOG_FILE opened in %s:%s:%d\n", FillTime(), filename, func, line);

    fclose(LOG_FILE);
}

void Log( const char* str )
{
    LOG_FILE = fopen(LOG_FILENAME, "a");
    assert(LOG_FILE);

    fprintf(LOG_FILE, "%s\n", str);

    fclose(LOG_FILE);
}

char* FillTime()
{
    time_t now = time(NULL);
    struct timespec time = {};
    char miliseconds[20] = {};
    struct tm* time_struct = localtime(&now);

    clock_gettime(CLOCK_REALTIME, &time);
    sprintf(miliseconds, "%ld", time.tv_nsec / 1000000);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S:", time_struct);
    strcat(time_buffer, miliseconds);

    return time_buffer;
}

char* FillTimeForNames()
{
    time_t now = time(NULL);
    struct timespec time = {};
    char miliseconds[20] = {};
    struct tm* time_struct = localtime(&now);

    clock_gettime(CLOCK_REALTIME, &time);
    sprintf(miliseconds, "%ld", time.tv_nsec / 1000000);
    strftime(time_buffer, sizeof(time_buffer), "%H_%M_%S_", time_struct);
    strcat(time_buffer, miliseconds);

    return time_buffer;
}

void EndLog( const char* filename, const char* func, int line )
{
    LOG_FILE = fopen(LOG_FILENAME, "a");
    assert(LOG_FILE);

    fprintf(LOG_FILE, "[%s] LOG_FILE closed in %s:%s:%d\n", FillTime(), filename, func, line);

    fclose(LOG_FILE);
}
