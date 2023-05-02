#include "list_structure.h"

struct directory_entry_list_t *list_init(const struct dirent *new_member)
{
	struct directory_entry_list_t *head;
	if((head = (struct directory_entry_list_t *)malloc(sizeof(struct directory_entry_list_t))) == NULL)
		return NULL;
	head->next_member = NULL;
	head->directory_entry_member = *new_member;
	return head;
}

int list_add(struct directory_entry_list_t *head, const struct dirent *new_member)
{
	struct directory_entry_list_t *temp = head;
	while(temp->next_member != NULL)
		temp = temp->next_member;
	if((temp->next_member = (struct directory_entry_list_t *)malloc(sizeof(struct directory_entry_list_t))) == NULL)
		return -1;
	temp = temp->next_member;
	temp->next_member = NULL;
	temp->directory_entry_member = *new_member;
	return 0;
}

bool list_search(struct directory_entry_list_t *head, const struct dirent *search_member)
{
	struct directory_entry_list_t *temp = head;
	while(!((strcmp(temp->directory_entry_member.d_name, search_member->d_name) == 0) &&
		   (temp->directory_entry_member.d_ino == search_member->d_ino)))
	{
		if(temp->next_member == NULL)
			return FALSE;
		temp = temp->next_member;
	}
	return TRUE;
}