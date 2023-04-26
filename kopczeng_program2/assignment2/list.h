//List header file
#ifndef __LIST_H
#define __LIST_H

#include "movie.h"

/*
* Structure used to represent a linked list.
*/
struct node;
struct list;

/*
* Functions to edit, create and remove a list
*/
struct list* list_create();
void free_list(struct list* list);
void insert_list(struct list* list, struct movie* movie);
struct node* get_head(struct list* list);
struct node* get_next(struct node* node);
struct movie* get_movie(struct node* node);


#endif
