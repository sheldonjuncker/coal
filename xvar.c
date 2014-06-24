#include "xvar.h"

//A Blank Value
xval *blank_xval()
{
	xval *v = coal_alloc(sizeof(xval));
	v->s = NULL;
	v->m = NULL;
	v->h = NULL;
	v->o = NULL;
	return v;
}

//A Blank Variable
xvar *blank_xvar()
{
	xvar *var = coal_alloc(sizeof(xvar));
	var->val = blank_xval();
	var->strong = 'a';
	var->refs = 0;
	return var;
}

//An Integer Variable
xvar *integer_xvar(int i)
{
	xvar *v = blank_xvar();
	v->type = 'i';
	v->val->i = i;
	return v;
}

//A Double Variable
xvar *double_xvar(double d)
{
	xvar *v = blank_xvar();
	v->type = 'd';
	v->val->d = d;
	return v;
}

//A String Variable
xvar *string_xvar(char *s)
{
	xvar *v = blank_xvar();
	v->type = 's';
	v->val->s = s;
	return v;
}

//MySQL Connectoin Variable
xvar *mysql_con_xvar(MYSQL *con)
{
	xvar *v = blank_xvar();
	v->type = 'm';
	v->val->m = coal_alloc(sizeof(mysqls));
	v->val->m->con = con;
	return v;
}

//MySQL Result Variable
xvar *mysql_res_xvar(MYSQL_RES *res)
{
	xvar *v = blank_xvar();
	v->type = 'r';
	v->val->m = coal_alloc(sizeof(mysqls));
	v->val->m->res = res;
	return v;
}

//MySQL Stmt Variable
xvar *mysql_stmt_xvar(MYSQL_STMT *stmt)
{
	xvar *v = blank_xvar();
	v->type = 't';
	v->val->m = coal_alloc(sizeof(mysqls));
	v->val->m->stmt = stmt;
	return v;
}

//A List Variable
xvar *list_xvar(List *l)
{
	xvar *v = blank_xvar();
	v->type = 'l';
	v->val->l = dup_list(l);
	free_list(l);
	return v;
}

//Object Xvar
xvar *object_xvar(Object *o)
{
	xvar *v = blank_xvar();
	v->type = 'o';
	v->val->o = o;
	return v;
}

//Hash Xvar
//Object Xvar
xvar *hash_xvar(Hash **h)
{
	xvar *v = blank_xvar();
	v->type = 'h';
	v->val->h = h;
	return v;
}

//File Xvar
xvar *file_xvar(FILE *f)
{
	xvar *v = blank_xvar();
	v->type = 'f';
	v->val->f = f;
	return v;
}

//Get Double Value from Variable
double dubVal(xvar *v)
{
	if(v->type == 'd')
		return v->val->d;
	else if(v->type == 'i')
		return (double) v->val->i;
	else if(v->type == 's')
		return atof(v->val->s);
	else if(v->type == 'l')
	{
		int length = xvar_length(v);
		return (double) length;
	}
	else
		return 0;
}

//Get String Value from Variable
char *strVal(xvar *v)
{
	if(v->type == 's')
		return strdup(v->val->s);
	else if(v->type == 'd')
		return f2s(v->val->d);
	else if(v->type == 'i')
		return i2s(v->val->i);
	else if(v->type == 'l')
		return join_list(v->val->l, "");
	else
		return 0;
}

//Get List Value from Variable
List *listVal(xvar *v)
{
	if(v->type == 'l')
		return dup_list(v->val->l);
	else
	{
		List *l = coal_alloc(sizeof(List));
		l->next = NULL;
		l->var = dup_xvar(v);
		return l;
	}
}

//Add Two Variables
xvar *add_xvar(xvar *v1, xvar *v2)
{
	//Integers
	if(v1->type == 'i' && v2->type == 'i')
	{
		int res = v1->val->i + v2->val->i;
		return integer_xvar(res);
	}
	
	//Lists
	else if(v1->type == 'l' || v2->type == 'l')
	{
		xvar *return_val;
		
		List *l1 = listVal(v1);
		if(l1 == NULL)
			return list_xvar(listVal(v2));
			
		List *l2 = listVal(v2);
		if(l2 == NULL)
		{
			free_list(l1);
			return list_xvar(listVal(v1));
		}
		
		List *l = l1;
		List *p;
		
		while(l != NULL)
		{
			p = l;
			l = l->next;
		}
		p->next = l2;
		
		return_val = list_xvar(l1);
	
		return return_val;
	}
	
	//Doubles and Strings
	else
	{
		double d1 = dubVal(v1);
		double d2 = dubVal(v2);
		return double_xvar(d1 + d2);
	}
}

//Subtract Two Variables
xvar *sub_xvar(xvar *v1, xvar *v2)
{
	//Integers
	if(v1->type == 'i' && v2->type == 'i')
	{
		int res = v1->val->i - v2->val->i;
		return integer_xvar(res);
	}
	
	else
	{
		double d1 = dubVal(v1);
		double d2 = dubVal(v2);
		return double_xvar(d1 - d2);
	}
}

//Multiply Two Variables
xvar *mul_xvar(xvar *v1, xvar *v2)
{
	//Integers
	if(v1->type == 'i' && v2->type == 'i')
	{
		int res = v1->val->i * v2->val->i;
		return integer_xvar(res);
	}
	
	else
	{
		double d1 = dubVal(v1);
		double d2 = dubVal(v2);
		return double_xvar(d1 * d2);
	}
}

//Divide Two Variables
xvar *div_xvar(xvar *v1, xvar *v2)
{
	//Integers
	if(v1->type == 'i' && v2->type == 'i')
	{
		if(v2->val->i == 0)
		{
			printf("Error: division by zero is not allowed.\n");
			exit(1);
		}
		int res = v1->val->i / v2->val->i;
		return integer_xvar(res);
	}
	
	else
	{
		double d1 = dubVal(v1);
		double d2 = dubVal(v2);
		
		if(d2 == 0)
		{
			printf("Error: division by zero is not allowed.\n");
			exit(1);
		}
		
		return double_xvar(d1 / d2);
	}
}

//Modulo Operation
xvar *mod_xvar(xvar *left, xvar *right)
{
	int l = intVal(left);
	int r = intVal(right);
	return integer_xvar(l % r);
}

//Is an xvar a pointer?
int is_xvar_pointer(xvar *v)
{
	if(v->type == 'h' || v->type == 'o' || v->type == 'm' || v->type == 'r' || v->type == 't' || v->type == 'f')
		return 1;
	return 0;
}

//Is an xvar numeric?
int is_xvar_numeric(xvar *v)
{
	if(v->type == 'i' || v->type == 'd')
		return 1;
	else
		return 0;
}

//In Xvar (element IN list/string)
xvar *in_xvar(xvar *el, xvar *list)
{
	//List
	if(list->type == 'l')
	{
		List *l = list->val->l;
		
		while(l != NULL)
		{
			if(if_xvar(eq_xvar(el, l->var)))
				return integer_xvar(1);
			l = l->next;
		}
		
		return integer_xvar(0);
	}
	
	//String
	else if(list->type == 's')
	{
		char *s1 = list->val->s;
		char *s2 = strVal(el);
		char *pos = strstr(s1, s2);
		free(s2);
		
		if(pos != NULL)
			return integer_xvar(1);
		return integer_xvar(0);
	}
	
	//Other Types not Implemented
	else
	{
		return integer_xvar(0);
	}
}

//Are Two Variables Equal?
xvar *eq_xvar(xvar *v1, xvar *v2)
{
	//Strongly Typed Stuff
	if((v1->strong != 'a' || v2->strong != 'a') && v1->type != v2->type)
		return integer_xvar(0);
	
	//If they are both pointers, compare references
	if(is_xvar_pointer(v1) && is_xvar_pointer(v2))
		return integer_xvar(v1 == v2);
	
	//One pointer, one non-pointer
	else if(is_xvar_pointer(v1) || is_xvar_pointer(v2))
		return integer_xvar(0);
		
	//A Numeric Value and Somethin Else
	else if(is_xvar_numeric(v1) || is_xvar_numeric(v2))
		return integer_xvar(dubVal(v1) == dubVal(v2));
		
	//A String and Something Else
	else if(v1->type == 's' || v2->type == 's')
	{
		char *s1 = strVal(v1);
		char *s2 = strVal(v2);
		
		int cmp = strcmp(s1, s2);
		
		free(s1);
		free(s2);
		
		return integer_xvar(cmp == 0);
	}
	
	//Two Lists
	else if(v1->type == 'l' && v2->type == 'l')
	{
		//The lists
		List *l1 = v1->val->l;
		List *l2 = v2->val->l;
		
		//Is each element equal?
		while(l1 != NULL && l2 != NULL)
		{
			xvar *i = eq_xvar(l1->var, l2->var);
			if(!if_xvar(i))
			{
				return integer_xvar(0);
			}
			
			l1 = l1->next;
			l2 = l2->next;
		}
		
		//Wrong Lengths
		if(l1 != l2)
			return integer_xvar(0);
		return integer_xvar(1);
	}
	
	//Unknown Comparison
	return integer_xvar(0);
}

//Are Two Variables Completely Equal?
xvar *ceq_xvar(xvar *v1, xvar *v2)
{
	//Strongly Typed Stuff
	if((v1->strong != 'a' || v2->strong != 'a') && v1->type != v2->type)
		return integer_xvar(0);
	
	if(v1->type != v2->type)
		return integer_xvar(0);
	
	//If they are both pointers, compare references
	if(is_xvar_pointer(v1) && is_xvar_pointer(v2))
		return integer_xvar(v1 == v2);
	
	//One pointer, one non-pointer
	else if(is_xvar_pointer(v1) || is_xvar_pointer(v2))
		return integer_xvar(0);
		
	//A Numeric Value and Somethin Else
	else if(is_xvar_numeric(v1) || is_xvar_numeric(v2))
		return integer_xvar(dubVal(v1) == dubVal(v2));
		
	//A String and Something Else
	else if(v1->type == 's' || v2->type == 's')
	{
		char *s1 = strVal(v1);
		char *s2 = strVal(v2);
		
		int cmp = strcmp(s1, s2);
		
		free(s1);
		free(s2);
		
		return integer_xvar(cmp == 0);
	}
	
	//Two Lists
	else if(v1->type == 'l' && v2->type == 'l')
	{
		//The lists
		List *l1 = v1->val->l;
		List *l2 = v2->val->l;
		
		//Is each element equal?
		while(l1 != NULL && l2 != NULL)
		{
			xvar *i = ceq_xvar(l1->var, l2->var);
			if(!if_xvar(i))
			{
				return integer_xvar(0);
			}
			
			l1 = l1->next;
			l2 = l2->next;
		}
		
		//Wrong Lengths
		if(l1 != l2)
			return integer_xvar(0);
		return integer_xvar(1);
	}
	
	//Unknown Comparison
	return integer_xvar(0);
}

//Are Two Variables Not Equal?
xvar *neq_xvar(xvar *v1, xvar *v2)
{
	xvar *res = eq_xvar(v1, v2);
	res->val->i = !res->val->i;
	return res;
}

//Are Two Variables Completely Not Equal?
xvar *cneq_xvar(xvar *v1, xvar *v2)
{
	xvar *res = ceq_xvar(v1, v2);
	res->val->i = !res->val->i;
	return res;
}

//Is Variable 1 > Variable 2?
xvar *gt_xvar(xvar *v1, xvar *v2)
{
	if(v1->type == 's' && v2->type == 's')
	{
		if(strcmp(v1->val->s, v2->val->s) > 0)
			return integer_xvar(1);
		else
			return integer_xvar(0);
	}
	
	else
	{
		double d1 = dubVal(v1);
		double d2 = dubVal(v2);
		return integer_xvar(d1 > d2);
	}
}

//Is Variable 1 < Variable 2?
xvar *lt_xvar(xvar *v1, xvar *v2)
{
	if(v1->type == 's' && v2->type == 's')
	{
		if(strcmp(v1->val->s, v2->val->s) < 0)
			return integer_xvar(1);
		else
			return integer_xvar(0);
	}
	
	else
	{
		double d1 = dubVal(v1);
		double d2 = dubVal(v2);
		return integer_xvar(d1 < d2);
	}
}

//Is Variable 1 >= Variable 2?
xvar *gte_xvar(xvar *v1, xvar *v2)
{
	xvar *gt_res = gt_xvar(v1, v2);
	xvar *eq_res = eq_xvar(v1, v2);
	
	if(gt_res->val->i || eq_res->val->i)
		return integer_xvar(1);
	else
		return integer_xvar(0);
}

//Is Variable 1 <= Variable 2?
xvar *lte_xvar(xvar *v1, xvar *v2)
{
	xvar *lt_res = lt_xvar(v1, v2);
	xvar *eq_res = eq_xvar(v1, v2);
	
	if(lt_res->val->i || eq_res->val->i)
		return integer_xvar(1);
	else
		return integer_xvar(0);
}

//Is a Variable True?
int if_xvar(xvar *v)
{
	int retval = 0;
	if(v->type == 's')
	{
		int len = strlen(v->val->s);
		if(len == 0)
			retval = 0;
		
		else if(*v->val->s == '0')
			retval = 0;
		else
			retval = 1;
	}
	
	else if(v->type == 'l')
	{
		if(v->val->l == NULL)
			retval = 0;
		else
			retval = 1;
	}
	
	else if(is_xvar_pointer(v))
		retval = 1;
	
	else
		retval = (dubVal(v) != 0);
	
	free_xvar(v);
	return retval;
}

//Is Variable 1 OR Variable 2 True?
xvar *or_xvar(xvar *v1, xvar *v2)
{
	double v1_res = if_xvar(dup_xvar(v1));
	double v2_res = if_xvar(dup_xvar(v2));
	
	if(v1_res)
		return dup_xvar(v1);
	else if(v2_res)
		return dup_xvar(v2);
	else
		return integer_xvar(0);
}

//Is Variable 1 AND Variable 2 True?
xvar *and_xvar(xvar *v1, xvar *v2)
{
	double v1_res = if_xvar(dup_xvar(v1));
	double v2_res = if_xvar(dup_xvar(v2));
	
	if(v1_res && v2_res)
		return integer_xvar(1);
	else
		return integer_xvar(0);
}

//Increment After
xvar *inca_xvar(xvar *v)
{
	//Shift
	if(v->type == 'l' || v->type == 's')
	{
		return inc_xvar(v);
	}
	
	xvar *return_val = dup_xvar(v);
	inc_xvar(v);
	return return_val;
}

//Increment Before
xvar *incb_xvar(xvar *v)
{
	return dup_xvar(inc_xvar(v));
}

//Decrement After
xvar *deca_xvar(xvar *v)
{
	//Pop
	if(v->type == 'l' || v->type == 's')
	{
		return dec_xvar(v);
	}
	
	xvar *return_val = dup_xvar(v);
	dec_xvar(v);
	return return_val;
}

//Decrement Before
xvar *decb_xvar(xvar *v)
{
	return dup_xvar(dec_xvar(v));
}

//Increment a Variable
xvar *inc_xvar(xvar *v)
{
	if(v->type == 's')
	{
		//Empty String
		if(v->val->s == '\0')
			return string_xvar(strdup(v->val->s));
			
		//Great feature!
		char *s = coal_alloc(2);
		s[0] = *(v->val->s);
		s[1] = '\0';
		
		char *dup = strdup(v->val->s + 1);
		
		free(v->val->s);
		v->val->s = dup;
		
		return string_xvar(s);
	}
	
	else if(v->type == 'd')
	{
		v->val->d += 1;
	}
	
	else if(v->type == 'i')
	{
		v->val->i += 1;
	}
	
	else if(v->type == 'l')
	{
		List *first = v->val->l;
		
		if(first == NULL)
			return list_xvar(NULL);
		
		xvar *return_val = dup_xvar(first->var);
		v->val->l = v->val->l->next;
		free_xvar(first->var);
		free(first);
		return return_val;
	}
	
	else
	{
		printf("Error: You cannot increment anything other than a string, double, integer, or list.");
		exit(1);
	}
	
	return v;
}

//Decrement a Variable
xvar *dec_xvar(xvar *v)
{
	if(v->type == 's')
	{
		//Great feature!
		if(v->val->s == '\0')
		{
			return string_xvar(strdup(v->val->s));
		}
		
		char *copy = v->val->s;
		while(*(copy+1) != '\0')
			copy++;
			
		char *s = coal_alloc(2);
		s[0] = *copy;
		s[1] = '\0';
		*copy = '\0';
		
		return string_xvar(s);
	}
	
	else if(v->type == 'd')
	{
		v->val->d -= 1;
	}
	
	else if(v->type == 'i')
	{
		v->val->i--;
	}
	
	else if(v->type == 'l')
	{
		if(v->val->l == NULL)
			return list_xvar(NULL);
		if(v->val->l->next == NULL)
		{
			
		}
		List *l = v->val->l;
		List *lc = NULL;
		
		while(l->next != NULL)
		{
			lc = l;
			l = l->next;
		}
		
		xvar *return_val = dup_xvar(l->var);
		
		if(lc == NULL)
		{
			v->val->l = NULL;
			free_xvar(l->var);
			free(l);
		}
		
		else
		{
			free_xvar(l->var);
			free(l);
			lc->next = NULL;
		}
		
		return return_val;
	}
	
	else
	{
		printf("Error: You cannot decrement anything other than a string, double, integer, or list.");
		exit(1);
	}
	
	return v;
}

//Concatenate Two Variables
xvar *cat_xvar(xvar *v1, xvar *v2)
{
	char *str1 = strVal(v1);
	char *str2 = strVal(v2);
	xvar *return_val = string_xvar(concat(str1, str2));
	free(str1);
	free(str2);
	return return_val;
}

//Pop a List Variable
xvar *pop_xvar(xvar *v)
{
	List *l = v->val->l;
	
	if(l == NULL)
		return NULL;
	
	List *p;
	while(l->next != NULL)
	{
		p = l;
		l = l->next;
	}
	
	xvar *r = dup_xvar(l->var);
	p->next = NULL;
	return r;
}

//Shift a List Variable
xvar *shift_xvar(xvar *v)
{
	List *l = v->val->l;
	
	if(l == NULL)
		return NULL;
	
	v->val->l = v->val->l->next;
	return l->var;
}

//Print a Variable
void print_xvar(xvar *exp)
{
	if(exp->type == 's')
		printf("%s", exp->val->s);
	else if(exp->type == 'd')
		printf("%f", exp->val->d);
	else if(exp->type == 'i')
		printf("%i", exp->val->i);
	else if(exp->type == 'l')
	{
		List *l = exp->val->l;
		while(l != NULL)
		{
			print_xvar(l->var);
			l = l->next;
		}
	}
	else
	{
		printf("Error: Can't print!\n");
	}
}

//Duplicate a List
List *dup_list(List *original)
{
	List *l = coal_alloc(sizeof(List));
	List *copy = l;
	List *p = copy;
	int i = 0;
	
	while(original != NULL)
	{
		copy->var = dup_xvar(original->var);
		copy->next = coal_alloc(sizeof(List));
		p = copy;
		copy = copy->next;
		original = original->next;
		i++;
	}
		
	if(i)
	{
		free(copy);
		p->next = NULL;
	}
		
	else
	{
		free(l);
		l = NULL;
	}
		
	return l;
}

//Increment Xvar References (If Applicable)
void xvar_inc_refs(xvar *v)
{
	char type = v->type;
	
	if(type == 'h' || type == 'o' || type == 'm' || type == 'r' || type == 't' || v->type == 'f')
		v->refs++;
		
	return;
}

//Free a List
void free_list(List *l)
{
	List *p = l;
	while(l != NULL)
	{
		free_xvar(l->var);
		l = l->next;
		free(p);
		p = l;
	}
	return;
}

//Duplicate an Xvar
xvar *dup_xvar(xvar *v)
{
	//MySQL, Object, and Hash Values are not Duplicated Unless the copy() function is used.
	if(v == NULL)
		return v;
		
	//Don't Duplicate Dictionaries or Objects, Database Stuff or Files
	if(v->type == 'h' || v->type == 'o' || v->type == 'm' || v->type == 'r' || v->type == 't' || v->type == 'f')
		return v;
	
	xvar *d = blank_xvar();
	d->type = v->type;
	d->strong = v->strong;
	
	if(v->type == 'i')
		d->val->i = v->val->i;
	else if(v->type == 'd')
		d->val->d = v->val->d;
	else if(v->type == 's')
		d->val->s = strdup(v->val->s);
	else if(v->type == 'l')
		d->val->l = dup_list(v->val->l);
	else
		return NULL;
	return d;
}

//Free a Variable
void free_xvar(xvar *v)
{
	if(v == NULL)
		return;
	
	//Don't Free Dictionaries or Objects
	if(v->type == 'h' || v->type == 'o' || v->type == 'm' || v->type == 'r' || v->type == 't' || v->type == 'f')
	{
		if(!v->refs)
		{
			destroy_xvar(v);
		}
		return;
	}
	
	//Free a String
	if(v->type == 's')
		free(v->val->s);
	
	//Free a List
	else if(v->type == 'l')
		free_list(v->val->l);
	
	free(v->val);
	free(v);
	return;
}

/*
	The destroy functions completely obliterate values, unless they are still being referenced by a variable somewhere.
*/

//Destroy List
void destroy_list(List *l)
{
	List *p = l;
	while(l != NULL)
	{
		destroy_xvar(l->var);
		l = l->next;
		free(p);
		p = l;
	}
	return;
}

//Destroy an Xvar (For Dictionaries, Objects, MySQL, and Files)
void destroy_xvar(xvar *v)
{
	if(v == NULL)
		return;
		
	//We can only free a variable if nothing else is referencing it.
	if(v->refs > 1)
	{
		//One Reference has been Destroyed
		v->refs--;
		return;
	}
	
	//Free a String
	if(v->type == 's')
		free(v->val->s);
	
	//File Handle
	else if(v->type == 'f')
		fclose(v->val->f);
	
	//MySQL Connection
	else if(v->type == 'm')
	{
		mysql_close(v->val->m->con);
		free(v->val->m);
	}
	
	//MySQL Result
	else if(v->type == 'r')
	{
		mysql_free_result(v->val->m->res);
		free(v->val->m);
	}
	
	//MySQL Stmt
	else if(v->type == 't')
	{
		mysql_stmt_close(v->val->m->stmt);
		free(v->val->m);
	}
	
	//Dictionary
	else if(v->type == 'h')
	{
		extern void hash_free(Hash **);
		hash_free(v->val->h);
		free(v->val->h);
	}
	
	//Object
	else if(v->type == 'o')
	{
		free(v->val->o->name);
		Attributes *a = v->val->o->attrs;
		Attributes *c = a;
		while(a != NULL)
		{
			free(a->name);
			destroy_xvar(a->value);
			a = a->next;
			free(c);
			c = a;
		}
		free(v->val->o);
	}
	
	//Free a List
	else if(v->type == 'l')
		destroy_list(v->val->l);
	
	free(v->val);
	free(v);
	return;
}

//Duplicate and Free
xvar *dup_free_xvar(xvar *v)
{
	xvar *dup = dup_xvar(v);
	free_xvar(v);
	return dup;
}

//Delete Element
void delete_element(xvar *v, int el)
{
	List *l = v->val->l;
	List *p = l;
	
	if(el == 0)
	{
		free_xvar(l->var);
		v->val->l = v->val->l->next;
		free(l);
	}
	
	while(l != NULL)
	{
		if(el == 0)
		{
			p->next = l->next;
			free_xvar(l->var);
			free(l);
			return;
		}
		
		el--;
		p = l;
		l = l->next;
	}
	return;
}