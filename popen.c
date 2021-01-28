#include <stdio.h>

#define MAXLINE 256

int main()
{
    FILE *fp;
    int state;

    char buff[MAXLINE];
    fp = popen("ls -al", "r");
    if (fp == NULL)
    {
        perror("erro : ");
        return -1;
    }

    while(fgets(buff, MAXLINE, fp) != NULL)
    {
        printf("jw : %s", buff);
    }

    state = pclose(fp);
    printf("state is %d\n", state);
	return 0;
}
