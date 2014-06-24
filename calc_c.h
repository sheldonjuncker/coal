%{
//The Coal Flex File
#include "calc_c.tab.c"
#include <stdio.h>
#include <stdlib.h>

//Line Numbers
extern YYLTYPE yylloc;
%}

%option yylineno
%option noyywrap
%x IN_COMMENT

%%
-?[0-9]+ { yylval.i = atoi(yytext); return INT_T; }

-?[0-9]+(\.[0-9]+)?  { yylval.d = atof(yytext); return DUB; }

\$[_a-zA-Z0-9]+	{ yylval.id = strdup(yytext); return VAR; }

\"([^\"\\]|\\.)*\"		{ yylval.s = strdup(yytext); return STRING; }

 <INITIAL>{
     "/*"              BEGIN(IN_COMMENT);
     }
     <IN_COMMENT>{
     "*/"      BEGIN(INITIAL);
     [^*\n]+   // eat comment in chunks
     "*"       // eat the lone star
     \n        ++line_number;
     }

\/\/.*	{ }
\#\!.* { ++line_number; /*cgi*/ }

\'[^\']*\'		{ yylval.s = strdup(yytext); return STRING; }

"print"	{ return PRINT; }
"function" { return FUNCTION; }
\-\-|\+\+	{ yylval.incop = strdup(yytext); return INCOP; }
"+"	{ return PLUS; }
"-"	{ return MINUS; }
"*"	{ return TIMES; }
"/"	{ return DIVIDE; }
"%"	{ return PERCENT; }
"#"	{ return CONCAT; }
"==="	{ return EQEQEQ; }
"!=="	{ return NEQEQ; }
"=="	{ return EQEQ; }
"!="	{ return NEQ; }
"="		{ return EQ; }
">="	{ return GE; }
"<="	{ return LE; }
">"		{ return GT; }
"<"		{ return LT; }
[tT][rR][uU][eE]	{ return TRUE_T; }
[fF][aA][lL][sS][eE]	{ return FALSE_T; }
[oO][rR]	{ return OR; }
[aA][nN][dD]	{ return AND; }
[iI][fF]	{ return IF; }
[eE][lL][sS][eE]	{ return ELSE; }
[eE][lL][sS][eE][iI][fF]	{ return ELSEIF; }
[wW][hH][iI][lL][eE]	{ return WHILE; }
[fF][oO][rR]			{ return FOR; }
[iI][nN]				{ return IN_T; }
[rR][eE][tT][uU][rR][nN]	{ return RETURN; }
"new"		{ return NEW; }
"object"	{ return OBJECT; }
"Attributes"	{ return ATTRS; }
"Methods"		{ return METHODS; }
(global|local)	{ yylval.scope = *yytext; return SCOPE; }
(string|int|double|list|object|mysql|result|hash|bool)	{yylval.strong = *yytext; return STRONG; }
(public|private|protected|privtectlic|readonly) {
	if(!strcmp(yytext, "public"))
		yylval.protect = 'x';
	else if(!strcmp(yytext, "private"))
		yylval.protect = 'y';
	else if(!strcmp(yytext, "protected"))
		yylval.protect = 'z';
	else if(!strcmp(yytext, "readonly"))
		yylval.protect = 'R';
	else
		yylval.protect = 'w';
	return PROTECTION;
}
[_a-zA-Z][_a-zA-Z0-9]*	{ yylval.id = strdup(yytext); return ID; }
"("      { return LPAREN; }
")"      { return RPAREN; }
"{"      { return LBRACE; }
"}"      { return RBRACE; }
"["		{ return LBRAK; }
"]"		{ return RBRAK; }
";"      { return SEMI; }
"?"		{ return QMARK; }
":"		{ return COLON; }
","		{ return COMMA; }
"."		{ return DOT;	}
<<EOF>>  { return 0; }
[\n]		{ yylloc.first_line++; line_number++; }
[ \t\r]+	{ }
.        { printf("Unrecognized token!\n"); exit(1); }
%%