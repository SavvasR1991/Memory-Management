#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>

#define MAX_CHAR 100

#define SEM_GENERATOR (key_t) 4444
#define SEM_MANAGER  (key_t) 4445
#define SEM_ORGANISER (key_t) 4446

#define SHM_ORG_GEN_MAN (key_t) 2222
#define SHM_GEN_MAN_ACTIVE_ST (key_t) 2223
#define SHM_GEN_MAN_COM (key_t) 2224
#define SHM_GEN_MAN_MEM_IN (key_t) 2225
#define PERMS 0666

union senum
{
    int val;
    struct semid *buf;
    unsigned short *semarray;
};

typedef struct shm_communication_insert_memory{
   int activation_process; 
}shm_communication_insert_memory;

typedef struct shm_communication_waiting_list{
   int activation_process; 
   int pid;
}shm_communication_waiting_list;

typedef struct shm_communication
{
    char message[MAX_CHAR];
    int pid;
    int memory_size;
    int arrival;
    int duration;
    int delay;
    int *hits_array;
    int key;
    int hits;
}shm_communication;

typedef struct memory_manager_info_table{
    int pid;
    int memory_size;
    int possition_pointer;
}memory_manager_info_table;

void down(int);

void up(int);


#endif
