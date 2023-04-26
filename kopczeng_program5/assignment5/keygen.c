/*************************************************************
* Name: Grant Kopczenski
* ONID: 933-968-416
* Assignment: Assignment 5
* Date: 12/7/2022
* Class: CS 344
* Description: This file will generate a key of capital letters and spaces a set number of characters long and print it out
*************************************************************/

//Included Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>


// Main Function
int main(int argc, char* argv[]) {
    
    int keylength = atoi(argv[1]); // Setting the desired size of the key to an int for later use

    srand(time(0)); // Seed rand

    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "; // Character array
    char key[keylength+1]; // Create the key array to hold the desired amount of characters + null terminator 

    int i;
    int selection;

    for (i = 0; i < keylength; i++) {
        selection = rand() % 27; // Random selection of characters from the set array above
        key[i] = alphabet[selection]; // Setting the characters into the new key array
    }

    key[i] = '\n'; // New line at the end

    printf("%s", key);

    return 0;
}
