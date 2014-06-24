//Determines Whether Scope Should be Local or Global
#define SET_SCOPE() if(scope == 'l')\
			hash_ptr = locals_ptr;\
		else if(scope == 'g')\
			hash_ptr = globals;\
		else\
			hash_ptr = (function_depth) ? locals_ptr : globals;

//Strong Typing

//Global Scope
#define GLOBAL_SCOPE() hash_ptr = globals

//Null Terminator for Strings
#define NUL '\0'

//String Data Type
#define String char *

//Don't Use Malloc
#define malloc(i) coal_alloc(i)

//Dereference Nodes
#define DEREF(name, value, type) type name = (type) value->sn

//True and False
#define false integer_xvar(0);
#define true integer_xvar(1);

//Fatal Error
#define fe(...) if(prints == 0){ printf("Content-type: text/html\n\n"); } printf(__VA_ARGS__); exit(1)
