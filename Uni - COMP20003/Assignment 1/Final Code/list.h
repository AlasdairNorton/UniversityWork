#define NOT_FOUND "Key Not Found"

typedef struct node node_t;

struct node{
	int key;
	char *value;
	node_t *next;
};

node_t* makedict();
node_t* insert(node_t* list_head, int new_key, char *new_value, int *comp_c);
char* search(node_t* list_head, int key, int *comp_c);
node_t* delete_node(node_t* list_head, int key, int *comp_c);
node_t* makenode(int key, char *value);
void printlist(node_t* list_head);
