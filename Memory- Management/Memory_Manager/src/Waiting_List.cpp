#include "Waiting_List.h"

/*********************creator********************/
Waiting_List::Waiting_List(){
    cout <<"->Waiting List  has been created"<<endl; //Destructor method
    head=NULL;                              //Initialize head to NULL
    curr=NULL;                              //Initialize curr to NULL
    endd=NULL;                              //Initialize endd to NULL
    temp=NULL;
    total_arrivals = 0;
}

/*********************distractor********************/
Waiting_List::~Waiting_List(){
    cout <<"->Waiting List  has been delete"<<endl; //Destructor method
}

/*********************eisodos kombou sthn lista********************/
void Waiting_List::insert_to_waiting_list(pid_t pid,int arrival,int duration,int key,int delay,int size_process){
    Waiting_List_Element* n =new Waiting_List_Element;         //Create a pointer that points to a node
    n->next = NULL;
    n->key = key;
    n->duration = duration;
    n->arrival = arrival;
    n->process_pid = pid;
    n->delay = delay;
    n->size_process = size_process;
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

/*********************eksodos prwtou kombou********************/
void Waiting_List::delete_from_waiting_list(){
    if(head!=NULL){
        curr = head;
        first_out_from_list.process_pid = head->process_pid;
        first_out_from_list.arrival = head->arrival;
        first_out_from_list.key = head->key;
        first_out_from_list.duration = head->duration;
        first_out_from_list.delay = head->delay;
        first_out_from_list.size_process = head->size_process;
        if(curr->next==NULL){
            delete curr;
            head = NULL;
        }
        else{
            head = head->next;
            delete curr;;
        }
        total_arrivals--;
    }
}

/*********************diagrafh listas********************/
void Waiting_List::delete_waiting_list(){
    if(head!=NULL){
        curr = head;
        while(curr->next != NULL){
            temp=curr;
            curr = curr->next;
            delete temp;
        }
    }
    cout<<"->Free Waiting List "<<endl;
}

/*********************ektupwsh listas********************/
void Waiting_List::print_list(){
    int i =1;
    if(head!=NULL){
        curr = head;
        cout<<"->____________________________________"<<endl;
        cout<<"->|------------WAITING LIST-----------|"<<endl;
        cout<<"->| NUM  |  PID  | ARRIVAL | DURATION |"<<endl;
        cout<<"->|-----------------------------------|"<<endl;
        while(curr != NULL){
            temp=curr;
            curr = curr->next;
            printf("->| %4d | %d | %7d | %8d |\n",i,temp->process_pid,temp->arrival,temp->duration);
            i++;
        }
        cout<<"->|-----------------------------------|"<<endl;
        cout<<"->|------------WAITING LIST-----------|"<<endl;
    }
    else{
        cout<<"->Waiting list is empty"<<endl;
    }
}

/*********************elengos an einai adeia listas********************/
int Waiting_List::check_list_empty(){
    if(head==NULL){
        return 0;
    }
    return 1;
}

/*********************taksinomish ths listas kata aukson xrono ekteleshs********************/
void Waiting_List::organiser_register_table(){
     int count = 0, i;
  Waiting_List_Element *start = head;
  Waiting_List_Element *trail = NULL;

  while(start != NULL) { //grab count
    count++;
    start = start->next;
  }

  for(i = 0; i<count; ++i) { //for every element in the list

    curr = trail = head; //set curr and trail at the start node

    while (curr->next != NULL) { //for the rest of the elements in the list
      if (curr->duration > curr->next->duration) { //compare curr and curr->next

        temp = curr->next; //swap pointers for curr and curr->next
        curr->next = curr->next->next;
        temp->next = curr;

        //now we need to setup pointers for trail and possibly head
        if(curr == head) //this is the case of the first element swapping to preserve the head pointer
          head = trail = temp;
        else //setup trail correctly
          trail->next = temp;
        curr = temp; //update curr to be temp since the positions changed
      }
      //advance pointers
      trail = curr;
      curr = curr->next;
    }
  }
}