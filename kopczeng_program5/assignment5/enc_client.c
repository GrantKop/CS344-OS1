/*************************************************************
* Name: Grant Kopczenski
* ONID: 933-968-416
* Assignment: Assignment 5
* Date: 12/7/2022
* Class: CS 344
* Description: This file will take a message and a pre set key and will attach them together before being sent to the encryption server for encryption
* ** it will also make sure the message has the correct type of characters
*************************************************************/

//Included Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

#define MAXCHAR 160000

/**
* Client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Take information from files (text and key) and send it as a message to the server.
* 3. Print the encrypted message received from the server and exit the program.
*/

// Error function used for reporting issues
void error(const char *msg) { 
    perror(msg); 
    exit(0); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, int portNumber, char* hostname) {
 
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Get the DNS entry for this host name
    struct hostent* hostInfo = gethostbyname(hostname); 
    if (hostInfo == NULL) { 
        fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
        exit(0); 
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*) &address->sin_addr.s_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
}

// Main Function
int main(int argc, char* argv[]) {
    
    int socketFD, portNumber, charsWritten, charsRead, bytesSent;
    int lineLen;
    struct sockaddr_in serverAddress;
    char buffer[MAXCHAR*2];
    char smallBuff[MAXCHAR];
    
    // Check usage & args
    if (argc < 3) { 
        fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
        exit(0); 
    } 

    FILE *fileT;
    FILE *keyFile;
    char line[MAXCHAR];
    char key[MAXCHAR];
    int i, j, k;

    for (j = 0; j < 2; j++) {

        if (j == 0) {
            if ((keyFile = fopen(argv[2], "r")) == NULL) {
                error("ERROR Failed to open file");
            }

            fgets(key, MAXCHAR, keyFile);

            int keyLen = strlen(key);
            for (i = 0; i < keyLen; i++) {
                if (key[i] == '\n') {
                    key[i] = ' '; // Set the \n from fgets to a ' '
                    break;    
                }
            }

            fclose(keyFile);

        } else {

            if ((fileT = fopen(argv[1], "r")) == NULL) {
                error("ERROR Failed to open file");
            }

            fgets(line, MAXCHAR, fileT);
            lineLen = strlen(line);

            if (strlen(key) < lineLen) {
                error("ERROR Key is too short"); // If key is shorter than the message
            }

            for (i = 0; i < lineLen; i++) {

                if (line[i] == '\n') {
                    line[i] = ' '; // Set the \n from fgets to a ' '
                    break;    
                }

                int asciiValue = line[i];
                
                // If the text going to be encrypted has a character that is a capital or a space
                if (!((asciiValue >= 65 && asciiValue <= 90) || asciiValue == 32)) {
                    error("ERROR Bad character found");
                }
            }

            fclose(fileT);
        }
    }

    // Create a socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    if (socketFD < 0) {
        error("CLIENT: ERROR opening socket");
    }

    // Set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        error("CLIENT: ERROR connecting");
    }

    // Clear out the buffer array
    memset(buffer, '\0', sizeof(buffer));
    char encIdentifier[] = "enc,";
    sprintf(buffer, "%s%s%s%s%s", encIdentifier, line, ",", key, "\n"); // Stitch the message together
    
    // Send message to server
    // Write to the server
    charsWritten = 0;
    bytesSent = 0;
    int messageLength = strlen(buffer);

    while (charsWritten < messageLength) {
        bytesSent = send(socketFD, buffer, messageLength, 0); 
        if (bytesSent < 0) {
            error("CLIENT: ERROR writing to socket");
            exit(1);
        }
        charsWritten += bytesSent;
    }

    // Get return message from server
    // Clear out the buffer again for reuse
    memset(buffer, '\0', sizeof(buffer));
    // Read data from the socket, leaving \0 at end

    int bytesReceived = 0;
    charsRead = 0;
    while (charsRead < lineLen) {
        memset(smallBuff, '\0', sizeof(smallBuff));
        bytesReceived = recv(socketFD, smallBuff, sizeof(smallBuff), 0);

        charsRead += bytesReceived;  

        if (bytesReceived == 0) {
            // Done reading
            break;
        }

        if (charsRead < 0) {
            error("CLIENT: Error reading from socket");
            break;
        }
        strcat(buffer, smallBuff);
    }

    buffer[strlen(buffer)-1] = '\n';
    printf("%s", buffer);

    // Close the socket
    close(socketFD); 
    return 0;
}
