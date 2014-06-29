/* Driver program for skiplist.c
 * Alasdair Norton - 587162
 * Takes a file name from the command line. Reads key/value pairs into a 
 * skiplist using the insert function from skiplist.c. 
 * Parameters for the skiplist; number of levels and probability of a node 
 * gaining a level, are defined in skiplist.h.
 * 
 * Outputs a summary of the insertion, formatted as 
 * [number of insertions] insertions [number of key comparisons made]
 * Takes keys from the standard input to search
 * Output formatted as
 * [key searched] [value found] [comparisons needed].
 *
 * Known issues: Insertion loop will sometimes lead to a segmentation fault.
 * This seems to occur more often for larger input.
 *
 * No header file was created for the driver programs since they only contain
 * main functions, there are no important declarations to be abstracted */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "skiplist.h"

#define MAX_WORD_LEN 256


int
main(int argc, char **argv){
	node_t *listhead;
	FILE *input;
	int tempkey, counter=0, inserts=0;
	char tempval[MAX_WORD_LEN];
  
	srand(time(NULL));
 
	/* Initialise list, file */
	listhead = makedict();
	if(argc!=2){
		printf("Read Error: No File Given\n");
		exit(EXIT_FAILURE);
	}
	if((input = fopen(argv[1], "r"))==NULL){
		printf("Read Error: File Not Found\n");
		exit(EXIT_FAILURE);
	}
	
	/* Read file, make list */
	while(fscanf(input, "%d %s", &tempkey, tempval)==2){
		insert(listhead, tempkey, tempval, &counter);
		inserts++;
	}
 
	printf("%d insertions %d\n", inserts, counter);
 
	/* Search from stdin */
	while(scanf("%d", &tempkey)==1){
		strcpy(tempval,search(listhead, tempkey, &counter));
		printf("%d %s %d\n", tempkey, tempval, counter); 
	}
	/* Search loop used for experimentation 
         
	int searches=0, overall = 0;
    	srand(time(NULL));
    	while(searches<1000){
    		searches++;
    		counter=0;
    		search(listhead, rand()%10000, &counter);
    		overall+=counter;
    	}
    	printf("%d searches, %d total comparisons, %f average comparisons\n",
    	searches, overall, (float)overall/searches);
    	*/
    	
	return 0;
}
