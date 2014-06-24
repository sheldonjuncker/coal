#include "functions.h"

//Lookup Functions
funcs *lookupF(char *name, funcs *v)
{
	if(v->name == NULL)
	{
		printf("Error: function %s does not exist.", name);
		exit(1);
		return NULL;
	}
	
	else if(strcmp(name, v->name) == 0)
	{
		return v;
	}
	
	else if(strcmp(name,v->name) < 0)
	{
		//Take a left
		return lookupF(name,v->left);
	}
	
	else
	{
		//Take a right
		return lookupF(name,v->right);
	}
}

//Create Function
double addF(function_node *n, funcs *v)
{
	if(v->name == NULL)
	{
		v->left = coal_alloc(sizeof(funcs));
		v->left->name = NULL;
		v->name = n->name;
		v->block = n->block;
		v->params = n->params;
		v->strong = n->strong;
		v->right = coal_alloc(sizeof(funcs));
		v->right->name = NULL;
		return 0;
	}
	
	else if(strcmp(n->name,v->name) == 0)
	{
		printf("Error: function %s cannot be refined.\n", n->name);
		exit(1);
	}
	
	else if(strcmp(n->name,v->name) < 0)
	{
		//Take left
		return addF(n, v->left);
	}
	
	else
	{
		//Take right
		return addF(n, v->right);
	}
}