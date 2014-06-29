/* Functions governing insertion, deletion and searching a skiplist dictionary.
 * Alasdair Norton, 587162
 * Dictionary structure: integer keys to string values
 * Functions: makedict()     : initialises a dictionary
 *	      insert()       : adds a new node to the dictionary
 * 	      search()       : finds a node in the dictionary and returns
 *                             its value
 *	      makenode()     : initialises a new node with a provided key/value
 *	      print_list()   : Prints the key/value pairs at each level of the
 *			       list structure.
 * 	      levels()	     : Determined the number of levels a new node has
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "skiplist.h"

/* Creates a node object to serve as the listhead, 
 * with MAX_LEVELS null pointers to nodes and 
 * value and key set to NULL and 0*/
node_t* 
makedict(){
	node_t* newdict;
	int i;
	/* Allocates the node and the array of pointers */
	newdict=(node_t*)malloc(sizeof(node_t));
	assert(newdict!=NULL);
	newdict->nodelist=(node_t**)malloc(MAX_LEVELS*sizeof(node_t*));
	assert(newdict->nodelist!=NULL);
	
	newdict->value = NULL;
	newdict->key = 0;
  
	for(i=0;i<MAX_LEVELS;i++){
		newdict->nodelist[i] = NULL;
	}
    
	return newdict;
}


/* Inserts a new node into the list
 * A void function where the equivilant in the linked list returned the
 * listhead because in the linked list, the listhead may change if insertions
 * change the first node in the list, whereas for the skiplist the listhead is
 * constant. */
void 
insert(node_t *listhead, int new_key, char *new_value, int *comp_c){
	node_t *temp, *new_node;
	int num_levels, i;
	num_levels = levels();
	temp = listhead;
 
	/* First, create the new node */
	new_node = makenode(new_key, new_value, num_levels);
	/* Second, add node into the list.
	 * For each level, starting from the highest, find the correct
	 * insertion point in that level*/
	for(i=MAX_LEVELS-1; ++(*comp_c) && i>=0; i--){
		while(++(*comp_c) && temp->nodelist[i]!=NULL
			&& temp->nodelist[i]->key < new_key){
			temp = temp->nodelist[i];
		}
		if(i < num_levels){
			new_node->nodelist[i] = temp->nodelist[i];
			temp->nodelist[i] = new_node;  
		}
	}
}


/* Searches for a key in the list */
char* 
search(node_t *listhead, int key, int *comp_c){ 
	node_t *temp, *next;
	int level = MAX_LEVELS-1;
	*comp_c = 0;
	temp = listhead;
	/* Similar to in list.c, the counter is updated in the loop guard, and
	 * therefore cannot start at 0. */
	while(level>=0){
		while(++(*comp_c) && temp->nodelist[level]!=NULL 
			&& temp->nodelist[level]->key < key){
			temp = temp->nodelist[level];
		}
      
		/* When it exits the inner loop, the next node on the given
		 * level is either NULL, or less than/equal to the key
		 * Test for equality, if the key is not found, drop a level */
		next = temp->nodelist[level]; 
		if(++(*comp_c) && next!=NULL && next->key == key){
			return temp->nodelist[level]->value;
		} 
		level--;
	}  
	return NOT_FOUND; 
}





/* Creates a new node with the values provided */
node_t* 
makenode(int key, char *value, int levels){
	node_t *new_node, **new_nodelist;
	char *newvalue;
	int i, len = strlen(value);

	/* memory allocations for new node, new string and array of pointers */
			
       	assert((new_node = (node_t*)malloc(sizeof(node_t)))!=NULL);
       	assert((newvalue = (char*)malloc(sizeof(char)*len))!=NULL);
        assert((new_nodelist=(node_t**)malloc(sizeof(node_t*)*MAX_LEVELS))
        	!=NULL);
	new_node->nodelist = new_nodelist;
	new_node->value=newvalue;
	
	for(i=0;i<MAX_LEVELS;i++){
		new_node->nodelist[i] = NULL;
	}
		
        new_node->level = levels;
	new_node->key = key;      
	strcpy(new_node->value, value);
        return new_node;
}


/* Prints the data for each node, organised by level in the list,
 * for debug purposes */
void
printlist(node_t *listhead){
	node_t *tempnode;
	int i;
	for(i=0; i<MAX_LEVELS; i++){
		printf("\nAt level %d\n", i+1);
		tempnode = listhead->nodelist[i];
		while(tempnode!=NULL){
			printf("Key: %d, Value: %s, Level: %d\n",
			tempnode->key, tempnode->value, tempnode->level);
			tempnode=tempnode->nodelist[i];
		}
	}    
}

/* Generates the number of levels a new node will have */
int 
levels(){
	int levels = 1;
	while((double)rand()/RAND_MAX<NEW_LEVEL_CHANCE && levels<MAX_LEVELS){
		levels++;
	}
	return levels;
}
