//Integer Node
typedef struct int_node int_node;
struct int_node
{
	int i;
};

//Double Node
typedef struct dub_node dub_node;
struct dub_node
{
	double d;
};

//String Node
typedef struct str_node str_node;
struct str_node
{
	char *s;
};

//List Node
typedef struct list_node list_node;
struct list_node
{
	//List
	node_t *l;
};

//Id Node
typedef struct id_node id_node;
struct id_node{
	char *name;
};

//Ternary Node
typedef struct ternary_node ternary_node;
struct ternary_node
{
	//Condition
	node_t *condition;
	
	//If Expression
	node_t *if_exp;
	
	//Else Expression
	node_t *else_exp;
	
	//Else
	node_t *opt_else;
};

//Cast Node
typedef struct cast_node cast_node;
struct cast_node
{
	//Type
	char type;
	
	//Expression
	node_t *exp;
};

//If Node
typedef struct if_node if_node;
struct if_node
{
	//Condition
	node_t *condition;
	
	//Block
	node_t *block;
	
	//Else-Ifs
	node_t *opt_elseifs;
	
	//Else
	node_t *opt_else;
};

//ElseIf Node
typedef struct elseif_node elseif_node;
struct elseif_node
{
	//Condition
	node_t *condition;
	
	//Block
	node_t *block;
};

//Else Node
typedef struct else_node else_node;
struct else_node
{
	//Block
	node_t *block;
};

//While Loop
typedef struct while_node while_node;
struct while_node
{
	//Condition
	node_t *condition;
	
	//Block
	node_t *block;
};

//In Node
typedef struct in_node in_node;
struct in_node{
	//Element
	node_t *el;
	
	//List
	node_t *list;
};

//For In Node
typedef struct forin_node forin_node;
struct forin_node
{
	/*
		for($el in $list)
		{
			print $el # "\n";
		}
	*/
	//Element
	node_t *in;
	
	//Block
	node_t *block;
};

//Function
typedef struct function_node function_node;
struct function_node
{
	//Name
	char *name;
	
	//Parameters
	node_t *params;
	
	//Block
	node_t *block;
	
	//Return Type
	char strong;
};

//Function
typedef struct method_node method_node;
struct method_node
{
	//Name
	char *name;
	
	//Parameters
	node_t *params;
	
	//Block
	node_t *block;
	
	//Protection
	char protect;
	
	//Return Type
	char strong;
};

typedef struct param_node param_node;
struct param_node{
	//Parameter
	node_t *param;
	
	//Default
	node_t *def;
};


//Object Node
typedef struct object_node object_node;
struct object_node{
	//Name
	char *name;
	
	//Parents
	node_t *parents;
	
	//Attributes
	node_t *attrs;
	
	//Methods
	node_t *methods;
};

//Object Instance Node
typedef struct instance_node instance_node;
struct instance_node{
	char *name;
};

//Property Node
typedef struct property_node property_node;
struct property_node{
	node_t *obj;
	char *prop;
};

//Property Assign Node
typedef struct property_assign_node property_assign_node;
struct property_assign_node{
	node_t *prop;
	node_t *value;
};

//List Element
typedef struct element_node element_node;
struct element_node
{
	//List
	node_t *value;
	
	//Element
	node_t *el;
};

//Operator Node
typedef struct op_node op_node;
struct op_node
{
	//Left Value
	node_t *left;
	
	//Operator
	char *op;
	
	//Right Value
	node_t *right;
};

//Assignment Node
typedef struct assign_node assign_node;
struct assign_node
{
	//Name
	node_t *var;
	
	//Value
	node_t *value;
};

//List Assignment Node
typedef struct list_assign_node list_assign_node;
struct list_assign_node
{
	//Name
	node_t *name;
	
	//Element
	node_t *el;
	
	//Value
	node_t *value;
};

//Hash Assign Node
typedef struct hash_assign_node hash_assign_node;
struct hash_assign_node{
	node_t *var;
	node_t *key;
	node_t *val;
};

//Hash Lookup Node
typedef struct hash_lookup_node hash_lookup_node;
struct hash_lookup_node{
	node_t *var;
	node_t *key;
};

//Variable Node
typedef struct var_node var_node;
struct var_node
{
	//Name
	char *name;
	
	//Scope
	char scope;
	
	//Strong Type
	char strong;
};

//Hash Table Node
typedef struct hash_node hash_node;
struct hash_node{
	//Key
	node_t *key;
	
	//Value
	node_t *val;
};

//Function Call
typedef struct call_node call_node;
struct call_node
{
	//Name
	char *name;
	
	//Arguments
	node_t *args;
};

//Method Call
typedef struct method_call_node method_call_node;
struct method_call_node
{
	//Object
	node_t *obj;
	
	//Method
	char *method;
	
	//Arguments
	node_t *args;
};

//Return Statement
typedef struct return_node return_node;
struct return_node
{
	//Value
	node_t *value;
};

//Print Statement
typedef struct print_node print_node;
struct print_node
{
	//Value
	node_t *value;
};

//(Inc/Dec)rement Node
typedef struct incdec_node incdec_node;
struct incdec_node
{
	//Variable
	node_t *var;
};