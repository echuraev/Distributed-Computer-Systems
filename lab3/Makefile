# Makefile

.PHONY: all

all: server client

server: server.c roaster.c
	$(CC) $^ -o $@

client: client.c roaster.c
	$(CC) $^ -lpthread -o $@

clean:
	$(RM) server client
	
