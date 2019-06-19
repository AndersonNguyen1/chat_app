#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "functions.h"

#ifndef ACCOUNT_H
#define ACCOUNT_H

char *ask_account(char *str, int i);

int create_account(char *str);

<<<<<<< account.h
char *ask_delete(char *str, int i);

int delete_account(char *str);

=======
int delete_account(char *str);

>>>>>>> 1.9
char *login_account(char *str);

char *ask_friend(char *str);

char *ask_block(char *str);

char *ask_unblock(char *str);

int add_friend(char *str);

char *ask_unfriend(char *str);

int remove_friend(char *str);

char *access_friends(char *str, char *friends);

<<<<<<< account.h
int block_user(char *str);

int unblock_user(char *str);

=======
void hint();

>>>>>>> 1.9
/*typedef char string[50] STRING;

typedef struct user {
	STRING friend_list[50];
}*/

#endif
