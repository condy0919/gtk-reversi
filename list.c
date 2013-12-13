#include "list.h"

void init_list(list_t* list)
{
	list->size = 0;
}

int empty(list_t* list)
{
	return list->size <= 0;
}

void pop(list_t* list)
{
	--list->size;
}

void push(list_t* list, point_t elem)
{
	if (list->size == 65 - 1)
		return;
	list->s[list->size++] = elem;
}

point_t top(list_t* list)
{
	return list->s[list->size - 1];
}
