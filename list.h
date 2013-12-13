#ifndef STACK_H
#define STACK_H

#include "commondefs.h"

typedef struct list_t {
	point_t s[65 * 8];
	int size;
} list_t;

void init_list(list_t* list);
void pop(list_t* list);
point_t top(list_t* list);
void push(list_t* list, point_t elem);
int empty(list_t* list);

#endif
