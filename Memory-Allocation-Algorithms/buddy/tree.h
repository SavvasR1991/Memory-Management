#ifndef TREE_H
#define TREE_H

typedef struct treenode {
	int id;
	int size;
	int start;
	int occupied;
	int end;
	int biggest_available_space;
	treenode* left;
	treenode* right;
	treenode* parent;
}  *node;

class Tree {
private:
	int curr_occupied_space;
	int SIZE;
	int LOWER_LIMIT;
	node curr;
	node temp;
public:
	node root;	
	int start_value;
	void initialize(int, int);

	int search_size(int);
	int check_quantum(int);

	int add_node(int, int);
	void split(node);
	void occupied(node);

	void merge(node);
	void delete_node(int);
	node search_id(node, int);
	void unoccupied(node);

	void empty(node);
	
	int find_available_space(node);
};

#endif