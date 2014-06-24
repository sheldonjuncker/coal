//The Coal Bison File
%{
	#include "tokens/header.h"
%}

//Unioon with Token Types
%union {
  double val;
  int i;
  double d;
  char *s;
  char *id;
  char *var;
  char *incop;
  char scope;
  char strong;
  char protect;
  stmt_t *stmts;
  node_t *exp;
};

%token PRINT
%token RETURN
%token FUNCTION
%token OBJECT
%token ATTRS METHODS
%token NEW
%token <scope> SCOPE
%token <strong> STRONG
%token <protect> PROTECTION
%token <s> STRING
%token <d> DUB
%token <i> INT_T
%token TRUE_T FALSE_T
%token <id> ID
%token <var> VAR
%token IF
%token ELSE 
%token ELSEIF
%token WHILE
%token FOR
%token SEMI COMMA DOT
%token <incop> INCOP
%left COLON
%left EQ
%left QMARK
%left AND OR
%left GE LE GT LT EQEQ NEQ EQEQEQ NEQEQ
%left CONCAT
%left PLUS MINUS
%left TIMES DIVIDE PERCENT
%left IN_T
%left LBRAK RBRAK
%left LPAREN RPAREN
%left LBRACE RBRACE
%type <exp> exp value variable increment call list instance property property_assign element assignment method_call param hash hash_lookup exp_in_exp property_var prop
%type <stmts> program block args optargs  optparams params optelseifs optelse stmts stmt_semi stmt control methods method opt_methods hashes opt_hashes opt_ids ids opt_props props function

%error-verbose
%locations
//-- GRAMMAR RULES ---------------------------------------
%%
//Program
program: stmts 		{
	$$ = $1;
	root = $$->b;
	$$->e->next = end_node();
	return 0;
}

//Statements
stmts: stmt	{ 
	$$ = $1;
}
|	stmts stmt	{
	$$ = link_stmts($1, $2);
}

//Control Structure 
stmt: control	{
	$$ = $1;
}

//Statement
stmt: stmt_semi SEMI	{
	$$ = $1;
}

//Code Block
block:	stmt	{
	$$ = $1;
}
|	LBRACE stmts RBRACE		{
	$$ = $2;
}
//Memory Leak, probably
|	SEMI	{
	stmt_t *s = coal_alloc(sizeof(stmt_t));
	s->b = NULL;
	s->e = NULL;
	$$ = s;
}

//Optional Else If Statements
optelseifs:	optelseifs ELSEIF exp block	{
	node_t *n = new_elseif_node($3, $4);
	stmt_t *ns = node_to_stmt(n);
	$$ = link_stmts($1, ns);
}
|	{ $$ = NULL; }

//Optional Hashes (For Hash Tables)
opt_hashes :  hashes	{ $$ = $1; }
|						{ $$ = NULL; }

//One or More Hash
hashes : value COLON exp		{
	node_t *n = new_hash_node($1, $3);
	$$ = node_to_stmt(n);
}
|	hashes COMMA value COLON exp		{
	node_t *n = new_hash_node($3, $5);
	stmt_t *s = node_to_stmt(n);
	$$ = link_stmts($1, s);
}

//Optional Methods (For Objects)
opt_methods : METHODS COLON methods	{ $$ = $3; }
|						{ $$ = NULL; }

methods : method		{ $$ = $1; }
| methods method		{
	$$ = link_stmts($1, $2);
}

//Optional Arguments
optargs: args		{
	$$ = $1;
}
|	{
	$$ = NULL;
}

args: exp	{
	$$ = node_to_stmt($1);
}
|	args COMMA exp	{
	stmt_t *n = node_to_stmt($3);
	$$ = link_stmts($1, n);
}

//Optional Ids
opt_ids: ids			{
	$$ = $1;
}
|	{
	$$ = NULL;
}

ids: ID	{
	$$ = node_to_stmt(new_id_node($1));
}
|	ids COMMA ID	{
	stmt_t *n1 = node_to_stmt(new_id_node($3));
	$$ = link_stmts($1, n1);
}

//Optional Property Vars
opt_props: ATTRS COLON props			{
	$$ = $3;
}
|	{
	$$ = NULL;
}

props: prop SEMI	{
	$$ = node_to_stmt($1);
}
|	props prop SEMI	{
	stmt_t *n1 = node_to_stmt($2);
	$$ = link_stmts($1, n1);
}

prop:	property_var			{
	$$ = new_param_node($1, NULL);
}
|		property_var COLON exp	{
	$$ = new_param_node($1, $3);
}


//Optional Parameters
optparams: params			{
	$$ = $1;
}
|	{
	$$ = NULL;
}

params: param	{
	$$ = node_to_stmt($1);
}
|	params COMMA param	{
	stmt_t *n1 = node_to_stmt($3);
	$$ = link_stmts($1, n1);
}

param:	variable			{
	$$ = new_param_node($1, NULL);
}
|		variable COLON exp	{
	$$ = new_param_node($1, $3);
}

//Optional Else Statement
optelse:	ELSE block		{
	$$ = $2;
}
|	{ $$ = NULL; }

//Control Structures (No Ending Semicolon)
control: WHILE LPAREN exp RPAREN block	{
	node_t *n = new_while_node($3, $5);
	$$ = node_to_stmt(n);
}
|	function		{ $$ = $1; }
|	IF exp block optelseifs optelse	{
	node_t *n = new_if_node($2, $3, $4, $5);
	$$ = node_to_stmt(n);
}

|	OBJECT ID LBRACE opt_props opt_methods RBRACE{
	$$ = node_to_stmt(new_object_node($2, NULL, $4, $5));
}

|	OBJECT ID COLON LBRAK opt_ids RBRAK LBRACE opt_props opt_methods RBRACE{
	$$ = node_to_stmt(new_object_node($2, $5, $8, $9));
}

|	FOR LPAREN exp_in_exp RPAREN block{
	node_t *n = new_forin_node($3, $5);
	$$ = node_to_stmt(n);
}

//Statement (Ends in Semicolon)
stmt_semi: PRINT exp	{
	$$ = node_to_stmt(new_print_node($2));
}
|	RETURN exp	{
	$$ = node_to_stmt(new_return_node($2));
}
|	exp		{
	$$ = node_to_stmt($1);
}

//Mathematical
exp: exp PLUS exp     	{ $$ = new_op_node(MOP_NODE, "+", $1, $3); }
|	exp MINUS exp		{ $$ = new_op_node(MOP_NODE, "-", $1, $3); }
|	exp TIMES exp		{ $$ = new_op_node(MOP_NODE, "*", $1, $3); }
|	exp DIVIDE exp		{ $$ = new_op_node(MOP_NODE, "/", $1, $3); }
|	exp PERCENT exp		{ $$ = new_op_node(MOP_NODE, "%", $1, $3); }
|	exp CONCAT exp		{ $$ = new_op_node(MOP_NODE, "#", $1, $3); }

//Comparison
|	exp NEQEQ exp			{ $$ = new_op_node(COP_NODE, "!==", $1, $3); }
|	exp EQEQEQ exp		{ $$ = new_op_node(COP_NODE, "===", $1, $3); }
|	exp NEQ exp			{ $$ = new_op_node(COP_NODE, "!=", $1, $3); }
|	exp EQEQ exp		{ $$ = new_op_node(COP_NODE, "==", $1, $3); }
|	exp LE exp		{ $$ = new_op_node(COP_NODE, "<=", $1, $3); }
|	exp GE exp		{ $$ = new_op_node(COP_NODE, ">=", $1, $3); }
|	exp GT exp		{ $$ = new_op_node(COP_NODE, ">", $1, $3); }
|	exp LT exp		{ $$ = new_op_node(COP_NODE, "<", $1, $3); }
|	exp AND exp		{ $$ = new_op_node(COP_NODE, "AND", $1, $3); }
|	exp OR exp		{ $$ = new_op_node(COP_NODE, "OR", $1, $3); }

//Ternary Operator
|	exp QMARK exp COLON exp	{ $$ = new_ternary_node($1, $3, $5); }

//Type Casting
|	LPAREN STRONG RPAREN exp	{ $$ = new_cast_node($2, $4); }

//Other
|	value				{ $$ = $1; }
|	variable			{ $$ = $1; }
|	hash_lookup			{ $$ = $1; }
|	hash				{ $$ = $1; }
|	increment			{ $$ = $1; }
| 	element				{ $$ = $1; }
|	assignment			{ $$ = $1; }
|	property			{ $$ = $1; }
|	property_assign		{ $$ = $1; }
|	call				{ $$ = $1; }
|	method_call			{ $$ = $1; }
|	instance			{ $$ = $1; }
|	list				{ $$ = $1; }
|	exp_in_exp			{ $$ = $1; }
|	LPAREN exp RPAREN	{ $$ = $2; }

//Hash Table
hash : LBRACE opt_hashes RBRACE	{
		$$ = new_hash_thingy($2);
}

//Function
function : FUNCTION ID LPAREN optparams RPAREN block	{
	$$ = node_to_stmt(new_function_node('a', $2, $4, $6));
}
| STRONG FUNCTION ID LPAREN optparams RPAREN block	{
	$$ = node_to_stmt(new_function_node($1, $3, $5, $7));
}

//Method
method : FUNCTION ID LPAREN optparams RPAREN block	{
	$$ = node_to_stmt(new_method_node('x', 'a', $2, $4, $6));
}
| STRONG FUNCTION ID LPAREN optparams RPAREN block	{
	$$ = node_to_stmt(new_method_node('x', $1, $3, $5, $7));
}
| PROTECTION FUNCTION ID LPAREN optparams RPAREN block	{
	$$ = node_to_stmt(new_method_node($1, 'a', $3, $5, $7));
}
| PROTECTION STRONG FUNCTION ID LPAREN optparams RPAREN block	{
	$$ = node_to_stmt(new_method_node($1, $2, $4, $6, $8));
}

//Property Access
property : variable DOT VAR	{
	$$ = new_property_node($1, $3);
}


//Property Assignment

property_assign : variable DOT VAR EQ exp {
	node_t *prop = new_property_node($1, $3);
	$$ = new_property_assign_node(prop, $5);
}

//Hash Lookup
hash_lookup : exp LBRACE exp RBRACE	{
	$$ = new_hash_lookup_node($1, $3);
}

//Assignment
assignment : variable EQ exp	{
	$$ = new_assign_node($1, $3);
}
| exp LBRAK exp RBRAK EQ exp	{
	if($1->p != VAR_NODE)
	{
		printf("Error: Assignment to a Value!\n");
		exit(1);
	}
	$$ = new_list_assign_node($1, $3, $6);
}
| exp LBRACE exp RBRACE EQ exp	{
	if($1->p != VAR_NODE)
	{
		printf("Error: Assignment to a Value!\n");
		exit(1);
	}
	$$ = new_hash_assign_node($1, $3, $6);
}


//Property Variable
property_var : VAR	{ $$ = new_var_node($1, 'x', 'a'); }
| STRONG VAR		{ $$ = new_var_node($2, 'x', $1); }
| PROTECTION VAR	{ $$ = new_var_node($2, $1, 'a'); }
| PROTECTION STRONG VAR	{ $$ = new_var_node($3, $1, $2); }

//Variables (Scoped or Unscoped)
variable : VAR	{ $$ = new_var_node($1, 'a', 'a');}
|	SCOPE VAR 	{ $$ = new_var_node($2, $1, 'a'); }
|	SCOPE STRONG VAR { $$ = new_var_node($3, $1, $2); }
|	STRONG VAR		{ $$ = new_var_node($2, 'a', $1); }



//Increment ($x++, $x--, ++$x, --$x)
increment :	variable INCOP	{
	$$ = new_incdec_node($1, $2, 1);
}
|	INCOP variable	{
	$$ = new_incdec_node($2, $1, 0);
}

//Function Call
call : ID LPAREN optargs RPAREN	{
	$$ = new_call_node($1, $3);
}

//Method Call
method_call : variable DOT ID LPAREN optargs RPAREN {
	$$ = new_method_call_node($1, $3, $5);
}

//New Object Instance
instance : NEW ID	{
	$$ = new_instance_node($2);
}

//List (Array)
list : LBRAK optargs RBRAK	{
	$$ = new_list_node($2);
}

//exp in exp, example, for($x in [1,2,3])
exp_in_exp : exp IN_T exp		{
	$$ = new_in_node($1, $3);
}


//Element (List or String)
element : exp LBRAK exp RBRAK {
	$$ = new_element_node($1, $3);
}

//Values (int, dub, str)
//MySQL has reserved the keyword "INT"

value :	INT_T			{ $$ = new_int_node($1); }
|	DUB					{ $$ = new_dub_node($1); }
|	STRING				{ $$ = new_str_node($1); }
|	TRUE_T				{ $$ = new_int_node(1); }
|	FALSE_T				{ $$ = new_int_node(0); }

%%//-- FUNCTION DEFINITIONS ---------------------------------

void coal_init()
{
	//Initialize Global and Local Variables
	hash_init_all();
	
	//Initialize Functions
	functions = coal_alloc(sizeof(funcs));
	functions->name = NULL;
	return;
}

int coal_parse(char *file)
{
	yyin = fopen(file, "r");
	if(!yyin)
	{
		printf("Error: Could not open file %s.\n", file);
		exit(1);
	}
	
	mmgr_init();
	do {
		yyparse();
	} while (!feof(yyin));
	fclose(yyin);
	free_xvar(evaluate_all(root));
	mmgr_free();
	return 0;
}

int main(int argc, char **argv)
{
	//Initialize
	coal_init();
	cgi_init();
	
	//First Line Number
	yylloc.first_line = 1;
	
	line_number = 0;
	coal_parse(argv[1]);
	
	if(prints == 0)
	{
		printf("Content-type: text/html\n\n<h2>This page didn't output any data!</h2><hr /><i>from Coal</i>");
	}
	return 0;
}