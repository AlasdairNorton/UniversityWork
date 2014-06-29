/* Driver program for list.c
 * Alasdair Norton - 587162
 * Takes a file name from the command line. Reads key/value pairs into a linked
 * list using the insert function from list.c. 
 * Outputs a summary of the insertion, formatted as 
 * [number of insertions] insertions [number of key comparisons made]
 * Takes keys from the standard input to search
 * Output formatted as
 * [key searched] [value found] [comparisons needed].
 *
 * No header file was created for the driver programs since they only contain
 * main functions, there are no important declarations to be abstracted */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include <time.h>

#define MAX_WORD_LEN 256

int
main(int argc, char** argv){
	node_t *listhead;
	FILE *input;
	int insert_comp=0, inserts=0, tempkey, search_comp;
	char tempval[MAX_WORD_LEN];
 
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
		listhead = insert(listhead, tempkey, tempval, &insert_comp);
		inserts++;
	}
	
	fclose(input);
	printf("%-7d insertions %-15d\n", inserts, insert_comp);
    
	
	
         /* Search from stdin */
   	 while(scanf("%d", &tempkey)==1){
   		 strcpy(tempval,search(listhead, tempkey, &search_comp));
   		 printf("%-7d %-20s %-10d\n", tempkey, tempval, search_comp); 
   		 }
   
    /* Loop used for automated testing of search function, not part of program
         
	int searches=0, overall = 0;
    	srand(time(NULL));
    	while(searches<1000){
    		searches++;
    		search(listhead, rand()%10000, &search_comp);
    		overall+=search_comp;
    	}
    	printf("%d searches, %d total comparisons, %f average comparisons\n",
    	searches, overall, (float)overall/searches);
    	
     */                    
	return 0;
}

