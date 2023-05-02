#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "log.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

void fatal(char *fmt, ...);

#endif