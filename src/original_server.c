
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include "account.h"
#include "functions.h"
/* #define DEBUG */	/* be verbose */

/*** type definitions ****************************************************/

typedef void (*ClientHandler)(int DataSocketFD);
typedef void (*TimeoutHandler)(void);

typedef struct client{
	char name[50];
	int SocketFD;
} CLIENT;

CLIENT clients[20];
/*** global variables ****************************************************/

const char *Program	/* program name for descriptive diagnostics */
	= NULL;
int Shutdown		/* keep running until Shutdown == 1 */
	= 0;
char ClockBuffer[26]	/* current time in printable format */
	= "";
static int counter = 0;
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

int MakeServerSocket(		/* create a socket on this server */
	uint16_t PortNo)
{
	int ServSocketFD;
	struct sockaddr_in ServSocketName;

	/* create the socket */
	ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
	if (ServSocketFD < 0)
	{   FatalError("service socket creation failed");
	return -1;
	}
	/* bind the socket to this server */
	ServSocketName.sin_family = AF_INET;
	ServSocketName.sin_port = htons(PortNo);
	ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(ServSocketFD, (struct sockaddr*)&ServSocketName,
		sizeof(ServSocketName)) < 0)
	{   FatalError("binding the server to a socket failed");
	return -1;
	}
	/* start listening to this socket */
	if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
	{   FatalError("listening on socket failed");
	return -1;
	}
	return ServSocketFD;
} /* end of MakeServerSocket */

void PrintCurrentTime(void)	/*  print/update the current real time */
{
	time_t CurrentTime; /* seconds since 1970 (see 'man 2 time') */
	char   *TimeString;	/* printable time string (see 'man ctime') */
	char   Wheel,
	   *WheelChars = "|/-\\";
	static int WheelIndex = 0;

	CurrentTime = time(NULL);	/* get current real time (in seconds) */
	TimeString = ctime(&CurrentTime);	/* convert to printable format */
	strncpy(ClockBuffer, TimeString, 25);
	ClockBuffer[24] = 0;	/* remove unwanted '/n' at the end */
	WheelIndex = (WheelIndex+1) % 4;
	Wheel = WheelChars[WheelIndex];
	printf("\rClock: %s %c",	/* print from beginning of current line */
	ClockBuffer, Wheel);	/* print time plus a rotating wheel */
	fflush(stdout);
} /* end of PrintCurrentTime */

void ProcessRequest(		/* process a time request by a client */
	int DataSocketFD)
{
	int  l, n,i=0;
	char RecvBuf[256];	/*recceving*/
	char SendBuf[256];	/* message buffer for sending a response */
	char MSGBuf[512];

	n = read(DataSocketFD, RecvBuf, sizeof(RecvBuf)-1);

	if (n < 0) 
	{   FatalError("reading from data socket failed");
	}
	RecvBuf[n] = 0;
#ifdef DEBUG
	printf("%s: Received message: %s\n", Program, RecvBuf);
#endif
	if (0 == strncmp(RecvBuf, "TM", 2))
	{   strncpy(SendBuf, "OK TIME: ", sizeof(SendBuf)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
	strncat(SendBuf, ClockBuffer, sizeof(SendBuf)-1-strlen(SendBuf));
	}
	else if (0 == strncmp(RecvBuf, "SH", 2))
	{   Shutdown = 1;
	strncpy(SendBuf, "OK SHUTDOWN", sizeof(SendBuf)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
	}
	else if(NULL != strstr(RecvBuf, "CR")) {
	if(create_account(RecvBuf)){
	strncpy(SendBuf, "An account file has been created.", sizeof(SendBuf)-1);
	}
	else {
	strncpy(SendBuf, "Account creation has failed, try another username", sizeof(SendBuf)-1);
	}
	SendBuf[sizeof(SendBuf)-1] = 0;
	}
	else if(NULL != strstr(RecvBuf, "LG")) {
	strncpy(SendBuf, login_account(RecvBuf), sizeof(SendBuf)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
		if(0==(strncmp(SendBuf,"Login was successful",20))){
			char *first=strstr(SendBuf,"Login");
			first +=20;
			char *second=strstr(SendBuf,"%");
			int length = second - first;
			strncpy(clients[counter].name,first,length);
		
			clients[counter].SocketFD = DataSocketFD;
			counter++;
#ifdef DEBUG
			printf("Name from LOGIN is: %s, SocketFD LOGIN is %d, counter is %d\n", clients[counter-1].name,clients[counter-1].SocketFD,counter-1);
#endif 
		}
	}
	else if(0 == strncmp(RecvBuf, "AD", 2)) {
	    if(add_friend(RecvBuf)) {
	            strncpy(SendBuf, "You have a new friend", sizeof(SendBuf)-1);
	    }
	    else {
	            strncpy(SendBuf, "You were unable to make a new friend", sizeof(SendBuf)-1);
	    }
	    SendBuf[sizeof(SendBuf)-1] = 0;
	}
	else if(0 == strncmp(RecvBuf, "MSG", 3)){
#ifdef DEBUG
#endif
	char *first, *second, username[30];
	int length;
	strncpy(SendBuf,"You sent a message: ", sizeof(SendBuf)-1);
	first = strstr(RecvBuf,"MSG");
	first += 3;
	second = strstr(RecvBuf,"#");
	length = second-first;
	strcpy(MSGBuf,second);
	strncpy(username,first,length);
	strcat(SendBuf,MSGBuf);
	SendBuf[sizeof(SendBuf)-1]=0;
#ifdef DEBUG
	printf("\n User NAME IS : %s", username);
#endif
	for(i=0;i<20;i++){
#ifdef DEBUG
		printf("SAVED NAME is %s, SOCKETFD is %d\n",clients[i].name,clients[i].SocketFD);
#endif
		if(strcmp(clients[i].name,username)==0){
			l = strlen(RecvBuf);
			n=write(clients[i].SocketFD,RecvBuf,l);
#ifdef DEBUG
			printf("\nClients Name Written %s, SocketFD written %d\n",clients[i].name,clients[i].SocketFD);
#endif
		}
	}
	memset(username, 0, sizeof(username));
	}/*ifend*/
	else
	{   strncpy(SendBuf, "Unknown Command: ", sizeof(SendBuf)-1);
	SendBuf[sizeof(SendBuf)-1] = 0;
	strncat(SendBuf, RecvBuf, sizeof(SendBuf)-1-strlen(SendBuf));
	}
	l = strlen(SendBuf);
#ifdef DEBUG
	printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif
/*	n = write(DataSocketFD, SendBuf, l); */

	n = write(DataSocketFD, SendBuf,l);
  
	if (n < 0)
	{   FatalError("writing to data socket failed");
	}
} /* end of ProcessRequest */

void ServerMainLoop(		/* simple server main loop */
	int ServSocketFD,		/* server socket to wait on */
	ClientHandler HandleClient,	/* client handler to call */
	TimeoutHandler HandleTimeout,	/* timeout handler to call */
	int Timeout
/*	CLIENT *clients	*/	)			/* timeout in micro seconds */
{
	int DataSocketFD;	/* socket for a new client */
	socklen_t ClientLen;
	struct sockaddr_in
	ClientAddress;	/* client address we connect with */
	fd_set ActiveFDs;	/* socket file descriptors to select from */
	fd_set ReadFDs;	/* socket file descriptors ready to read from */
	struct timeval TimeVal;
	int res, i;

	FD_ZERO(&ActiveFDs);		/* set of active sockets */
	FD_SET(ServSocketFD, &ActiveFDs);	/* server socket is active */
	while(!Shutdown)
	{   ReadFDs = ActiveFDs;
	TimeVal.tv_sec  = Timeout / 1000000;	/* seconds */
	TimeVal.tv_usec = Timeout % 1000000;	/* microseconds */
	/* block until input arrives on active sockets or until timeout */
	res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);/*TimeVal*/
	if (res < 0)
	{   FatalError("wait for input or timeout (select) failed");
	}
	if (res == 0)	/* timeout occurred */
	{
#ifdef DEBUG
/*		printf("%s: Handling timeout...\n", Program);
*/
#endif
		HandleTimeout();
	}
	else		/* some FDs have data ready to read */
	{   for(i=0; i<FD_SETSIZE; i++)
		{   if (FD_ISSET(i, &ReadFDs))
		{   if (i == ServSocketFD)
			{	/* connection request on server socket */
#ifdef DEBUG
			printf("%s: Accepting new client %d...\n", Program, i);
#endif
			ClientLen = sizeof(ClientAddress);
	/*		clients.SocketFD[i] = DataSocketFD;*/
			DataSocketFD = accept(ServSocketFD,
				(struct sockaddr*)&ClientAddress, &ClientLen);
#ifdef DEBUG
			printf("DATASOCKETDF IS %d\n",DataSocketFD);
#endif
			if (DataSocketFD < 0)
			{   FatalError("data socket creation (accept) failed");
			}
#ifdef DEBUG
			printf("%s: Client %d connected from %s:%hu.\n",
				Program, i,
				inet_ntoa(ClientAddress.sin_addr),
				ntohs(ClientAddress.sin_port));
#endif
			FD_SET(DataSocketFD, &ActiveFDs);
			}
			else
			{   /* active communication with a client */
#ifdef DEBUG
			printf("%s: Dealing with client %d...\n", Program, i);
#endif
			HandleClient(i);
#ifdef DEBUG
			printf("%s: Closing client %d connection.\n", Program, i);
#endif
			close(i);
			FD_CLR(i, &ActiveFDs);
			}
		}
		}
	}
	}
} /* end of ServerMainLoop */

/*** main function *******************************************************/

int main(int argc, char *argv[])
{
	int ServSocketFD;	/* socket file descriptor for service */
	int PortNo = 5000 ;		/* port number */
 /*   CLIENT clients;*/
	Program = argv[0];	/* publish program name (for diagnostics) */
#ifdef DEBUG
	printf("%s: Starting...\n", Program);
#endif
	/*if (argc < 2)
	{   fprintf(stderr, "Usage: %s port\n", Program);
	exit(10);
	}
	PortNo = atoi(argv[1]);	 get the port number 
	*/

/*Search for a suitable port number to use*/
	if(argc > 1) {
		PortNo = atoi(argv[1]);  /*get the port number*/
	}
	else {
		PortNo = 5000;  /*If a port No wasn't supplied, default is 5000*/
	}
	int i=0; 
   	for( i = 0; i < 3; i++) {
		ServSocketFD = MakeServerSocket(PortNo);

		/*If a suitable socket was created, break*/
		if(ServSocketFD >= 0) {
			break;
		}
		/*Otherwise, search through other port numbers*/
		else {
			PortNo++;
		}
	}

	if (PortNo <= 2000)
	{   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
	    exit(10);
	}
#ifdef DEBUG
	printf("%s: Creating the server socket...\n", Program);
#endif
	/*ServSocketFD = MakeServerSocket(PortNo);*/
#ifdef DEBUG
	printf("The ServerSocketFd is %d\n",ServSocketFD);
#endif
	printf("%s: Providing current time at port %d...\n", Program, PortNo);
	ServerMainLoop(ServSocketFD, ProcessRequest,
			PrintCurrentTime, 250000);
	printf("\n%s: Shutting down.\n", Program);
	close(ServSocketFD);
	return 0;
}

/* EOF ClockServer.c */
