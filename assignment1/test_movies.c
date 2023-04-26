/*************************************************************
* Name: Grant Kopczenski
* ONID: 933-968-416
* Assignment: Assignment 1
* Date: 10/10/2022
* Class: CS 344
* Description: This program will take a file containing info about movies, 
* will store the data and display it accordingly 
*************************************************************/
//Main file
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "movie.h"
#include "list.h"

#define MAXCHAR 5000

/*
* Function: Parse File
* Parameters: char* path, struct list* list
* Description: Will take a path string containing a file name, and will write its contents onto a list
*/
int parse_file(char* path, struct list* list) {

    FILE *movies_s;
    char line[MAXCHAR];
    int numMovies = 0;

    if ((movies_s = fopen(path, "r")) == NULL) {
        printf("Error opening file for reading: %s\n", path);
        exit(1);
    }
    
    fgets(line, MAXCHAR, movies_s);
    fgets(line, MAXCHAR, movies_s);
    
    while (feof(movies_s) != true) {
        

        struct movie *newMovie;
        newMovie = movie_create(line);
        insert_list(list, newMovie);
        numMovies++;
        fgets(line, MAXCHAR, movies_s);
    }

    fclose(movies_s);

    return numMovies;
}

/*
* Function: Movies in a Year
* Parameters: struct list* list
* Description: Will take a linked list of movies and display them if they were released in a given year 
*/
void moviesInYear(struct list* list) {
    
    struct node* selNode = get_head(list);
    int foundMovie = 0;
    int selectedYear = 0;
    
    
    while (selectedYear < 1900 || selectedYear > 2021) {
        printf("\nYou must choose a year between 1900 and 2021");
        printf("\nEnter the year you would like to see movies from: ");
        scanf("%d", &selectedYear);
    }
    
    while (selNode != NULL) {
        
        struct movie* selMovie = get_movie(selNode);
        if (get_release(selMovie) == selectedYear) {
            char* movieName = get_title(selMovie);
            printf("%s\n", movieName);
            foundMovie = 1;
        }
        selNode = get_next(selNode);
    }

    if (foundMovie == 0) {
        printf("\nNo movies found for the year %d.\n", selectedYear);
        return;
    }
    return;
}

/*
* Function: Highest Rated Movie
* Parameters: struct list* list
* Description: Will display the highest rated movie of each year between 1900-2021
*/
void highestRated(struct list* list) {

    for (int year = 1900; year < 2021; year++) {

        char* selMovTitle = NULL;
        float mRating = 0.0;
        struct node* selNode = get_head(list);

        while (selNode != NULL) {

            struct movie* selMovie = get_movie(selNode);
            if (get_release(selMovie) != year) {

            }
            else if (get_rating(selMovie) > mRating) {
                selNode = get_next(selNode);
                mRating = get_rating(selMovie);
                selMovTitle = get_title(selMovie);

            } 
            selNode = get_next(selNode);
          
        }
        if (selMovTitle != NULL) {
            printf("\n%d %0.1f %s", year, mRating, selMovTitle);
        }
    }
}

/*
* Function: Specific Language
* Parameters: struct list* list
* Description: Will display movies if they are in a certain language
*/
void specLang(struct list* list) {

    struct node* selNode = get_head(list);
    struct movie* selMovie;

    int foundMovie = 0;
    char line[20];
    char** langGroup;

    printf("\nEnter the language you would like to see movies in: ");
    scanf("%s", line);

    if (strlen(line) > 0) {
        line[strlen(line)] = '\0';
    }

    while (selNode != NULL) {
        
        selMovie = get_movie(selNode);
        langGroup = get_language(selMovie);
        
        for (int i = 0; i < 5; i++) {
            if (langGroup[i] == NULL) {
                i = 5;
            }
            else if (strcmp(langGroup[i], line) == 0) {
                printf("%d ", get_release(selMovie));
                printf("%s\n", get_title(selMovie));
                foundMovie = 1;
                i = 5;
            }
        }
        selNode = get_next(selNode);
    }

    if (foundMovie == 0) {
        printf("\nNo movies found in %s.\n", line);
        return;
    }
    return;

}

/*
* Function: User Choice
* Parameters: struct list* list
* Description: This function will prompt the user to choose how they want to view the data
*/
void userChoice(struct list* list) {

    int choice = 0;

    printf("\n\n1) Show movies released in a specified year");
    printf("\n2) Show the highest rated movie in each year");
    printf("\n3) Show the title and year released for all movies in a specific language");
    printf("\n4) Exit the program");
    printf("\nEnter a choice from 1-4: ");
    scanf("%d", &choice);

    if (choice == 1) {
        moviesInYear(list);

    } else if (choice == 2) {
        highestRated(list);

    } else if (choice == 3) {
        specLang(list);

    } else if (choice == 4) {
        return;

    } else {
        printf("\nInvalid answer! Must be between 1-4! Try again.\n");
    }
    userChoice(list);
}

/*
* Main Function
*/
int main(int argc, char **argv) {

    //Checks to make sure user entered arguments
    if (argc < 2) {
        printf("Please input a movie CSV file to parse.\n Usage: ./title file.csv\n");
        return 1;
    }

    //Creates the linked list and fills it with data from a file
    struct list *list = list_create();
    int numMovies = parse_file(argv[1], list);
    printf("\nSuccesfully loaded %d movies from %s!\n", numMovies, argv[1]);

    userChoice(list);

    //Frees all the memory used
    free_list(list);

    printf("\nProgram Exited.\n");

    return 0;
}
