/*************************************************************
* Name: Grant Kopczenski
* ONID: 933-968-416
* Assignment: Assignment 2
* Date: 10/24/2022
* Class: CS 344
* Description: This Program will take a file based on user input and store the info on that file to a linked list. It will then create a new directory and 
* write info from that linked list into new files in the new directory
*************************************************************/
//Main file
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>

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
* Function: Random Number
* Description: Will return a random number between 0-99999
*/
int randNum() {
    return rand() % 100000;
}


/*
* Function: Write Files
* Parameters: struct list* list, char* name
* Description: Will take a linked list and a directory name and create files named by years full of movie names
*/
void writeFiles(struct list* list, char* name) {

    char dirName[30] = "./";
    char* movieName;
    strcat(dirName, name);
    strcat(dirName, "/");

    FILE *fp;

    struct node* selNode = get_head(list);
    int currYear = 0;

    while (selNode != NULL) {

        struct movie* selMovie = get_movie(selNode);
        currYear = get_release(selMovie);
        char sYear[8];
        sprintf(sYear, "%d.txt", currYear);
        char tempName[120];
        strcpy(tempName, dirName);
        strcat(tempName, sYear);
        fp = fopen(tempName, "a+");
        chmod(sYear, 0630);

        if (get_release(selMovie) == currYear) {
            movieName = get_title(selMovie);
            fwrite(movieName, sizeof(char), strlen(movieName), fp);
            fprintf(fp, "\n");
        }

        fclose(fp);
        selNode = get_next(selNode);
    }

    printf("\n");
}


/*
* Function: Create Directory
* Parameters: struct list* list, char* path
* Description: Will take a linked list and a csv file name and will create a new directory
*/
void createDir(struct list* list, char* path) {
    
    int numMovies = parse_file(path, list);
    printf("\nFound %d movies on file %s\n", numMovies, path);
    int rNum = randNum();
    char rChar[6];
    sprintf(rChar, "%d", rNum);
    char name[25] = ("kopczeng.movies.");
    strcat(name, rChar);


    if (mkdir(name, 0750) == -1) {
        perror("\nError");
    } else {
        printf("\nCreated directory with name %s", name);
        writeFiles(list, name);
    }
}


/*
* Function: Largest File
* Parameters: struct list* list
* Description: Will find the largest .csv file in the main directory
*/
void largestFile(struct list* list) {

    struct dirent *dir;
    DIR *d = opendir(".");
    struct stat statbuf;
    int i = 0;
    char *name = malloc(50*sizeof(char));

    while((dir = readdir(d)) != NULL) {
        lstat(dir->d_name, &statbuf);
        if (S_ISREG(statbuf.st_mode)) {

            char *dot = strrchr(dir->d_name, '.');
            if (dot != NULL && strcmp(dot, ".csv") == 0) {

                if (statbuf.st_size > i) {
                    i = (statbuf.st_size);
                    strcpy(name, dir->d_name);
                }
            }
        }
    }
    printf("\nLargest file found. File: %s\n", name);
    closedir(d);
    createDir(list, name);
    free(name);
}


/*
* Function: Smallest File
* Parameters: struct list* list
* Description: Will find the Smallest .csv file in the main directory
*/
void smallestFile(struct list* list) {

    struct dirent *dir;
    DIR *d = opendir(".");
    struct stat statbuf;
    int i = -1;
    char *name = malloc(50*sizeof(char));

    while((dir = readdir(d)) != NULL) {
        lstat(dir->d_name, &statbuf);
        if (S_ISREG(statbuf.st_mode)) {
            char *dot = strrchr(dir->d_name, '.');
            if (dot != NULL && strcmp(dot, ".csv") == 0) {

                if (statbuf.st_size < i || i == -1) {
                    i = statbuf.st_size;
                    strcpy(name, dir->d_name);
                }
            }
        }
    }
    printf("\nSmallest file found. File: %s\n", name);
    closedir(d);
    createDir(list, name);
    free(name);
}


/*
* Function: Name File
* Parameters: struct list* list
* Description: Will find a file in the main directory based on user input
*/
int nameFile(struct list* list) {

    struct dirent *dir;
    DIR *d = opendir(".");
    bool fileFound = false;
    
    char line[30];

    printf("\nEnter the name of the file you would like to search for: ");
    scanf("%s", line);

    if (strlen(line) > 0) {
        line[strlen(line)] = '\0';
    }

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, line) == 0) {
            printf("\nSuccessfully found file %s!\n", line);
            fileFound = true;
            createDir(list, line);
            
            closedir(d);
            return 3;
        }
    }
    if (!fileFound) {
        printf("\nNo file '%s' found.\n", line);
        closedir(d);
        return 0;
    }
}


/*
* Function: Pick File
* Parameters: struct list* list
* Description: User choice function allowing the user to decide how they want to use the program
*/
void pickFile(struct list* list) {

    int choice = 0;

    while (choice < 1 || choice > 3) {
        printf("\nWhich file do you want to process?");
        printf("\nEnter 1 to pick the largest file");
        printf("\nEnter 2 to pick the smallest file");
        printf("\nEnter 3 to pick a file by name\n");
        printf("\nEnter a choice 1 / 2 / 3: ");
        scanf("%d", &choice);
        if (choice == 1) {
            largestFile(list);
        } else if (choice == 2) {
            smallestFile(list);
        } else if (choice == 3) {
            choice = nameFile(list);
        } else {
            printf("\n\nInvalid choice! Please try again.\n");
        }
    }
}

/*
* Function: User Choice
* Description: Will allow the user to enter the main menu or exit the program
*/
void userChoice() {

    int choice = 1;

    while (choice == 1) {

        printf("\n1) Select a file to process\n2) Exit the program\n");

        printf("\nEnter a choice 1 / 2: ");
        scanf("%d", &choice);

        if (choice == 1) {
            struct list *list = list_create();
            pickFile(list);
            free_list(list);
        }
        else if (choice > 2 || choice < 1) {
            printf("\n\nInvalid choice! Please try again.\n");
            choice = 1;
        }
    }
}

//Main function
int main() {

    srand(time(0));
    userChoice();

    printf("\nProgram Exiting...\n\n");
    return 0;
}