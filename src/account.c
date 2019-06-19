#include "account.h"

char *ask_account(char *str, int i) {
	char username[50], password[50], hint[100];
	int l;
	strncpy(username, "", 49);
	strncpy(password, "", 49);
	strncpy(hint, "", 99);
	/*Ask username and password*/
	printf("Please type in username: ");
	fgets(username, sizeof(username), stdin);
	l = strlen(username);
	if(username[l-1]=='\n')
	{
		username[--l] = 0;
	}
	printf("Please type in password: ");
	fgets(password,sizeof(password), stdin);

	encrypt(password);

        l = strlen(password);
        if(password[l-1]=='\n')
        {
                password[--l] = 0;
        }

	if(i == 0){
		strcpy(str, "CR");
		/*This is the hint in case the user forgets their password*/
        	printf("In case you forget your password, please type yourself up a hint: ");
        	fgets(hint, sizeof(hint), stdin);
        	l = strlen(hint);
        	if(hint[l-1]=='\n')
        	{
                	hint[--l] = 0;
        	}

	}
	else {
		strcpy(str, "LG");
	}
	strcat(str, "*");
	strcat(str, username);
	strcat(str, "$");
	strcat(str, password);
	if(i == 0){
		strcat(str, "%");
        	strcat(str, hint);
	}

	return str;
}

int create_account(char *str) {
	FILE *f;
	char *first, *second, *third, username[50], password[50], hint[100];
	int length;

	first = strstr(str, "*");
	first++;
	second = strstr(str, "$");
	second++;
	third = strstr(str, "%");
	third++;

	length = (second - first) - 1;
	strncpy(username, first, length);
	username[length] = 0;
	/*If an account with the same username already exists, prevent account creation and return a fail*/	
	f = fopen(username, "r");
	if(f){
		fclose(f);
		return 0;
	}
	f = fopen(username, "w");
	/*Create ac account with username and store password in the account file*/
	length = (third - second) - 1; /*Copy password*/
	strncpy(password, second, length);
	password[length] = 0;
	fprintf(f, "%s\n", password);
	/*This is the hint in case the user forgets their password*/
	strncpy(hint, "", 99);     /*Copy hint*/
        strcpy(hint, third);
        fprintf(f, "%s\n", hint);

	fclose(f);
	return 1;
}
int delete_account(char *str) {
	FILE *f, *f2, *f3;
	char username[50], temp[50], temp2[50], check[50], *first, *same, *tolkien;
	int getout, stopit;
		
	first = strstr(str, "$");
	first++;
	strcpy(username, first);
	
        while (1) {
		f = fopen(username, "r");/*opens the user file*/
		fgets(temp, 50, f);/*skips the password*/
		fgets(temp, 50, f);/*skips hint*/
		fgets(temp, 50, f);/*looks at the first friend*/
		if(temp2[0] != '\0'){/*checks that we haven't gone through the first friend yet*/
			while((strstr(temp2, temp)) != NULL){
                		fgets(temp, 50, f);
			}
			fgets(temp, 50, f);
		}
		fgets(check, 50, f);
		if(feof(f)){
			stopit = 1;
		}
		fclose(f);/*closes user file*/
		strcpy(temp2, temp);/*line to check that we don't re look at first friend*/

<<<<<<< account.c
char *ask_delete(char *str, int i) {
        
	char username[50], password[50];
        int l;
        strncpy(username, "", 49);
        strncpy(password, "", 49);
        /*Ask username and password*/
        printf("Please type in username: ");
        fgets(username, sizeof(username), stdin);
        l = strlen(username);
        if(username[l-1]=='\n')
        {
                username[--l] = 0;
        }
        printf("Please type in password: ");
        fgets(password,sizeof(password), stdin);

        l = strlen(password);
        if(password[l-1]=='\n')
        {
                password[--l] = 0;
        }
	/*need to remove the name from the list but when new accounts are created, where are they stored?*/
        return str;
}

int delete_account(char *str) {
	FILE *f;
	//int ret;
	char *first, *second, word[50];
	int length;

	first = strstr(str, "*");
        first++;
        second = strstr(str, "$");
        second++;

        length = (second - first) - 1;
        strncpy(word, first, length);
        word[length] = 0;

	f = fopen(word, "r");
        if(f){
		f = fopen(word, "w");	
                fclose(f);
                return 1;
        }
	return 0;
}	
	

=======
		tolkien = strchr(temp, '\n');/*removes the \n so that we can open friend's file without error*/
		*tolkien = '\0';
		f2 = fopen(temp, "r");/*open friend's file*/
                f3 = fopen("other", "w");/*open the rewrite file*/
		fgets(check, 50, f2);/*skip first line*/
		fprintf(f3, "%s", check);
		fgets(check, 50, f2);
		same = NULL;
		/*loop to check if username is in friend's list and if it is, delete it*/
		while(!feof(f2)) {
			if(same == NULL){
				fprintf(f3, "%s", check);
			}
			same = NULL;
			fgets(check, 50, f2);
			same = strstr(check, username); /*see if user's name is in that line*/
			if(same != NULL){
				if((strlen(check)-1) != strlen(username)){
                                	same = NULL;
                        	}
			}
		}
		fclose(f3);
		fclose(f2);
		getout = remove(temp);
		if(getout == 0){
			rename("other", temp);
		}
		else{
			return 0;
		}
		if(stopit){
			break;
		}
	}
	getout = remove(username);
	if (getout == 0){
		return 1;
	}
	else{
		return 0;
	}
	
}
>>>>>>> 1.15
char *login_account(char *str) {
	FILE *f;
        char username[50], password[50], *first, *second, actual_password[50], friend[50];
        int length, password_len, done = 0, count = 0;
	strncpy(password, "", 49);
	strncpy(username, "", 49);
	strncpy(friend, "", 49);
	strncpy(actual_password, "", 49);
	/*Find the identifiers/location of the username and password*/
        first = strstr(str, "*");
        first++;
        second = strstr(str, "$");
        second++;
	/*Using the identifiers, copy the username and password onto strings*/
        length = (second - first) - 1;
        strncpy(username, first, length);
        username[length] = 0;
        strcpy(password, second);
        password_len = strlen(password);
        password[password_len] = '\n';
        password[++password_len] = 0;
	/*Default success message.  The username is added at the end*/
        strcpy(str, "Login was successful");
        strcat(str, username);
	/*Open file with username.  If file doen't exist, return unsuccessful login*/
        f = fopen(username, "r");
        if(f == NULL) {
                strcpy(str, "Login was unsuccessful");
		str[strlen(str)] = 0;
                return str;
        }
	/*Get password in file.  If passwords do not match up, return unsuccessful login*/
        fgets(actual_password, 100, f);
/*	
	decrypt(actual_password);
*/	
        actual_password[strlen(actual_password)] = 0;
        if(strcmp(actual_password, password) != 0) {
                strcpy(str, "Login was unsuccessful");
		str[strlen(str)] = 0;
		return str;
        }
	/*Add list of friends to the return string*/
	while(!done) {
		if(!count) {
			fgets(friend, 49, f);
			strcat(str, "%");
		}
		else {
			if(fgets(friend, 49, f)) {
			strcat(str, friend);
			}
			else {
			str[strlen(str)] = 0;
			break;
			}
		}
		count++;
	}

        fclose(f);

        return str;
}

char *ask_friend(char *str) {
        char friend[50];

        printf("Please enter the username of the person you would like to befriend:\n");
        fgets(friend,sizeof(friend),stdin);
 	int l;	
        l = strlen(friend);
        if(friend[l-1]=='\n')
        {
                friend[--l] = 0;
        }

        strcpy(str, "AD");
        strcat(str, "*");
        strcat(str, friend);

        return str;
}

char *ask_block(char *str) {
        char block[50];

        printf("Please enter the username of the person you would like to block:\n");
        fgets(block,sizeof(block),stdin);
        int l;
        l = strlen(block);
        if(block[l-1]=='\n')
        {
                block[--l] = 0;
        }

        strcpy(str, "BL");
        strcat(str, "*");
        strcat(str, block);

        return str;
}

char *ask_unblock(char *str) {
        char unblock[50];

        printf("Please enter the username of the person you would like to unblock:\n");
        fgets(unblock,sizeof(unblock),stdin);
        int l;
        l = strlen(unblock);
        if(unblock[l-1]=='\n')
        {
                unblock[--l] = 0;
        }

        strcpy(str, "UB");
        strcat(str, "*");
        strcat(str, unblock);

        return str;
}

int add_friend(char *str) {
        FILE *f, *f2;
        char username[50], friend[50], *first, *second;
        int length;
	strncpy(username, "", 49);
	strncpy(friend, "", 49);

        first = strstr(str, "*");
        first++;
        second = strstr(str, "$");
        second++;

        length = (second - first) - 1;
        strncpy(friend, first, length);
        friend[length] = 0;
        strcpy(username, second);
        username[strlen(username)] = 0;

	/*Determine if target friend account even exists*/
        f2 = fopen(friend, "r");
        if(f2 == NULL) {
                return 0;
        }
	fclose(f2);

	/*Add user to list of friends to target account*/
	f2 = fopen(friend, "a");
	fprintf(f2, "%s\n", username);
	fclose(f2);
	/*Add target account to user's list of friends*/
        f = fopen(username, "a");
        fprintf(f, "%s\n", friend);
        fclose(f);

        return 1;

}

char *ask_unfriend(char *str) {
        char friend[50];
	strncpy(friend, "", 49);

        printf("Please enter the username of the person you would like to unfriend:\n");
        fgets(friend,sizeof(friend),stdin);
        int l;
        l = strlen(friend);
        if(friend[l-1]=='\n')
        {
                friend[--l] = 0;
        }

        strcpy(str, "RM");
        strcat(str, "*");
        strcat(str, friend);

        return str;
}


int remove_friend(char *str) {
	FILE *f, *f2;
	char *c1a, *c1b, *c2a, *c2b;
	char username[50], friend[50], temp[50],  *first, *second, *same, *newsame;
	int length, rem; 

	first = strstr(str, "*");
        first++;
        second = strstr(str, "$");
        second++;

        length = (second - first) - 1;
        strncpy(friend, first, length);
        friend[length] = 0;
        strcpy(username, second);
        username[strlen(username)] = 0;
	same = NULL;
	
	/*Determine if the target friend account exists*/
	f2 = fopen(friend, "r");
	if(f2 == NULL) {
		return 0;
	}
	fclose(f2);
	
	/*Remove user from list of friends of the target account*/
	f2 = fopen(friend, "r");
	f = fopen("other", "w");
	fgets(temp, 50, f2);
	fprintf(f, "%s", temp);
	fgets(temp, 50, f2);
	while (!feof(f2)) {
		if (same == NULL) {/*if username, then skip line*/
			fprintf(f, "%s", temp);/*add name into other file*/
		}
		same = NULL;
		fgets(temp, 50, f2);
		same = strstr(temp, username);
		if (same != NULL){
                        if((strlen(temp)-1) != strlen(username)){
                                same = NULL;
                        }
		}
	}
	fclose(f);
	fclose(f2);
	rem = remove(friend);
	if(rem == 0){
		rename("other", friend);
	}
	else{
		return 0;
	}
	same = NULL;

	/*Remove target account from list of friends of the user*/
        f = fopen(username, "r");
	f2 = fopen("other", "w");
	fgets(temp, 50, f);
	fprintf(f2, "%s", temp);
	fgets(temp,50, f);
        while (!feof(f)) {
		if (same == NULL){
                        fprintf(f2, "%s", temp);
                }
		same = NULL;
                fgets(temp, 50, f);
                same = strstr(temp,friend);
		if (same != NULL){
                        if((strlen(temp)-1) != strlen(friend)){
                                same = NULL;
                        }
	
		}
        }
	fclose(f2);
	fclose(f);
	rem = remove(username);
        if(rem == 0){
                rename("other", username);
        }
        else{
                return 0;
        }
	return 1;
}



char *access_friends(char *str, char *friends) {
	char friend[50], username[50];
	FILE *f;
	int done = 0, count = 0;

	strncpy(username, "", 49);
	strcpy(username, str);

	f = fopen(username, "r");

	/*Add list of friends to the return string*/
        while(!done) {
                if(count < 2) {
                        fgets(friend, 49, f);
                }
                else {
                        if(fgets(friend, 49, f)) {
				/*If its the first friend*/
				if(count == 2) {
				strcpy(friends, friend);
				}
				else {
                        	strcat(friends, friend);
				}
                        }
                        else {
                        str[strlen(str)] = 0;
                        break;
                        }
                }
                count++;
        }

        fclose(f);

        return friends;
}

void hint(){
	char username[50], hint[100];
	FILE *f;
	int l;
	strncpy(username, "", 49);

	printf("Username: ");
	fgets(username, sizeof(username), stdin);
        l = strlen(username);
        if(username[l-1]=='\n')
        {
                username[--l] = 0;
        }

	f = fopen(username, "r");

	if(f == NULL) {
		printf("That account doesn't exist\n");
	}
	else {
		fgets(hint, 99, f);/*An initial gets to skip over password*/
		fgets(hint, 99, f);/*I gets to to get the ACTUAL hint*/
		printf("Your hint is: %s ", hint);
	}

} 
<<<<<<< account.c

int block_user(char *str) {
        FILE *f, *f2;
        char username[50], block[50], *first, *second;
        int length;
        strncpy(username, "", 49);
        strncpy(block, "", 49);

        first = strstr(str, "*");
        first++;
        second = strstr(str, "$");
        second++;

        length = (second - first) - 1;
        strncpy(block, first, length);
        block[length] = 0;
        strcpy(username, second);
        username[strlen(username)] = 0;

        /*Determine if target friend account even exists*/
        f2 = fopen(block, "r");
        if(f2 == NULL) {
                return 0;
        }
        fclose(f2);

        /*Add user to list of blocked users to target account*/
        f2 = fopen(block, "a");
        fprintf(f2, "%s\n", username);
        fclose(f2);
        /*Add target account to user's blocked list*/
        f = fopen(username, "a");
        fprintf(f, "%s\n", block);
        fclose(f);

        return 1;
}

int unblock_user(char *str) {
        FILE *f, *f2;
        char username[50], unblock[50], *first, *second;
        int length;
        strncpy(username, "", 49);
        strncpy(unblock, "", 49);

        first = strstr(str, "*");
        first++;
        second = strstr(str, "$");
        second++;

        length = (second - first) - 1;
        strncpy(unblock, first, length);
        unblock[length] = 0;
        strcpy(username, second);
        username[strlen(username)] = 0;

        /*Determine if target user account even exists*/
        f2 = fopen(unblock, "r");
        if(f2 == NULL) {
                return 0;
        }
        fclose(f2);

        /*remove users from list of blocked users from target account*/
        f2 = fopen(unblock, "a");      
	fprintf(f2, "%s\n", username);
        fclose(f2);
        /*remove target account from block list*/
        f = fopen(username, "a");
        fprintf(f, "%s\n", unblock);
        fclose(f);

        return 1;
}

=======
>>>>>>> 1.15
