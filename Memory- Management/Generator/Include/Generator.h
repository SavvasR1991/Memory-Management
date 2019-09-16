#ifndef GENERATOR_H
#define GENERATOR_H

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
#include <pthread.h>
#include "Poisson.h"
#include "Operations.h"
#include "Virtual_Process.h"
#include "Register_Table.h"

using namespace std;

void generator(int,int,int,int,int,int);

#endif