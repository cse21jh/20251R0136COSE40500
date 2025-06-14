/* 2024 Fall COSE341 Operating System */
/* Project 1 */
/* 2021320051 Yoon JiHyeon */

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define size 100

int stack[size];
int top = -1;
int value = 0;

SYSCALL_DEFINE1(os2024_push, int, a){
	int i, j, k = 0;

	if(top >= 99) //stack is full
	{
		printk("Full Stack Error\n");
		return -1;
	}

	if(top == -1) //first element
	{
		stack[++top] = a;
	}
	else
	{
		for(i = 0; i <= top; i++) //check redundancy
		{
			if(stack[i] == a) //already exists
			{
				printk("[System Call] os2024_push(); ----\n");
				printk("Stack Top -----------------------\n");
				for(j = top; j >= 0; j--) printk("%d\n", stack[j]);
				printk("Stack Bottom --------------------\n");
				return a;
			}
		}
		stack[++top] = a;
	}
	printk("[System Call] os2024_push(); ----\n");
	printk("Stack Top -----------------------\n");
	for(k = top; k >= 0; k--) printk("%d\n", stack[k]);
	printk("Stack Bottom --------------------\n");

	return a;
}

SYSCALL_DEFINE0(os2024_pop) {
	int i = 0;

	if(top == -1) //stack is empty
	{
		printk("Empty Stack Error\n");
		return -1;
	}

	value = stack[top];
	top = top - 1;

	printk("[System Call] os2024_pop(); -----\n");
	printk("Stack Top -----------------------\n");
	for(i = top; i >= 0; i--) printk("%d\n", stack[i]);
	printk("Stack Bottom --------------------\n");

	return value;
}
