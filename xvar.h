#define intVal(x) (int) dubVal(x)

//The Join List Function
char *join_list(List *, char *);

//The Xvar Length Function
int xvar_length(xvar *);

//A Blank Value
xval *blank_xval(void);

//A Blank Variable
xvar *blank_xvar(void);

//An Integer Variable
xvar *integer_xvar(int);

//A Double Variable
xvar *double_xvar(double);

//A String Variable
xvar *string_xvar(char *);

//A List Variable
xvar *list_xvar(List *);

//Get Double Value from Variable
double dubVal(xvar *);

//Get String Value from Variable
char *strVal(xvar *);

//Get List Value from Variable
List *listVal(xvar *);

//Add Two Variables
xvar *add_xvar(xvar *, xvar *);

//Subtract Two Variables
xvar *sub_xvar(xvar *, xvar *);

//Multiply Two Variables
xvar *mul_xvar(xvar *, xvar *);

//Subtract Two Variables
xvar *div_xvar(xvar *, xvar *);

//Concatenate Two Variables
xvar *cat_xvar(xvar *, xvar *);

//Modulo Operation
xvar *mod_xvar(xvar *, xvar*);

//Is an element in a list?
xvar *in_xvar(xvar *, xvar *);

//Are Two Variables Equal?
xvar *eq_xvar(xvar *, xvar *);

//Are Two Variables Not Equal?
xvar *neq_xvar(xvar *, xvar *);

//Is Variable 1 < Variable 2?
xvar *lt_xvar(xvar *, xvar *);

//Is Variable 1 > Variable 2?
xvar *gt_xvar(xvar *, xvar *);

//Is Variable 1 <= Variable 2?
xvar *lte_xvar(xvar *, xvar *);

//Is Variable 1 >=  Variable 2?
xvar *gte_xvar(xvar *, xvar *);

//Is a Variable True?
int if_xvar(xvar *);

//Is Variable 1 OR Variable 2 True?
xvar *or_xvar(xvar *, xvar *);

//Is Variable 1 AND Variable 2 True?
xvar *and_xvar(xvar *, xvar *);

//Increment a Variable
xvar *inc_xvar(xvar *);

//Decrement a Variable
xvar *dec_xvar(xvar *);

//Concatenate Two Variables
xvar *cat_xvar(xvar *, xvar *);

//Pop a List Variable
xvar *pop_xvar(xvar *);

//Shift a List Variable
xvar *shift_xvar(xvar *);

//Print a Variable
void print_xvar(xvar *);

//Duplicate a List
List *dup_list(List *);

//Free a List
void free_list(List *);

//Duplicate an Xvar
xvar *dup_xvar(xvar *);

//Destroy an Xvar
void destroy_xvar(xvar *);

//Destroy List
void destroy_list(List *);

//Free an Xvar
void free_xvar(xvar *);

//Duplicate and Free
xvar *dup_free_xvar(xvar *);

//Delete Element
void delete_element(xvar *, int);