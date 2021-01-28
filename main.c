#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

#include<linux/sched.h>
#include<linux/kernel.h>

#define KEY_NUM		9527
#define MEM_SIZE	64

void *func1(void* data){
	system("./fxmark_jw/bin/run-fxmark.py");
	/*while(1){
	  printf("func1()\n");
	  sleep(1);
	  }*/
}
void *func2(void* data){
	system("./pcm_jw/pcm-memory.x");
	/*while(1){
	  printf("func2()\n");
	  sleep(1);
	  }*/
}
int main()
{
	pthread_t t1, t2;
	int tid, status;
	char p1[] = "thread_1";
	char p2[] = "thread_2";
	char pm[] = "thread_main";

	float readBW, writeBW;

	int   shm_id;
	void *shm_addr;

	if ( -1 == ( shm_id = shmget( (key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666)))
	{
		printf( "shared memory creation fail\n");
		return -1;
	}

	//thread_1 for func1 (fxmark)
	tid = pthread_create(&t1, NULL, func1, (void*)p1);
	if(tid<0){
		printf("thread_1 creation fail");
		return -1;
	}

	//thread_2 for func2 (pcm profiling tool)
	tid = pthread_create(&t2, NULL, func2, (void*)p2);
	if(tid<0){
		printf("thread_2 creation fail");
		return -1;
	}

	while(1){
		if ( ( void *)-1 == ( shm_addr = shmat( shm_id, ( void *)0, 0)))
		{
			printf( "shared memory attach fail\n");
			return -1;
		}
		else
		{
			printf( "shared memory attach success!\n");
		}

		void *tmp = shm_addr;
		readBW = atof((char*)shm_addr);
		shm_addr += sizeof(float);
		writeBW = atof((char*)shm_addr);
		shm_addr = tmp;

		printf("readBW : %f\n", readBW);
		printf("writeBW : %f\n", writeBW);

		shm_addr = tmp;

		if ( -1 == shmdt(shm_addr))
		{
			printf( "shared memory detach fail\n");
			return -1;
		}
		else
		{
			printf( "shared memory detach success\n");
		}

		sleep(1);

	}
	pthread_join(t1, (void**)&status);
	pthread_join(t2, (void**)&status);

	return 0;
}
