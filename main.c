#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 10

typedef struct pt
{
	float x;
	float y;
	float a;
} Point;

typedef struct stack
{
	Point elements[CHUNK];
	int top;
} Stack;

/* ####################  Stack Functions  #################### */
Stack* newStack(void)
/*
 * Function for creating empty stack structure.
 * Returns Stack* | pointer to the new stack
*/
{
	int i;
	static Stack local_stack;
	local_stack.top = 0;

	for (i = 0; i < CHUNK; i++)
	{
		memset(&local_stack.elements[i], 0, sizeof(Point));
	}

	return &local_stack;
}

int put(Stack *ptr_stack, Point element)
/*
 * Method of adding element to stack structure.
 * *ptr_stack (required) ---> Stack* | pointer to stack structure
 * element (required) ---> Point | value to be stored
 * Returns int | 0 if succeeded, -1 if failed
*/
{
	if (ptr_stack->top >= 0 && ptr_stack->top < sizeof(ptr_stack->elements)/sizeof(ptr_stack->elements[0]))
	{
		ptr_stack->elements[ptr_stack->top] = element;
		ptr_stack->top++;

		return 0;
	}

	printf("Unable to add to stack\n");
	return -1;
}

Point peek(Stack *ptr_stack, int pos)
/*
 * Method of looking at a given position of the stack strucuture.
 * *ptr_stack (required) ---> Stack* | pointer to stack structure
 * pos (optional) ---> int | position where to peek, starting from 0 (top)
 * Returns Point | element of the stack
*/
{
	Point tmp;
	memset(&tmp, 0, sizeof(Point));

	if (pos < 0){printf("[ ERROR ] Position to peek has to be a positive integer\n");}
	if (ptr_stack->top <= 0){printf("Stack is empty\n");}

	if (ptr_stack->top > 0)
	{
		tmp = (ptr_stack->top > pos) ? ptr_stack->elements[ptr_stack->top - (1 + pos)] : ptr_stack->elements[ptr_stack->top - 1];
	}
	
	return tmp;
}

Point pop(Stack *ptr_stack)
/*
 * Method of removing element at the top of the stack strucuture.
 * *ptr_stack (required) ---> Stack* | pointer to stack structure
 * Returns Point | element removed from the stack
*/
{
	Point tmp;
	memset(&tmp, 0, sizeof(Point));

	if (ptr_stack->top <= 0){printf("Stack is empty\n");}

	if (ptr_stack->top > 0)
	{
		tmp = ptr_stack->elements[ptr_stack->top - 1];
		memset(&(ptr_stack->elements[ptr_stack->top - 1]), 0, sizeof(Point));
		ptr_stack->top--;
	}

	return tmp;
}
/* ########################################################### */


int main(void)
{
	return 0;
}
