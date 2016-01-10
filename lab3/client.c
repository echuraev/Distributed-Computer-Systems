#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "roaster.h"
#include "messages.h"
#include <pthread.h>
#include <assert.h>

#define SHMPATH "/dev/null"
#define SHMID 1
#define SEMPATH "/dev/null"
#define SEMID 1
#define MSQPATH "/dev/null"
#define MSQID 1
pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;

void writeMessage(int msqid, long frompid, char* name, long topid, char* msg);

typedef struct
{
	int msqid;
} thread_data;

void *threadTask(void *arg)
{
	thread_data * data;
	data = (thread_data *) arg;
	Message message;
	if(msgrcv(data->msqid, (struct msgbuf *) &message, sizeof(message.info), getpid(), 0) < 0)
	{
		perror("Error! Can\'t receive message from queue!\n");
		exit(1);
	}
	pthread_mutex_lock (&m_mutex);
	if (message.type == getpid())
	{
		printf("\nNEW MESSAGE!\n");
		printf("From: %s (pid: %ld)\n", message.info.name, message.info.frompid);
		printf("Message:\n%s\n\n", message.info.msg);
	}
	sleep (1);
	pthread_mutex_unlock (&m_mutex);
	return NULL;
}

int main (int argc, char * argv[])
{
	int shmid, semid, msqid;
	int size = 0;
	key_t shmkey, semkey, msqkey;
	struct sembuf sem_lock = {0, 1, 0};
	struct sembuf sem_unlock = {0, -1, 0};
	char name [256];
	char param [10];
	
	shmkey = ftok(SHMPATH, SHMID);
	if (shmkey == (key_t)-1)
	{
		perror("ftok");
		exit(3);
	}
	
	semkey = ftok(SEMPATH, SEMID);
	if (semkey == (key_t)-1)
	{
		perror("ftok sem");
		exit(3);
	}
	
	msqkey = ftok(MSQPATH, MSQID);
	if (msqkey == (key_t)-1)
	{
		perror("ftok msq");
		exit(3);
	}
	
	printf ("Enter your name: ");
	scanf ("%255s", name);
	
	int c;
	while ((c = getchar()) != '\n' && c != EOF); // Clear stdin
	
	shmid = shmget(shmkey, sizeof(Roaster)*USERS_LIMIT, 0666);
	if (shmid < 0)
	{
		perror("shmget");
		exit(1);
	}
	semid = semget(semkey, 1, IPC_CREAT | 0666);
	if (semid < 0)
	{
		perror("semget");
		exit(1);
	}
	msqid = msgget(msqkey, 0666 | IPC_CREAT);
	if(msqid < 0)
	{
		perror("msgget");
		exit(1);
	} 
	if((semop(semid, &sem_lock, 1) == -1))
	{
		perror("semop lock");
		exit(1);
	}

	Roaster * roaster = (Roaster*)shmat(shmid, NULL, 0);
	addToRoaster(&roaster, getpid(), name);
	
	if((semop(semid, &sem_unlock, 1) == -1))
	{
		perror("semop unlock");
		exit(1);
	}
	
	pthread_t thread;
	thread_data tdata;
	tdata.msqid = msqid;
	int rc = pthread_create(&thread, NULL, threadTask, (void *) &tdata);
	assert(rc == 0);
	/*int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);*/
	while (1)
	{
		char msg [MSGLEN];
		int i;
		for (i = 0; i < 10; ++i)
		{
			param[i] = 0;
		}

		printf ("\nWelcome to our chat!\n");
		printf ("Please enter option:\n");
		printf ("\tsr - show roaster\n");
		printf ("\twm <pid> - write message\n");
		printf ("\texit - exit from chat\n");
		printf ("> ");

		//time_t end = time(0) + 10; // 10 seconds time limit.

		while(//time(0) < end  // not timed out
		    /*&&*/ fgets (param, 9, stdin) == NULL // not read a word
		    && errno == EAGAIN); // no error, but would block
		
		/*if (!param[0])
		{
			printf("Else\n");
			Message message;
			int m = 1;
        	if(msgrcv(msqid, (struct msgbuf *) &message, sizeof(message.info), 0, 0) < 0 || m > 0)
        	{
				perror("Error! Can\'t receive message from queue!\n");
				//exit(1);
			}
			if (message.type == getpid())
			{
				show_header = 1;
				printf("\nNEW MESSAGE!\n");
				printf("From: %s (pid: %ld)\n", message.info.name, message.info.frompid);
				printf("Message:\n%s\n", message.info.msg);
			}	
			show_header = 1;
			printf("Else\n");
			continue;
		}*/
		
		//show_header = 1;
		
		/* Remove \n */
		if ((strlen(param)>0) && (param[strlen (param) - 1] == '\n'))
        	param[strlen (param) - 1] = '\0';
        	
        pthread_mutex_lock (&m_mutex);
			
		if (!strcmp(param, "sr"))
		{
			showRoaster(roaster, getpid());
		}
		else if (!strcmp(param, "exit"))
		{
			if((semop(semid, &sem_lock, 1) == -1))
			{
				perror("semop lock");
				exit(1);
			}

			removeFromRoaster(&roaster, getpid());
	
			if((semop(semid, &sem_unlock, 1) == -1))
			{
				perror("semop unlock");
				exit(1);
			}
			break;
		}
		else
		{
			char* ch_part = strtok(param, " ");
			if (!strcmp(param, "wm"))
			{
				ch_part = strtok(NULL, " ");
				if (!ch_part)
				{
					perror("Error! Cannot parse pid!\n");
					continue;
				}
				int topid = atoi(ch_part);
				printf("Enter your message:\n");
				msg[0] = 0;
				while(strlen(msg) == 0)
				{
					fgets (msg, MSGLEN, stdin);
				}
				/* Remove \n */
				if ((strlen(msg)>0) && (msg[strlen (msg) - 1] == '\n'))
				   	msg[strlen (msg) - 1] = '\0';
				writeMessage(msqid, getpid(), name, topid, msg);
			}
			else
			{
				perror("Error! Unknown option!\n");
			}
		}
		pthread_mutex_unlock (&m_mutex);
	}

	return 0;
}

void writeMessage(int msqid, long frompid, char* name, long topid, char* msg)
{
	if (topid <= 0 || frompid <= 0)
	{
		perror("Error! Invalid pid!");
		return;
	}
	Message message;
	message.type = topid;
	strcpy(message.info.msg, msg);
	strcpy(message.info.name, name);
	message.info.frompid = frompid;
	if (msgsnd(msqid, (struct msgbuf *) &message, sizeof(message.info), 0) < 0)
	{
		printf("Error! Cannot send message!\n");
		return;
	}
}
