//The Bison Header Information
#include <stdio.h>
#include <mysql.h>
#include <pcre.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../coal_stdlib.h"
#include "../macros.h"
#include "../types.h"
#include "../xvar.c"
#include "../nodes.c"
#include "../hash.c"
#include "../functions.c"
#include "../functions/cgi.c"
#include "../functions/mysql.c"
#include "../functions/list.c"
#include "../lookup.c"
#include "../evaluate.c"

extern int yylex();
extern int yyparse();
extern FILE *yyin;

//Abstract Syntax Tree
node_t *root;

//Line Number
int line_number = 1;

//Number of Outputs
int prints = 0;

int yylex();
int yyerror(const char *p)
{
	printf("Content-type:text/html\n\n%s on line %i\n", p, line_number);
	exit(1);
}