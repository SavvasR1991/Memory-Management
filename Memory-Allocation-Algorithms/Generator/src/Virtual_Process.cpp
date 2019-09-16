#include "Virtual_Process.h"


/////////////////////////////////////////////////////////
//////////////////   VIRTUAL PROCESS ////////////////////
/////////////////////////////////////////////////////////
/*********Diegrasia Virtual ***************/
void virtual_process(int hits,int sem_id){
    int i,shmidvp,j,k,semidvp,a,b,c,d,*memory_data_vp;
    for(i=0;i<hits;i++){ // eikoniki diergasia 
        down(sem_id);
            b=9;
            a=b*23-2-1+33;
            c=a+b+a/b+b-a;
            b=a+b+a/b+b-a;
            if(a<b)
                d=a+b+c+d -1 +2;
            else
                d=a+b+c+d -1+22 +2;
    }    
    exit(EXIT_SUCCESS);
}
