#ifndef _WORKERS_H_
#define _WORKERS_H_

#define READ 0
#define WRITE 1
#define MAXEXP 1024
#define WFREE 1
#define WBUSY 0

typedef struct {
	int pid;
	int fd1 [2];
	int fd2 [2];
	int status;
} Workers;

static int workers_count = 0;

#endif
