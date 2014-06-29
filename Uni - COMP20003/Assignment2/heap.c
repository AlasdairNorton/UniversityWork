#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"



/* Makes and returns an empty array of edges, for use as a priority queue
   Sets queue[0] to a edge with travelTime -1; less than all possible values*/
edge_t* makePQ(int size){
	edge_t* queue;
	if((queue = (edge_t*)malloc(sizeof(edge_t)*(size+1)))==NULL){
		/* Malloc failed */
		printf("Memory error in creating queue\n");
		exit(EXIT_FAILURE);
	}
        
	queue[0].travelTime=-1;
	return queue;
}

/* Inserts a new item into the heap */
edge_t* enQ(edge_t* queue, int* queue_len, edge_t data){
	*queue_len += 1;
	queue[*queue_len] = data;
	queue = upheap(queue, *queue_len);
	return queue;
}

/* Removes and returns first item from queue. */
edge_t deQ(edge_t* queue, int* queue_len){    
	/* Assign value to be returned to a temp variable */
	edge_t temp = queue[1]; 
	/* Set the last element to the first */
	queue[1] = queue[*queue_len];          
	/* Reduce the queue length */
	*queue_len -= 1;                 
	 /* Fix the heap */
	queue = downheap(queue, *queue_len, 1);                   
	return temp;
}


edge_t* changeWeight(edge_t* queue, int queue_len, edge_t item, int new_weight){
	int i;
	/* update weight */
	for(i=1;i<=queue_len;i++){
		if(queue[i].cityID == item.cityID){
			queue[i].travelTime = new_weight;
			break;
		}
	}
        
        /* rebalance heap:
           The new key is either a) Less than its parent, 
           				and therefore needs to move up
                                 b) Greater than its smaller child, 
                                 	and therefore needs to move down
                                 c) In the correct position.
           upheap(queue,i) will solve situation a), 
           downheap(queue, queue_len, i) solves situation b),
           and neither function has any affect in situation c) */
        upheap(queue, i);
        downheap(queue, queue_len, i);
        return queue;
}

edge_t* upheap(edge_t* queue, int k){
	edge_t temp;

	temp = queue[k];
	while(queue[k/2].travelTime >= temp.travelTime){
		queue[k] = queue[k/2];
		k = k/2;
	}
   
	queue[k] = temp;
       
	return queue;
}

edge_t* downheap(edge_t* queue, int queue_len, int k){
	edge_t temp;
	int j;
	temp = queue[k];
	while(k<=queue_len/2){
		j=2*k;
		if(j < queue_len && queue[j].travelTime>queue[j+1].travelTime){
			j++;
		}
		if(queue[j].travelTime>temp.travelTime){
			break;
		}
		queue[k] = queue[j];
		k=j;
	}
	queue[k] = temp;
	return queue;
}

