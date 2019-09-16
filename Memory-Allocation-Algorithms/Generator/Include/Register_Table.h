#ifndef REGISTER_TABLE
#define REGISTER_TABLE

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
   
using namespace std;

class Register_Table{
    public:
        typedef struct Register_Table_Element{
            pid_t process_pid;
            int arrival;
            int memory_size;
            int duration;
            int delay;
            int key;
            int hits;
            int **hits_table;
            int current_hit;
            int waiting_time;
            int sem_virtual_process;
            int active;
            int finish_time;
            int swap_in;
            int swap_out;
            Register_Table_Element *next;
        }Register_Table_Element;

        Register_Table_Element *head;           //Pointer that points to the head of the chain
        Register_Table_Element *endd;           //Pointer that points to the end of the chain
        Register_Table_Element *curr;           //Pointer that points to one current node of the chain
        Register_Table_Element *temp;           //Pointer that points to one current node of the chain
        int total_arrivals;     
        int total_active;
        int total_sleeping;
    public:
        Register_Table();                         //Constructor method
        ~Register_Table();                        //Destructor  method
        void insert_to_register_table(pid_t,int,int,int,int,int,int**,int,int,int);
        int delete_from_register_table();
        int check_register_table_empty();
        int number_of_active();
        int number_of_sleeping();
        void delete_register_table();
        void print_register_table();
        void organiser_register_table(int);
        void check_disactive_process(int);
};


#endif