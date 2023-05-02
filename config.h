#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "exception.h"

/*a data structure containing source directory path
and destination directory path*/
typedef struct{
	char source_dir[FILENAME_MAX];
	char dest_dir[FILENAME_MAX];
}config_t;

/*opens the config file at the default location and fills
in the config_t struct. returns:
0 on success.
-1 on failing to find the config file.*/
int get_config(config_t *config_struct);

#endif