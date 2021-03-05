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

void *pcm(void* data){
	system("./pcm_jw/pcm-memory.x -csv > BW.log");
}
void *cpu_util(void* data){
	system("./cpu_util -csv");
}
void *numa(void* data){
	system("./pcm_jw/pcm-numa.x -csv > numa.log");
}
void *app1(void* data){
	//system("numactl -N 0 -m 0 ./filebench/filebench -f filebench/mywebserver0.f");
	system("numactl -N 0 -m 0 ./filebench/filebench -f filebench/mywebserver0.f");
}
void *app2(void* data){
	//system("numactl -N 1 -m 1 ./filebench/filebench -f filebench/mywebserver1.f");
	system("numactl -N 1 -m 0 ./filebench/filebench -f filebench/mywebserver1.f");
}
void *app3(void* data){
	//system("numactl -N 2 -m 2 ./filebench/filebench -f filebench/mywebserver2.f");
	system("numactl -N 1 -m 1 ./filebench/filebench -f filebench/mywebserver2.f");
}
void *app4(void* data){
	//system("numactl -N 3 -m 3  ./filebench/filebench -f filebench/mywebserver3.f");
	system("numactl -N 1 -m 1 ./filebench/filebench -f filebench/mywebserver3.f");
}

int main()
{
	pthread_t t_pcm, t_util, t_numa;
	pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
	pthread_t t11, t12, t13, t14, t15, t16, t17, t18, t19, t20;

	int tid, status;
	char p1[] = "thread_1";
	char p2[] = "thread_2";
	char p3[] = "thread_3";
	char p4[] = "thread_4";
	char p5[] = "thread_5";
	char p6[] = "thread_6";
	char p7[] = "thread_7";
	char p8[] = "thread_8";
	char p9[] = "thread_9";
	char p10[] = "thread_10";
	char p11[] = "thread_11";
	char p12[] = "thread_12";
	char p13[] = "thread_13";
	char p14[] = "thread_14";
	char p15[] = "thread_15";
	char p16[] = "thread_16";
	char p17[] = "thread_17";
	char p18[] = "thread_18";
	char p19[] = "thread_19";
	char p20[] = "thread_20";

	char p_pcm[] = "thread_pcm";
	char p_util[] = "thread_util";
	char p_numa[] = "thread_numa";

	//thread for pcm profiling tool
	tid = pthread_create(&t_pcm, NULL, pcm, (void*)p_pcm);
	if(tid<0){
		printf("thread_pcm creation fail");
		return -1;
	}

	//thread for cpu utilization tool
	tid = pthread_create(&t_util, NULL, cpu_util, (void*)p_util);
	if(tid<0){
		printf("thread_util creation fail");
		return -1;
	}

	//thread for numa access pattern
	tid = pthread_create(&t_numa, NULL, numa, (void*)p_numa);
	if(tid<0){
		printf("thread_numa creation fail");
		return -1;
	}

	//thread for application 1
	/*tid = pthread_create(&t1, NULL, app1, (void*)p1);
	  if(tid<0){
	  printf("thread creation fail");
	  return -1;
	  }*/

	//thread for application 2
	/*tid = pthread_create(&t2, NULL, app2, (void*)p2);
	  if(tid<0){
	  printf("thread creation fail");
	  return -1;
	  }*/

	//thread for application 3
	/*tid = pthread_create(&t3, NULL, app3, (void*)p3);
	  if(tid<0){
	  printf("thread creation fail");
	  }

	//thread for application 4
	tid = pthread_create(&t4, NULL, app4, (void*)p4);
	if(tid<0){
	printf("thread creation fail");
	return -1;
	}*/

	pthread_join(t_pcm, (void**)&status);
	pthread_join(t_util, (void**)&status);
	pthread_join(t_numa, (void**)&status);

	//pthread_join(t1, (void**)&status);
	//pthread_join(t2, (void**)&status);
	//pthread_join(t3, (void**)&status);
	//pthread_join(t4, (void**)&status);

	return 0;
}
