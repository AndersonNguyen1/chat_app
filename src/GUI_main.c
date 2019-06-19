#include "GUI.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int x;
	x = LoginScreen(argc, argv);
	printf("%s\t%s\n",gui_get_username,gui_get_password);
	x = RegisterScreen(argc, argv);
	printf("%s\t%s\n",gui_get_username,gui_get_password);
	x = ChatAppHome(argc, argv, "HELLO!\n");
	printf("message is: %s\n", gui_send_message);


	return 0;
}

