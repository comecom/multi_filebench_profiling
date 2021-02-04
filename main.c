#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<linux/sched.h>
#include<linux/kernel.h>

#define KEY_NUM		9527
#define MEM_SIZE	64
#define num_node 1
#define core_per_node 20

int past[121][4]={0, }, present[121][4]={0, };
int diff[121][4], total[121];

void get_cpu_util(){
    int n_core = num_node * core_per_node;
    int n4, n5, n6, n7, n8, n9;

    FILE* statFile = fopen("/proc/stat", "r");
    char cpuId[6]={0};

    memset(total, 0, sizeof(total));

    for(int i=0;i<=n_core;i++){
        fscanf(statFile, "%s %d %d %d %d %d %d %d %d %d %d",
                cpuId, &present[i][0],  &present[i][1], &present[i][2], &present[i][3],
                &n4, &n5, &n6, &n7, &n8, &n9);
        //printf("%s\n", cpuId);

        for(int j=0;j<4;j++){
            diff[i][j] = present[i][j] - past[i][j];
            total[i] += diff[i][j];
        }
        //printf("diff : %d, total : %d\n", diff[i][3], total[i]);
        printf("cpu%d usage : %f%%\n", i, 100.0*(1.0-(diff[i][3]/(double)total[i])));

        memcpy(past[i], present[i], sizeof(int)*4);
    }

    //utilization per NODE
    double t, idle;

    for(int i=0;i<n_core;i++){
        if(i%core_per_node==0){
            t = 0;
            idle = 0;
        }
        t+=(double)total[i+1];
        idle+=(double)diff[i+1][3];

        if(i%core_per_node==core_per_node-1){
            //printf("%lf %lf\n", idle, t);
            printf("NODE %d utilization : %lf%%\n", i/core_per_node, 100.0*(1.0-idle/t));
        }
    }
    fclose(statFile);

    return ;
}

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

		//cpu utilization
		get_cpu_util();
		sleep(1);

	}
	pthread_join(t1, (void**)&status);
	pthread_join(t2, (void**)&status);

	return 0;
}
