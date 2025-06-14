#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define fcfs 337
#define sjf 338

int main(int argc, char** argv){
        int jobTime;
        int delayTime;
        char name[4];
        int wait = 0;
        int response = -1;

        if (argc < 4){
                printf("\nInsufficient Arguments..\n");
                return 1;
        }

        /*      first argument: job time (second)
                second argument: delay time before execution (second)
                third argument: process name
        */

        jobTime = atoi(argv[1]);
        delayTime = atoi(argv[2]);
        strcpy(name,argv[3]);

        // wait for 'delayTime' seconds before execution
        sleep(delayTime);
        printf("\nProcess %s : I will use CPU by %ds.\n", name, jobTime);
        jobTime *= 10; // execute system call in every 0.1 second

        // continue requesting the system call as long as the jobTime remains
        while(jobTime){
                // if request is rejected, increase wait time
                int i = syscall(fcfs, name, jobTime);
		//int i = syscall(sjf, name, jobTime);
                if(i == 0) { //accept
                        if(response == -1) response = (wait+5)/10; // if first $
                        jobTime--;
                }
                else if(i == 1){
			wait++;
		}
		else {
			printf("Program Died!\n");
			return -1;
		}
		usleep(100000);
        }

        syscall(fcfs, name, 0);
        //syscall(sjf, name, 0);
        printf("\nProcess %s : Finish! My response time is %ds and my total wait time is %ds. ", name, response, (wait+5)/10);
        return 0;
}
