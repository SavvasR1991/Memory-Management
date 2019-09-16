#include "Register_Table.h"

/*********************creator********************/
Register_Table::Register_Table(){
    cout <<"->Register table has been created"<<endl; //Destructor method
    head=NULL;                              //Initialize head to NULL
    curr=NULL;                              //Initialize curr to NULL
    endd=NULL;                              //Initialize endd to NULL
    temp=NULL;
    total_arrivals = 0;
}
/*********************distractor********************/
Register_Table::~Register_Table(){
    cout <<"->Register table has been delete"<<endl; //Destructor method
}

/*********************eisagwgh kombou********************/
void Register_Table::insert_to_register_table(pid_t pid,int arrival,int hits,int duration,int delay,int key,int **hits_table,int active,int memory_size,int sem_virtual_process){
    int i,ii;
    Register_Table_Element* n =new Register_Table_Element;         //Create a pointer that points to a node
    n->hits_table = new int *[hits];
    for( ii = 0; ii < hits;ii++)
        n->hits_table[ii] = new int[2];
    n->next = NULL;
    n->memory_size = memory_size;
    n->process_pid = pid;
    n->sem_virtual_process = sem_virtual_process;
    n->arrival = arrival;
    n->hits = hits;
    n->swap_in = 0;
    n->swap_out = 0;
    n->waiting_time = 0;
    n->duration = duration;
    n->delay = delay;
    n->active = active;
    n->current_hit = 0;
    for(i=0;i<hits;i++){
        n->hits_table[i][0] = hits_table[i][0];
        n->hits_table[i][1] = hits_table[i][1];
    }
    if(head != NULL){
        curr = head;
        while(curr->next != NULL){
            curr = curr->next;
            endd=curr;
        }
        curr -> next = n;
    }
    else{
        head = n ;
    }
    total_arrivals++;
}

/*********************diagrafh prwtou kombou**********************/
int Register_Table::delete_from_register_table(){
    int key;
    if(head!=NULL){
        curr = head->next;
        key = head->key;
        if(curr==NULL){
            delete head;
            head = NULL;
        }
        else{
            delete head;
            head = curr;
        }
        total_arrivals--;
    }
    return key;
}

/*********************elenxos adeia listas********************/
int Register_Table::check_register_table_empty(){
    if(head==NULL){
        return 0;
    }
    return 1;
}

/*********************organiser********************/
void Register_Table::organiser_register_table(int second){
    if(head!=NULL){
        curr = head;
        while(curr!= NULL){
            if(curr->active!=-1){
                if(curr->active == 1){
                    curr->hits_table[curr->current_hit][0]--;
                    if(curr->hits_table[curr->current_hit][0]<=0){
                        curr->active = 3;
                    }
                    if(curr->current_hit>=curr->hits){
                        curr->active = -1;
                    }
                }
                else if(curr->active == 3){
                    curr->active = 4;
                }
                if(curr->active == 4){
                    curr->hits_table[curr->current_hit][1]--;
                    if(curr->hits_table[curr->current_hit][1]<=0){
                        curr->active = 2;
                        curr->current_hit++;
                    }
                    if(curr->current_hit>=curr->hits){
                        curr->active = -1;
                    }
                }
                if(curr->active == 0){
                    curr->waiting_time++;
                }
            }
            else{
                curr->finish_time = second;
                curr->active = -2;
            }
            curr = curr->next;
        }
    }
}

/*********************des posoi einai anenergoi********************/
void Register_Table::check_disactive_process(int pid){
    if(head!=NULL){
        curr = head;
        while(curr != NULL){
            if(curr->process_pid==pid){
                curr->active = 1;
                break;
            }
            curr = curr->next;
        }
    }
}

/*********************diagrafh register********************/
void Register_Table::delete_register_table(){
    if(head!=NULL){
        curr = head;
        while(curr->next != NULL){
            temp=curr;
            curr = curr->next;
            delete temp;
        }
    }
    cout<<"->Free Register Table "<<endl;
}

/*********************ektupwsh register********************/
void Register_Table::print_register_table(){
    curr = head;
    int i =1,k,l=0,m=0;;
    int u = 0;
    if(curr == NULL){
        cout<<"->Register Table is empty"<<endl;
    }
    else{
        cout<<"->______________________________________________________________________________\n";
        cout<<"->|--------------------------------REGISTER TABLE-------------------------------|\n";
        cout<<"->|  NUM  |  PID  | ARRIVAL | FINISH |   HITS  |   STATUS  | WAITTING| TURN_ARR |\n";
        cout<<"->|-----------------------------------------------------------------------------|\n";
        while(curr!=NULL){
            printf("->|  %4d |  %d | %7d |",i,curr->process_pid,curr->arrival);
            if(curr->active == -4){
                printf("%7d | %3d/%3d |",curr->finish_time,curr->current_hit,curr->hits);
            }
            else{
                printf("  ----  | %3d/%3d |",curr->current_hit,curr->hits);
            }
            if(curr->active == 0){
                cout<<" STOPED WL |";
                m++;
            }
            else if(curr->active == -4){
                cout<<" FINISHED  |";
                u++;
            }
            else if(curr->active == 1){
                cout<<"  ACTIVE   |";
            }
            else if(curr->active == 4){
                cout<<"  SLEEPING |";
            }
            else {
                cout<<" WAITTING  |";
                l++;
            }
            printf("    %4d |",curr->waiting_time);
            if(curr->active == -4){
                printf("   %4d | \n",curr->finish_time - curr->arrival);
            }
            else{
                printf("  ----  |\n");
            }
            curr=curr->next;
            i++;
        }
        cout<<"->______________________________________________________________________________\n";
        cout<<"->|------------------------------------STATISTICS-------------------------------|\n";
        printf("->| *Percentile executed processes      :                %2.2f%%                 |\n",((float)u/(float)i)*100);
        printf("->| *Percentile non executed processes  :                %2.2f%%                 |\n",((float)(i-u)/(float)i)*100);
        printf("->| *Percentile waiting processes       :                %2.2f%%                  |\n",((float)l/(float)i)*100);
        printf("->| *Percentile stoped processes        :                %2.2f%%                   |\n",((float)m/(float)i)*100);
        printf("->| *Total processes                    :                %5d                 |\n",i);
        cout<<"->______________________________________________________________________________\n";
    }
}

/*********************arithmos anenergoi********************/
int Register_Table::number_of_active(){
    int i = 0;
    if(head!=NULL){
        curr = head;
        while(curr != NULL){
            if(curr->active==1){
                i++;
            }
            curr = curr->next;
        }
    }
    return i;
}

/*********************arithmos energoi********************/
int Register_Table::number_of_sleeping(){
    int i = 0;
    if(head!=NULL){
        curr = head;
        while(curr != NULL){
            if(curr->active==4){
                i++;
            }
            curr = curr->next;
        }
    }
    return i;    
}
        