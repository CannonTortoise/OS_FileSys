#include <stdio.h>
#include "filesys.h"

struct hinode 	hinode[NHINO];

struct dir 		dir;
struct file 	sys_ofile[SYSOPENFILE];
struct filsys 	filsys;
struct pwd 		pwd[PWDNUM];
struct user 	user[USERNUM];
struct inode* 	cur_path_inode;
int 			user_id;
char   			disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];
char 			str[100];
FILE			*fd;//WR

int main(){
	int username;
	char password[16];
	user_id = -1;
	//WR start
	printf("Do you want to format the disk?(y or n)\n");
	if (getchar() == 'y')
	{
		format();
		printf("Format successful!\n");
	}
	//WR end
	install();
	printf("Welcome to mini filesystem!\n");
	while(user_id == -1){
		printf("Login:");
		scanf("%d",&username);
		printf("Password:");
		scanf("%s",password);
		user_id = login(username,password);
	}
	do{
		printf("> ");
		fflush(stdin);
		fgets(str,100,stdin);
	}while(shell(user_id,str));
	logout(username);
	halt();
	return 0;
}