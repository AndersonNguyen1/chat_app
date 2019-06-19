/* ClockClient.c: simple interactive TCP/IP client for ClockServer
 * Author: Rainer Doemer, 2/16/15 (updated 2/20/17)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include "account.h"
#include "functions.h"
#include "client.h"

/* #define DEBUG */	/* be verbose */

/*** global variables ****************************************************/

const char *Program	/* program name for descriptive diagnostics */
	= NULL;

/*** global functions ****************************************************/

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg)
{
	fputs(Program, stderr);
	fputs(": ", stderr);
	perror(ErrorMsg);
	fputs(Program, stderr);
	fputs(": Exiting!\n", stderr);
	/*exit(20);*/
} /* end of FatalError */

int main(int argc, char *argv[])
{
	int l, n, logged_in = 0, connected = 0;
	int SocketFD,	/* socket file descriptor */
	PortNo;		/* port number */
	struct sockaddr_in
	ServerAddress;	/* server address we connect with */
	struct hostent
	*Server;	/* server host */
	char SendBuf[256];	/* message buffer for sending a message */
	char RecvBuf[256];	/* message buffer for receiving a response */
	char username[50];
	/*char friends[1000];*/

	Program = argv[0];	/* publish program name (for diagnostics) */
#ifdef DEBUG
	printf("%s: Starting...\n", argv[0]);
#endif
	/*if (argc < 3)
	{   fprintf(stderr, "Usage: %s hostname port\n", Program);
	exit(10);
	}
	Server = gethostbyname(argv[1]);
	if (Server == NULL)
	{   fprintf(stderr, "%s: no such host named '%s'\n", Program, argv[1]);
	    exit(10);
	}
	PortNo = atoi(argv[2]);
	if (PortNo <= 2000)
	{   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
	    exit(10);
	}*/

/*
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PortNo);
	ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];
*/
	do{
	SocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(SocketFD<0){
		FatalError( "socket creation failed");
	}
#ifdef BEBUG
	printf("%s: Preparing the server address...\n", argv[0]);
#endif
	/*Default portNo and hostname*/
	if(!connected) {
		if(argc > 1) {
			PortNo = atoi(argv[1]);
		}
		else {
			PortNo = 5000;
		}
	}
	/*default PortNo*/
        Server = gethostbyname("localhost");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PortNo);
	memcpy(&ServerAddress.sin_addr.s_addr,
		Server->h_addr_list[0], Server->h_length);
	/*printf("%s: Connecting to the server...\n", argv[0]);
        if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
                sizeof(ServerAddress)) < 0)
        {
                FatalError( "connecting to server failed");
        }*/


	int i;
	for(i = 0; i < 3; i++) {
		printf("%s: Connecting to the server...at %d\n", argv[0], PortNo);
		if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(ServerAddress)) >= 0) {
			connected = 1;
			break;
		}
		else {
			PortNo++;
			ServerAddress.sin_port = htons(PortNo);
			FatalError( "connecting to server failed");
		}
	}


	printf("%s: Enter a command to send to the clock server:\n"
		"	     'TIME' to obtain the current time,\n"
		"	     'SHUTDOWN' to terminate the server,\n"
		"	     'CREATE' to create an account,\n"
		"	     'LOGIN' to login to an existing account,\n"
		"            'ADD' to add a new contact to your account,\n"
		"            'MSG to talk to someone,\n"
		"            'ACCESS' to access your contacts\n"
		"            'HINT' if you forgot your password\n"
		"	     or 'bye' to quit this client\n"
		"command: ", argv[0]);
	fgets(SendBuf, sizeof(SendBuf), stdin);
	/*add an end to string*/
	    l = strlen(SendBuf);
	    if (SendBuf[l-1] == '\n')
	    {   SendBuf[--l] = 0;
	    }
	if (0 == strcmp("bye", SendBuf))
	{   break;
	}
	if (l){
	 /*Add ocrrect identifiers to sendbuf*/
		if(0 == strcmp("TIME", SendBuf)){
		strncpy(SendBuf, "TM", sizeof(SendBuf)-1); 
		}
		else if(0 == strcmp("SHUTDOWN", SendBuf)) {
		strncpy(SendBuf, "SH", sizeof(SendBuf)-1);
		}
		else if(0 == strcmp("CREATE", SendBuf)) {
		    if(!logged_in){
		    strncpy(SendBuf, ask_account(SendBuf, 0), sizeof(SendBuf)-1);
        	    }
        	    else {
        	    strncpy(SendBuf, "You are already logged in", sizeof(SendBuf)-1);
        	    }
	        }
		else if(0 == strcmp("LOGIN", SendBuf)) {
			if(!logged_in) {
			strncpy(SendBuf, ask_account(SendBuf, 1), sizeof(SendBuf)-1);
			}
			else {
			strncpy(SendBuf, "You are already logged in", sizeof(SendBuf)-1);
			}
		}
		else if(0 == strcmp("ADD", SendBuf)) {
	            if(logged_in) {
	            strncpy(SendBuf, ask_friend(SendBuf), sizeof(SendBuf)-1);
	            strcat(SendBuf, "$");
	            strcat(SendBuf, username);
	            }
	            else{
	            strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
	            }
	        }
		#ifndef SK
		else if(0 == strcmp("MSG", SendBuf)){
		if(logged_in){	
			strncpy(SendBuf, "MSG", sizeof(SendBuf)-1);
			strcat(SendBuf,ask_msg());
		}
		else{
			strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
		}
		}
		#endif
		else if(0 == strcmp("ACCESS", SendBuf)){
                        if(logged_in) {
				strncpy(client_friends, access_friends(username, client_friends), sizeof(client_friends)-1);
                                strncpy(SendBuf, "Accessing", sizeof(SendBuf)-1);
                        }
                        else{
                                strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
                        }
                }
		else if(0 == strcmp("HINT", SendBuf)){
			if(!logged_in) {
				hint();
				strncpy(SendBuf, "Gave hint", sizeof(SendBuf)-1);
			}
			else {
				strncpy(SendBuf, "You are already logged in", sizeof(SendBuf)-1);
			}
		}

		/*add an end to string*/
	        l = strlen(SendBuf);
	        if (SendBuf[l-1] == '\n')
	        {   SendBuf[--l] = 0;
	        }

/*		SocketFD = socket(AF_INET, SOCK_STREAM, 0);
*/
#ifdef DEBUG
		printf("SOCKETFD CLIENT is %d\n",SocketFD);
		printf("BUFFER IS %s",SendBuf);
#endif
/*		if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
			sizeof(ServerAddress)) < 0)
		{   	printf("THERE IS An ERROR!");
			FatalError("connecting to server failed");
		}
*/
		
		printf("%s: Sending message '%s'...\n", Program, SendBuf);
#ifdef DEBUG
		printf("SOCKETFD: %d\n",SocketFD);
		printf("SENDBUF: %s\n",SendBuf);
#endif	
		n = write(SocketFD, SendBuf, l);
		if (n < 0)
		{   FatalError("writing to socket failed");
		}
#ifdef DEBUG
		printf("%s: Waiting for response...\n", Program);
#endif
		n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
		if (n < 0) 
		{   FatalError("reading from socket failed");
		}
		RecvBuf[n] = 0;

		/*If response was a successful login, let the client know it is logged in and can access the account in question*/
		if(0 == strncmp("Login was successful", RecvBuf, 20)) {
                        int length;
                        char *s;
                        logged_in = 1;

                        s = strstr(RecvBuf, "%");
                        s++;
                        length = s - (RecvBuf + 20) - 1;
                        strncpy(username, (RecvBuf + 20), length);
                        username[length] = 0;
                        strncpy(client_friends, "", 999);
                        strcpy(client_friends, s);
                        client_friends[strlen(client_friends)] = 0;
                }
                else if(strncmp("MSG", RecvBuf,3)){
                        printf("%s\n",RecvBuf);
                }

		printf("%s: Received response: %s\n", Program, RecvBuf);

#ifdef DEBUG
		printf("%s: Closing the connection...\n", Program);
#endif

	}
	}
while(0 != strcmp("SH", SendBuf));
	printf("%s: Exiting...\n", Program);
	close(SocketFD);
	return 0;
}

/* EOF ClockClient.c */
