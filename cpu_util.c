#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

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
int main(void){
	
	while(1){
		get_cpu_util();

		sleep(1);
	}

	return 0;
}
