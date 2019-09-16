#include "Memory_Manager.h"
#include "Memory_Algorithms.h"

long long int counter_zero = 0;
long long int counter_memory_status = 0;
long long int diakumansh = 0;
long long int diakumansh_a = 0;
long long int diakumansh_b = 0;
long long int expected_value = 0;
int e_counter = 0;

/*********************synarthsh gia grapsimo ths mnhmhs sto katallhlo arxeio********************/
void memory_write_to_file(ofstream &fd,int memory[],int size_memory){
    int i,memory_allocated;
    fd<<"->                           _____________"<<endl;
    fd<<"->__________________________|MEMORY STATUS|_________________________"<<endl;
    fd<<"->[";
    memory_allocated = 0;
    for(i=0;i<size_memory;i++){
        if(memory[i]==1){
            memory_allocated++;
        }
        else{
            counter_zero++;
        }
        if(i%64==0&&i!=0){
            fd<<"] {"<<i<<"}"<<endl;
            if(i!=size_memory-1)
            fd<<"->[";
        }
        fd<<memory[i];
    }
    fd<<"] {"<<size_memory<<"}"<<endl;
    fd<<"->-------------------------------["<<(100*memory_allocated)/size_memory<<"]%%----------------------------"<<endl;
}
/////////////////////////////////////////////////////////
//////////////////   MANAGER  PROCESS ///////////////////
/////////////////////////////////////////////////////////
/*********Diegrasia gia thn topothetish vp sthn mnhmh***************/
void memory_manager(int algo,int size_memory,int duration){
    int shmid,pid2,c,o,cc,semid_generator,shmid3,u,semid_manager,shmidvp,pid,size_pro,clock=0,*hits_array,best_start,best_end;
    char array_mess[100],check_active_vp,memory_allocated = 0,cwd[100];
    int *memory,*memory_data_wait_list,extract_from_list,semid_waiting_list2,semid_waiting_list,i,k,j,shmid2,memory_size,check_insert_memory,curr_effort,best_effort;
    int pointer_memory_start,pointer_memory_fin,flag,curr_search;
    int vp_duration,arrival,delay,key,repeats,*memory_data_vp;
    int *buddy_array;
    int processes_in_memory_table[size_memory][3],waiting_pid,semid_organiser;
    float E;
    shm_communication* memory_data;
    shm_communication_insert_memory *memory_data_ins;
    Algorithm algorithm;

    Waiting_List list;
    ofstream VP_Information_File,VP_Information_File_Status,Memory_Information_File_Status;
    senum arg;
    memory = new int [size_memory];
    buddy_array = new int [size_memory];
     
    ///////////////////////////////////////////////////////////////////////////////////
    ////////////// Shared memories arxikopoihseis ,attaching kai semaphores ///////////
    ///////////////////////////////////////////////////////////////////////////////////
    semid_generator = semget(SEM_GENERATOR,1,PERMS|IPC_CREAT);
    semid_manager = semget(SEM_MANAGER,1,PERMS|IPC_CREAT);
    semid_organiser = semget(SEM_ORGANISER,1,PERMS|IPC_CREAT);

      
    if ((shmid = shmget(SHM_GEN_MAN_COM,sizeof(shm_communication), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,communucation...\n";
        exit(EXIT_FAILURE);
    }   
    if ((shmid2 = shmget(SHM_GEN_MAN_MEM_IN,sizeof(shm_communication_insert_memory), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,insert memory...\n";
        exit(EXIT_FAILURE);
    }
    if ((shmidvp = shmget(SHM_GEN_MAN_ACTIVE_ST,sizeof(int)*(size_memory), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,active...\n";
        exit(EXIT_FAILURE);
    }
    if ((shmid3 = shmget(SHM_ORG_GEN_MAN,sizeof(int)*(duration/2), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,disactive...\n";
        exit(EXIT_FAILURE);
    }
    memory_data_ins =(shm_communication_insert_memory *)shmat(shmid2, (void *)0, 0);
    if (memory_data_ins == (shm_communication_insert_memory *)(-1)) {
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    memory_data =(shm_communication *)shmat(shmid, (void *)0, 0);
    if (memory_data == (shm_communication *)(-1)) {
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    
    memory_data_vp =(int *)shmat(shmidvp, (void *)0, 0);
    if (memory_data_vp == (int *)(-1)) {
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    memory_data_wait_list =(int *)shmat(shmid3, (void *)0, 0);
    if (memory_data_wait_list == (int *)(-1)) {
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    arg.val=1;
    semctl(semid_generator,0,SETVAL,arg); 
    arg.val=1;
    semctl(semid_manager,0,SETVAL,arg);
    arg.val=1;
    semctl(semid_organiser,0,SETVAL,arg);  
    cout<<"->Process Memory Manager created ,id ["<<getpid()<<"]..."<<endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////

/**************************arxikopoihsh sharge mem,files ,opening file k.t.l***********************/
    strcpy(memory_data->message,"NULL");
    for(i=0;i<size_memory;i++){
        memory[i] = 0;
        buddy_array[i] = 0;
        memory_data_vp[i] = -1;
        for(j=0;j<3;j++){
            processes_in_memory_table[i][j]=0;
            
        }
    }
    for(i=0;i<duration/2;i++){
        memory_data_wait_list[i] = -1 ;
    }
    memory_data_vp[i] = 1;
    pointer_memory_start = 0;
    getcwd(cwd, sizeof(cwd));
    strcat(cwd,"/Information File/VP_Information_File.txt");
    VP_Information_File.open (cwd);
    getcwd(cwd, sizeof(cwd));
    strcat(cwd,"/Information File/VP_Information_File_Status.txt");
    VP_Information_File_Status.open (cwd);
    getcwd(cwd, sizeof(cwd));
    strcat(cwd,"/Information File/Memory_Information_File_Status.txt");
    Memory_Information_File_Status.open (cwd);
    VP_Information_File <<"    ***INFORMATION FOR VP_PROCESSES AND MEMORY ***\n";
    VP_Information_File_Status <<"    ***INFORMATION FOR VP_PROCESSES AND MEMORY STATUS***\n";
    Memory_Information_File_Status <<"    ***INFORMATION MEMORY STATUS***\n";
    if(algo == 1){
        VP_Information_File <<"\n --> BEST FIT ALGORITHM SELECTED\n\n";
        Memory_Information_File_Status <<"\n --> BEST FIT ALGORITHM SELECTED\n\n";

    }
    if(algo == 2){
        VP_Information_File <<" \n --> NEXT FIT ALGORITHM SELECTED\n\n";
        Memory_Information_File_Status <<" \n --> NEXT FIT ALGORITHM SELECTED\n\n";

    }
    if(algo == 3){
        VP_Information_File <<"\n --> BUDDY FIT ALGORITHM SELECTED\n\n";
        Memory_Information_File_Status <<"\n --> BUDDY FIT ALGORITHM SELECTED\n\n";

    }
    VP_Information_File <<"PID   SIZE  ARRIVAL TERMINAL TOTAL_EX_T TURNAROUND\n";
/***************************************************************************************************/
   
    ///////////////////// ENARKSH EKTELESHS MANAGER ///////////////////////
    curr_search = 0;
    do{
        extract_from_list = 0;
        if(list.check_list_empty()==1){ //elegxos an uparxei kapoios sthn lista
            list.delete_from_waiting_list();
            check_insert_memory = 0;
            pointer_memory_fin = 0;
            pid =  list.first_out_from_list.process_pid;
            memory_size = list.first_out_from_list.size_process;
            arrival =  list.first_out_from_list.arrival;
            vp_duration =list.first_out_from_list.duration;
            delay = list.first_out_from_list.delay;
            key = list.first_out_from_list.key;
            check_insert_memory = algorithm.memory_algorithms(algo,memory,curr_search,size_memory,memory_allocated,memory_size,pid);
            curr_search =  algorithm.elements.a; //des ama mpainei sthn mnhmh des
            if(check_insert_memory==0){
                //cout<<"->Process :["<<pid<<"] with size "<<memory_size<<" bytes from waiting list , goes to waiting list."<<endl;
                list.insert_to_waiting_list(pid,arrival,vp_duration,key,delay,memory_size);
                list.organiser_register_table();    //den xwrese
            }
            else{
                cout<<"->Process :["<<pid<<"] with size "<<memory_size<<" bytes from waiting list , execution in memory."<<endl;
                
                for(k=0;k<size_memory;k++){     //xwrese
                    if(processes_in_memory_table[k][0]==0){
                        processes_in_memory_table[k][0] = algorithm.elements.x;
                        processes_in_memory_table[k][1] = algorithm.elements.y;
                        processes_in_memory_table[k][2] = algorithm.elements.z;
                        break;
                    }
                }
                for(k=algorithm.elements.y;k<algorithm.elements.y+algorithm.elements.z;k++){
                    memory[k] = 1;          //grapse sthn mnhmh
                }
                counter_memory_status++;
                counter_zero=counter_zero +(size_memory - k);
                diakumansh_b = diakumansh_b+counter_zero*counter_zero;
                e_counter = e_counter+memory_size*vp_duration;
                algorithm.memory_printer(memory,size_memory);
                down(semid_organiser);      //enhmerwse ton generator
                    o = 1;
                    memory_data_wait_list[0] = 1;
                    while(memory_data_wait_list[o]!=-1){
                        o++;
                    }
                    memory_data_wait_list[o] = pid;
                up(semid_organiser);
            }
        }
        down(semid_manager);
            strcpy(array_mess,memory_data->message);
            if(strcmp(array_mess,"TERM")==0){       //termatise
                break;
            }
            if(strcmp(array_mess,"VP_Start")==0){
                for(u=0;u<100;u++)          //irthre afiksh 
                    cout<<"->"<<endl;
                //check_insert_memory = 0;
                pointer_memory_fin = 0;
                pid =  memory_data->pid;
                memory_size = memory_data->memory_size;
                arrival =  memory_data->arrival;
                vp_duration =memory_data->duration;
                delay = memory_data->delay;
                key = memory_data->key;
                repeats = memory_data->hits;
                strcpy(memory_data->message,"NULL");
                cout<<"->Process :("<<pid<<")->["<<memory_size<<" bytes], arrived ["<<arrival<<" sec], duration ["<<vp_duration<<" sec]"<<endl;
                VP_Information_File_Status<<"->Process :("<<pid<<")->["<<memory_size<<" bytes], arrived ["<<arrival<<" sec], duration ["<<vp_duration<<" sec]"<<endl;
                check_insert_memory = algorithm.memory_algorithms(algo,memory,curr_search,size_memory,memory_allocated,memory_size,pid);
                curr_search =  algorithm.elements.a;    //des ama xwraei
                if(check_insert_memory==0){                 //xwrese den
                    cout<<"->Process :["<<pid<<"], size "<<memory_size<<" bytes arrived at "<<arrival<<" sec goes to waiting list."<<endl;
                    VP_Information_File_Status<<"->     *Process :["<<pid<<"], size "<<memory_size<<" bytes arrived at "<<arrival<<" sec goes to waiting list."<<endl;
                    list.insert_to_waiting_list(pid,arrival,vp_duration,key,delay,memory_size);
                    list.organiser_register_table();
                    memory_data_ins->activation_process = 0;
                }
                else{
                    for(k=0;k<size_memory;k++){             //xwrese
                        if(processes_in_memory_table[k][0]==0){
                            processes_in_memory_table[k][0] = algorithm.elements.x;
                            processes_in_memory_table[k][1] = algorithm.elements.y;
                            processes_in_memory_table[k][2] = algorithm.elements.z;
                            break;
                        }
                    }
                    cout<<"->Process :["<<pid<<"], size "<<memory_size<<" bytes arrived at "<<arrival<<" sec ";
                    cout<<" ,execution in memory ["<<processes_in_memory_table[k][1]<<" - "<<processes_in_memory_table[k][1]+processes_in_memory_table[k][2]<<"]."<<endl;
                    VP_Information_File_Status<<"->     *Process :["<<pid<<"], size "<<memory_size<<" bytes arrived at "<<arrival<<" sec ";
                    VP_Information_File_Status<<" ,execution in memory ["<<processes_in_memory_table[k][1]<<" - "<<processes_in_memory_table[k][1]+processes_in_memory_table[k][2]<<"]."<<endl;
                    Memory_Information_File_Status<<"->     *Process :["<<pid<<"], size "<<memory_size<<" bytes arrived at "<<arrival<<" sec ";
                    Memory_Information_File_Status<<" ,execution in memory ["<<processes_in_memory_table[k][1]<<" - "<<processes_in_memory_table[k][1]+processes_in_memory_table[k][2]<<"]."<<endl;
                    memory_data_ins->activation_process = 1;
                    for(k=algorithm.elements.y;k<algorithm.elements.y+algorithm.elements.z;k++){
                        memory[k] = 1;
                    }
                    counter_memory_status++;
                    counter_zero=counter_zero +(size_memory - k);
                    diakumansh_b = diakumansh_b+counter_zero*counter_zero;
                    e_counter = e_counter+memory_size*vp_duration;
                }
               
                algorithm.memory_printer(memory,size_memory);
                memory_write_to_file(Memory_Information_File_Status,memory,size_memory);
                up(semid_generator);
                down(semid_manager);
            }
            else if(strcmp(array_mess,"VP_Stop")==0){       //irthe eksodos 
                strcpy(memory_data->message,"NULL");
                for(i=0;i<size_memory;i++){
                    for(j=0;j<size_memory;j++){
                        if(processes_in_memory_table[j][0]==memory_data_vp[i]){
                            for(k=processes_in_memory_table[j][1];k<(processes_in_memory_table[j][1]+processes_in_memory_table[j][2]);k++){
                                memory[k]=0;
                            }
                            cout<<"->Process :["<<pid<<"], size "<<memory_size<<" bytes stopped , free ["<< processes_in_memory_table[j][1]<<","<<processes_in_memory_table[j][1]+processes_in_memory_table[j][2]<<"]\n";
                            Memory_Information_File_Status<<"->Process :["<<pid<<"], size "<<memory_size<<" bytes stopped , free ["<< processes_in_memory_table[j][1]<<","<<processes_in_memory_table[j][1]+processes_in_memory_table[j][2]<<"]\n";
                            VP_Information_File_Status<<"->     *Process :["<<pid<<"], size "<<memory_size<<" bytes stopped , free ["<< processes_in_memory_table[j][1]<<","<<processes_in_memory_table[j][1]+processes_in_memory_table[j][2]<<"]\n";
                            for(u=0;u<100;u++)
                                cout<<"->"<<endl;
                            algorithm.memory_printer(memory,size_memory);
                            memory_write_to_file(Memory_Information_File_Status,memory,size_memory);
                            processes_in_memory_table[j][0]=0;
                            processes_in_memory_table[j][1]=0;
                            processes_in_memory_table[j][2]=0;
                            memory_data_vp[i]=-1;
                            break;
                        }
                    }
                }
                up(semid_generator);
            }
            else if(strcmp(array_mess,"Write_To_File")==0){ //grapse sto log file
                VP_Information_File << memory_data->pid ;
                VP_Information_File <<"  ";
                VP_Information_File << memory_data->memory_size;
                VP_Information_File <<"     ";
                VP_Information_File << memory_data->arrival ;
                VP_Information_File <<"        ";
                VP_Information_File << memory_data->delay ;
                VP_Information_File <<"     ";
                VP_Information_File << memory_data->duration ;
                VP_Information_File <<"     ";
                VP_Information_File << (memory_data->delay - memory_data->arrival );
                VP_Information_File <<"\n";
                strcpy(memory_data->message,"NULL");
                up(semid_generator);
            }
        up(semid_manager);
        clock++;
    }while(strcmp(memory_data->message,"TERM")!=0); 
    ///////////////////// TELOS   EKTELESHS MANAGER ///////////////////////
   
    VP_Information_File <<"\n   ***WAITING LIST*** \n";
    if(list.head!=NULL){
        list.curr = list.head;          //grapse thn lista sto log
        while(list.curr != NULL){
            VP_Information_File << list.curr->process_pid;
            VP_Information_File <<"  ";
            VP_Information_File << list.curr->size_process;
            VP_Information_File <<"     ";
            VP_Information_File << list.curr->arrival ;
            VP_Information_File <<"\n";
            list.curr = list.curr->next;
        }
    }
    cout<<"->Process Memory Manager exiting ,id ["<<getpid()<<"]..."<<endl;
     E = (float)e_counter/((float)(size_memory*duration));
    expected_value = counter_zero/counter_memory_status;
    diakumansh = (2*expected_value-2*(expected_value*expected_value)*expected_value+diakumansh_b)/counter_memory_status;
    VP_Information_File <<"\n   ***MEMORY STATISTICS*** \n ";
    VP_Information_File <<"  E = ";
    VP_Information_File << E;
    VP_Information_File <<"\n  X = ";
    VP_Information_File << expected_value;      //grapse sto log
    VP_Information_File <<"  σ^2 = ";
    VP_Information_File << diakumansh;
            
    VP_Information_File.close();
    VP_Information_File_Status.close();
    Memory_Information_File_Status.close();
    
    shmdt(&shmid);
    shmdt(&shmid2);             //deattaching
    shmdt(&shmid3);
    shmdt(&shmidvp);
    
    shmctl(shmid,IPC_RMID,(struct shmid_ds*)0); //diagrafh memories
    shmctl(shmid2,IPC_RMID,(struct shmid_ds*)0);
    shmctl(shmid3,IPC_RMID,(struct shmid_ds*)0);
    shmctl(shmidvp,IPC_RMID,(struct shmid_ds*)0);

    semctl(semid_generator,1,IPC_RMID,0);       //diagrafh semaphores
    semctl(semid_manager,1,IPC_RMID,0);
    semctl(semid_organiser,1,IPC_RMID,0);
    
    list.print_list();                          //statistika
    cout<<"->|-------------STATISTICS------------|"<<endl;
    printf("->| * E = %3.5f                     |\n",E);
    printf("->| * X = %5lld                       |\n",expected_value);
    printf("->| * Σ = %6lld                     |\n",diakumansh);
    cout<<"->|-----------------------------------|"<<endl;
    list.delete_waiting_list();
    exit(EXIT_SUCCESS);
}