#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "tree.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void memory_printer(int memory[],int size_memory){
    int i,memory_allocated,counter_zero;
    cout<<"->                           _____________"<<endl;
    cout<<"->__________________________|MEMORY STATUS|_________________________"<<endl;
    cout<<"->[";
    memory_allocated = 0;
    for(i=0;i<size_memory;i++){
        if(i%64==0&&i!=0){
            cout<<"] {"<<i<<"}"<<endl;
            if(i!=size_memory-1)
            cout<<"->[";
        }
        cout<<memory[i];
    }
    cout<<"] {"<<size_memory<<"}"<<endl;
    cout<<"->-------------------------------["<<(100*memory_allocated)/size_memory<<"]%%----------------------------"<<endl;
}
int main (int argc, char** argv) {
    char *pch,input[50],*pch1,file_name[20];
    int read,end_of_prog = 0,size,quantum;
    int a,i;
    float paranomastis,b;
    float grades_sum;
    char *line = NULL;
    int *memory,start, check;
    Tree buddy;
   
    printf("->Insert memory size...\n->");
    while(1){
        cin >> size;
        if(size%2!=0&&size<64){
            printf("->Insert memory size...\n->");
        }
        else{
            break;
        }
    }
    memory = new int [size];
    for(i=0;i<size;i++){
        memory[i] = 0;
    }
    buddy.initialize(size,32);
    printf("->-------------------------------------------------------|\n");
    printf("->                   _________________\n");
    printf("->                  |PROGRAMM COMMANDS|\n");
    printf("->-------------------------------------------------------|\n");
    printf("->To add process to memory    : [add] bytes              |\n");
    printf("->To delete process to memory : [delete] bytes           |\n");
    printf("->To exit programm press      : [exit]                   |\n");
    printf("->-------------------------------------------------------|\n");

    while(1)                                    //bronxos eisagwghs entolwn user
    {
        if(end_of_prog==1)                      //telos programmatos
        {
            break;
        }
        printf("\n->Insert your command...\n->");
        fgets(input,50,stdin);                  //stdin command
        pch = strtok (input," ,.-\n");          //analuse tis entoles
        while (pch != NULL)
        {                                       //entolh load
            if(strcmp(pch,"add")==0)
            {
                pch = strtok (NULL,"\n");
                if(pch==NULL)
                {
                    printf("->Error No value selected...\n");
                    break;
                }
				start= check=0;
				quantum = buddy.search_size(atoi(pch));
				check=buddy.check_quantum(quantum);
//				cout<<endl<<endl<<check<<endl<<endl<<endl<<endl;
				if (!check) {
					cout<<"->Tryng to Add "<<quantum<<" bytes\n";
					start = buddy.add_node(atoi(pch),quantum);
				}
                if(start>0){
                    for(i =start-1;i<start+atoi(pch);i++){
                        memory[i]=1;
                    }
                    memory_printer(memory,size);
                }
                else{
					if (check==-2)
						cout<<"->Cannot allocate this process size now. Try again later."<<endl;
					else if (check==-1) 						
						cout<<"->There is no space for this process size. Try with smaller process size."<<endl;
					//else 
						
                }
                break;                
            }
            else if(strcmp(input,"delete")==0)
            {
                pch = strtok (NULL,"\n");
                if(pch==NULL)
                {
                    printf("->Error No value selected...\n");
                    break;
                }
                buddy.delete_node(atoi(pch));
				cout<<"start point "<<buddy.start_value<<endl;
                if(start>0){
                    for(i =buddy.start_value;i<buddy.start_value+atoi(pch);i++){
                        memory[i]=0;
                    }
                    memory_printer(memory,size);
                }
                break;
            }  
            else if(strcmp(input,"exit")==0)
            {
                end_of_prog=1;
                break;
            }  
            else{
                printf("->Wrong input from command line...\n");
                printf("->-------------------------------------------------------|\n");
                printf("->                   _________________\n");
                printf("->                  |PROGRAMM COMMANDS|\n");
                printf("->-------------------------------------------------------|\n");
                printf("->To add process to memory    : [add] bytes              |\n");
                printf("->To delete process to memory : [delete] bytes           |\n");
                printf("->To exit programm press      : [exit]                   |\n");
                printf("->-------------------------------------------------------|\n");

                break;
            }
        }
    }
	
	buddy.empty(buddy.root);
	delete buddy.root;
	delete [] memory;
	return 0;
}