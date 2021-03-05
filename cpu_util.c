#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<linux/sched.h>
#include<linux/kernel.h>

#define num_node 2
#define core_per_node 28

int past[121][4]={0, }, present[121][4]={0, };
int diff[121][4], total[121];

void get_cpu_util_csv(){
        int n_core = num_node * core_per_node;
        int n4, n5, n6, n7, n8, n9;

        FILE* statFile = fopen("/proc/stat", "r");
	FILE* resFile = fopen("util.log", "a");
        char cpuId[6]={0};

        memset(total, 0, sizeof(total));

        for(int i=0;i<=n_core;i++){
                fscanf(statFile, "%s %d %d %d %d %d %d %d %d %d %d",
                                cpuId, &present[i][0],  &present[i][1], &present[i][2], &present[i][3],
                                &n4, &n5, &n6, &n7, &n8, &n9);

                for(int j=0;j<4;j++){
                        diff[i][j] = present[i][j] - past[i][j];
                        total[i] += diff[i][j];
                }
                memcpy(past[i], present[i], sizeof(int)*4);
        }
        
	//	utilization per NODE
        double t, idle;

	//	real cpu utilization value
        for(int i=0;i<n_core;i++){
                if(i%core_per_node==0){
                        t = 0;
                        idle = 0;
                }
                t+=(double)total[i+1];
                idle+=(double)diff[i+1][3];

                if(i%core_per_node==core_per_node-1){
                        //printf("%lf %lf\n", idle, t);
                        printf("%lf\t", 100.0*(1.0-idle/t));
                       	fprintf(resFile, "%lf\t", 100.0*(1.0-idle/t));
                }
        }
	printf("\n");
	fprintf(resFile, "\n");
        
	fclose(statFile);
	fclose(resFile);

        return ;
}

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
		//printf("cpu%d usage : %f%%\n", i, 100.0*(1.0-(diff[i][3]/(double)total[i])));

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

int main(int argc, char *argv[])
{
	int time = 0;
	if(argc == 1){
		while(time<60){
			get_cpu_util();
			time++;
			sleep(1);
		}
	}
	else if(argc == 2 && strcmp(argv[1], "-csv")==0){
		FILE* resFile = fopen("util.log", "w");

		for(int i=0;i<num_node;i++){
			fprintf(resFile, "NODE #%d\t", i);
                	printf("NODE #%d\t\t", i);
		}
		fprintf(resFile, "\n");
                printf("\n");
		fclose(resFile);

		while(time<63){
			get_cpu_util_csv();
			time++;
			sleep(1);
		}
	}
	return 0;
}

