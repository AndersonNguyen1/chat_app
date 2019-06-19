#ifndef GUI_H
#define GUI_H

char gui_get_username[512];
char gui_get_password[512];
char gui_send_message[512];

int LoginScreen(int argc, char *argv[]);

int RegisterScreen(int argc, char *argv[]);

int ChatAppHome(int argc, char *argv[], char recieve_message[512]);

#endif
	
