
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>

#define IDLE 0 //check idle state through pid

typedef struct job {
        pid_t pid;
        int time;
} job_t;

int front = 0, rear = 0;
job_t waitQueue[10] = { {0, 0}, };
pid_t now = 0;

bool ku_is_empty(void){
        return (front == rear);
}

void ku_push(job_t j){
        waitQueue[rear] = j;
        rear = (rear+1) % 10;
}

job_t ku_pop(void){
        if(ku_is_empty()){
                job_t j = {0, 0};
                 return j;
        }

        job_t r = waitQueue[front];
        front = (front+1) % 10;
        return r;
}

bool ku_is_new_id(pid_t pid){
        if (ku_is_empty()) return true;
        int i;
        for(i = front; i <= rear; i = (i+1) % 10){
                if(waitQueue[i].pid == pid){
		 	return false;
		}
        }
        return true;
}

void ku_queue_sort(void) {
	int i = 0, j = 0;
	job_t temp;

	// bubble sort for sorting based on job time
	for (i = front; i != rear; i = (i+1) % 10) {
		for (j = (i+1) % 10; j != rear % 10; j = (j+1) % 10) {
			if (waitQueue[i].time > waitQueue[j].time) {
				// Swap
				temp = waitQueue[i];
				waitQueue[i] = waitQueue[j];
				waitQueue[j] = temp;
			}
		}
	}
}


SYSCALL_DEFINE2(os2024_ku_cpu_fcfs, char*, name, int, jobTime){
        // store pid of current process as pid_t type
        job_t newJob = {current->pid, jobTime};

        //register the process if virtual CPU is idle
        if(now == IDLE) {
		now = newJob.pid;
	}

        // If the process that sent the request is currently using virtual CPU
        if (now == newJob.pid) {
                // If the job has finished
                if (jobTime == 0) {
                        printk("Process Finished: %s\n", name);
                        // if queue is empty, virtual CPU becomes idle
                        if (ku_is_empty()) now = IDLE;
                        // if not, get next process from queue
                        else now = (ku_pop()).pid;
                }
                else printk("Working: %s\n", name);
                //request accepted
                return 0;
        }
        else {
                // if the request is not from currently handling process
                if (ku_is_new_id(newJob.pid)) ku_push(newJob);
                printk("Working Denied:%s \n", name);
        	// request rejected
       		return 1;
	}
}

SYSCALL_DEFINE2(os2024_ku_cpu_sjf, char*, name, int, jobTime){
	// store pid of current process as pid_t type
        job_t newJob = {current->pid, jobTime};

        //register the process if virtual CPU is idle
        if(now == IDLE) {
                now = newJob.pid;
        }

        // If the process that sent the request is currently using virtual CPU
        if (now == newJob.pid) {
                // If the job has finished
                if (jobTime == 0) {
                        printk("Process Finished: %s\n", name);
                        // if queue is empty, virtual CPU becomes idle
                        if (ku_is_empty()) now = IDLE;
                        // if not, get next process from queue
                        else now = (ku_pop()).pid;
                }
                else printk("Working: %s\n", name);
                //request accepted
                return 0;
        }
        else {
                // if the request is not from currently handling process
                if (ku_is_new_id(newJob.pid)) {
			ku_push(newJob);
			ku_queue_sort(); //if new element comes in, need sort
		}
                printk("Working Denied:%s \n", name);
                // request rejected
                return 1;
        }
}
