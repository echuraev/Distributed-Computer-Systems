#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "calc.h"
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "workers.h"

#define WORKER_COUNT 10

int worker_count = WORKER_COUNT;
Workers * workers = NULL;

void parameters_parse (int argc, char* argv[], int* worker_count, char** instream, char** outstream);
char* remove_spaces(char* str, int len);
void status(int sign);

int main (int argc, char* argv [])
{
	char* instream;
	char* outstream;
	FILE* fdin;
	FILE* fdout;
	struct epoll_event ev, events[worker_count];
	printf("create\n");
	int epollfd = epoll_create(worker_count);
	if (epollfd == -1) 
	{
		perror("Error: epoll_create");
		exit(1);
	}
	
	/* Signal event */
	struct sigaction sig;	
	memset(&sig, 0, sizeof(sig));
	
	sig.sa_handler = status;
	
	sigemptyset(&sig.sa_mask);                                                            

	/* Signal event end */
	
	parameters_parse (argc, argv, &worker_count, &instream, &outstream);
	
	printf("wc=%d\ninstr=%s\noutstr=%s\n", worker_count, instream, outstream);	
	
	if (instream == NULL || outstream == NULL)
	{
		perror("Files error!");
		exit (1);
	}
	fdin = fopen (instream, "rt");
	if (fdin == NULL)
	{
		perror("fdin open");
		exit(1);
	}
	fdout = fopen (outstream, "wt");
	if (fdout == NULL)
	{
		perror("fdout open");
		exit (1);
	}
	
	size_t len = 0;
	char * line = NULL;
	int counter = 0;
	while (getline(&line, &len, fdin) != -1) 
	{
		counter++;
	}
	fclose(fdin);
	fdin = fopen (instream, "rt");
	if (fdin == NULL)
	{
		perror("fdin open");
		exit(1);
	}
	char ** lines = malloc(sizeof(char*)*counter);
	counter = 0;
	while (getline(&line, &len, fdin) != -1) 
	{
		lines[counter] = malloc(sizeof(char)*len);
		strcpy(lines[counter++], remove_spaces(line, len));
	}
	fclose(fdin);
	workers = malloc(sizeof(Workers)*worker_count);
	int i;
	for(i = 0; i < worker_count; ++i)
	{
		workers[i].status = WFREE;
		if (pipe(workers[i].fd1) < 0)
		{
			fprintf(stderr, "Pipe1 worker # %d error!!!\n", i);
			exit(1);
		}
		if (pipe(workers[i].fd2) < 0)
		{
			fprintf(stderr, "Pipe2 worker # %d error!!!\n", i);
			exit(1);
		}
		workers[i].pid = fork();
		if (workers[i].pid < 0)
		{
			fprintf(stderr, "Fork worker # %d error!!!\n", i);
			exit(1);
		}
		if (workers[i].pid == 0)
		{
			srand(time(NULL) + i);
			close(workers[i].fd1[WRITE]);
			close(workers[i].fd2[READ]);
			char * expr = malloc(MAXEXP*sizeof(char));
			while(1)
			{
				sig.sa_handler = SIG_IGN;
				sigaction(SIGINT, &sig, NULL);
				printf("child %d\n", i);
		
				if (read(workers[i].fd1[READ], expr, MAXEXP) < 0)
					break;
				char * res = calc(expr);
				sleep((rand()%5 + 5));
				write(workers[i].fd2[WRITE], res, strlen(res));
			}
			free(expr);
			close(workers[i].fd1[READ]);
			close(workers[i].fd2[WRITE]);
			break;
		}
	}
	
	int siga = sigaction(SIGINT, &sig, NULL);
	for (i = 0; i < worker_count; ++i)
	{
		ev.events = EPOLLIN;
		ev.data.fd = workers[i].fd2[READ];
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, workers[i].fd2[READ], &ev) == -1) {
			perror("epoll_ctl: listen_sock");
			exit(EXIT_FAILURE);
		}
	}
	
	for (i = 0; i < worker_count && i < counter; ++i)
	{
		workers[i].status = WBUSY;
		write(workers[i].fd1[WRITE], lines[i], (int)strlen(lines[i]));
	}
	int finished_worker = 0;
	printf("counter %d\n", counter);
	while(1)
	{
		int nfds = epoll_wait(epollfd, events, worker_count, 15);
		if(nfds > 0) 
		{
			int n;
			for(n = 0; n < worker_count; ++n) 
			{
				if(events[0].data.fd == workers[n].fd2[READ])
				{
					break;
				}
			}
			
			if(n < worker_count) 
			{
				workers[n].status = WFREE;
				char * res = malloc(MAXEXP*sizeof(char));
				read(workers[n].fd2[READ], res, MAXEXP);
				fprintf(fdout, "%s\n", res);
				free(res);
				if (i < counter)
				{
					write(workers[n].fd1[WRITE], lines[i], (int)strlen(lines[i]));
					i++;
					workers[n].status = WBUSY;
				}
				else
				{
					finished_worker++;
					close(workers[n].fd1[WRITE]);
					close(workers[n].fd2[READ]);
				}
			}
		}
		else if (nfds < 0) 
		{
			//perror("Error in epoll work!");
			//exit(1);
		}
		else
		{
			if (finished_worker == worker_count)
				break;
		}
	}
	return 0;
}

void parameters_parse (int argc, char* argv[], int* worker_count, char** instream, char** outstream)
{
	*worker_count = WORKER_COUNT;
	*instream = NULL;
	*outstream = NULL;
	if (argc > 1)
	{
		int i;
		for (i = 1; i < argc; ++i)
		{
			if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
			{
				printf("\n==================================================================\n");
				printf("=====================   LAB DISPATCHER   =========================\n");
				printf("==================================================================\n\n");
				printf("Usage: %s [OPTION...]\n", argv[0]);
				printf("LAB DISPATCHER is used for computing arithmetic operations on several processes\n");
				printf("Examples:\n");
				printf("\t%s\t\t\t# Run dispatcher with default parameters\n", argv[0]);
				printf("\t%s -n 5\t\t# Run dispatcher on 5 processes\n", argv[0]);
				printf("\t%s -i file.txt\t# Read arithmetic expression from file.txt\n", argv[0]);
				printf("\t%s -o out.txt\t\t# Write result of computing to out.txt\n\n", argv[0]);
				printf("Options:\n");
				printf("\t-n <number>\t\t\t# Count of processes (Default: 10)\n");
				printf("\t-i <input_stream>\t\t# Input stream (Default: stdin)\n");
				printf("\t-o <output_stream>\t\t# Output stream (Defauld: stdout)\n\n");
				exit(0);
			}
			else if (strcmp(argv[i], "-n") == 0)
			{
				*worker_count = atoi(argv[++i]);
			}
			else if (strcmp(argv[i], "-i") == 0)
			{
				*instream = argv[++i];
			}
			else if (strcmp(argv[i], "-o") == 0)
			{
				*outstream = argv[++i];
			}
			else
			{
				perror("Error! Unknown key!");
				exit(1);
			}
		}
	}
}
char* remove_spaces(char* str, int len)
{
	char* new_str = calloc(len, sizeof(char));
	int i, j;
	j = 0;
	for (i = 0; i < len; ++i)
	{
		if (str[i] == ' ')
			continue;
		if (str[i] == '\n')
			str[i] = '\0';
		new_str[j++] = str[i];
	}
	return new_str;
}
void status(int sign)
{
	int i;
	printf("\n=================================================\n");
	for (i = 0; i < worker_count; ++i)
	{
		if (workers[i].status == WBUSY)
			printf("%d\tWorker %d\t\tBUSY\n", getpid(), i);
		else
			printf("%d\tWorker %d\t\tFREE\n", getpid(), i);
	}
	printf("=================================================\n");
}


