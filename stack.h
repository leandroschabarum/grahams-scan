/*------------------ HEADERFILE ------------------*/
/*CREATOR: Leandro Schabarum                      */
/*DESCRIPTION: Stack data structure               */
/*------------------------------------------------*/

#ifndef STACK_H  // once only header guard (wrapper)
#define STACK_H

#ifdef _cplusplus
extern "C" {
#endif


// Stack struct prototypes
// file: stack.c
typedef struct stack Stack;

// Stack function prototypes
// file: stack.c
Stack* newStack(void);
void put(Stack *ptr_stack, void *element, unsigned int size);
void* peek(Stack *ptr_stack, unsigned int position);
void* pop(Stack *ptr_stack);


#ifdef _cplusplus
}
#endif
#endif // once only header guard (wrapper)

