#ifndef POISSON_H
#define POISSON_H

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
#include <stdio.h>              // Needed for printf()
#include <math.h>
 
using namespace std;

int poisson(double);

double expon(double);

double rand_val(int);

#endif