#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include <time.h>    
#include "Operations.h"
#include "Waiting_List.h"
#include <fstream>

  
using namespace std;

void memory_manager(int,int,int);

#endif