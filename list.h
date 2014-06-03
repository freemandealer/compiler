#ifndef LIST_H_H
#define LIST_H_H

typedef struct _list_head_t {
	struct _list_head_t *prev;
	struct _list_head_t *next;
} list_head_t;

#endif
