/* Define data types*/
typedef struct city city_t;
typedef struct edge edge_t;

struct edge{
       int travelTime;
       int cityID;
       edge_t* next;
};


struct city{
      int cityID;
      char cityName[20], reliefAvailable[6];
      edge_t* cityConnections;
};




/* Priority Queue functions */
edge_t* makePQ(int size);
edge_t* enQ(edge_t* queue, int* queue_len, edge_t data);
edge_t deQ(edge_t* queue, int* queue_len);
edge_t* changeWeight(edge_t* queue, int queue_len, edge_t item, int new_weight);
/* Other functions: emptyQ, delete, replace not needed as the 
   queue is created from a database and the data not modified during use. */
/* Heap functions */
edge_t* upheap(edge_t* queue, int k); /* point of insertion: bottom of queue */
edge_t* downheap(edge_t* queue, int queue_len, int k); /* point where data was removed: almost always 1 */
