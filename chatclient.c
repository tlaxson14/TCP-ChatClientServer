/************************************************************************************************
* Program Title: 
	chatclient.c 
* Author: 
	Travis Laxson
* Description: 
 	This C program serves as the chat client component for the TCP chat application. 
	The program will accept a host and port number command-line argument to establish a 
	connection with the server host. The user will first be prompted to enter a screen 
	name handle before initiating a connection with the server. An initial message is then 
	sent to the server host to establish a connection upon which it will receive an initial 
	server response. Once a connection with the server has been established, the chat client 
	may exchange messages with the chat server.The client will close the TCP chat session
	with a simple client  message "\quit".
* Sources:
	1) OSU Professor Brewster - CS344 Block 4 - Lecture 4.2 slides 9-14, 16-18, 21, 22
	2) strcat() man - https://linux.die.net/man/3/strcat
	3) Beej Guide to Networking - Section 6.2: A Simple Stream Client; Pgs 33-35
	4) atoi() man - https://linux.die.net/man/3/atoi
* Created: 
	30 April 2019
* Last Modified:
	05 May 2019 
* Course:
	CS372 - Introduction to Networking
* Source code:
	https://github.com/tlaxson14/TCPChat/blob/master/chatclient.c
************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

/* Global vars */
int MAX_MSG_LEN = 500;
int MAX_NAME_LEN = 10;

/* Function declarations */
void error(const char *msg);

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char packetBuffer[511];
	char msgBuffer[500];
	char screenName[11];
	
	/* Check that command line has valid args */
	if (argc != 3){ 
		fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); 
		exit(0); 
	}

	/* Clear memory of buffers */
	memset(screenName, '\0', sizeof(screenName)); // Clear out the buffer array
	memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer again for reuse
	memset(msgBuffer, '\0', sizeof(msgBuffer)); // Clear out the buffer again for reuse

	/* Get the client handle screen name */
	printf("Enter screen name: ");
	fgets(screenName, sizeof(screenName)-1, stdin);

	/* Remove the trailing newline \n character added by fgets */
	screenName[strcspn(screenName, "\n")] = '\0';

	/* Get the port number from the command line */	
	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	
	/* Set up the server address struct */
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
	
	/* Error check that server host info setup correctly */
	if(serverHostInfo == NULL){ 
		fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
		exit(0); 
	}
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	/* Build up initial client request message to send to host */
	strcat(packetBuffer, screenName);
	strcat(packetBuffer, " connected at ");
	strcat(packetBuffer, argv[1]);
	strcat(packetBuffer, ":");
	strcat(packetBuffer, argv[2]);
	
	/* Set up the TCP socket */
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFD < 0){
		 error("CLIENT: ERROR opening socket");
	}

	/* Connect socket to server address */
	if(connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
		error("CLIENT: ERROR connecting");
	}	

	/* Write and send initial packet message */
	charsWritten = send(socketFD, packetBuffer, strlen(packetBuffer), 0);
	
	/* Error check for writing to socket and check that all data sent */
	if(charsWritten < 0){
		 error("CLIENT: ERROR writing to socket");
	}
	if(charsWritten < strlen(packetBuffer)){
		printf("CLIENT: WARNING: Not all data written to socket!\n");	
	}

	/* Get response message from server host */
	/* Clear out buffer to receive response msg */
	memset(packetBuffer, '\0', sizeof(packetBuffer));
	
	/* Read data from the socket - leaves \0 at the end */
	charsRead = recv(socketFD, packetBuffer, sizeof(packetBuffer) - 1, 0);
	if(charsRead < 0){
		error("CLIENT: ERROR reading from socket");
	}

	/* Print received server message */
	printf("%s\n", packetBuffer);
	
	/* Continue getting chat message input from user */
	while(true){
		/* Print screen name handle at start of message */
		printf("%s> ", screenName);
		
		/* Get user input, truncate to size of message buffer-1, leaving the \0 */
		fgets(msgBuffer, sizeof(msgBuffer) - 1, stdin);
		
		/* Remove the trailing new line \n added by fgets */
		msgBuffer[strcspn(msgBuffer, "\n")] = '\0';
		
		/* Check if chat user enters 'quit' to close connection to server */
		if(strcmp(msgBuffer, "\\quit") == 0){
			memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer to reuse
		
			/* Build the packet buffer to be sent to server */
			strcat(packetBuffer, screenName);
			strcat(packetBuffer, "> ");
			strcat(packetBuffer, msgBuffer);

			/* Send message to server */
			charsWritten = send(socketFD, packetBuffer, strlen(packetBuffer), 0);
			
			/* Error check for writing to socket and check that all data sent */
			if(charsWritten < 0){
				error("CLIENT: ERROR writing to socket");
			}
			if(charsWritten < strlen(packetBuffer)){ 
				printf("CLIENT: WARNING: Not all data written to socket!\n");
			}

			/* Close socket */
			close(socketFD);
			break;
		}
		else{
			memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear packet buffer to reuse
			
			/* Build packet message to send to server */
			strcat(packetBuffer, screenName);
			strcat(packetBuffer, "> ");
			strcat(packetBuffer, msgBuffer);
			
			/* Send message to server */
			charsWritten = send(socketFD, packetBuffer, strlen(packetBuffer), 0); // Write to the server
			
			/* Error check for writing to socket and check that all data sent */
			if(charsWritten < 0){
				error("CLIENT: ERROR writing to socket");
			}
			if(charsWritten < strlen(packetBuffer)){
				printf("CLIENT: WARNING: Not all data written to socket!\n");
			}

			// Get return message from server
			memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear packet buffer to reuse
			
			/* Read data from the socket - leaves \0 at the end */
			charsRead = recv(socketFD, packetBuffer, sizeof(packetBuffer) - 1, 0); 
			if(charsRead < 0){
				error("CLIENT: ERROR reading from socket");
			}
			
			/* Print received server message */
			printf("%s\n", packetBuffer);
		}
	}
	printf("Client ended connection\n");
	return 0;
}


/**************************** error function **************************** 
* Name: error
* Description: 
	Takes a character string argument
 	and prints the error message.
* Pre-conditions: 
	Must be passed a valid character string message 
* Post-conditions:
	Prints detailed error message
* Sources:
	1) Professor Brewster - CS344 Block 4 Lectures - Network clients   	
	2) perror man - http://man7.org/linux/man-pages/man3/perror.3.html
* Created:
	30 April 2019
* Last modified:
 	05 May 2019
*************************************************************************/
void error(const char *msg)
{ 
	perror(msg); 
	exit(0); 
}

