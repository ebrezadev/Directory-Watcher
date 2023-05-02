#ifndef _TIMING_H_
#define _TIMING_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

void get_time(char *formatted_time_string, size_t size);
long get_cputime();

#endif