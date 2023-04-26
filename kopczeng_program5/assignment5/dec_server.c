/*************************************************************
* Name: Grant Kopczenski
* ONID: 933-968-416
* Assignment: Assignment 5
* Date: 12/7/2022
* Class: CS 344
* Description: This file works as a server to decrypt and encrypted message with a pre set key
*************************************************************/

//Included Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXCHAR 160001

// Error function used for reporting issues
void error(const char *msg) {
    perror(msg);
    exit(1);
} 

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, int portNumber) {
 
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);
    // Allow a client at any address to connect to this server
    address->sin_addr.s_addr = INADDR_ANY;
}

//Main function
int main(int argc, char *argv[]) {
    int connectionSocket, charsRead, charsWritten, bytesSent;
    char buffer[MAXCHAR];
    char smallBuff[1000];
    
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t sizeOfClientInfo = sizeof(clientAddress);

    // Check usage & args
    if (argc < 2) { 
        fprintf(stderr,"USAGE: %s port\n", argv[0]); 
        exit(1);
    } 
  
    // Create the socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        error("ERROR opening socket");
    }

    // Set up the address struct for the server socket
    setupAddressStruct(&serverAddress, atoi(argv[1]));

    // Associate the socket to the port
    if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        error("ERROR on binding");
    }

    // Start listening for connetions. Allow up to 5 connections to queue up
    listen(listenSocket, 5); 
  
    // Accept a connection, blocking if one is not available until one connects
    while(1) {
        connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept the connection request which creates a connection socket
        if (connectionSocket < 0) {
            error("ERROR on accept");
        }

        pid_t childPID = fork();
        switch(childPID) {
        case 0:

            close(listenSocket);

            // Get the message from the client and display it
            memset(buffer, '\0', sizeof(buffer));
            // Read the client's message from the socket
            while (strstr(buffer, "\n") == NULL) {
                memset(smallBuff, '\0', sizeof(smallBuff));
                charsRead = recv(connectionSocket, smallBuff, sizeof(smallBuff), 0);
                
                if (charsRead < 0) {
                    error("SERVER: Error reading from socket");
                }
                strcat(buffer, smallBuff);
            }

            char *check, *text, *key;
            int i;

            // Break up the message received
            check = strtok(buffer, ",");
            text = strtok(NULL, ",");
            key = strtok(NULL, ",");

            int textSize = strlen(text);
            char dec[MAXCHAR];

            // Make sure the file came from a decryption client
            if (strcmp(check, "dec") != 0) {
                int notDec = send(connectionSocket, "Client error: Not decryption file\n", 35, 0);
                close(connectionSocket);
                break;
            }

            // Decryption 
            for (i = 0; i < textSize; i++) {
                if (key[i] == ' ') {
                    key[i] = 26;
                } else {
                    key[i] = key[i] - 'A';
                } 

                if (text[i] == ' ') {
                    text[i] = 26;
                } else {
                    text[i] = text[i] - 'A';
                }

                dec[i] = (text[i] - key[i]) % 27; 

                // With out this if statement the decryption wont work properly
                if (dec[i] < 0) {
                    dec[i] = dec[i] + 27;
                } 
                
                if (dec[i] == 26) {
                    dec[i] = ' ';
                } else {
                    dec[i] = dec[i] + 'A';
                }
            }

            // Send a Success message back to the client
            charsWritten = 0;
            bytesSent = 0;

            while (charsWritten < strlen(dec)) {
                bytesSent = send(connectionSocket, dec, textSize, 0); 

                if (bytesSent < 0) {
                    error("SERVER: ERROR writing to socket");
                    exit(1);
                }
                charsWritten += bytesSent;
            }

            // Close the connection socket for this client
            close(connectionSocket); 
            exit(0);
        case -1:
            perror("fork()\n");
            close(listenSocket);
            exit(1);
            break;
        default:
            break;
        }
        close(connectionSocket); 
    }  
    // Close the listening socket
    close(listenSocket); 
    return 0;
}
