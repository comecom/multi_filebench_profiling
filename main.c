#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#include<linux/sched.h>
#include<linux/kernel.h>

//#include<linux-nova/arch/x86/include/asm/current.h>
//#include"pcm_jw/pcm-memory.cpp"

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

	pthread_join(t1, (void**)&status);
	pthread_join(t2, (void**)&status);

	return 0;
}
