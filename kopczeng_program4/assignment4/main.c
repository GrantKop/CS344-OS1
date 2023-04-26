/*************************************************************
* Name: Grant Kopczenski
* ONID: 933-968-416
* Assignment: Assignment 4
* Date: 11/21/2022
* Class: CS 344
* Description: This program will create two threads that will work with one another to count up and print a variable from 0-10
*************************************************************/

//Included Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define SIZE 11 // Size of the buffer, will hold 0-10

//Global thread Variables
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER; // Global mutex initializer
pthread_cond_t myCond1, myCond2 = PTHREAD_COND_INITIALIZER; // Global condition 1 & 2 initializer

//Global int variables
int myCount; // The counter variable
bool holder; // This variable is crucial to the conditionals in each thread

/*
* Function: Producer Function
* Description: This function acts as the producer thread for the program
*/
void producerFunction() {
  
  while (myCount < SIZE-1) {
    //sleep(1); uncomment this if waiting lines are not showing up

    pthread_mutex_lock(&myMutex);
    printf("PRODUCER: myMutex locked\n");

    while(holder) {
      printf("PRODUCER: waiting on myCond1\n");
      pthread_cond_wait(&myCond1, &myMutex);
    }

    myCount++; // Will increase myCount if its not 10 yet
    holder = true;
    pthread_mutex_unlock(&myMutex);
    printf("PRODUCER: myMutex unlocked\n");
    printf("PRODUCER: signaling myCond2\n");
    pthread_cond_signal(&myCond2);
  
  }
}

/*
* Function: Consumer Thread
* Description: This function is run by the created thread and acts as the consumer for the program
*/
void* consumerThread() {

  while (1) {
    //sleep(1); uncomment this if waiting lines are not showing up
    if (myCount > 0) {
      pthread_mutex_lock(&myMutex);
      printf("CONSUMER: myMutex locked\n");

      while (!holder) {
        printf("CONSUMER: waiting on myCond2\n");
        pthread_cond_wait(&myCond2, &myMutex);
      }

      printf("myCount: %d -> %d\n", myCount-1, myCount); // Print statement
      holder = false;

      pthread_mutex_unlock(&myMutex);
      printf("CONSUMER: myMutex unlocked\n");
      
      if (myCount == 10) {
        return NULL;
      }

      printf("CONSUMER: signaling myCond2\n");
      pthread_cond_signal(&myCond1);

    } 
  }
}

//Main Function
int main(int argc, char* argv[]) {
    
  printf("PROGRAM START\n"); // Program starts
  
  pthread_cond_init(&myCond1, NULL);
  pthread_cond_init(&myCond2, NULL);

  myCount = 0; // Set myCount to 0 at the start of the program
  holder = false; // Set holder to be false to encourage the consumer thread to run first
  pthread_t conThread; // Consumer thread
  
  if (pthread_create(&conThread, NULL, consumerThread, NULL) != 0) { // Create the consumer thread
    perror("Failed to create consumer thread\n");
  } else {
    printf("CONSUMER THREAD CREATED\n"); // Statement that the CONSUMER thread has been created
    producerFunction();
  }

  holder = true;
  pthread_join(conThread, NULL); // I use this to free the memory used by pthread_create(), thats it
  
  printf("PROGRAM END\n"); // Program ends
  return 0;
}
