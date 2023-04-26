#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "movie.h"

/*
* Movie Structure
*/
struct movie {
    char* title;
    int releaseYear;
    char** language;
    double rating;
};

/*
* Function: Movie Create
* Parameters: char* line
* Description: Will create a movie struct full of info from a line off a file
*/

struct movie* movie_create(char* line) {
    struct movie* newMovie = malloc(sizeof(struct movie));
    assert(newMovie);

    //Set the title
    char* title = strtok(line, ",");
    newMovie->title = (char*) malloc(sizeof(char) * (strlen(title) + 1));
    strcpy(newMovie->title, title);

    //Set the release year
    char* release = strtok(NULL, ",");
    newMovie->releaseYear = atoi(release); 

    //Store the languages
    char* languages = strtok(NULL, ",");

    //Set the rating as a double
    char* rating = strtok(NULL, "\n");
    newMovie->rating = strtod(rating, NULL);

    //Set the languages by calling change_language
    int length = strlen(languages); 
    change_language(newMovie, languages, length);

    return newMovie;
}

/*
* Function: Movie Free
* Parameters: struct movie* movie
* Description: This function will free the memory used by a movie struct and all of its components
*/
void movie_free(struct movie* movie) {
    
    //Free the title char*
    free(movie->title);

    //Free each language char*
    for (int i = 0; i < 5; i++) {
        if (movie->language[i] != NULL) {
            free(movie->language[i]);
        }
    }

    //Free the language char** and the movie struct
    free(movie->language);
    free(movie);
    
    return;
}

/*
* Function: Change Language
* Parameters: struct movie* movie, char* languages, int loopLen
* Description: This function will change the language options of a movie 
*/
void change_language(struct movie* movie, char* languages, int loopLen) {

    //Create a language 2D array and set its values to NULL
    movie->language = malloc(5*sizeof(char*));
    for (int j = 0; j < 5; j++) {
        movie->language[j] = NULL;
    }

    //Set the NULL terminator
    languages[0] = '\0';
    languages = &languages[1];
    languages[loopLen - 1] = '\0';
    loopLen = loopLen - 2;
    int countLen = 0;
    int i = 0;
    char* lang = strtok(languages, ";");
    
    //Set the languages
    while (countLen < loopLen) {
        movie->language[i] = (char*) malloc(sizeof(char) * (strlen(lang) + 1));
        strcpy(movie->language[i], lang);
        countLen = countLen + (strlen(lang) + 1);
        lang = strtok(NULL, ";]");
        i++;
    }
    
    return;
}

/*
* Function: Get Title
* Parameters: struct movie* movie
* Description: This function returns the title of a movie
*/
char* get_title(struct movie* movie) {

    return movie->title;
}

/*
* Function: Get Release Year
* Parameters: struct movie* movie
* Description: This function will return the release year of a movie
*/
int get_release(struct movie* movie) {

    return movie->releaseYear;
}

/*
* Function: Get Language
* Parameters: struct movie* movie
* Description: This function will give the language options of a movie
*/
char** get_language(struct movie* movie) {
    return movie->language;
}

/*
* Function: Get Rating
* Parameters: struct movie* movie
* Description: This function will return the rating of a movie
*/
double get_rating(struct movie* movie) {
    return movie->rating;
}
