#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

	int a, push, pop = 0;

	for(int i = 0; i < 3; i++){
		a = rand()%100;
		push = syscall(335, a);
		printf("Push: %d\n", push);
	}

	for(int i = 0; i < 3; i++){
		pop = syscall(336);
		printf("Pop: %d\n", pop);
	}

	return 0;
}
