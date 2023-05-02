/*polling in a blocked state. v2.0 using inotify instead of polling.
to be fixed: 
file name change in case of the files with the same name in destination directory
recursive moving of created directories inside source directory
creating destination directory if it doesnt exist
signal handling
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "log.h"
#include "exception.h"
#include "config.h"
#include "list_structure.h"
#include <sys/inotify.h>
#include <libdrm/drm.h>
#include <linux/usb/gadgetfs.h>
#include <linux/usbdevice_fs.h>

#define UMASK_FLAGS 0

/*The len field indicates how many bytes are actually allocated 
for the name field.The length of an individual inotify event is 
sizeof(struct inotify_event) + len. If the buffer passed to read() 
is too small to hold the next inotify_event structure, then read() 
fails with the error EINVAL to warn the application of this fact. the 
problem can be avoided altogether by ensuring that the buffer is always
large enough to hold at least one event: the buffer given to read() 
should be at least (sizeof(struct inotify_event) + NAME_MAX + 1) bytes,
where NAME_MAX is the maximum length of a filename, plus one for the 
terminating null byte.*/
#define DUMMY_LEN	(10 * (sizeof(struct inotify_event) + PATH_MAX + 1))

void move_file(const config_t config, const char *source_file_name);

int main(int argc, char *argv[])
{
	// file/directory attributes are [user r w x / group r w x / others r w x]
	// each of these 9 bits being 1 in umask, means that attribute
	// cannot be set in file/directory creation. these attributes
	// are later set in mode parameter of the open() system call for
	// file creation or mkdir() system call for directory creation.
	int previous_umask = umask(UMASK_FLAGS);

	if (log_init() != 0)
		exit(EXIT_FAILURE);
	log_printf("Program started. Process ID = %d.", (int)getpid());

	config_t config;
	if (get_config(&config) == -1)
		fatal(strerror(errno));

	/*DIR is called a directory stream. we use opendir() to obtain
	the handle structure and to use it in our later calls*/
	DIR *source_dir = opendir(config.source_dir);
	if (source_dir == NULL)
		fatal(strerror(errno));
	log_printf("Source directory  \"%s\" opened successfully.", config.source_dir);

	DIR *dest_dir = opendir(config.dest_dir);
	if (dest_dir == NULL)
		fatal("Destination directory  \"%s\" does not exist.", config.dest_dir);
	log_printf("Destination directory  \"%s\" opened successfully.", config.dest_dir);

	/*dirent is a structure that is used with readdir(). readdir()
	reads the next entry in a directory and saves the info in a struct
	of type dirent on heap; and rewrites this struct on each call, till
	it reaches the last entry and returns NULL.*/
	struct dirent *directory_entry;

	/*now we can read all the entries info in the source directory and 
	add them to a linked list in memory*/
	struct directory_entry_list_t *source_dir_list;
	if ((directory_entry = readdir(source_dir)) != NULL)
	{
		if ((source_dir_list = list_init(directory_entry)) == NULL)
			fatal("Memory allocation error on directory entries.");
		while ((directory_entry = readdir(source_dir)) != NULL)
			if (list_add(source_dir_list, directory_entry) == -1)
				fatal("Memory allocation error on directory entries.");
	}

	/*The inotify mechanism can be used to monitor files or 
	directories. When monitoring a directory, the application 
	will be informed about events for the directory itself and 
	for files inside the directory.*/

	/*An inotify file descriptor can be monitored using select(), 
	poll(), epoll, and, since Linux 2.6.25, signal-driven I/O. If 
	events are available to be read, then these interfaces indicate 
	the inotify file descriptor as being readable.*/

	/*initializing inotify and getting a file descriptor*/
	//int inotifyFD = inotify_init1(O_NONBLOCK);
	int inotifyFD = inotify_init();
	if (inotifyFD == -1)
		fatal("Cannot use inotify.");
/*adding source directory to the watch descriptor.*/
		int inotifyWD = inotify_add_watch(inotifyFD, config.source_dir, IN_CREATE | IN_MOVED_TO);
		if (inotifyWD == -1)
			fatal("Cannot add watch descriptor.");
	/*waitning for notifications in a loop*/
	struct inotify_event *event;
	for(;;)
	{
		char dummy[DUMMY_LEN];
		/*
		int number_of_bytes;
		ioctl(inotifyFD, FIONREAD, number_of_bytes);
		*/
		int numRead = read(inotifyFD, dummy, DUMMY_LEN);
		if (numRead == 0 || numRead == -1)
			fatal("inotify() read error.");
		rewinddir(source_dir);
		while ((directory_entry = readdir(source_dir)) != NULL)
			if (list_search(source_dir_list, directory_entry) == FALSE)
				move_file(config, directory_entry->d_name);	
		sleep(1);
	}
	return 0;
}

void move_file(const config_t config, const char *source_file_name)
{
	char source_path[FILENAME_MAX];
	memset(source_path, 0, FILENAME_MAX);
	strcpy(source_path, config.source_dir);
	strcat(source_path, "/");
	strcat(source_path, source_file_name);

	char destination_path[FILENAME_MAX];
	memset(destination_path, 0, FILENAME_MAX);
	strcpy(destination_path, config.dest_dir);
	strcat(destination_path, "/");
	strcat(destination_path, source_file_name);

	FILE *source_file = fopen(source_path, "r+b");
	FILE *destination_file = fopen(destination_path, "w");
	if (source_file == NULL)
		fatal("%s cannot be opened.", source_path);
	if (destination_file == NULL)
		fatal("%s cannot be created.", destination_path);

	int c;
	while ((c = fgetc(source_file)) != -1)
		fputc(c, destination_file);

	if (remove(source_path) != 0)
		fatal("Unable to delete %s.", source_path);

	fclose(source_file);
	fclose(destination_file);
}