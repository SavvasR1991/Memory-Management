#include "Generator.h"

/////////////////////////////////////////////////////////
//////////////////   ORGANISER PROCESS //////////////////
/////////////////////////////////////////////////////////
/*********Diegrasia gia thn waiting list ,lhpsh dedomenwn***************/
void waiting_list_organiser_process(int memory_size){
    int semid_organiser,shmid3,semid_manager,semid_generator,*pid_array;
    int *memory_data_wait_list,i;
    cout<<"->Process Organiser created ,id ["<<getpid()<<"]..."<<endl;
    semid_generator = semget(SEM_GENERATOR,1,PERMS|IPC_CREAT);  //lhpse semaphorwn
    semid_manager = semget(SEM_MANAGER,1,PERMS|IPC_CREAT);      //diko toy manager,generator
    semid_organiser = semget(SEM_ORGANISER,1,PERMS|IPC_CREAT);
    if ((shmid3 = shmget(SHM_ORG_GEN_MAN,sizeof(int)*memory_size/2, PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error Organiser creating virtual shared memory...\n";  //shared data create
        exit(EXIT_FAILURE);
    }
    memory_data_wait_list =(int *)shmat(shmid3, (void *)0, 0);          //attach
    if (memory_data_wait_list == (int *)(-1)) {
        cout<<"->Error Organiser attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    while(1){
        down(semid_organiser);              //enalaks o manager kai generator ton xrisimopoioun gia antalagh dedomenwn
            if(memory_data_wait_list[0]==-100){         //telos ekteleshs
                break;
            }
            if(memory_data_wait_list[0] == 1){          //o manager exei grapsei
                memory_data_wait_list[0] == 0;
            }
        up(semid_organiser);
    }
    cout<<"->Process Organiser exiting ,id ["<<getpid()<<"]..."<<endl;
    exit(EXIT_SUCCESS); 
}

/////////////////////////////////////////////////////////
//////////////////   GENERATOR PROCESS //////////////////
/////////////////////////////////////////////////////////
/*********Diegrasia gia thn gennesh vp processes******************/
void generator(int duration,int t,int lo,int hi,int T,int S){
    int i,b,check_active_vp,check_disactive_vp,c,shmid3,shmid2,ii,jj,p,j=0,status,sem_virtual_process,temp_val,semid_manager,semid_generator,shmid,temp,vp_duration;
    int vp_birth,shmidvp,semid_waiting_list,event_result,vp_event,sem_child_vp,memory_size,repeats,k,sum_hits; 
    int poisson_result,semid_waiting_list2,rand_result,rand_result2,delay=9,**hits_array,active;
    int *memory_data_vp,*memory_data_wait_list,print_c,swap_rand;
    int *processes_active_table,waiting_pid,semid_organiser;
    pid_t wpid,pid;
    senum arg;
    shm_communication *memory_data;
    shm_communication_insert_memory *memory_data_ins;
    key_t key;
    time_t seconds;
    Register_Table register_table;
    seconds = time(NULL);
    srand(seconds);
    processes_active_table = new int [S];
    //        hits_array = new int *[repeats];

    cout<<"->Process Generator created ,id ["<<getpid()<<"]..."<<endl;
    ///////////////////////////////////////////////////////////////////////////////////
    ////////////// Shared memories arxikopoihseis ,attaching kai semaphores ///////////
    ///////////////////////////////////////////////////////////////////////////////////
    semid_generator = semget(SEM_GENERATOR,1,PERMS|IPC_CREAT);  //shemapores
    semid_manager = semget(SEM_MANAGER,1,PERMS|IPC_CREAT);
    semid_organiser = semget(SEM_ORGANISER,1,PERMS|IPC_CREAT);

      
    if ((shmid = shmget(SHM_GEN_MAN_COM,sizeof(shm_communication), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,communucation...\n"; //memory gia epikoinwnia generator - manager
        exit(EXIT_FAILURE);
    }   
    if ((shmid2 = shmget(SHM_GEN_MAN_MEM_IN,sizeof(shm_communication_insert_memory), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,insert memory...\n";//memory gia gen-man gia otan kapoios xwraeei sthn mnhmh
        exit(EXIT_FAILURE);
    }
    if ((shmidvp = shmget(SHM_GEN_MAN_ACTIVE_ST,sizeof(int)*(S), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,active...\n";    //memory gia otan kapoios teleiwse thn ektelesh tou
        exit(EXIT_FAILURE);
    }
    if ((shmid3 = shmget(SHM_ORG_GEN_MAN,sizeof(int)*(duration/2), PERMS | IPC_CREAT)) == -1) {
        cout<<"->Error creating virtual shared memory ,disactive...\n"; //memory gia org-man-gen gia otan kapoios bgei apo waiting list
        exit(EXIT_FAILURE);
    }
    memory_data_ins =(shm_communication_insert_memory *)shmat(shmid2, (void *)0, 0);
    if (memory_data_ins == (shm_communication_insert_memory *)(-1)) {   //attach
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    memory_data =(shm_communication *)shmat(shmid, (void *)0, 0);
    if (memory_data == (shm_communication *)(-1)) {//attach
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    
    memory_data_vp =(int *)shmat(shmidvp, (void *)0, 0);
    if (memory_data_vp == (int *)(-1)) {//attach
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    memory_data_wait_list =(int *)shmat(shmid3, (void *)0, 0);//attach
    if (memory_data_wait_list == (int *)(-1)) {
        cout<<"->Error attaching virtual shared memory...\n";
        exit(EXIT_FAILURE);
    }
    wpid = fork();
    if(wpid==0){                    //gennesh organiser
        waiting_list_organiser_process(duration);
    }
    sleep(1);
    //////////////////////////////////////////////////////////////////////////////////////////
    
    down(semid_generator);
    ////////// Arxikopoihseis timwn kai poisson ///////////
    rand_val(t);        
    rand_result = rand()%9+1;
    rand_result2 = rand()%20+5;
    vp_birth = poisson(1.0/rand_result);
    vp_event = 5;
    temp_val =8000;
    //////////////////////////////////////////////////////
    
    ////////////// ENARKSH EKTELESHS GENERATOR //////////////////
    for(i=0;i<duration;i++){
        print_c = 0;
        cout<<"->"<<i<<" sec :"<<endl;
        down(semid_organiser);      //elenkse an o manager exei bgalei kanenan apo thn waiting list
            b = 1;
            while(memory_data_wait_list[b]!=-1){            //des an exei bgalei
                if(register_table.head!=NULL){
                    register_table.curr = register_table.head;
                    while(register_table.curr != NULL){
                        if(register_table.curr->process_pid==memory_data_wait_list[b]){ //des an uparxei ston register
                            register_table.curr->active = 1;                        //energopoihse ton 
                            cout<<"->   * Process VP ["<<memory_data_wait_list[b]<<"] execution,extracted from waiting list..."<<endl;
                        }
                        register_table.curr = register_table.curr->next;
                    }
                }
                memory_data_wait_list[b]=-1;                //adeiase thn thesi
                b++;
            }
        up(semid_organiser);        //des posoi teleiwsan thn ektelesh tous
        check_active_vp = 0;
        if(register_table.head!=NULL){
            register_table.curr = register_table.head;
            while(register_table.curr != NULL){
                if(register_table.curr->active==3){ //des poioi einai
                    processes_active_table[check_active_vp] = register_table.curr->process_pid;
                    check_active_vp++;
                }
                register_table.curr = register_table.curr->next;
            }
        }
        if(check_active_vp!=0){
            down(semid_manager);
                strcpy(memory_data->message,"VP_Stop"); //pes ston manager na adeiasei tis thesis mnhmhss
                memory_data_vp[0] = check_active_vp;
                for(ii=0;ii<check_active_vp;ii++){   //pes tou poioi einai
                    memory_data_vp[ii]=processes_active_table[ii];
                    cout<<"->   * Process VP ["<<memory_data_vp[ii]<<"] finished execution,waiting..."<<endl;
                }
            up(semid_manager);
            down(semid_generator);
        }
        if(register_table.head!=NULL){              //des poioi exoun teleiwsei thn adrania
            register_table.curr = register_table.head;
            while(register_table.curr != NULL){
                if(register_table.curr->active==2){ // an exoun teleiwsei thn adrrania
                    down(semid_manager);
                        strcpy(memory_data->message,"VP_Start"); //steile ton ston manager to vp
                        memory_data->pid = register_table.curr->process_pid;
                        memory_data->memory_size = register_table.curr->memory_size;
                        memory_data->arrival = register_table.curr->arrival;
                        memory_data->duration = register_table.curr->hits_table[register_table.curr->current_hit][0];;
                        memory_data->delay = register_table.curr->delay;
                        memory_data->key = register_table.curr->key;
                        memory_data->hits = register_table.curr->hits;
                        cout<<"->   * Process VP ["<<pid<<"]->["<<memory_size<<"] bytes finished delay time"<<endl;
                    up(semid_manager);
                    down(semid_generator);
                    active = memory_data_ins->activation_process;       //des ama mphke sthn mnhmh
                    register_table.curr->active = active;
                    if(active == 0){
                        cout<<"->   * Process VP ["<<pid<<"]->["<<memory_size<<"] bytes waiting"<<endl;
                        register_table.curr->active = 0;    //oxi
                    }
                    else{
                        cout<<"->   * Process VP ["<<pid<<"]->["<<memory_size<<"] bytes execution in memory"<<endl;
                        register_table.curr->active = 1;   //naii
                        up(register_table.curr->sem_virtual_process);
                    }
                    up(semid_manager);
                }
                register_table.curr = register_table.curr->next;
            }
        }
        if(vp_birth==i){    //gennish diergasias
            temp_val++;     //epomeno kleidi gia gennesi
            poisson_result = poisson(1.0/rand_result); //epomenh gennisi se
            if(poisson_result==0){
                poisson_result++;
            }
            vp_birth =vp_birth + poisson_result; //epomenh gennish sto 
            key = temp_val;                                     //neo kleidi gia vp
            k = rand()%18+2;
            repeats = 2*(k)+1;                                  //ripes
            sum_hits = 0;
            hits_array = new int *[repeats];                    //pinakas ripwn
            for( ii = 0; ii < repeats;ii++)
                hits_array[ii] = new int[2];
            for(p=0;p<repeats;p++){                             // gemisma me tuxaio ekthetika pinaka 
                temp = poisson(1.0/rand_result);                //thn riph kai adrania riphs
                hits_array[p][0] = temp;
                temp = rand()%(T)+1;
                hits_array[p][1] = temp;
                sum_hits = sum_hits + hits_array[p][0];         //sunolikos xronos ekteleshs vp
            }
            memory_size = rand()%(hi-lo)+lo;                    //megethos
            vp_duration = sum_hits ;
            sem_virtual_process = semget(key,1,PERMS|IPC_CREAT); //dhmiourgia semaphorou gia vp
            arg.val=0;                                          //arxikopoihsh se blocked                     
            semctl(sem_child_vp,0,SETVAL,arg);
           
            pid = fork();                                       //gennesh
            if(pid == 0){
                virtual_process(repeats,sem_virtual_process);
            }
            down(semid_manager);                                //apostolh twn dedomenwn ston manager
                strcpy(memory_data->message,"VP_Start");
                memory_data->pid = pid;
                memory_data->memory_size = memory_size;
                memory_data->arrival = i;
                memory_data->duration = hits_array[0][0];
                memory_data->delay = delay;
                memory_data->key = sem_virtual_process;
                memory_data->hits = repeats;
            up(semid_manager);
            down(semid_generator);
            active = memory_data_ins->activation_process;     //anamonh apantishs gia to an mpike sthn mnhmh apo ton manager
            up(semid_manager);                                  //eisodos sto register table
            register_table.insert_to_register_table(pid,i,repeats,vp_duration,delay,key,hits_array,active,memory_size,sem_virtual_process);
            if(active==1){
                up(sem_virtual_process);
            }
            cout<<"->   *<< Process VP ["<<pid<<"]->["<<memory_size<<"] bytes arrived at "<<i<<" sec,duration :"<<hits_array[0][0]<<" >>"<<endl;
            for(ii = 0; ii < repeats; ii++){
                delete [] hits_array[ii];
            }
            delete [] hits_array;
        }
        register_table.organiser_register_table(i);  //fiksare tous xronoys olwn twn diergasiwn ana sec
        register_table.curr = register_table.head;
        while(register_table.curr != NULL){
            if(register_table.curr->active==-2){        //opoios teleiwnei pes ston manager na ton grapsei sto log file
                down(semid_manager);
                    strcpy(memory_data->message,"Write_To_File");
                    memory_data->pid = register_table.curr->process_pid;
                    memory_data->memory_size = memory_size;
                    memory_data->arrival = register_table.curr->arrival;
                    memory_data->delay =register_table.curr->finish_time;
                    memory_data->duration =register_table.curr->duration;
                    register_table.curr->active= -4;
                up(semid_manager);
                down(semid_generator);
            }
            register_table.curr = register_table.curr->next;
        }
        cout<<"->\n";
    }
        //////////////   TELOS EKTELESHS GENERATOR //////////////////

    down(semid_organiser);      //termatise ton organiser
        memory_data_wait_list[0]=-100;
    up(semid_organiser);

    down(semid_manager);        //termatise ton manager
        strcpy(memory_data->message,"TERM");
    up(semid_manager);
    register_table.curr = register_table.head;
    while(register_table.curr != NULL){ //termatuse tuxon vp paidia
        for(i=0;i<register_table.curr->hits;i++){
            up(register_table.curr->sem_virtual_process);
        }
        register_table.curr = register_table.curr->next;
    }
    while ((wpid = wait(&status)) > 0);         //perimene tous olous 
    cout<<"->Process Generator exiting ,id ["<<getpid()<<"]..."<<endl;
    sleep(1);
    register_table.print_register_table();//typwse lista
    register_table.delete_register_table();//eleutherwse 
    
    shmdt(&shmid);  //dettaching
    shmdt(&shmid2);
    shmdt(&shmid3);
    shmdt(&shmidvp);
    
    exit(EXIT_SUCCESS); //telos
}