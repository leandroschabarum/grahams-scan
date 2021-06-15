#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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


/* ################  Graham's Scan Functions  ################ */
void relAngle(const Point ref, Point *p)
/**/
{
	float dy, dx;
	dy = (p->y) - ref.y;
	dx = (p->x) - ref.x;

	p->a = (dx > 0) ? atan(dy/dx) : (dx < 0) ? M_PI + atan(dy/dx) : M_PI/2;
}

float detLeft(const Point ref, const Point p1, const Point p2)
/**/
{
	Point op1, op2;
	float det;

	op1.x = p1.x - ref.x;
	op1.y = p1.y - ref.y;
	op2.x = p2.x - ref.x;
	op2.y = p2.y - ref.y;
	
	det = ((op1.x * op2.y) - (op1.y * op2.x));

	return (det <= 0) ? 1 : 0;
}

int refPoint(const Point *points, const int *vector_size)
/**/
{
	Point mp;
	int i, minY = 0;

	mp.x = points->x;
	mp.y = points->y;
	points++;

	for (i = 1; i < *vector_size; i++)
	{
		if (mp.y > points->y || (mp.y == points->y && mp.x > points->x))
		{
			minY = i;
			mp.x = points->x;
			mp.y = points->y;
		}

		points++;
	}

	return minY;
}

void SortByAngle(Point *points, int *vector_size, Stack *ptr_stack)
/**/
{
	int checkAngle(const void* p1, const void* p2)
	/**/
	{
		Point *P1 = 0, *P2 = 0;
		P1 = (Point*) p1;
		P2 = (Point*) p2;

		if (P2->a < P1->a){return 1;}
		if (P2->a > P1->a){return -1;}
		
		if (P2->a == P1->a)
		{
			if (P2->x < P1->x){return 1;}
			if (P2->x > P1->x){return -1;}
		}

		return 0;
	}

	Point refPoint, *tmp = 0;
	int i, refPointIndex = refPoint(points, vector_size);
	tmp = points;

	tmp += refPointIndex;
	memcpy(&refPoint, tmp, sizeof(Point));
	put(ptr_stack, refPoint);
	// printf("\n>>> ref: %d\n\tX = %.2f Y = %.2f a = %.6f\n", refPointIndex, refPoint.x, refPoint.y, refPoint.a);  // DEBUG LINE
	i = 0;

	for (i = 0; i < (*vector_size - 1); i++)
	{
		if (i < refPointIndex)
		{
			relAngle(refPoint, points);
		}
		else
		{
			tmp = points;
			tmp++;
			relAngle(refPoint, tmp);
			memcpy(points, tmp, sizeof(Point));
		}

		points++;
	}

	memset(points, 0, sizeof(Point));
	*vector_size = *vector_size - 1;
	points -= *vector_size;

	qsort(points, *vector_size, sizeof(Point), checkAngle);
}

void genConvHull(const Point *points, const int *vector_size, Stack *ptr_stack)
/**/
{
	int i;

	for (i = 0; i < *vector_size; i++)
	{
		while (ptr_stack->top > 1 && detLeft(peek(ptr_stack, 1), peek(ptr_stack, 0), *points))
		{
			pop(ptr_stack);
		}
		
		put(ptr_stack, *points);
		points++;
	}
}
/* ########################################################### */


int main(void)
{
	return 0;
}
