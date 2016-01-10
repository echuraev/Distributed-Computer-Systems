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
#include <time.h>

#define TCP 0
#define UDP 1
#define TIME_SHIFT 2208988800

void parse_params(int argc, char ** argv, int * protocol, char ** address, unsigned int * port);

int main (int argc, char ** argv)
{
	int protocol;
	char * address;
	unsigned int port;
	parse_params(argc, argv, &protocol, &address, &port);
	printf("protocol: %i\naddress: %s\nport: %d\n", protocol, address, port);
	
	int sockfd = 0;
    unsigned int recvBuff;
    struct sockaddr_in serv_addr; 
	if (protocol == TCP)
	{
    	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
    	    perror("Socket error!\n");
       		return 1;
    	}
	}
	else
	{
		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
    	    perror("Socket error!\n");
       		return 1;
    	}

	}

	struct hostent *he;

	/* resolve hostname */
	if ((he = gethostbyname(address)) == NULL ) {
		perror("Error! Hostname resolving!"); 
		exit(2); /* error */
	}

	/* copy the network address to sockaddr_in structure */
	memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 
	int serv_len = sizeof(serv_addr);
	if (protocol == TCP)
	{
    	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
       		printf("\n Error : Connect Failed \n");
       		return 1;
    	}
	}
	else
	{
		sendto(sockfd, NULL, 0, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	}

	int n;
	if (protocol == TCP)
	{
    	while ( (n = read(sockfd, &recvBuff, sizeof(recvBuff))) > 0)
    	{
			unsigned int val = ntohl(recvBuff);
			time_t time_string = (time_t)val - TIME_SHIFT;
			printf("%.24s\n", ctime(&time_string));
    	}
	}
	else
	{
 		recvfrom(sockfd, &recvBuff, sizeof(recvBuff), 0, (struct sockaddr *)&serv_addr, &serv_len);
    	unsigned int val = ntohl(recvBuff);
		time_t time_string = (time_t)val - TIME_SHIFT;
		printf("%.24s\n", ctime(&time_string));
	}

    if(n < 0)
    {
        printf("\n Read error \n");
    } 	

	free(address);
	return 0;
}

void parse_params(int argc, char ** argv, int * protocol, char ** address, unsigned int * port)
{
	if (argc < 3)
	{
		printf("\nUse: %s [options] <address>[:port]\n", argv[0]);
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
		perror("Protocol wasn't recognize\n");
		exit(1);
	}
	char * addr = malloc(sizeof(char)*strlen(argv[2]));
	char * port_str = malloc(sizeof(char)*10);
	int j = 0;
	int separator = 0;
	int i;
	for (i = 0; i < strlen(argv[2]); ++i)
	{
		if (separator == 0)
		{
			if (argv[2][i] == ':')
			{
				separator = 1;
				addr[i] = '\0';
				continue;
			}
			addr[i] = argv[2][i];
		}
		else
		{
			port_str[j++] = argv[2][i];
		}
	}
	port_str[j] = '\0';
	if (separator == 0)
		*port = 0;
	else
		*port = atoi(port_str);
	*address = addr;
	free(port_str);
}
