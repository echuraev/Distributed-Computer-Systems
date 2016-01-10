#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include "roaster.h"

#define SHMPATH "/dev/null"
#define SHMID 1
#define SEMPATH "/dev/null"
#define SEMID 1
#define MSQPATH "/dev/null"
#define MSQID 1

int main (int argc, char * argv[])
{
	int shmid, semid, msqid;
	int size = 0;
	key_t shmkey, semkey, msqkey;
	Roaster* shm;
	struct sembuf sem_lock = {0, 1, 0};
	struct sembuf sem_unlock = {0, -1, 0};

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

	shmid = shmget(shmkey, sizeof(Roaster)*USERS_LIMIT, IPC_CREAT | 0666);
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
	
	shm = (Roaster*)shmat(shmid, NULL, 0);

	if ((int)shm == -1)
	{
		perror("shmat");
		exit(2);
	}
	
	serverInit(&shm); // Clear array
	
	if((semop(semid, &sem_unlock, 1) == -1))
	{
		perror("semop unlock");
		exit(1);
	}

	while(1) {
		printf("exit (y/n): ");
		char ch;
		scanf("%c", &ch);
		if (ch == 'y')
			break;
	}
	
	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 1, IPC_RMID);
	msgctl(msqid, IPC_RMID, NULL);
	
	return 0;
}
