#ifndef ROASTER_H
#define ROASTER_H

#define USERS_LIMIT 1000

typedef struct {
	int pid;
	char name [256];
} Roaster;

void serverInit(Roaster ** roaster);
void addToRoaster(Roaster ** roaster, int pid, char* name);
void removeFromRoaster(Roaster ** roaster, int pid);
void showRoaster(Roaster * roaster, int pid);

#endif
