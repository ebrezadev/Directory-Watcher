#ifndef _LIST_STRUCTURE_H_
#define _LIST_STRUCTURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/*data structure for a block of linked list; containing 
directory entry members information.*/
struct directory_entry_list_t{
	struct dirent directory_entry_member;
	struct directory_entry_list_t *next_member;
};

#ifndef bool

typedef enum{
	FALSE = 0,
	TRUE = 1
}bool;

#endif

/*creates a new list and returns its head address. returns:
head address on success
NULL on memory allocation error*/
struct directory_entry_list_t *list_init(const struct dirent *new_member);

/*adds a new 'struct dirent' entry at the end of the given list. returns:
0 on success
-1 on memory allocation error*/
int list_add(struct directory_entry_list_t *head, const struct dirent *new_member);

/*will search inside given list for the given struct dirent. returns:
TRUE on success
FALSE on failure*/
bool list_search(struct directory_entry_list_t *head, const struct dirent *search_member);

#endif