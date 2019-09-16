#ifndef WAITING_LIST
#define WAITING_LIST

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

typedef struct Hits_Table{
    int number_hits;
    int duration_hits;
}Hits_Table;

class Waiting_List{
    public:
        typedef struct Waiting_List_Element{
            pid_t process_pid;
            int arrival;
            int duration;
            int key;
            Hits_Table *pointer_hits_table;
            int *hits_table;
            int delay;
            int size_process; 
            Waiting_List_Element *next;
        }Waiting_List_Element;
        Waiting_List_Element first_out_from_list;
        Waiting_List_Element *head;           //Pointer that points to the head of the chain
        Waiting_List_Element *endd;           //Pointer that points to the end of the chain
        Waiting_List_Element *curr;           //Pointer that points to one current node of the chain
        Waiting_List_Element *temp;           //Pointer that points to one current node of the chain
        int total_arrivals;         
    public:
        Waiting_List();                         //Constructor method
        ~Waiting_List();                        //Destructor  method
        void insert_to_waiting_list(pid_t,int,int,key_t,int,int);
        void delete_from_waiting_list();
        int check_list_empty();
        void delete_waiting_list();
        void print_list();
        void organiser_register_table();
};


#endif