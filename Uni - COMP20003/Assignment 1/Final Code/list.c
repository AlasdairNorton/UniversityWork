/* Functions governing insertion, deletion and searching a sorted linked list
 * dictionary.
 * Alasdair Norton, 587162
 * Dictionary structure: integer keys to string values
 * Functions: makedict()     : initialises a dictionary
 *	      insert()       : adds a new node to the dictionary
 *	      delete_node()  : removes a node from the dictionary
 * 	      search()       : finds a node in the dictionary and returns
 *                             its value
 *	      makenode()     : initialises a new node with a provided key/value
 *	      print_list()   : traverses the list, printing each key/value pair
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Creates a new null pointer to a node object, to be used as
   the listhead. Placeholder for a larger function in stage 2, where
   the listhead is a distinct data structure from the nodes */
node_t* 
makedict(){
	node_t* newdict = NULL;
	return newdict;
}


/* Inserts a new node into the list */
node_t* 
insert(node_t *listhead, int new_key, char *new_value, int *comp_c){
	node_t *temp, *new_node;
	temp = listhead;
  
 	/* First, create the new node */
 	new_node = makenode(new_key, new_value);
 
	/* Second,  find the correct insertion point.
	 * If the list is empty (listhead==NULL),
	 * set the listhead to the new node */
	if(++(*comp_c) && listhead==NULL){
		listhead = new_node;
	   	new_node->next = NULL;
	   	return listhead;
	}
	
	/* Check if the first key is greater than the new key */
	if(++(*comp_c) && temp->key > new_key){
		new_node->next = temp;
		return new_node;
	}
	/* Otherwise traverse the list until the insertion point is found */
	while(++(*comp_c) && temp->next != NULL && temp->next->key <= new_key){
		temp = temp->next;
	}
	new_node->next = temp->next;
	temp->next = new_node;
	return listhead;
}


/* Searches for a key in the list */
char* 
search(node_t *listhead, int key, int *comp_c){
	node_t *temp;
	temp = listhead;
	/* Traverse the list until the key is found or the key is found to
	 * not exist */
	*comp_c=0;
	while(++(*comp_c) && temp != NULL && temp->key <= key){
		if(temp->key == key){
			return temp->value;
		}
		temp = temp->next;
	}

	return NOT_FOUND;
}


/* Deletes the node with the given key from the list */
node_t* 
delete_node(node_t *listhead, int key, int *comp_c){
	node_t *lead, *tail;
	int not_found=1;
	lead = listhead;
	
	/* Check first element */
	if( lead->key == key ){
		listhead = lead->next;
		free(lead);
		return listhead;
	}
	
	/* Otherwise, traverse the list until the key to be deleted is found */
	while(++(*comp_c) && lead != NULL && lead->key <= key && not_found){
		tail=lead;
		lead=lead->next;
		if(lead->key == key){
			lead = lead->next;
			not_found=0;
		};
	}
	
	if(not_found==0){
		free(tail->next->value);
		free(tail->next);
		tail->next = lead;
	}
	return listhead;
}


/* Creates a new node with the values provided */
node_t* 
makenode(int key, char *value){
        node_t *new_node;
        int len = strlen(value);
        
        /* Memory allocations for node and string */
       	new_node = (node_t*)malloc(sizeof(node_t));
       	assert(new_node!=NULL);
       	new_node->value = (char*)malloc(sizeof(char)*len);
       	assert(new_node->value!=NULL);
             
	new_node->key = key;
        strcpy(new_node->value, value);
        return new_node;
}


/* Prints the key/value pairs for the entire list, for debug purposes */
void
printlist(node_t *listhead){
	node_t *tempnode;
	tempnode = listhead;
	while(tempnode!=NULL){
		printf("Key: %d, Value: %s\n", tempnode->key, tempnode->value);
		tempnode=tempnode->next;
	}
}
