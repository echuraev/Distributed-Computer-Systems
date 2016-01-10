#include "calc.h"
#include "stack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char * calc(char * exp)
{
	Stack * num_stack = initStackNum(strlen(exp));
	Stack * oper_stack = initStackOper(strlen(exp));
	int i;
	int bool_num = 0;
	int bool_min = 0;
	int cursor;
	double res = 0;
	char ch;
	double numVal;
	char * num = calloc(strlen(exp), sizeof(char));
	for (i = 0; i < strlen(exp); ++i)
	{
		if (exp[i] >= '0' && exp[i] <= '9' || exp[i] == '.')
		{
			if (bool_num == 0)
			{
				bool_num = 1;
				cursor = 0;
			}
			else
			{
				cursor++;
			}
			num[cursor] = exp[i];
			num[cursor+1] = '\0';
		}
		else if (exp[i] == '+' || exp[i] == '-')
		{
			if (bool_num == 1)
			{
				char oper = exp[i];
				double number = atof(num);
				if (bool_min > 0)
				{
					bool_min = 0;
					number = -number;
				}
				if (getCursorOper() >= 0)
				{
					if (!topOper(oper_stack, &ch))
						return NULL;
					if (ch == '*')
					{
						popOper(oper_stack, &ch);
						if (!popNum(num_stack, &numVal))
							return NULL;
						number = numVal * number;
					}
					else if (ch == '/')
					{
						popOper(oper_stack, &ch);
						if (!popNum(num_stack, &numVal))
							return NULL;
						number = numVal / number;
					}
				}
				if (oper == '-')
				{
					oper = '+';
					bool_min = 1;
				}
				if (!pushNum(num_stack, number))
					return NULL;
				int j;
				for (j = 0; j < cursor+1; ++j)
				{
					num[j] = '\0';
				}
				bool_num = 0;
				if (!pushOper(oper_stack, oper))
					return NULL;
			}
			else
			{
				return NULL;
			}
		}
		else if (exp[i] == '*' || exp[i] == '/')
		{
			if (bool_num == 1)
			{
				double number = atof(num);
				if (bool_min > 0)
				{
					bool_min = 0;
					number = -number;
				}
				if (getCursorOper() >= 0)
				{
					if (!topOper(oper_stack, &ch))
						return NULL;
					if (ch == '*')
					{
						popOper(oper_stack, &ch);
						if (!popNum(num_stack, &numVal))
							return NULL;
						number = numVal * number;
					}
					else if (ch == '/')
					{
						popOper(oper_stack, &ch);
						if (!popNum(num_stack, &numVal))
							return NULL;
						number = numVal / number;
					}
				}
				if (!pushNum(num_stack, number))
					return NULL;
				int j;
				for (j = 0; j < cursor+1; ++j)
				{
					num[j] = '\0';
				}
				bool_num = 0;
				if (!pushOper(oper_stack, exp[i]))
					return NULL;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
		if (i == strlen(exp) - 1)
		{
			if (strlen(num) > 0)
			{
				double number = atof(num);
				if (bool_min > 0)
				{
					bool_min = 0;
					number = -number;
				}
				if (getCursorOper() >= 0)
				{
					if (!topOper(oper_stack, &ch))
						return NULL;
					if (ch == '*')
					{
						popOper(oper_stack, &ch);
						if (!popNum(num_stack, &numVal))
							return NULL;
						number = numVal * number;
					}
					else if (ch == '/')
					{
						popOper(oper_stack, &ch);
						if (!popNum(num_stack, &numVal))
							return NULL;
						number = numVal / number;
					}
				}
				if (!pushNum(num_stack, number))
					return NULL;
				int j;
				for (j = 0; j < cursor+1; ++j)
				{
					num[j] = '\0';
				}
				bool_num = 0;
			}
			else
			{
				return NULL;
			}
		}
	}
	while (getCursorNum() > 0 && getCursorOper() >= 0)
	{
		double second;
		if(!popNum(num_stack, &second))
			return NULL;
		double first;
		if(!popNum(num_stack, &first))
			return NULL;
		char oper;
		if (!popOper(oper_stack, &oper))
			return NULL;
		if (oper == '+')
		{
			double tmp_res = first + second;
			if(!pushNum(num_stack, tmp_res))
				return NULL;
		}
		else if (oper == '-')
		{
			double tmp_res = first - second;
			if (!pushNum(num_stack, tmp_res))
				return NULL;
		}
	}
	
	if (!popNum(num_stack, &res))
		return NULL;
	char buff [100];
	sprintf(buff,"%f", res);
	char * chres = malloc(sizeof(char)*strlen(buff));
	strcpy(chres, buff);
	return chres;
}
