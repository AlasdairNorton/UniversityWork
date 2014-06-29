void dijkstra(city_t** cities, int city_count, int source, int dist[], int pred[]);
int findSupply(char supply, city_t** cities, int num_cities, int dist[], int pred[]);
void printPath(char supply, city_t** cities, int num_cities, int dist[], int pred[], int min_id);
