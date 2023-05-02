#include "config.h"

#define TEMP_SIZE 				(FILENAME_MAX + FILENAME_MAX)

int get_config(config_t *config_struct)
{
	memset(config_struct->dest_dir, '\0', FILENAME_MAX); 
	memset(config_struct->source_dir, '\0', FILENAME_MAX);

	FILE *config_file;
	config_file = fopen("config", "r");
	if(config_file == NULL)
		return -1;

	char temp_string[TEMP_SIZE];
	while(fgets(temp_string, TEMP_SIZE, config_file) != NULL)
	{
		char *temp_pointer;
		if((temp_pointer = strstr(temp_string, "source")) != NULL)
		{
			temp_pointer += strlen("source");
			while(isspace(*temp_pointer))
				temp_pointer++;
			if(*temp_pointer != '=')
				continue;
			if((temp_pointer = strchr(temp_pointer, '"')) == NULL)
				continue;
			for(int index = 0; *(++temp_pointer) != '"'; index++)
				config_struct->source_dir[index] = *temp_pointer;
		}
		else if((temp_pointer = strstr(temp_string, "destination")) != NULL)
		{
			temp_pointer += strlen("destination");
			while(isspace(*temp_pointer))
				temp_pointer++;
			if(*temp_pointer != '=')
				continue;
			if((temp_pointer = strchr(temp_pointer, '"')) == NULL)
				continue;
			for(int index = 0; *(++temp_pointer) != '"'; index++)
				config_struct->dest_dir[index] = *temp_pointer;
		}
	}
	return 0;
}