#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct element
{
	void *data;
	void *previous;
} Element;

typedef struct stack
{
	void *elements;
	unsigned int size;
} Stack;


/**
 * Function for creating empty stack structure.
 * 
 * @return *Stack
 */
Stack* newStack(void)
{
	static Stack *local_stack = 0;

	local_stack = (Stack *) malloc(sizeof(Stack));
	memset(local_stack, 0, sizeof(Stack));

	if (local_stack)
	{
		return local_stack;
	}

	return 0;
}

/**
 * Method for adding elements to the stack structure.
 * 
 * @param  Stack *ptr_stack
 * @param  void *element
 * @param  unsigned int size
 */
void put(Stack *ptr_stack, void *element, unsigned int size)
{
	if (element && size > 0)
	{
		Element *tmp_element = (Element *) malloc(sizeof(Element));

		if (tmp_element)
		{
			memset(tmp_element, 0, sizeof(Element));
			tmp_element->data = (void *) malloc(size);

			if (tmp_element->data)
			{
				memcpy(tmp_element->data, element, size);
				tmp_element->previous = ptr_stack->elements;

				ptr_stack->elements = tmp_element;
				ptr_stack->size += 1;
			}
			else
			{
				fprintf(stderr, "[unsufficient memory] unable to allocate <%u bytes> for data at <%p>\n", size, element);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			fprintf(stderr, "[unsufficient memory] unable to allocate <%lu bytes> on stack for new element\n", sizeof(Element));
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * Method for looking at a given position from
 * the top of the stack strucuture.
 * 
 * @param  Stack *ptr_stack
 * @param  unsigned int position
 * @return void*
 */
void* peek(Stack *ptr_stack, unsigned int position)
{
	if (ptr_stack->size > 0 && position < ptr_stack->size)
	{
		Element *ref_element = 0;
		ref_element = ptr_stack->elements;

		while (position)
		{
			ref_element = ref_element->previous;
			position--;
		}

		return ref_element->data;
	}

	return 0;
}

/**
 * Method for removing element at the top of the stack strucuture.
 * 
 * @param  Stack *ptr_stack
 * @return void*
 */
void* pop(Stack *ptr_stack)
{
	void *top_data = 0;
	Element *top_element = 0;
	top_element = ptr_stack->elements;

	if (ptr_stack->size > 0)
	{
		top_data = top_element->data;
		top_element = top_element->previous;
		ptr_stack->elements = top_element;
		ptr_stack->size--;
	}

	return top_data;
}

