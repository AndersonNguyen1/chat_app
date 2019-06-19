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
	exit(20);
} /* end of FatalError */

int main(int argc, char *argv[])
{
	int l, n, logged_in;
	int SocketFD,	/* socket file descriptor */
	PortNo;		/* port number */
	struct sockaddr_in
	ServerAddress;	/* server address we connect with */
	struct hostent
	*Server;	/* server host */
	char SendBuf[256];	/* message buffer for sending a message */
	char RecvBuf[256];	/* message buffer for receiving a response */
	char username[50];
	char friends[1000];

	Program = argv[0];	/* publish program name (for diagnostics) */
#ifdef DEBUG
	printf("%s: Starting...\n", argv[0]);
#endif
	if (argc < 3)
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
	}

	/*PortNo = 5001;*/
	/*Server = gethostbyname("localhost");*/
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
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PortNo);
	memcpy(&ServerAddress.sin_addr.s_addr,
		Server->h_addr_list[0], Server->h_length);
	printf("%s: Connecting to the server...\n", argv[0]);
	if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(ServerAddress)) < 0)
	{
		FatalError( "connecting to server failed");
	}
/*	fd_set READFD;
	fd_set WRITEFD;
	fd_set EXECEPTION;
	int sel;

while(1){

	FD_ZERO(&READFD);
	FD_ZERO(&WRITEFD);
	FD_ZERO(&EXECEPTION);

	FD_SET(STDIN_FILENO,&READFD);
	FD_SET(SocketFD,&READFD);	
	FD_SET(SocketFD,&WRITEFD);
	FD_SET(SocketFD,&EXECEPTION);

	sel = select(&READFD,&WRITEFD,&EXECEPTION, NULL,NULL);
}
*/
	printf("%s: Enter a command to send to the clock server:\n"
		"	     'TIME' to obtain the current time,\n"
		"	     'SHUTDOWN' to terminate the server,\n"
		"	     'CREATE' to create an account,\n"
		"	     'DELETE' to delete an account,\n"
		"	     'LOGIN' to login to an existing account,\n"
		"            'ADD' to add a new contact to your account,\n"
		"            'REMOVE' to remove a contact from your account,\n"
<<<<<<< client.c
		"	     'BLOCK' to block a user from your account,\n"
		"	     'UNBLOCK' to unclock a user from your account,\n"
=======
		"            'DELETE' to delete your account,\n"
>>>>>>> 1.10
		"            'MSG to talk to someone,\n"
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
	            strncpy(SendBuf, ask_account(SendBuf, 0), sizeof(SendBuf)-1);
	        }
		else if(0 == strcmp("DELETE", SendBuf)) {
			if(logged_in) {
				strncpy(SendBuf, ask_account(SendBuf, 0), sizeof(SendBuf)-1); 
			}
			else {
				strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
			}
		}
		else if(0 == strcmp("LOGIN", SendBuf)) {
		strncpy(SendBuf, ask_account(SendBuf, 1), sizeof(SendBuf)-1);
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
                else if(0 == strcmp("REMOVE", SendBuf)) {
                    if(logged_in) {
                    strncpy(SendBuf, ask_unfriend(SendBuf), sizeof(SendBuf)-1);
                    strcat(SendBuf, "$");
                    strcat(SendBuf, username);
                    }
                    else{
                    strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
                    }
                }
<<<<<<< client.c
		else if(0 == strcmp("BLOCK", SendBuf)) {
			if(logged_in) {
			strncpy(SendBuf, ask_block(SendBuf), sizeof(SendBuf)-1);
			strcat(SendBuf, "$");
			strcat(SendBuf, username);
			}
			else{
			strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
			}
		}
                else if(0 == strcmp("UNBLOCK", SendBuf)) {
                        if(logged_in) {
                        strncpy(SendBuf, ask_block(SendBuf), sizeof(SendBuf)-1);
                        strcat(SendBuf, "$");
                        strcat(SendBuf, username);
                        }
                        else{
                        strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
                        }
		}
=======
                else if(0 == strcmp("DELETE", SendBuf)) {
                    if(logged_in) {
                    strncpy(SendBuf, "DL", sizeof(SendBuf)-1);
                    strcat(SendBuf, "$");
                    strcat(SendBuf, username);
                    }
                    else{
                    strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
                    }
                }

>>>>>>> 1.10
		else if(0 == strcmp("MSG", SendBuf)){
		if(logged_in){	
			strncpy(SendBuf, "MSG", sizeof(SendBuf)-1);
			strcat(SendBuf,ask_msg());
		}
		else{
			strncpy(SendBuf, "Not logged in yet", sizeof(SendBuf)-1);
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

	/*Search for a suitable port Number*/
/*	int i=0;
	for( i = 0; i < 3; i++) {
		printf("%s: Connecting to the server at port %d...\n",Program, PortNo);
		If a suitable port number is found, stop searching
	            if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
	            sizeof(ServerAddress)) >= 0){
			break;
	        	}
		If a suitable port number isnt found yet, keep looking
	        	else {
	            	PortNo++;
	            	ServerAddress.sin_port = htons(PortNo);
	        	}
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
	            username[strlen(username)] = 0;
		strcpy(friends, s);
		friends[strlen(friends)] = 0;
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
while(0 != strcmp("SHUTDOWN", SendBuf));
	printf("%s: Exiting...\n", Program);
	close(SocketFD);
	return 0;
}

/* EOF ClockClient.c */
