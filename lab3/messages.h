#ifndef MESSAGES_H
#define MESSAGES_H

#define MSGLEN 1000

typedef struct mymsgbuf
{
	long type;
	struct 
	{
		long frompid;
		char name[256];
		char msg[MSGLEN];
	} info;
}
Message;

#endif
