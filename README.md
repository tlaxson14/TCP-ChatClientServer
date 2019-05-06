# TCP-Chat
## Description
Implementation of client-server network application using socket programming and TCP protocol to exchange chat messages betwen two hosts (i.e. client and server)
## Instructions  
   Instructions for program execution involve using two terminal windows logged into different OSU Flip servers. The chat server must be executed with a command-line-user-specified port number before running the chat client. The user will be prompted to enter a server screen name up to 10 characters max.  

   The client will use the server host address and port number of the chat server to establish a TCP connection. The user will be prompted to enter a client screen name immediately upon execution, and an initial set of request-response messages are sent to initalize the client-server connection. Once a connection has been established, the two hosts can send and receive chat messages using their user-specified screen names.  

   Either the client or server may close the connection using the command '\quit', upon which the server host continues listening for further incoming connections from the client.  

The following list of instructions illustrates the execution process:
  1. Open two terminal windows, hereafter referred as T1 and T2. 
  2. In each terminal, login to a different Flip server (e.g. T1 logged into Flip1 and T2 logged into Flip2).  
   **Note:** Make sure each terminal logs into a different Flip server.
  3. Navigate to the working directory in each terminal and ensure the following files exist:
     + ```Makefile```
     + ```chatserve.py```
     + ```chatclient.c```
  4. Select one terminal (T1) and begin running the chat server by executing the following command:  
     ```./chatserve.py <PORT NUM>```  
     
     **Note:** The ```PORT NUM``` command argument represents the specified port number to begin listening for incoming connections. For simplicity, please select a port number between 10000 - 65535.  
     _Sample command_: ```./chatserv.py 12345```  
   5. From the other terminal (T2) execute the following command to compile the ```chatclient.c``` source file into an executable: ```make```  
   6. From the same terminal T2 the ```chatclient``` is executed using the command structure:  
   ```./chatclient <SERVER HOST> <PORT NUM>```   
   + The ```SERVER HOST``` command-line argument represents the hostname (e.g. flip1.engr.oregonstate.edu) and ```PORT NUM``` represents the specified port number the chat server is listening for incoming connections.  
   + Both ```PORT NUM``` command-line arguments in each program execution must be identical to establish a connection.**  
     _Sample command_: ```./chatclient flip1.engr.oregonstate.edu 12345```  

