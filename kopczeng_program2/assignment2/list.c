#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "list.h"

/*
* Node Structure
*/
struct node {
    struct movie* movie;
    struct node* next;
};

/*
* List Structure
*/
struct list {
    struct node* head;
};

/*
* Function: List Create
* Parameters:
* Description: This function will create a brand new linked list
*/
struct list* list_create() {

    struct list* newList = malloc(sizeof(struct list));
    newList->head = NULL;

    return newList;
}

/*
* Function: Free List 
* Parameters: struct list* list
* Description: This function will free the memory used by a linked list and all of its data/components
*/
void free_list(struct list* list) {
    assert(list);

    //Free all individual nodes.
    struct node* next, * curr = list->head;
    while (curr != NULL) {
        next = curr->next;
        movie_free(curr->movie);
        free(curr);
        curr = next;
    }

    //Free the list
    free(list);
}

/*
* Function: Insert List
* Parameters: struct list* list, struct movie* movie
* Description: This function will insert a new node and its data into a pre-existing linked list
*/
void insert_list(struct list* list, struct movie* movie) {

    struct node* newNode = malloc(sizeof(struct node));
    newNode->movie = movie;
    newNode->next = list->head;
    list->head = newNode;

    return;
}


/*
* Function: Get Head
* Parameters: struct list* list
* Description: This function will return the head of a linked list
*/
struct node* get_head(struct list* list) {

    return list->head;
    
}

/*
* Function: Get Next
* Parameters: struct node* node
* Description: This function will return the next node in a linked list (NULL if its the end)
*/
struct node* get_next(struct node* node) {

    return node->next;
}

/*
* Function: Get Movie
* Parameters: struct node* node
* Description: This function will return the movie stored in a node
*/
struct movie* get_movie(struct node* node) {

    return node->movie;
}

