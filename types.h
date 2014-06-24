typedef struct Hash Hash;
typedef struct node_t node_t;

//Node Types
typedef enum node_type node_type;
enum node_type {
	VALUE_NODE,
	INT_NODE,
	DUB_NODE,
	STR_NODE,
	BOOL_NODE,
	LIST_NODE,
	HASH_NODE,
	INSTANCE_NODE,
	ELEMENT_NODE,
	ASSIGN_NODE,
	LIST_ASSIGN_NODE,
	VAR_ASSIGN_NODE,
	PROPERTY_ASSIGN_NODE,
	HASH_ASSIGN_NODE,
	HASH_LOOKUP_NODE,
	VAR_NODE,
	VAR_LOOKUP_NODE,
	PROPERTY_NODE,
	TERNARY_NODE,
	IF_NODE,
	ELSEIF_NODE,
	ELSE_NODE,
	WHILE_NODE,
	FORIN_NODE,
	IN_NODE,
	FUNCTION_NODE,
	METHOD_NODE,
	OBJECT_NODE,
	CALL_NODE,
	RETURN_NODE,
	PRINT_NODE,
	START_NODE,
	CONTROL_NODE,
	OP_NODE,
	MOP_NODE,
	COP_NODE,
	CAST_NODE,
	INCDEC_NODE,
	INC_NODE,
	DEC_NODE,
	INCA_NODE,
	INCB_NODE
};


//Node Template
struct node_t
{
	//Primary Type
	node_type p;
	
	//Secondary Type
	node_type s;
	
	//Tertiary Type
	node_type t;
	
	//Specific Node
	void *sn;
	
	//Next Node
	node_t *next;
};

//Statement
typedef struct stmt_t stmt_t;
struct stmt_t
{
	//Beginning
	node_t *b;
	
	//End
	node_t *e;
};

//MySQL Values
typedef union mysqls mysqls;
union mysqls{
	//Connection
	MYSQL *con;
	
	//Result
	MYSQL_RES *res;
	
	//Statement (type 't')
	MYSQL_STMT *stmt;
};

//Function Depth
int function_depth = 0;

//Return Depth
int return_depth = 0;

typedef struct List List;
typedef union xval xval;
typedef struct xvar xvar;

typedef struct Object Object;
typedef struct Attributes Attributes;
typedef struct Methods Methods;

//Attribute
struct Attributes{
	char *object;
	char *name;
	xvar *value;
	char strong;
	char protect;
	Attributes *next;
};

//Method
struct Methods{
	char *name;
	char strong;
	char protect;
	node_t *params;
	node_t *block;
	Methods *next;
};

//Object
struct Object{
	char *name;
	Attributes *attrs;
	Methods *methods;
};

//List or Array
struct List{
	xvar *var;
	List *next;
};

//Xval -- A Variable's Value
union xval{
	//String Value
	char *s;
	//Integer Value
	int i;
	//Double Value
	double d;
	//MySQL Values (Connection, Result, Row)
	mysqls *m;
	//List Value
	List *l;
	//Object Value
	Object *o;
	//Hash Table Value
	Hash **h;
	//File
	FILE *f;
};

//Xvar -- A Variable
struct xvar{
	char type;
	char strong;
	int refs;
	xval *val;
};

//Evaluate All Nodes
xvar *evaluate_all(node_t *);

//Evaluate a Node
xvar *evaluate(node_t *);