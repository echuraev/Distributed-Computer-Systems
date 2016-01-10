#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char* argv[])
{
int x;
	for(x=0; x<argc; x++) {
			printf("> %s", argv[x]);
	}
	int params_size = argc;
	if (strcmp(argv[argc - 2], ">") == 0 || strcmp(argv[argc - 2], "2>") == 0
		|| strcmp(argv[argc - 2], "<") == 0 || strcmp(argv[argc - 2], ">>") == 0)
	{
		params_size -= 2;
	}
	char ** params = malloc(sizeof(char *) * params_size);
	int i;
	if (argc < 2)
	{
		printf("Error! You didn't enter command!\n");
		exit(2);
	}
	params[0] = argv[1];
	for (i = 2; i < params_size; ++i)
	{
		params[i - 1] = argv[i];
	}
	params[params_size - 1] = NULL;
	int status;
 
	printf("\nTerminal pid: %d\n", getppid());
	printf("labbash pid: %d\tParent pid: %d\n\n", getpid(), getppid());
	int pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		printf("\nlabbash subprocess pid: %d\tParent pid: %d\n\n", getpid(), getppid());
		int fd;
		if (strcmp(argv[argc - 2], ">") == 0)
		{
			close(1);
			fd = open(argv[argc -1], O_CREAT | O_RDWR, 0644);
			dup(fd);
		}
		else if (strcmp(argv[argc - 2], "2>") == 0)
		{
			close(2);
			fd = open(argv[argc -1], O_CREAT | O_RDWR, 0644);
			dup(fd);
		}
		else if (strcmp(argv[argc - 2], "<") == 0)
		{
			close(0);
			fd = open(argv[argc -1], O_RDONLY, 0644);
			dup(fd);
		}
		else if (strcmp(argv[argc - 2], ">>") == 0)
		{
			close(1);
			fd = open(argv[argc -1], O_APPEND | O_RDWR, 0644);
			dup(fd);
		}
		
		perror("Test error stream\n");
		if (execvp(params[0], params) < 0)
		{
			perror("exec() error: ");
		}
		close(fd);
	}
	else
	{
		wait(&status);
	//			while(1)
	//	if(wait(&status)!=EINTR) break; 
	}
	
	free(params);		
	return 0;
}
