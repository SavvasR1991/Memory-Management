#include "Memory_Algorithms.h"

/*********************constructor********************/
Algorithm::Algorithm(){
    cout <<"->Algorithm has been created"<<endl; //Destructor method
}

/*********************distractor********************/
Algorithm::~Algorithm(){
    cout <<"->Algorithm has been delete"<<endl; //Destructor method
}

/*********************ulopoihsh algorithmwn********************/
int Algorithm::memory_algorithms(int algo, int memory[],int curr_search,int size_memory,int memory_allocated,int memory_size,int pid){
    int i,h,j,pointer_memory_fin = 0,k,tem_pointer_memory_start=0,min_difference = size_memory;
    int check_insert_memory = 0,pointer_memory_start = 0,at_least_one_free = 0;
    int counter_zero = 0,current,l,u;
    /*********************NEXT FIT**********************/
    if(algo == 1){
        for(i=0;i<size_memory;i++){
            if(memory[i]==0){
                if(counter_zero<1){
                    tem_pointer_memory_start = i;
                }
                counter_zero++;
                if(counter_zero==memory_size){
                    at_least_one_free = 1;
                    check_insert_memory = 1;
                }
            }
            else{
                if(counter_zero>0&&at_least_one_free==1){
                    if(counter_zero-memory_size<min_difference){
                        min_difference = counter_zero-memory_size;
                        pointer_memory_start = tem_pointer_memory_start;
                    }
                }
                counter_zero = 0;
                at_least_one_free = 0;
            }
            if(i==size_memory-1){
                if(counter_zero>0&&at_least_one_free==1){
                    if(counter_zero-memory_size<min_difference){
                        min_difference = counter_zero-memory_size;
                        pointer_memory_start = tem_pointer_memory_start;
                    }
                }
                counter_zero = 0;
                at_least_one_free = 0;
            }

        }
        if(check_insert_memory == 1){
            elements.x = pid;
            elements.y = pointer_memory_start;
            elements.z = memory_size;                            
        }
    }
    else if(algo == 2){
        /*********************BEST FIT**********************/
        for(i=0;i<size_memory;i++){
            if(memory[curr_search]==0){
                if(pointer_memory_fin<1){
                    pointer_memory_start = curr_search;
                }
                pointer_memory_fin++;
                if(pointer_memory_fin==memory_size){
                    for(k=pointer_memory_start;k<pointer_memory_start+pointer_memory_fin;k++){
                        memory_allocated++;
                    }
                    elements.x = pid;
                    elements.y = pointer_memory_start;
                    elements.z = memory_size;                            
                    check_insert_memory =1;
                    break;
                }
            }
            else{
                pointer_memory_fin=0;
            }
            curr_search ++;
            if(curr_search==size_memory){
                curr_search = 0;
                pointer_memory_fin=0;
            }
        }
        elements.a = curr_search;    
    }
    else if(algo == 3){
        /*********************BUDDY ALGORITHM********************/
        current = size_memory;
        l = 32;
        u = 128;
        while(current>=memory_size){
            current = power_of_two(current);
            
            
        }
        
    }
    else{
        
        cout<<"-> Error no algorithm exists .."<<endl;
    }

    return check_insert_memory;
    
    
};

/*********************ektupwsh mnhmhs********************/
void Algorithm::memory_printer(int memory[],int size_memory){
    int i,memory_allocated;
    cout<<"->                           _____________"<<endl;
    cout<<"->__________________________|MEMORY STATUS|_________________________"<<endl;
    cout<<"->[";
    memory_allocated = 0;
    for(i=0;i<size_memory;i++){
        if(memory[i]==1){
            memory_allocated++;
        }
        else{
            counter_zero++;
        }
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


int Algorithm::power_of_two(int size){
    
    int i =2;
    while(i!=size){
        i=i*2;
    }
    i=i/2;
    return i;
    
    
}
