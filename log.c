#include "log.h"

static FILE *log_file;

int log_init()
{
	log_file = fopen(LOGFILE_PATH, "a");
	
	if(log_file != NULL)
		return 0;
	else
		return -1;
}

void log_printf(char *fmt, ...)
{
	va_list args;
    va_start(args, fmt);
	char time_string[1024];
	get_time(time_string, 1024);
    fprintf(log_file, "[%s]: ", time_string);
    vfprintf(log_file, fmt, args);
    fprintf(log_file, "\n");
    va_end(args); 
	fflush(log_file);
}

void log_deinit()
{
	fclose(log_file);
}

