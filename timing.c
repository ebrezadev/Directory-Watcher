#include "timing.h"

void get_time(char *formatted_time_string, size_t size)
{
	/*get epoch time from kernel*/
	time_t time_var = time(NULL);
	/*compute local time in struct tm from epoch time*/
	struct tm *time_tm = localtime(&time_var);
	memset((void *)formatted_time_string, 0, 1024);
	strftime(formatted_time_string, 1024, "%c", time_tm);
}

long get_cputime()
{
	return (long)(clock()/CLOCKS_PER_SEC);
}