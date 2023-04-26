//Movie header file
#ifndef __MOVIE_H
#define __MOVIE_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
/*
* Structure used to represent an individual movie
*/
struct movie;

/*
* Functions used to create, edit and destroy movie structs
*/
struct movie* movie_create(char* line);
void movie_free(struct movie* movie);
void change_language(struct movie* movie, char* token, int loopLen);

char* get_title(struct movie* movie);
int get_release(struct movie* movie);
char** get_language(struct movie* movie);
double get_rating(struct movie* movie);

#endif
