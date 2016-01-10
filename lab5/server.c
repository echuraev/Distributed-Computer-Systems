#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define TCP 0
#define UDP 1
#define TIME_SHIFT 2208988800

void parse_params(int argc, char ** argv, int * protocol, unsigned int * port);

int main (int argc, char ** argv)
{
	int protocol;
	unsigned int port;
	parse_params(argc, argv, &protocol, &port);
	printf("protocol: %i\nport: %d\n", protocol, port);
	
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr, clt_addr; 

	unsigned int sendBuff;
	time_t ticks; 
	if (protocol == TCP)
	{
    	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	}
	else
	{
		listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	}
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	
	if (protocol == TCP)
    	listen(listenfd, 10); 

    while(1)
    {
		ticks = time(NULL);
		ticks += TIME_SHIFT;
		sendBuff = htonl((unsigned int)ticks);
		if (protocol == TCP)
		{
        	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        	
       		write(connfd, &sendBuff, sizeof(sendBuff)); 

        	close(connfd);
        	sleep(1);
		}
		else
		{
			int clt_len = sizeof(clt_addr);
			if (recvfrom(listenfd, NULL, 0, 0, (struct sockaddr*)&clt_addr, &clt_len)==-1)
				err("recvfrom()");
			sendto(listenfd, &sendBuff, sizeof(sendBuff), 0, (struct sockaddr*)&clt_addr, sizeof(clt_addr));
		}
     }	

	return 0;
}

void parse_params(int argc, char ** argv, int * protocol, unsigned int * port)
{
	if (argc < 4)
	{
		printf("\nUse: %s [Oprions] -p port_num\n", argv[0]);
		printf("Option:\n");
		printf("\t-u\tSet UDP protocol\n");
		printf("\t-t\tSet TCP protocol\n");
		exit(0);
	}
	if (strcmp(argv[1], "-t") == 0)
		*protocol = TCP;
	else if (strcmp(argv[1], "-u") == 0)
		*protocol = UDP;
	else
	{
		perror("Error! Protocol wasn't recognize\n");
		exit(1);
	}
	if (strcmp(argv[2], "-p"))
	{
		perror("Error! Key for port not found!\n");
	}
	char * port_str = malloc(sizeof(char)*10);
	
	strcpy(port_str, argv[3]);
	*port = atoi(port_str);

	free(port_str);
}
