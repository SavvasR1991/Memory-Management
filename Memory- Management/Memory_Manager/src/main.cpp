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
#include "Memory_Manager.h"
#include "Operations.h"

using namespace std;

int main (int argc, char** argv){
    int D,lo,hi,t,T,S,*memory;
    int check_algorithm_input = 0,algorithm,status,i,j;
    int shmid,shmid2,semid;
    char stdin_input[MAX_CHAR],*pch,yes_no='p';
    pid_t pid,wpid;
    shm_communication *memory_data;
    senum arg;
    if(argc>3)
    {
        cout<<"->You have insert to many arguments..."<<endl;
        cout<<"->You must insert programm like this :"<<endl;
        cout<<"->   *\"./Memory_Placement [S]\" "<<endl;
        cout<<"->       -S  : Total bytes for memory"<<endl;
        cout<<"->       -D  : Duration"<<endl;
        return 0;
    }
    if(argc!=3)
    {
        cout<<"->You have insert wrong arguments..."<<endl;
        cout<<"->You must insert programm like this :"<<endl;
        cout<<"->   *\"./Memory_Placement [S]\" "<<endl;
        cout<<"->       -S  : Total bytes for memory"<<endl;
        cout<<"->       -D  : Duration"<<endl;
        return 0;
    }
    S  = atoi(argv[1]);
    if(S<=0){
        cout<<"->Error ,no element must be smaller than zero ..."<<endl;
        return 0;
    }
    D  = atoi(argv[2]);
    cout<<"->Choose algorithm for Memory Placement ..."<<endl;
    cout<<"->   * Best Fit : [Best]"<<endl;
    cout<<"->   * Next Fit : [Next]"<<endl;
    cout<<"->   * Buddy    : [Buddy]\n-> ";
    while(check_algorithm_input ==0){
        fgets(stdin_input,MAX_CHAR,stdin); 
        pch = strtok (stdin_input,"\n");    
        if((strcmp(pch,"Best")==0)||((strcmp(pch,"best")==0))){   
            check_algorithm_input = 1;
            algorithm = 1;
            break;
        }
        if((strcmp(pch,"Next")==0)||((strcmp(pch,"next")==0))){   
            check_algorithm_input = 1;
            algorithm = 2;
            break;
        }
        if((strcmp(pch,"Buddy")==0)||((strcmp(pch,"buddy")==0))){   
            check_algorithm_input = 1;
            algorithm = 3;
            break;
        }
        cout<<"Error ,choose algorithm for Memory Placement ..."<<endl;
        cout<<"->   * Best Fit : [Best]"<<endl;
        cout<<"->   * Next Fit : [Next]"<<endl;
        cout<<"->   * Buddy    : [Buddy]\n-> ";
    }
    cout<<"->                      _______________  "<<endl;
    cout<<"-> |--------------------You have insert---------------------------|"<<endl;
    cout<<"->   -S  : Total bytes for memory,        "<<S<<" Bytes"<<endl;
    cout<<"->   -D  : Duration,                      "<<D<<" Seconds"<<endl;
    if(algorithm==1){
        cout<<"->   -A  : Algorithm for memory  ,        Best Fit"<<endl;
    }
    if(algorithm==2){
        cout<<"->   -A  : Algorithm for memory  ,        Next Fit"<<endl;
    }
    if(algorithm==3){
        cout<<"->   -A  : Algorithm for memory  ,        Buddy"<<endl;
    }
    cout<<"->|-----------------------Programm Starts------------------------------|"<<endl;
    while(yes_no!='n'&&yes_no!='N'&&yes_no!='y'&&yes_no!='Y'){
        cout<<"->Continue ? [y/n] :";
        cin >> yes_no;
        if(yes_no=='n'||yes_no=='N'){
            cout<<"->|-----------------------Programm Ends--------------------------------|"<<endl;
            return 0;
        }
    }
    pid = fork();
    if(pid==0)
    {
        memory_manager(algorithm,S,D);
    }
    while ((wpid = wait(&status)) > 0);  
    cout<<"->Free semaphores and shared memories..."<<endl;
    cout<<"->|-----------------------Programm Ends--------------------------------|"<<endl;
            
    return 0;
}
