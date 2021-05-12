#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "schedule.h"

int fcfs(int tick); //function declaration
int sjf(int tick);
int srtf(int tick);
int number(int num);


typedef struct PCB{
	int pid;
	int burst;
	int begintick;
	int bursttick;
	int first;
	int finish;
}PCB;

int procnum=1;

PCB proc[256];

int sch=0;

int stat=0;

int shortest;

void read_proc_list(const char* file_name){
	//printf("init\n");
	FILE *fp;
	int line=0;
	char data[81];
	fp = fopen(file_name, "r");
	while(procnum>=line){
		char* temp;
		fgets(data, 80, fp);
		if(line==0) {
			procnum=atoi(data);
			//printf("%d\n",procnum);
		}else {
			temp=strtok(data," ");
			proc[line-1].pid = atoi(temp);
			//printf("%s\n",temp);
			temp=strtok(NULL," ");
			proc[line-1].begintick = atoi(temp);
			//printf("%s\n",temp);
			temp=strtok(NULL," ");
			proc[line-1].bursttick = atoi(temp);
			//printf("%s\n",temp);
			proc[line-1].burst=0;
			proc[line-1].finish=-1;
		}
		line++;
	}

	fclose(fp);
//printf("init\n");
}

void set_schedule(int method){
	//printf("init\n");
	sch=method;
}

int do_schedule(int tick){
	//printf("%d\n",sch);
	int result;
	if(sch==1) result = fcfs(tick);
	else if(sch==2) result = sjf(tick);
	else if(sch==3) result = srtf(tick);
	return result;
}

int fcfs(int tick){
	if(stat>procnum) return -1;
	if(stat==0) stat=1;
	int index=0;
	int result;
	//printf("%d\n",stat);
	index = stat-1;
	if(proc[index].burst==0){
		proc[index].first = tick;
	}

	if(proc[index].burst<proc[index].bursttick){
		proc[index].burst++;
		//printf("%d\n",proc[index].burst);
	}else {
		proc[index].finish = tick;
		stat++;
		stat = fcfs(tick);
	}

	return stat;
}

int sjf(int tick){
	if(stat==-1) return -1;
	if(stat==0) stat=1;
	int index=0;
	index=stat-1;
	int check=0;
	shortest=2147483647;
	if(proc[index].burst==0){
		proc[index].first = tick;
	}

	if(proc[index].burst<proc[index].bursttick){
		proc[index].burst++;
		//printf("%d\n",proc[index].burst);
	}else {

		for(int i=0;i<procnum;i++){
			if(proc[i].begintick<tick&&proc[i].finish==-1){
				if(proc[i].bursttick<shortest){
					shortest=proc[i].bursttick;
					stat=i+1;
				}
			}
			if(proc[i].finish!=-1){
				check++;
			}
		}
		proc[index].finish = tick;
		if(check==procnum) stat=-1;
		stat = sjf(tick);
	}
	return stat;
}

int srtf(int tick){
	if(stat==-1) return -1;
	if(stat==0) stat=1;
	int index=0;
	int check=0;
	index=stat-1;
	if(proc[index].burst==0){
		proc[index].first = tick;
	}
	shortest=2147483647;
	for(int i=0;i<procnum;i++){
		if(proc[i].begintick<=tick&&proc[i].finish==-1){
			if(proc[i].bursttick-proc[i].burst<shortest){
				shortest=proc[i].bursttick-proc[i].burst;
				stat=i+1;
				//printf("%d %d\n",shortest,stat);
			}
		}
		if(proc[i].finish!=-1){
			check++;
		}
		if(check==procnum) stat=-1;
	}
	if(index==stat-1){
		proc[index].burst++;
	}else{
		stat = srtf(tick);
	}

	if(proc[index].burst==proc[index].bursttick){
		proc[index].finish=tick;
	}
	return stat;
}

void print_performance(){
	//printf("init\n");
	int i=0;
	int rsum=0;
	int wsum=0;
	int tsum=0;
	printf("======================================================================================\n");
	printf("  PID       begin     finish   Turn around time     Waiting time      Response time\n");
	printf("======================================================================================\n");
	while(i!=procnum){
		for(int j = 0;j<4-number(proc[i].pid);j++){
			printf(" ");
		}
		printf("%d",proc[i].pid);

		for(int j = 0;j<11-number(proc[i].begintick);j++){
			printf(" ");
		}
		printf("%d",proc[i].begintick);

		for(int j = 0;j<11-number(proc[i].finish);j++){
			printf(" ");
		}
		printf("%d",proc[i].finish);

		for(int j = 0;j<13-number(proc[i].finish-proc[i].begintick);j++){ //Turn around time
			printf(" ");
		}
		printf("%d",proc[i].finish-proc[i].begintick);
		tsum += proc[i].finish-proc[i].begintick;

		for(int j = 0;j<19-number(proc[i].finish-proc[i].begintick-proc[i].bursttick);j++){ //Waiting time
			printf(" ");
		}
		printf("%d",proc[i].finish -proc[i].begintick -proc[i].bursttick);
		wsum += proc[i].finish -proc[i].begintick -proc[i].bursttick;

		for(int j = 0;j<20-number(proc[i].first-proc[i].begintick);j++){ //Response time
			printf(" ");
		}
		printf("%d",proc[i].first-proc[i].begintick);
		rsum += proc[i].first-proc[i].begintick;

		printf("\n");
		i++;
	}
	printf("--------------------------------------------------------------------------------------\n");
	//average
	printf(" average:");
	for(int j = 0;j<30-number(tsum/procnum);j++){
		printf(" ");
	}
	printf("%.2f",1.0*tsum/procnum);
	for(int j = 0;j<16-number(wsum/procnum);j++){
		printf(" ");
	}
	printf("%.2f",1.0*wsum/procnum);
	for(int j = 0;j<17-number(rsum/procnum);j++){
		printf(" ");
	}
	printf("%.2f\n",1.0*rsum/procnum);
	printf("======================================================================================\n"); //end
}

int number(int num) {
	int i = 1, num_len = 0;
	while( num >= i ) {
		i *= 10;
		num_len++;
	}

	if(num==0||num==1) num_len=1;

	return num_len;
}
