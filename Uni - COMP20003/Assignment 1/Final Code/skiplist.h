#define NOT_FOUND "Key Not Found"
#define MAX_LEVELS 10
#define NEW_LEVEL_CHANCE 0.5

typedef struct node node_t;

struct node{
	int key;
	int level;
	char *value;
	node_t **nodelist;
};


node_t* makedict();
void insert(node_t* list_head, int new_key, char *new_value, int *comp_c);
char* search(node_t* list_head, int key, int *comp_c);
node_t* makenode(int key, char *value, int levels);
void printlist(node_t* list_head);
int levels();

