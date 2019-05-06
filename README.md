# TCP-Chat
## Description
Implementation of client-server network application using socket programming and TCP protocol to exchange chat messages betwen two hosts (i.e. client and server)
## Instructions  
   Instructions for program execution involve using two terminal windows logged into different OSU Flip servers. The chat server must be executed with a command-line-user-specified port number before running the chat client. The user will be prompted to enter a server screen name up to 10 characters max.  

   The client will use the server host address and port number of the chat server to establish a TCP connection. The user will be prompted to enter a client screen name immediately upon execution, and an initial set of request-response messages are sent to initalize the client-server connection. Once a connection has been established, the two hosts can send and receive chat messages using their user-specified screen names.  

   Either the client or server may close the connection using the command '\quit', upon which the server host continues listening for further incoming connections from the client.  

## Execution Steps
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
   5. When prompted, enter a short, 10 character-max chat server screen name from the command line.              
      _Terminal output_: ```Enter screen name: ```  
      _Sample server input_: ```Enter screen name: Serv3r```  
   6. From the other terminal (T2) execute the following command to compile the ```chatclient.c``` source file into an executable named ```chatclient```:  
      ```make```  
   7. From the same terminal T2 the ```chatclient``` is executed using the command structure:  
   ```./chatclient <SERVER HOST> <PORT NUM>```   
      + The ```SERVER HOST``` command-line argument represents the hostname (e.g. flip1.engr.oregonstate.edu) and ```PORT NUM``` represents the specified port number the chat server is listening for incoming connections.  
      + Both ```PORT NUM``` command-line arguments in each program execution must be identical.  
 _Sample command matching Step 4_: ```./chatclient flip1.engr.oregonstate.edu 12345```  
   8. (_Same as Step 5_) When prompted, enter a short, 10 character-max chat client screen name from the command line.  
      _Terminal output_: ```Enter screen name: ```  
      _Sample client input_: ```Enter screen name: Cli3nt```  
   9. If a successful connection is established, chat client and server exchange the following initial messages:    
      ```Chat server (T1): "-- Welcome to the TCP Chat Server! --"```  
      ```Chat client (T2): <Screen Name> connected at <Host name>:<PORT NUM>"```  
      _Sample chat server output_: ```Cli3nt connected at flip1.engr.oregonstate.edu:12345```  
   10. The first message is sent from the chat client, but all subsequent requests and responses are sent from both chat client and server processes successively. The chat history and screen names are also shown with each message.  
      _Sample client message_: 
          ```Cli3nt> Hey there friend!```  
      _Sample server client message + server input message_:   
          ```Cli3nt> Hey there friend!```  
          ```Serv3r> What's up my guy?```  
   11. To close the connection from the client or server, send the following message:  
      ```\quit```  
   12. To establish a new chat client connection (in T2) with the chat server process (in T1) start again from Step 7  .
   13. To kill the server process enter ```ctrl + c``` in T1.  
   14. Run ```make clean``` to delete the ```chatclient.o``` and ```chatclient``` files in the working directory.
