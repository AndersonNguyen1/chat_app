#include "functions.h"

char *ask_msg(char *str){
	char msg[512];
	char username[50];
	printf("Please type your message: ");
	fgets( msg,sizeof(msg),stdin);
	int l;	
        l = strlen(msg);
        if(msg[l-1]=='\n')
        {
                msg[--l] = 0;
        }

	printf("Type the username of the person you want to sent to: ");
	fgets(username,sizeof(username),stdin);
	
        l = strlen(username);
        if(username[l-1]=='\n')
        {
                username[--l] = 0;
        }

	strcat(str,username);
	strcat(str,"#");
	strcat(str,msg);
#ifdef DEBUG	
	printf("your message: %s\n",str);
#endif
	return str;

}

char *encrypt(char *str){
	int i;
	for(i=0; i<strlen(str);i++){
		str[i]-=1;
	}	
	return str;


}

char *decrypt(char *str){
	int i;
	for(i=0;i<strlen(str);i++){
		str[i]+=1;
	}
	return str;
}

int savesocketFD(int FDnum, char *username){
	FILE *f;
	f = fopen(username, "w");
	fprintf(f, "%s%d","!", FDnum);
	
	fclose(f);
	return FDnum;
}

int readsocketFD(char *username){
	int FDnum;
	FILE *f;
	char *first;
	char buff[512];
	f = fopen(username, "r");
	fgets(buff, 512,f);
	first = strstr(buff, "!");
	first++;
	FDnum = (int)*first;

	fclose(f);
	return FDnum;
}

