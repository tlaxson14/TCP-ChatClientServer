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
* Created: 
	30 April 2019
* Last Modified:
	05 May 2019 
* Course:
	CS372 - Introduction to Networking
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

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char packetBuffer[510];
	char msgBuffer[500];
	char screenName[10];
	if (argc != 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

	/* Clear memory of buffers */
	memset(screenName, '\0', sizeof(screenName)); // Clear out the buffer array
	memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer again for reuse
	memset(msgBuffer, '\0', sizeof(msgBuffer)); // Clear out the buffer again for reuse

	/* Get the client handle screen name */
	printf("Enter screen name: ");
	fgets(screenName, sizeof(screenName)-1, stdin);
	screenName[strcspn(screenName, "\n")] = '\0'; // Remove the trailing \n that fgets adds

	/* Get the port number from the command line */	
	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	
	/* Set up the server address struct */
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	/* Set up initial client packet message to send to host */
	strcat(packetBuffer, "Incoming connection from ");
	strcat(packetBuffer, screenName);
	strcat(packetBuffer, " at ");
	strcat(packetBuffer, argv[1]);
	strcat(packetBuffer, ":");
	strcat(packetBuffer, argv[2]);
	/*printf("%s\n", packetBuffer);
	*/
	
	/* Set up the TCP socket */
	socketFD = socket(AF_INET, SOCK_STREAM, 0); /* Create socket */
	if (socketFD < 0) error("CLIENT: ERROR opening socket");

	/* Connect to server */
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");
	
	/* Send initial packet message */
	charsWritten = send(socketFD, packetBuffer, strlen(packetBuffer), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(packetBuffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");	

	// Get return message from server
	memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer again for reuse
	charsRead = recv(socketFD, packetBuffer, sizeof(packetBuffer) - 1, 0); // Read data from the socket, leaving \0 at end
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	/* Print received server message */
	printf("%s\n", packetBuffer);
	
	/* Get input message from user */
	while(true){
		printf("%s> ", screenName);
		//memset(msgBuffer, '\0', sizeof(msgBuffer)); // Clear out the buffer again for reuse
		fgets(msgBuffer, sizeof(msgBuffer) - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0
		msgBuffer[strcspn(msgBuffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds
		if(strcmp(msgBuffer, "\\quit") == 0){
			memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer again for reuse
			strcat(packetBuffer, screenName);
			strcat(packetBuffer, "> ");
			strcat(packetBuffer, msgBuffer);
			/* Send message to server */
			charsWritten = send(socketFD, packetBuffer, strlen(packetBuffer), 0); // Write to the server
			if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten < strlen(packetBuffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");
			close(socketFD);
			break;
		}
		else{
			memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer again for reuse
			strcat(packetBuffer, screenName);
			strcat(packetBuffer, "> ");
			strcat(packetBuffer, msgBuffer);
			/*printf("Packet buffer: %s\n", packetBuffer);
			*/
			// Send message to server
			charsWritten = send(socketFD, packetBuffer, strlen(packetBuffer), 0); // Write to the server
			if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
			if (charsWritten < strlen(packetBuffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

			// Get return message from server
			memset(packetBuffer, '\0', sizeof(packetBuffer)); // Clear out the buffer again for reuse
			charsRead = recv(socketFD, packetBuffer, sizeof(packetBuffer) - 1, 0); // Read data from the socket, leaving \0 at end
			if (charsRead < 0) error("CLIENT: ERROR reading from socket");
			/* Print received server message */
			printf("%s\n", packetBuffer);
		}
	}
	printf("Client ended connection\n");
	return 0;
}
