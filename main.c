#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
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

/* ####################  Stack functions  #################### */
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


/* ################  Graham's Scan functions  ################ */
void relAngle(const Point ref, Point *p)
/*
 * Auxiliary function for calculating the relative angle
 * between reference point and given point in the set.
 * ref (required) ---> Point | reference point
 * p (required) ---> Point | point by which angle is calculated
*/
{
	float dy, dx;
	dy = (p->y) - ref.y;
	dx = (p->x) - ref.x;

	p->a = (dx > 0) ? atan(dy/dx) : (dx < 0) ? M_PI + atan(dy/dx) : M_PI/2;
}

float detLeft(const Point p0, const Point p1, const Point p2)
/*
 * Auxiliary function for calculating determinant and
 * checking if the given points constitute a left turn.
 * p0 (required) ---> Point | first point to be checked
 * p1 (required) ---> Point | second point to be checked
 * p2 (required) ---> Point | third point to be checked
 * Returns int | 1 if left turn, 0 if not
*/
{
	Point op1, op2;
	float det;

	op1.x = p1.x - p0.x;
	op1.y = p1.y - p0.y;
	op2.x = p2.x - p0.x;
	op2.y = p2.y - p0.y;
	
	det = ((op1.x * op2.y) - (op1.y * op2.x));

	return (det <= 0) ? 1 : 0;
}

int refPoint(const Point *points, const int *vector_size)
/*
 * Auxiliary function for finding starting point to
 * to be used as reference position on Graham's Scan.
 * *points (required) ---> Point* | pointer to array of points
 * *vector_size (required) ---> int* | positive integer refering to size of array
 * Returns int | index of reference element in array of points
*/
{
	if (*vector_size < 0){printf("[ ERROR ] Array size has to be a positive integer\n");}

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
/*
 * Auxiliary function for sorting array of points
 * accordingly to angle formed with reference point.
 * *points (required) ---> Point* | pointer to array of points
 * *vector_size (required) ---> int* | positive integer refering to size of array
 * *ptr_stack (required) ---> Stack* | pointer to stack that will be holding the convex hull points
*/
{
	if (*vector_size < 0){printf("[ ERROR ] Array size has to be a positive integer\n");}

	int checkAngle(const void* p1, const void* p2)
	/*
	 * Rule function for quicksort algorithm.
	 * p1 (required) ---> void* | void pointer
	 * p2 (required) ---> void* | void pointer
	 * Return int | 0 if p1 == p2, 1 if p1 > p2 or -1 if p1 < p2
	*/
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

	Point ref_p, *tmp = 0;
	int i, ref_pi = refPoint(points, vector_size);
	tmp = points;

	tmp += ref_pi;
	memcpy(&ref_p, tmp, sizeof(Point));
	put(ptr_stack, ref_p);
	// printf("\n>>> ref: %d\n\tX = %.2f Y = %.2f a = %.6f\n", ref_pi, ref_p.x, ref_p.y, ref_p.a);  // DEBUG LINE
	i = 0;

	for (i = 0; i < (*vector_size - 1); i++)
	{
		if (i < ref_pi)
		{
			relAngle(ref_p, points);
		}
		else
		{
			tmp = points;
			tmp++;
			relAngle(ref_p, tmp);
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
/*
 * Function implementing Graham's Scan logic.
 * *points (required) ---> Point* | pointer to array of points
 * *vector_size (required) ---> int* | positive integer refering to size of array
 * *ptr_stack (required) ---> Stack* | pointer to stack that will be holding the convex hull points
*/
{
	if (*vector_size < 0){printf("[ ERROR ] Array size has to be a positive integer\n");}

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


/* ###############  Function for reading data  ############### */
Point* ReadDataFile(const char filename[32])
/*
 * Simple function for reading data points from text file.
 * filename (required) ---> char* | string with the filename
*/
{
	FILE *data_file = 0;
	data_file = fopen(filename, "r");
	if (data_file == 0){printf("[ ERROR ] Unable to read file %s\n", filename);}

	int c, l, t;
	char character, line[32], seg[8];
	static Point data_points[CHUNK];
	Point *ptr_dp = 0;
	ptr_dp = &data_points[0];	

	regex_t pattern;
	if (regcomp(&pattern, "^[0-9]+( +)?,|;( +)?[0-9]+", REG_EXTENDED) != 0){printf("[ ERROR ] Regular expression not compiled\n");}

	l = 0;

	while (feof(data_file) == 0)
	{
		/* character:
		 *  0 ---> null            (\x00)
		 * 10 ---> new line        (\x0A)
		 * 13 ---> carriage return (\x0D)
		 * 32 ---> space           (\x20)
		 * 44 ---> comma           (\x2C)
		 * 59 ---> semicolon       (\x3B)
		*/
		memset(&line, 0, sizeof(line));
		character = fgetc(data_file);
		c = 0;
		
		while (character != EOF && character != 0 && character != 10)
		{
			line[c] = character;
			character = fgetc(data_file);
			c++;
		}

		line[c] = 0;
		
		if (regexec(&pattern, line, 0, 0, 0) == 0)
		{
			memset(&seg, 0, sizeof(seg));
			t = 0;
			c = 0;

			/* ---- X value for Point ---- */
			while (line[c] != 0 && line[c] != 44 && line[c] != 59)
			{
				if (line[c] == 32){c++;continue;}
				seg[t] = line[c];
				c++;
				t++;
			}

			seg[t] = 0;
			ptr_dp->x = atoi(seg);
			/* --------------------------- */

			memset(&seg, 0, sizeof(seg));
			t = 0;
			c++;

			/* ---- Y value for Point ---- */
			while (line[c] != 0 && line[c] != 44 && line[c] != 59)
			{
				if (line[c] == 32){c++;continue;}
				seg[t] = line[c];
				c++;
				t++;
			}

			seg[t] = 0;
			ptr_dp->y = atoi(seg);
			/* --------------------------- */

			ptr_dp++;
		}

		l++;
	}

	fclose(data_file);

	return &data_points[0];
}
/* ########################################################### */


int main(void)
{
	int i, s = 10;
	Stack *ptrStack = newStack();
	
	Point *ptrDataPoints = ReadDataFile("data_points.txt");
	Point *dp = 0;
	
	dp = ptrDataPoints;
	printf("# ORIGINAL DATA POINTS\n");
	for (i = 0; i < s; i++){printf("[ %d ]\t(%.2f, %.2f, %.6f)\n", i, dp->x, dp->y, dp->a); dp++;}

	SortByAngle(ptrDataPoints, &s, ptrStack);
	genConvHull(ptrDataPoints, &s, ptrStack);
	
	dp = ptrDataPoints;
	printf("\n# AFTER SORTING BY ANGLE\n");
	for (i = 0; i < s; i++){printf("[ %d ]\t(%.2f, %.2f, %.6f)\n", i, dp->x, dp->y, dp->a); dp++;}

	Point test;
	printf("\n# CONVEX HULL\n");
	for (i = 0; i < ptrStack->top; i++)
	{test = peek(ptrStack, i); printf("[ %d ]\t(%.2f, %.2f, %.6f)\n", i, test.x, test.y, test.a);}

	return 0;
}
