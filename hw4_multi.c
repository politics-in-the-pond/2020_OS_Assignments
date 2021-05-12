#include <stdio.h> //201935151 Hwang JeongYeon
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define true 1
#define false 0

pthread_mutex_t mutex;
int result=0;

typedef struct rangearg
{
   int a;
   int b;
}ra;



void *cntPrime(void* argu)
{
	ra* point = (ra*)argu;
	int range1=point->a;
	int range2=point->b;
	int i;
	int temprst=0;
	int cnt;

	for (int num=range1; num<=range2; num++) {
		cnt = 0;
		for (i=1; i*i<=num; i++) {
			if (num%i == 0){  cnt++;}
		}
		if (i*i == num) cnt++;

		if (cnt == 1){
			pthread_mutex_lock(&mutex);
 			result++;
 			pthread_mutex_unlock(&mutex);
 			temprst++; }

	}

	printf("thread for range( %d ~ %d )",range1,range2);
	printf("  the number of prime numbers of this range : %d\n",temprst);
}

int main(int argc, char* argv[])
{
	pthread_t thread[10];
	int cnt = 0;
	int range = atoi(argv[1]);
	ra rangenum[10];
	pthread_mutex_init(&mutex, NULL);

	struct timespec start, finish;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	int num = range/10;

	rangenum[0].a = 2; //argument initalize
	rangenum[0].b = num-1;
	for(int i =1;i<9;i++){
		rangenum[i].a = num*i;
		rangenum[i].b = num*(i+1)-1;
	}
	rangenum[9].a = num*9;
	rangenum[9].b = range;

	pthread_create(&thread[0],NULL,cntPrime,(void* )&rangenum[0]); //create thread
	for(int i = 1;i<9;i++){
		pthread_create(&thread[i],NULL,cntPrime,(void*)&rangenum[i]);
	}
	pthread_create(&thread[9],NULL,cntPrime,(void*)&rangenum[9]);

	for(int i = 0; i<10;i++){ //wait for thread
		pthread_join(thread[i],NULL);
	}

   	clock_gettime(CLOCK_MONOTONIC, &finish);
  	elapsed = (finish.tv_sec - start.tv_sec);
  	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

	printf("elapsed time: %f sec \n", elapsed);
	printf("The number of prime numbers between 1~%d is %d\n", range, result);

	return 0;

}
