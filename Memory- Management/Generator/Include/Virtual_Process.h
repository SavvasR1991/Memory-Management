#ifndef VIRTUAL_PROCESS
#define VIRTUAL_PROCESS

#include <cstdio>
#include <iostream>
#include <cstdlib>
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
#include <math.h>
#include "Operations.h"

using namespace std;

void virtual_process(int,int);


#endif