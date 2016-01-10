#ifndef _STACK_H_
#define _STACK_H_

static int sizeNum;
static int sizeOper;
static int cursorNum;
static int cursorOper;

typedef struct
{
	double num;
	char oper;
} Stack;

Stack* initStackNum(int size);
Stack* initStackOper(int size);
int pushNum(Stack* stack, double num);
int pushOper(Stack* stack, char oper);
int popNum(Stack* stack, double*);
int popOper(Stack* stack, char*);
int topNum(Stack* stack, double*);
int topOper(Stack* stack, char*);
int getCursorNum();
int getCursorOper();

#endif
