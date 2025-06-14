#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int count = 0; //counting 120 times for running this process
	int pid = 0;
	int pmem, vmem, smem = 0;
	char buffer[256]; //store procfs path

	if (argc != 2){
		printf("Invalid input argument......\n");
		return 0;
	}

	pid = atoi(argv[1]);

	size_t size = sysconf(_SC_PAGESIZE); //page size in byte
	size_t psize = size / 1024; //page size into KB

	while(count < 120){
		//printf("Count: %d\n", count);
		printf("Memory check for PID %d:\n", pid);

		snprintf(buffer, sizeof(buffer), "/proc/%s/statm", argv[1]);
		FILE* f = fopen(buffer, "r");

		if(!f){
			printf("Invalid proc file......\n");
			return 0;
		}

		fscanf(f, "%d %d %d", &vmem, &pmem, &smem);

		fclose(f);

		pmem *= psize;
		vmem *= psize;
		smem *= psize;

		printf("Physical Memory current: %d KB\n", pmem);
		printf("Virtual Memory current: %d KB\n", vmem);
		printf("Shared Memory current: %d KB\n", smem);
		printf("\n");
		count++;
		usleep(500000); //wait for 0.5s
	}
	//printf("Final count: %d\n", count);
	return 0;
}
