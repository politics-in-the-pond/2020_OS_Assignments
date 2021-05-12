#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_BUF 1024

void receive(int check);

int main(int argc, char *argv[]){
	
	int check;
	pid_t pid;
	int pidnum=0;
	pidnum = getpid();

	pid = fork();
	pid = fork();
	//printf("%d\n",pidnum);


	if(pid == 0){
		//printf("%d\n",getpid());
		if(getpid()==(pidnum+2)){ printf("son process created.\n"); receive(0);}
		else if(getpid()==(pidnum+3)){ printf("daughter process created.\n"); receive(1);}
		
	}
	else if(pid > 0){
		wait(NULL);
		
	}

	

}

void receive(int check){
	char message[MAX_BUF];
	char buff[MAX_BUF];
	if(check == 0){	
		int file_desc_son;
		char *fifo_son = "/tmp/hw3_fifo_son";
		file_desc_son = open(fifo_son, O_RDONLY);
		while(1){
			read(file_desc_son,message,MAX_BUF);
			if(message!=buff){
				strcpy(buff, message);
				if(buff[0] == '0'){close(*fifo_son); break;}
				else printf("son process received : %s\n",buff);
			}
		}
	}
	else if(check == 1){
		int file_desc_dau;
		char *fifo_dau = "/tmp/hw3_fifo_dau";
		file_desc_dau = open(fifo_dau, O_RDONLY);
		while(1){
			read(file_desc_dau,message,MAX_BUF);
			if(message!=buff){
				strcpy(buff, message);
				if(buff[0] == '0'){close(*fifo_dau); break;}
				else printf("daughter process received : %s\n",buff);
			}
		}
	}

	return 0;
}

