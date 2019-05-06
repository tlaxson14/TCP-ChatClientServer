# TCP-Chat
## Description
Implementation of client-server network application using socket programming and TCP protocol to exchange chat messages betwen two hosts (i.e. client and server)
## Instructions
Instructions for program execution involve using two terminal windows logged into different OSU Flip servers (e.g. Flip1 and Flip2). The chat server must be executed with a command-line-user-specified port number before running the chat client. The user will be prompted to enter a server screen name up to 10 characters max. The client will use the server host address and port number of the chat server to establish a TCP connection. The user will be prompted to enter a client screen name immediately upon execution, and an initial set of request-response messages are sent to initalize the client-server connection. Once a connection has been established, the two hosts can send and receive chat messages using their user-specified screen names. Either the client or server may close the connection using the command '\quit', upon which the server host continues listening for further incoming connections from the client. 
The following list of instructions illustrates the process:
  1. Open two terminal windows.
  2. In each terminal, login to a different Flip server.
   **Note:** Make sure each terminal window logs into a different Flip server.
  3. Ensure that both of the following files exist within the chosen working directory:
   + chatserve.py
   + chatclient.c
  4. Begin running the chat server by using the command:
   ```chatserve.py <PORT NUM>```
   where PORT NUM represents the specified port number to begin listening for incoming connections

