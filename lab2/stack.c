#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* initStackNum(int size)
{
	Stack * stack = (Stack*)malloc(sizeof(Stack) * size);
	sizeNum = size;
	cursorNum = -1;
	return stack;
}

Stack* initStackOper(int size)
{
	Stack * stack = (Stack*)malloc(sizeof(Stack) * size);
	sizeOper = size;
	cursorOper = -1;
	return stack;
}


int pushNum(Stack* stack, double num)
{
	if (cursorNum + 1 == sizeNum)
	{
		return 0;
	}
	stack[++cursorNum].num = num;
	return 1;
}
int pushOper(Stack* stack, char oper)
{
	if (cursorOper + 1 == sizeOper)
	{
		return 0;
	}
	stack[++cursorOper].oper = oper;
	return 1;
}
int popNum(Stack* stack, double * num)
{
	if (cursorNum - 1 < -1)
	{
		return 0;
	}
	*num = stack[cursorNum--].num;
	return 1;
}
int popOper(Stack* stack, char * ch)
{
	if (cursorOper - 1 < -1)
	{
		return 0;
	}
	*ch = stack[cursorOper--].oper;
	return 1;
}
int topNum(Stack* stack, double * num)
{
	if (cursorNum < 0)
	{
		return 0;
	}
	*num = stack[cursorNum].num;
	return 1;
}
int topOper(Stack* stack, char * ch)
{
	if (cursorOper < 0)
	{
		return 0;
	}
	*ch = stack[cursorOper].oper;
	return 1;
}
int getCursorNum()
{
	return cursorNum;
}
int getCursorOper()
{
	return cursorOper;
}
