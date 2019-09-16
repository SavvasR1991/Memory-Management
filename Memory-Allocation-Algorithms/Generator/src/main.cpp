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
#include "Generator.h"
#include "Operations.h"

using namespace std;

int main (int argc, char** argv){
    int D,lo,hi,t,T,S;
    int check_algorithm_input = 0,algorithm,status,i,j;
    int shmid,semid;
    char stdin_input[MAX_CHAR],*pch,yes_no='p';
    pid_t pid,wpid;
    if(argc>7)
    {
        cout<<"->You have insert to many arguments..."<<endl;
        cout<<"->You must insert programm like this :"<<endl;
        cout<<"->   *\"./Memory_Placement [D] [lo] [hi] [t] [T] [S]\" "<<endl;
        cout<<"->       -D  : Total programm time execution"<<endl;
        cout<<"->       -lo : Low limit of VP-prosseces"<<endl;
        cout<<"->       -hi : High limit of VP-prosseces"<<endl;
        cout<<"->       -t  : Time duration for Poisson"<<endl;
        cout<<"->       -T  : Average duration"<<endl;
        cout<<"->       -S  : Size of memory"<<endl;
        return 0;
    }
    if(argc!=7)
    {
        cout<<"->You have insert wrong arguments..."<<endl;
        cout<<"->You must insert programm like this :"<<endl;
        cout<<"->   *\"./Memory_Placement [D] [lo] [hi] [t] [T] [S]\" "<<endl;
        cout<<"->       -D  : Total programm time execution"<<endl;
        cout<<"->       -lo : Low limit of VP-prosseces"<<endl;
        cout<<"->       -hi : High limit of VP-prosseces"<<endl;
        cout<<"->       -t  : Time duration for Poisson"<<endl;
        cout<<"->       -T  : Average duration"<<endl;
        cout<<"->       -S  : Size of memory"<<endl;
        return 0;
    }
    D  = atoi(argv[1]);
    lo = atoi(argv[2]);
    hi = atoi(argv[3]);
    t  = atoi(argv[4]);
    T  = atoi(argv[5]);
    S  = atoi(argv[6]);
    if(lo<=0||D<=0||hi<=0||t<=0){
        cout<<"->Error ,no element must be smaller than zero ..."<<endl;
        return 0;
    }
    if(lo>hi){
        cout<<"->Error ,low limit must be smaller than hi limit (lo)<(hi) ..."<<endl;
        return 0;
    }
    cout<<"->                         _______________  "<<endl;
    cout<<"-> |-----------------------You have insert----------------------------|"<<endl;
    cout<<"->   -D  : Total programm time execution ,"<<D<<" Time quantum"<<endl;
    cout<<"->   -lo : Low limit of VP-prosseces ,    "<<lo<<" Limit"<<endl;
    cout<<"->   -hi : High limit of VP-prosseces ,   "<<hi<<" Limit"<<endl;
    cout<<"->   -t  : Time duration for Poisson ,    "<<t<<" Time quantum"<<endl;
    cout<<"->   -T  : Average duration ,             "<<T<<" Time quantum"<<endl;
    cout<<"->   -S  : Size of memory ,               "<<S<<" Bytes"<<endl;
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
        generator(D,t,lo,hi,T,S);
    }
    while ((wpid = wait(&status)) > 0);  
    cout<<"->Free semaphores and shared memories..."<<endl;
    cout<<"->|-----------------------Programm Ends--------------------------------|"<<endl;
            
    return 0;
}
