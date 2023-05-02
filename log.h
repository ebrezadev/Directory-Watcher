#ifndef _LOG_H_
#define _LOG_H_

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/time.h>
#include "timing.h"

#define LOGFILE_PATH				".log"

/*initializes logger; creates log file.*/
int log_init();

/*writes to log file.*/
void log_printf(char *fmt, ...);

void log_deinit();

#endif