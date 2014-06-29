/* Alasdair Norton - 587162
 * COMP20003 Alg&DS Assignment 2, Semester 2 2013 
 * Contains the driver program */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "graph.h"

#define LINE_LEN 256
#define NUM_EDGES 30
#define MAX_SUPPLIES 5
#define NAME_LEN 30

city_t** readfile(FILE* input, int *num_cities);
void printcity(city_t*);


int main(int argc, char** argv){
	FILE* input;
	int num_cities=0, i, num_supplies=0, searchID=-1;
	int next_id, min_id, *pred, *dist;
	city_t **citylist;
	city_t *temp;
	char searchName[NAME_LEN], searchSupplies[MAX_SUPPLIES], one_supply; 
	char allSupplies[] = {'B', 'F', 'W', 'D', 'M'};
    
	/* Open and read database, initialise graph arrays */
	if(argc!=2){
		printf("Error: No database provided\n");
		exit(EXIT_FAILURE);
	}
	if((input = fopen(argv[1], "r"))==NULL){
		printf("Read Error: File Not Found\n");
		exit(EXIT_FAILURE);
	}

	/* Read input file */
	citylist = readfile(input, &num_cities);
    
	/* Create dist[] and pred[] */
	if((dist = (int*)malloc(sizeof(int)*(num_cities)))==NULL){
		printf("Memory error in creating dist array\n");
		exit(EXIT_FAILURE);
	}
	if((pred = (int*)malloc(sizeof(int)*(num_cities)))==NULL){
		printf("Memory error in creating dist array\n");
		exit(EXIT_FAILURE);
	}
    
	/* Get input */
    
	printf("Enter the location of the disaster:\n");
	scanf("%s", searchName);
	scanf("%c", &one_supply); /* Collect newline character from previous scan */
    
	printf("Enter the required supplies:\n");
	while(scanf("%c", &one_supply)==1 && num_supplies<MAX_SUPPLIES){
		if(one_supply != '\n'){
			searchSupplies[num_supplies] = one_supply;
			num_supplies++;
		}else{
			break;
		}
	}
     
	/* Find source city */
	for(i=0;i<num_cities;i++){
		if(strcmp(searchName, citylist[i]->cityName)==0){
			searchID = i;
			break;
		}
	}
	if(searchID == -1){
		/* City was not found */
		printf("The city entered is not in the database. Exiting program.\n");
		exit(EXIT_FAILURE);
	}
        /* run graph algorithm */
        dijkstra(citylist, num_cities, searchID, dist, pred); 
        
        /* For each resource requested run findResource */
        if(num_supplies > 0){
        	for(i=0;i<num_supplies;i++){
        		min_id = findSupply(searchSupplies[i], citylist, num_cities, dist, pred);
        		printPath(searchSupplies[i], citylist, num_cities, dist, pred, min_id);
        	}
        }else{
        	/* If no Supplies were requested, find nearest instance of any resource */
        	min_id = num_cities+1;
        	/* Find closest resource */
        	for(i=0;i<MAX_SUPPLIES;i++){
        		next_id = findSupply(allSupplies[i], citylist, num_cities, dist, pred);
        		if(dist[next_id] < dist[min_id]){
        			min_id = next_id;
        			one_supply = allSupplies[i];
                	}
                }
                /* Print closest resource */
                if(min_id<num_cities){
                	printf("You requested no supplies.");
                	printf("The closest supplies available are:\n");
                	printPath(one_supply, citylist, num_cities, 
                		dist, pred, min_id);
                }else{
                	printf("This city can access no supplies. \n");
                }
        }    
        return 0;
}

city_t** readfile(FILE* input, int *num_cities){
	char line[LINE_LEN];
	int tempCityID, tempTravelTime,  i;
	city_t **citylist;
	city_t *temp, *edge_dest;
	edge_t *temp_edge, *insert_point;
	char *edgestr, *field;
  

	/* Read first line, initialise citylist */
	if(fgets(line, LINE_LEN, input)==NULL){
		printf("Error: Empty file");
		exit(EXIT_FAILURE);
	}
	*num_cities = atoi(line);
	if(*num_cities == 0){
		printf("Error: Database format invalid (Line 1)"); 
		exit(EXIT_FAILURE);
	}  
	if((citylist = (city_t**)malloc(sizeof(city_t*)*(*num_cities)))==NULL){
		printf("Memory Error: Initialise citylist failed");
		exit(EXIT_FAILURE);    
	}

	/* Initialise all cities */
	for(i=0;i<*num_cities;i++){
		if((citylist[i] = (city_t*)malloc(sizeof(city_t)))==NULL){
			printf("Memory Error: Populate citylist failed");
			exit(EXIT_FAILURE);    
		}
		citylist[i]->cityConnections= NULL; 
	}

	while(fgets(line, LINE_LEN, input)!=NULL){
		/* For each line */
		/* 1. Create new city_t to store data */

		/* Tokenise string on | and assign to fields in city_t */
		field = strtok(line, "|");
		temp = citylist[atoi(field)];
		temp->cityID = atoi(field);
        
		field = strtok(NULL, "|");
		strcpy(temp->cityName, field);
		
		field = strtok(NULL, "|");
		strcpy(temp->reliefAvailable, field);
		
		edgestr = strtok(NULL, "|");
      
		/* Now manage list of CityID:Time pairs */
		field = strtok(edgestr, ",");

		while(field!=NULL){    
			/* While there are still edges to be read */
			if(sscanf(field, "%d:%d", &tempCityID, &tempTravelTime)
				==2){
				/* Make new edge */
				if((temp_edge = (edge_t*)malloc(sizeof(edge_t)))
					==NULL){
					printf("Memory Error: Create edge");
					exit(EXIT_FAILURE);    
				}  
				/* Fill in fields */   
				temp_edge->cityID = temp->cityID;
				temp_edge->travelTime = tempTravelTime;
				temp_edge->next = NULL;
				/* Insert into correct city's edge list */
				edge_dest = citylist[tempCityID];
				insert_point = edge_dest->cityConnections;
                
				/* Find insertion point */
				if(insert_point == NULL){
					edge_dest->cityConnections = temp_edge;
				}else{
					while(insert_point->next!=NULL){
						insert_point = insert_point->next;
					}
					/* Insert */
					insert_point->next = temp_edge;
				}

                                
			}
			/* Get next edge data */
			field = strtok(NULL, ",");
                
		}
	/* Put new city into city list */	
        citylist[temp->cityID] = temp;   
        }
        return citylist;            
}

/* Prints city data out as a new line - used in debugging readfile() */
void printcity(city_t* city){
	int i;
	edge_t *edge = city->cityConnections;
	printf("CityID: %d, City Name: %s, Supplies: %s, Connections:",  
		city->cityID, city->cityName, city->reliefAvailable);
	while(edge!=NULL){
		printf("(%d,%d), ", edge->cityID, edge->travelTime);
		edge = edge->next;
	}
	printf("\n");
}
