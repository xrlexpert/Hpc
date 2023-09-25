#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>
#include"common.h"
int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    struct timeval start,finish;
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        gettimeofday(&start, NULL);
        sleep(2);
        //Spin(2);
        gettimeofday(&finish, NULL);
         double duration = ((double)(finish.tv_sec-start.tv_sec)*1000000 + (double)(finish.tv_usec-		start.tv_usec)) / 1000000;
    printf("child:%lf\n",duration);
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
	       rc, (int) getpid());
	 gettimeofday(&start, NULL);      
	 sleep(5);
	 //Spin(5);
	 gettimeofday(&finish, NULL);
	 double duration = ((double)(finish.tv_sec-start.tv_sec)*1000000 + (double)(finish.tv_usec-		start.tv_usec)) / 1000000;
    printf("father:%lf\n",duration);
    }

    return 0;
}