//#include 
#include "filesys.h"
#define CLEN 10
#define CNUM 10
//enum ctype
char commands[CNUM][CLEN] = {
	"exit",
	"dir",
	"mkdir",
	"cd",
	"mkfile",
	"del",
	"write",
	"read",
	"user",
	"passwd"
};
int getcid(char *command){
	int i;
	if(command == NULL)
		return -1;
	for(i=0;i<CNUM;i++){
		if(strcmp(command,commands[i]) == 0)
			return i;
	}
	return -1;
}
int shell(int user_id,char *str){
	char seps[] =" \t\n\0";
	char* token,*tstr,*buf;
	short mode;
	short fd;
	int cid,size;
	token = strtok(str,seps);
	if(token == NULL)
		return 1;
	cid = getcid(token);
	switch(cid){
	case 1:
		_dir();
		break;
	case 2:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkdir�������ȷ��ʽΪmkdir dirname����������!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd�������ȷ��ʽΪcd dirname����������!\n");
			break;
		}
		chdir(token);
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkfile �������ȷ��ʽΪmkfile filename [mode]����������!\n");
			break;
		}
		tstr =token;
		mode = DEFAULTMODE;
		token = strtok(NULL,seps);
		if(token != NULL){
			sscanf(token,"%o",&mode);
		}
		mode = mode|DIFILE|0700;
		fd = creat(user_id,tstr,mode);
		if(fd == -1){
			printf("�����ļ�ʧ�ܣ�\n");
			break;
		}
		close(user_id,fd);
		break;
	case 5:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("del �������ȷ��ʽΪdel filename����������!\n");
			break;
		}
		delete(token);
		break;
	case 6:
		mode = WRITE;
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("write �������ȷ��ʽΪwrite filename bytes����������!\n");
			break;
		}
		if(token[0] == '-'){
			if(token[1] == 'a')
				mode = FAPPEND;
		}else{
			sscanf(token,"%d",&size);
		}
		fd = open(user_id,tstr,char(mode));
		if(fd<0) break;
		buf = (char*)malloc(size);
		size = write(fd,buf,size);
		printf("%d bytes have been writed in file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 7:
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("read �������ȷ��ʽΪread filename bytes����������!\n");
			break;
		}
		sscanf(token,"%d",&size);
		fd = open(user_id,tstr,READ);
		if(fd<0) break;
		buf = (char*)malloc(size+1);
		size = read(fd,buf,size);
		printf("%d bytes have been read in buf from file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 8:
		//�û�����
		token = strtok(NULL,seps);
		if (token == NULL){
			//user��ʾ�����û�
			usershow(user_id);
		} else if (!strcmp(token,"add")) {
			//user add �����û���[username]����Ϊ0��atoi�������ƣ�
			token = strtok(NULL,seps);
			if (token == NULL|| atoi(token)==0) {
				printf("user add �������ȷ��ʽΪuser add [username] (username����Ϊ0)\n��������!\n");
				break;
			}
			unsigned short username = (unsigned short) atoi(token);
			useradd(user_id,username);
		} else if (!strcmp(token, "del")) {
			//user del �����û���[username]����Ϊ0��atoi�������ƣ�
			token = strtok(NULL,seps);
			if (token == NULL|| atoi(token)==0){
				printf("user del �������ȷ��ʽΪuser del [username] (username����Ϊ0)\n��������!\n");
				break;
			}
			unsigned short username = (unsigned short) atoi(token);
			userdel(user_id,username);
		} else if (!strcmp(token,"auth")) {
			tstr = strtok(NULL,seps);
			if (tstr == NULL|| atoi(tstr)==0) {
				printf("user auth �������ȷ��ʽΪuser auth [username] [g_id]\n��������!\n");
				break;
			}
			unsigned short username = (unsigned short) atoi(tstr);
			buf = strtok(NULL,seps);
			if (buf == NULL|| atoi(buf)==0){
				printf("user auth �������ȷ��ʽΪuser auth [username] [g_id](g_id����Ϊ0)\n��������!\n");
				break;
			}
			unsigned short auth = (unsigned short) atoi(buf);
			userauth(user_id,username,auth);
		}
		else
			printf("user �������ȷ��ʽΪ\n1.\tuser\n2.\tuser [add/del] [username]\n3.\tuser auth [username] [g_id]\n��������!\n");
		break;
	case 9:
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("passwd �������ȷ��ʽΪpasswd [your_new_passwd]����������!\n");
			break;
		}
		passwd(user_id,token);
		break;
	case 0:
		return 0;
	default:
		printf("����:û������%s��\n",token);
		break;
	}
	return 1;
}