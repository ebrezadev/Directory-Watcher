#include "exception.h"

#define ERROR_STRING_SIZE			4096

void fatal(char *fmt, ...)
{
	va_list args;
    va_start(args, fmt);
	int string_size = ERROR_STRING_SIZE;
	char error_string[string_size];
	memset(error_string, 0, string_size);
	snprintf(error_string, string_size, "FATAL ERROR: ");
	int offset = sizeof("FATAL ERROR: ") - 1;
	string_size -= offset;
	vsnprintf(&error_string[offset], string_size, fmt, args);
	offset = (int)(strchr(error_string, '\0') - error_string);
	string_size -= offset;
	/*snprintf(&error_string[offset], string_size, " Exiting program.");*/
    log_printf("%s", error_string);
    va_end(args); 
	exit(EXIT_FAILURE);
}