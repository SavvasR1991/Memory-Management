#ifndef MEMORY_ALGORITHMS_H
#define MEMORY_ALGORITHMS_H

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
#include <fstream>


using namespace std;

extern  long long int counter_zero;
extern  long long int diakumansh;
extern  long long int diakumansh_a;
extern  long long int diakumansh_b;
extern  long long int counter_memory_status;
extern  long long int expected_value;

class Algorithm{
    public:
        typedef struct Three_Elements{
            int x;
            int y;
            int z;
            int a;
        }Three_Elements;
        Three_Elements elements;
    public:
        Algorithm();                         //Constructor method
        ~Algorithm();      
        int memory_algorithms(int,int [],int,int,int,int,int);
        void memory_printer(int [],int);
        int power_of_two(int);
};
#endif
