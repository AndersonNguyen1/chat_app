# Makefile: simple clock server with corresponding client
# Rainer Doemer, 2/16/15 (updated 2/20/17)

#FLAGS	= -Wall -ansi -O2
FLAGS	= -Wall -g
#FLAGS	= -Wall -ansi -g -DDEBUG

all: Client Server GUI

clean:
	rm -rf Client Server GUI
	rm *.o

test:
	@echo "To run the examples, first start the server in one terminal,"
	@echo "then start one (or multiple) client(s) in another."
	@echo
	@echo "For example:"
	@echo "crystalcove% ./ClockServer 10000"
	@echo "crystalcove% ./ClockClient crystalcove 10000"

GUI: GUI_main.o gui.o
	gcc -Wall -g gui.o GUI_main.o -o GUI `pkg-config --cflags gtk+-2.0` \
`pkg-config --libs gtk+-2.0`

GUI_main.o: GUI_main.c GUI.h
	gcc $(FLAGS) -c GUI_main.c -o GUI_main.o

gui.o:  GUI.c GUI.h
	gcc -Wall -g -c GUI.c -o gui.o `pkg-config --cflags gtk+-2.0` \
`pkg-config --libs gtk+-2.0`


Client: client.o account.o functions.o
	gcc $(FLAGS) client.o account.o functions.o -o Client

Server: server.o account.o functions.o
	gcc $(FLAGS) server.o account.o functions.o -o Server

server.o: server.c account.h
	gcc $(FLAGS) -c server.c -o server.o

client.o: client.c client.h account.h
	gcc $(FLAGS) -c client.c -o client.o

account.o: account.c account.h
	gcc $(FLAGS) -c account.c -o account.o

functions.o: functions.c functions.h
	gcc $(FLAGS) -c functions.c -o functions.o


#Spencer Kam Unit Test
Spencer: SK_client SK_server

SK_client: SK_client.o account.o functions.o
	gcc $(FLAGS) SK_client.o account.o functions.o -o sk_client

SK_server: SK_server.o account.o functions.o
	gcc $(FLAGS) SK_server.o account.o functions.o -o sk_server

SK_client.o: original_client.c client.h account.h
	gcc $(FLAGS) -c original_client.c -o SK_client.o

SK_server.o: original_server.c account.h
	gcc $(FLAGS) -c original_server.c -o SK_server.o

# EOF
