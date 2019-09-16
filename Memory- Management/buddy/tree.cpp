#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "tree.h"

using namespace std;
//o buddy ulopoieite me bash ena dentro analoga thn kathe eisodo
void Tree::initialize(int upper, int lower) {
	SIZE=upper;
	LOWER_LIMIT=lower;
	
	node n=new treenode;
	n->size=SIZE;
	n->start=1;
	n->end=SIZE;
	n->left=NULL;
	n->right=NULL;
	n->parent=NULL;
	n->occupied=0;
	n->biggest_available_space=n->size;
	
	curr_occupied_space=0;
	
	root=n;
	curr=NULL;
	temp=NULL;
}
//sunarthsh poy metarepei to bytes se dunamh tou 2
int Tree::search_size(int process_size) {
	int i=2;
	while (i<process_size)
		i*=2;
	if (i<LOWER_LIMIT)
		return LOWER_LIMIT;
	else
	return i;
}
//sunarthsh gia thn diaspash toy dentrou
void Tree::split(node myparent) {
	
	node left= new treenode;
	left->parent=myparent;
	left->left=NULL;
	left->right=NULL;
	left->size=myparent->size/2;
	left->occupied=0;
	left->start=left->parent->start;
	left->end=left->start+ left->size -1;
	left->biggest_available_space=left->size;
	myparent->left=left;

	node right=new treenode;
	right->parent=myparent;
	right->left=NULL;
	right->right=NULL;
	right->size=myparent->size/2;
	right->occupied=0;
	right->start=myparent->left->end+1;
	right->end=right->parent->end;
	right->biggest_available_space=right->size;
	myparent->right=right;
	
	//cout<<"in split: lc: "<<myparent->left->start<<"-"<<myparent->left->end<<"	rc: "<<myparent->right->start<<"-"<<myparent->right->end<<endl;
}


int Tree::check_quantum(int no) {
	if (no>SIZE ) {
		//cout<<no<<endl;
		return -1;
	}
	if ((curr_occupied_space+no)>SIZE) 
		return -2;
	else
		return 0;
}

//eisodos diergasias
int Tree::add_node(int id, int quantum) {
	
	//cout<<"Insert "<<quantum<<" Bytes"<<endl;
	if (root->size==quantum) {
		if (root->occupied)
			return -1;
		else {
			root->id=id;
			root->occupied=2;
			curr_occupied_space=quantum;
			find_available_space(root);
			return root->start;
		}
	}
	
	if (root->occupied==2)
		return -1;
	
	if (root->biggest_available_space<quantum) {
		//cout<<"try again later"<<endl;
		return 0;
	}
	
	temp=root;
	
	//cout<<endl<<"temp size: "<<temp->size;
	//cout<<"	start: "<<temp->start<<"	end: "<<temp->end<<endl<<endl;

	while (temp->size!=quantum && temp->occupied!=2 /* && temp->biggest_available_space>=quantum*/) {
		
		if (temp->left==NULL) {
			split(temp);
		}
		if (temp->left->biggest_available_space>=quantum && temp->left->occupied!=2) {
			//cout<<endl<<"go left"<<endl;
			//cout<<"size: "<<temp->left->size;
			//cout<<"	start: "<<temp->left->start<<"	end: "<<temp->left->end<<endl<<endl;
			//cout<<"available : "<<temp->biggest_available_space<<endl;
			temp=temp->left;
		}
		else {
			//cout<<endl<<"go right"<<endl;
			//cout<<"size: "<<temp->right->size;
			//cout<<"	start: "<<temp->right->start<<"	end: "<<temp->right->end<<endl<<endl;
			temp=temp->right;
		}		
	}
	

	
	if (temp->left!=NULL) {
		temp=temp->parent->right;
//		cout<<"after all, i didn't go left, i went right!"<< endl;
	}
		
	//cout<<"final:	quantum size: "<<temp->size;
	//cout<<"	start: "<<temp->start<<"	end: "<<temp->end<<endl;

	if (!temp->occupied) {
		temp->occupied=2;
		temp->id=id;
		curr_occupied_space+=temp->size;
		occupied(temp->parent);
		
		find_available_space(root);
		return temp->start;
	}
}

void Tree::occupied(node mynode) {
	curr=mynode;
	while (curr!=NULL) {
		if (curr->left->occupied==2 && curr->right->occupied==2) {
			curr->occupied=2;
		}
		else if (curr->left->occupied || curr->right->occupied)
			curr->occupied=1;
		else 
			break;
		curr=curr->parent;
	}
}
//diagrafh diergasias
void Tree::delete_node(int del_id) {
	node temporary;
	temporary=search_id(root, del_id);
	if (temporary==NULL) {
		//cout<<endl<<"There is no such process"<<endl<<endl;
	}
	else if (temporary==root){
		root->id=0;
		root->occupied=0;
		curr_occupied_space=0;
		root->biggest_available_space=root->size;
		//cout<<endl<<"process "<<del_id<<" was cleared. Memory is empty"<<endl<<endl;
	}
	else {
		temporary->occupied=0;
		unoccupied(temporary->parent);
		curr_occupied_space-=search_size(del_id);
		temporary->biggest_available_space=temporary->size;
		//cout<<endl<<"process "<<del_id<<" was cleared"<<endl<<endl;
	}
	find_available_space(root);
}
//anazhthsh dentrou
node Tree::search_id(node mynode, int myid){
	node temporaryLeft, temporaryRight;
	if (myid==mynode->id) {
		start_value=mynode->start;
		return mynode;
	}
	else if (mynode->left==NULL && mynode->right==NULL)
		return NULL;
	else {
		if (mynode->left!=NULL){
			temporaryLeft=search_id(mynode->left, myid); 
		}
		if (mynode->right!=NULL) {
			temporaryRight=search_id(mynode->right, myid);
		}
	}
	if (temporaryLeft==NULL && temporaryRight==NULL)
		return NULL;
	else if (temporaryLeft!=NULL)
		return temporaryLeft;
	else 
		return temporaryRight;
}

void Tree::unoccupied (node mynode) {
	curr=mynode;
	while (curr!=NULL) {
		if (!curr->left->occupied && !curr->right->occupied) {
			curr->occupied=0;
			merge(curr);
		}
		else
			curr->occupied=1;
		curr=curr->parent;
	}
}
//sunxwneush
void Tree::merge (node mynode) {
	//cout<<endl<<"Merge: "<<mynode->left->start<<"-"<<mynode->left->end<<" and "<<mynode->right->start<<"-"<<mynode->right->end<<endl;
	//cout<<"Merged:"<< mynode->start<<"-"<<mynode->end<<endl<<endl;
	delete mynode->left;
	delete mynode->right;
	mynode->left=NULL;
	mynode->right=NULL;
}

void Tree::empty(node mynode) {
	node temporary=mynode;

	if (temporary->left!=NULL) {
		if (temporary->left->left!=NULL) {
			empty (temporary->left);
//			empty (temporary->right);
		}
		if (temporary->right->left!=NULL) {
//			empty (temporary->left);
			empty (temporary->right);
		}
		if (temporary->left->left==NULL && temporary->right->left==NULL)
			merge(mynode);
	}
	else 
		delete (mynode);
}

int Tree::find_available_space(node mynode) {
	int leftchild, rightchild;
	
	if (mynode->left!=NULL) {
		if (mynode->left->occupied==0)
			leftchild=mynode->left->size;
		else if (mynode->left->occupied==1)
			leftchild=find_available_space(mynode->left);
		else 
			leftchild=0;
		if (mynode->right->occupied==0)
			rightchild=mynode->right->size;
		else if (mynode->right->occupied==1)
			rightchild=find_available_space(mynode->right);
		else 
			rightchild=0;
	}
	else  {
		if (mynode->occupied==0) {
			mynode->biggest_available_space=mynode->size;
			//cout<<"node: "<<mynode->size<<" available: "<<mynode->biggest_available_space<<endl;
			return mynode->size;
		}
		else {
			//cout<<"node: "<<mynode->size<<" available: "<<mynode->biggest_available_space<<endl;
			mynode->biggest_available_space=0;
			return 0;
		}
	}
	
	if (leftchild > rightchild) {
		mynode->biggest_available_space=leftchild;
		//cout<<"node: "<<mynode->size<<" available: "<<mynode->biggest_available_space<<endl;
		return leftchild;
	}
	else {
		mynode->biggest_available_space=rightchild;
		//cout<<"node: "<<mynode->size<<" available: "<<mynode->biggest_available_space<<endl;
		return rightchild;
	} 
}