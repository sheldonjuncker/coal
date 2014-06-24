typedef struct funcs funcs;

//Binary Tree for Functions
struct funcs{
	funcs *left;
	char *name;
	char strong;
	node_t *block;
	node_t *params;
	funcs *right;
};

//Add and Lookup Functions
funcs *lookupF(char *, funcs *);
double addF(function_node *, funcs *);

//Functions
funcs *functions;