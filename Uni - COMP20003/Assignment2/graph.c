#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "heap.h"
#include "graph.h"


/* Dijkstra's Algorithm */

void dijkstra (city_t** cities, int city_count, int source, int dist[], 
		int pred[]){
	int i, j,  queue_len = city_count;
	edge_t* PQ;
	edge_t u_dist, *u_to_v, v_dist;
	city_t *v, *u;
          
          
	/* Initialise */
	for(i=0;i<city_count;i++){
		dist[i] = INT_MAX/2; 
		/* INT_MAX/2 is the largest int such that if it is doubled, 
		it won't cause int overflow */
		pred[i] = -1;
	}
	dist[source] = 0;

	PQ = makePQ(city_count);     
     
	/* Populate PQ */
	for(i=1;i<=city_count;i++){
		PQ[i].cityID = cities[i-1]->cityID;
		PQ[i].travelTime = dist[i-1];
	}
         
	for(i=city_count/2;i>0;i--){
		downheap(PQ, city_count, i);
	}
         
	/* Run the algorithm */
	while(queue_len > 0){
		/* Get the closest city, u.cityID, where u is the first item 
		   in the queue */     
		u_dist = deQ(PQ, &queue_len);
		u = cities[u_dist.cityID];
		u_to_v = u->cityConnections;
		/* For each edge in temp_city */    
		while(u_to_v !=NULL){             
			v = cities[u_to_v->cityID];                
			/* Find the estimate of v's distance in the PQ */
			for(j=1;j<=queue_len;j++){                          
				if(PQ[j].cityID ==  v->cityID){
					v_dist = PQ[j];
					break;
				}
			}
			/* If the length of the edge u->v plus the distance to
			   u is less than the estimated distance to v, 
			   update the estimate */
			if(u_dist.travelTime + u_to_v->travelTime <  
				                             dist[v->cityID]){
				dist[v->cityID] = u_dist.travelTime + 
				                            u_to_v->travelTime;
				pred[v->cityID] = u->cityID;
				PQ = changeWeight(PQ, queue_len, 
					              v_dist, dist[v->cityID]);
			}
			/* Update u_to_v */
			u_to_v = u_to_v->next;
		}
	}
}


/* Dijkstra's algorithm returns two arrays; one of travel times and one of
   predecessors on the path to the source. Search the travel time list for the 
   closest city which possesses the required supply */   
int findSupply(char supply, city_t** cities, int num_cities, int dist[], 
		int pred[]){
	int i, j, min_dist = INT_MAX, min_id=-1; 
	/* min_id defaults to -1 as a flag for failure 
	   (i.e. supply cannot be found at all)*/
	city_t* tempCity;
	char* supplies;
	/* Linear sort since num_citites is likely to be small */
	for(i=0;i<num_cities;i++){
		tempCity = cities[i];
		supplies = tempCity->reliefAvailable;
          
		for(j=0;j<strlen(supplies);j++){
			if(supplies[j] == supply && dist[i]<min_dist){
				min_dist = dist[i];
				min_id= i;
			}
		}          
	}
	/* Have cityID and minimum distance for the supply. Return cityID */
	return min_id;    
}

void printPath(char supply, city_t** cities, int num_cities, int dist[],
		int pred[], int min_id){
	int min_dist;
	city_t *tempCity;
	if(min_id > -1){
		min_dist = dist[min_id];
	}else{
		min_dist = INT_MAX; /* Indicates supply unavailable */
	}
	if(min_dist<INT_MAX/2){
		tempCity = cities[min_id];
		/* Print top line, formatted as
		supplyCode cityID cityName distance
		*/
		printf("%c %4d %10s  %3d hrs\n", 
			supply, tempCity->cityID, tempCity->cityName, min_dist);
		while(pred[min_id]!=-1){ 
			/* Trace back the path using the pred array.
			   -1 is used as a flag for no predecessor */
			printf("                %5d %s\n", 
			     cities[min_id]->cityID, cities[min_id]->cityName);
			min_id = pred[min_id];
		}
		/* Last line to print destination city */
		printf("                %5d %s\n", 
			cities[min_id]->cityID, cities[min_id]->cityName);
	}else{
		printf("%c is not available\n", supply);
	}
}
