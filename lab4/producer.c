#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#define BLOCK_SIZE 8
#define NUM_THREADS 10
pthread_mutex_t m_file = PTHREAD_MUTEX_INITIALIZER;

char * strrev(char * str)
{
	int len = strlen(str);
	char * tmp_str = (char *) malloc(sizeof(char)*len);
	strcpy(tmp_str, str);
	int i;
	for (i = 0; i < (int)(len/2); ++i)
	{
		tmp_str[i] = str[len-1-i];
		tmp_str[len-1-i] = str[i];
	}
	return tmp_str;
}

typedef struct
{
	FILE * file;
	char * str;
	int num;
} thread_data;

void *threadTask(void *arg)
{
	thread_data * data;
	data = (thread_data *) arg;
	printf("threadTask run. Thread # %d\n", data->num);
	char * str = strrev(data->str);
	printf("%d\n", data->file);
	pthread_mutex_lock (&m_file);
	//printf("%d start\n", data->num);
	fwrite (str, sizeof(char), strlen(str), data->file);
	//printf("%d end\n", data->num);
	pthread_mutex_unlock (&m_file);
	
	return NULL;
}

int main (int argc, char ** argv)
{
	if (argc < 3)
	{
		printf("\n Use: %s <input_file> <output_file>\n\n", argv[0]);
		exit(1);
	}
	char * in_name = argv[1];
	char * out_name = argv[2];
	
	FILE* in = fopen(in_name, "rt");
	if (in == NULL)
	{
		perror("in open error!");
		exit(2);
	}
	FILE* out = fopen(out_name, "wt");
	if (out == NULL)
	{
		perror("out open error!");
		exit(2);
	}
	
	fseek(in, 0, SEEK_END);
	int file_size = ftell(in);
	fseek(in, 0, SEEK_SET);
	//printf("file size: %d\n", file_size);	
	char * in_buf = (char*)malloc(sizeof(char)*file_size);
	fread(in_buf, sizeof(char), file_size, in);
	
	close(in);

	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	thread_data tdata[NUM_THREADS];
	printf("%d\n", out);
	int i, j, k;
	for (j = 0; j < (file_size/BLOCK_SIZE); ++j)
	{
		int run_threads = 0;
		/* create all threads */
		for (i = 0; i < NUM_THREADS && (j*BLOCK_SIZE*NUM_THREADS + i*BLOCK_SIZE) < BLOCK_SIZE*(file_size/BLOCK_SIZE); ++i) 
		{
			tdata[i].file = out;
			tdata[i].num = i;
			char tmp_str [BLOCK_SIZE];
			for (k = 0; k < BLOCK_SIZE; ++k)
			{
				tmp_str[k] = in_buf[j*BLOCK_SIZE*NUM_THREADS + i*BLOCK_SIZE + k];
			}
			tdata[i].str = tmp_str;
			printf("In main: creating thread %d\n", i);
			int rc = pthread_create(&threads[i], NULL, threadTask, (void *) &tdata[i]);
			assert(rc == 0);
			run_threads++;
		}
	
		/* wait for all threads to complete */
		for (i=0; i < run_threads; ++i) 
		{
			int rc = pthread_join(threads[i], NULL);
			assert(0 == rc);
		}
	}
	close(out);

	return 0;
}
