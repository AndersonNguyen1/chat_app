# Makefile: makefile in the homedirectory, don't edit

CC = gcc
DEBUG = -g -DDEBUG

#FLAGS = -Wall -ansi -O2
FLAGS = -Wall -g
#FLAGS = -Wall -ansi -g -DDEBUG

all: Client Server

test: ClientDEBUG ServerDEBUG

clean:
	rm -rf Client Server ./src/ClientDEBUG ./src/ServerDEBUG
	rm ./bin/*.o

Client: client.o account.o functions.o
	$(CC) $(FLAGS) ./bin/client.o ./bin/functions.o ./bin/account.o -o ./bin/Client

Server: server.o account.o functions.o
	$(CC) $(FLAGS) ./bin/server.o ./bin/functions.o ./bin/account.o -o ./bin/Server

ServerDEBUG: server.o account.o functions.o
	$(CC) $(DEBUG) ./bin/server.o ./bin/account.o ./bin/functions.o -o ./src/ServerDEBUG

ClientDEBUG: client.o account.o functions.o
	$(CC) $(DEBUG) ./bin/client.o ./bin/account.o ./bin/functions.o -o ./src/ClientDEBUG

server.o: ./src/server.c ./src/account.h ./src/functions.h
	$(CC) $(FLAGS) -DDEBUG -c ./src/server.c -o ./bin/server.o

client.o: ./src/client.c ./src/account.h ./src/functions.h
	$(CC) $(FLAGS) -DDEBUG -c ./src/client.c -o ./bin/client.o

account.o: ./src/account.c ./src/account.h ./src/functions.h
	$(CC) $(FLAGS) -DDEBUG -c ./src/account.c -o ./bin/account.o

functions.o:./src/functions.c ./src/account.h ./src/functions.h
	$(CC) $(FLAGS) -DDEBUG -c ./src/functions.c -o ./bin/functions.o

tar:
	tar -zcvf Chat_V1.0.tar.gz README COPYRIGHT INSTALL ./bin/Client ./bin/Server ./doc/Chat_UserManual.pdf
	tar --transform='flags=r;s|READMEsrc|README|' --transform='flags=r;s|INSTALLsrc|INSTALL|' -zcvf Chat_V1.0_src.tar.gz READMEsrc COPYRIGHT INSTALLsrc Makefile ./bin ./src ./doc
