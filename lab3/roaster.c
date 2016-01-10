#include <stdio.h>
#include <stdlib.h>
#include "roaster.h"
#include <string.h>

int getSize(Roaster * roaster)
{
	int size;
	for (size = 0; size < USERS_LIMIT; ++size)
	{
		if (roaster[size].pid < 0)
			break;
	}
	return size;
}

void serverInit(Roaster ** roaster)
{
	int i;
	for (i = 0; i < USERS_LIMIT; ++i)
	{
		(*(*roaster + i)).pid = -1;
		strcpy((*(*roaster + i)).name, "");
	}
}

void addToRoaster(Roaster ** roaster, int pid, char* name)
{
	int size = getSize(*roaster);
	if (size == USERS_LIMIT - 1)
	{
		perror("Cannot add you to Roaster! Try later!\n\n");
		exit(1);
	}
	(*(*roaster + size)).pid = pid;
	strcpy((*(*roaster + size)).name, name);
}

void removeFromRoaster(Roaster ** roaster, int pid)
{
	int size = getSize(*roaster);
	int i, j;
	for (i = 0, j = 0; i < size; ++i)
	{
		if ((*(*roaster + i)).pid == pid)
			continue;
		*(*roaster + j) = *(*roaster + i);
		j++;
	}
	if (i == j)
	{
		perror("Cannot find target pid!\n\n");
		exit (1);
	}
	(*(*roaster + j)).pid = -1;
}

void showRoaster(Roaster * roaster, int pid)
{
	int size = getSize(roaster);
	printf("\nPID\tNAME\n\n");
	if (size <= 1)
	{
		printf("The roaster is empty!\n\n");
		return;
	}
	int i;
	for (i = 0; i < size; ++i)
	{
		if (roaster[i].pid == pid)
			continue;
		printf("%d\t%s\n", roaster[i].pid, roaster[i].name);
	}	
}
