#include "evaluate.h"
extern node_t *root;
extern prints;

xvar *evaluate(node_t *n)
{
	//Values: Numbers, Strings, Lists, Elements
	if(n->p == VALUE_NODE)
	{
		//List
		if(n->s == LIST_NODE)
		{
			List *l = coal_alloc(sizeof(List));
			List *copy = l;
			List *p = copy;
			int i = 0;
			
			node_t *base = ((list_node *) n->sn)->l;
			
			while(base != NULL)
			{
				copy->var = evaluate(base);
				
				//Referencing
				xvar_inc_refs(copy->var);
				
				copy->next = coal_alloc(sizeof(List));
				p = copy;
				copy = copy->next;
				base = base->next;
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
			return list_xvar(l);
		}
		
		//List Element
		else if(n->s == ELEMENT_NODE)
		{
			element_node *en = (element_node *) n->sn;
			
			//The Element
			xvar *element = evaluate(en->el);
			int el = intVal(element);
			free_xvar(element);
			
			int el_save = el;
			xvar *l = evaluate(en->value);
			
			int length = xvar_length(l);
			if(el < 0)
				el += length;
				
			if(el < 0)
			{
				printf("Error: List element %i out of bounds.", el);
				exit(1);
			}
			
			//Is it a string or a list?
			
			//List
			if(l->type == 'l')
			{
				List *list = l->val->l;
			
				while(el > 0 && list != NULL)
				{
					list = list->next;
					el--;
				}
			
				if(el > 0 || list == 	NULL)
				{
					printf("Error: list element %i out of bounds.\n", el_save);
					exit(1);
				}
			
				xvar *return_val = dup_xvar(list->var);
				
				//Free and Return
				free_xvar(l);
				return return_val;
			}
			
			//String
			else if(l->type == 's')
			{
				char *s = l->val->s;
				if(strlen(s) > el)
				{
					//Get Character
					char *c = malloc(2);
					c[0] = s[el];
					c[1] = '\0';
					
					//Free and Return
					free_xvar(l);
					return string_xvar(c);
				}
				
				//Out of Bounds String Character
				else
				{
					printf("Error: string element %i out of bounds.\n", el_save);
					exit(1);
				}
			}
		}
		
		//Hash Table Node
		else if(n->s == HASH_NODE)
		{
			node_t *h;
			if(n->sn == NULL)
			{
				h = NULL;
			}
			else
			{
				h = (node_t *) n->sn;
			}
			
			Hash **hash = malloc(sizeof(Hash) * HASHSIZE);
			Hash **hash_save = hash_ptr;
			
			hash_ptr = hash;
			hash_init();
			
			xvar *key;
			xvar *val;
			
			while(h != NULL)
			{
				hash_node *hn = (hash_node *) h->sn;
				
				hash_ptr = hash_save;
				key = evaluate(hn->key);
				val = evaluate(hn->val);
				
				//Referencing
				xvar_inc_refs(val);
				
				char *k = strVal(key);
				hash_ptr = hash;
				add(k, dup_xvar(val));
				
				free(k);
				free_xvar(key);
				free_xvar(val);
				
				h = h->next;
			}
			
			hash_ptr = hash_save;
			return hash_xvar(hash);
		}
		
		//String
		else if(n->s == STR_NODE)
		{
			//Small Memory Leak Somehwere In Here
			char *s = strdup(((str_node *) n->sn)->s);
			
			char first = *s;
			
			char *free_copy = s;
			s++;
			char *copy = s;
			while(*(copy+1) != '\0')
				copy++;
			*copy = '\0';
			char c;
			char next;
			char *ns = coal_alloc(strlen(s) + 1);
			copy = ns;
			char *base = ns;
			char *result = strdup("");
			
			//Parse Backslashes in Strings
			while(*s)
			{
				c = *s;
				if(c == '\\')
				{
					next = *(s+1);
					
					if(next == 'n')
						*copy = '\n';
					else if(next == 't')
						*copy = '\t';
					else if(next == '\r')
						*copy = '\r';
					else
						*copy = next;
					s++;
				}
				
				else if(c == '{' && first == '"')
				{
					//Interpolation
					//String One
					char save = *copy;
					*copy = '\0';
					char *one = strdup(base);
					*copy = save;
					
					s++;
					char *beg = s;
					
					while(*s && *s != '}')
					{
						s++;
					}
					*s = '\0';
					char *name = strdup(beg);
					*s = '}';
					
					//Add Semicolon
					char *nc = name;
					name = concat(name, ";");
					free(nc);
					
					
					//String Two
					node_t *nsave = root;
					
					/*
					yyin = tmpfile();
					char *nc = name;
					while(*nc)
					{
						putc(*nc, yyin);
						nc++;
					}
					putc(';', yyin);
					rewind(yyin);
					
					...
					
					do {
						yyparse();
					} while (!feof(yyin));
					
					fclose(yyin);
					*/
					
					mmgr_node *save_b = mmgr_b;
					mmgr_node *save_c = mmgr_c;
					
					mmgr_init();
					
					YY_BUFFER_STATE string_buffer = yy_scan_string(name);
					yyparse();
					yy_delete_buffer(string_buffer);
					
					xvar *v = evaluate_all(root);
					mmgr_free();
					
					mmgr_b = save_b;
					mmgr_b = save_b;
					
					root = nsave;
					char *two = strVal(v);
					
					//Update Result
					char *one_two = concat(one, two);
					free_xvar(v);
					free(one);
					free(two);
					free(name);
					char *result_free = result;
					result = concat(result, one_two);
					free(result_free);
					free(one_two);
					
					base = copy;
					
					copy--;
				}
				
				else
					*copy = *s;
		
				copy++;
				s++;
			}
			*copy = '\0';
			
			char *result_free = result;
			result = concat(result, base);
			
			free(result_free);
			free(free_copy);
			free(ns);
			return string_xvar(result);
		}
		
		//Double
		else if(n->s == DUB_NODE)
		{
			double d = ((dub_node *) n->sn)->d;
			return double_xvar(d);
		}
		
		//Integer
		else if(n->s == INT_NODE)
		{
			int i = ((int_node *) n->sn)->i;
			return integer_xvar(i);
		}
		
		//New Object Instance
		else if(n->s == INSTANCE_NODE)
		{
			instance_node *in = (instance_node *) n->sn;
			char *name = in->name;
			GLOBAL_SCOPE();
			xvar *object = lookup(name, 1);
			
			Object *x = object->val->o;
			Object *o = coal_alloc(sizeof(Object));
			o->name = strdup(x->name);
			o->attrs = coal_alloc(sizeof(Attributes));
			o->methods = x->methods;
			
			int i = 0;
			Attributes *oa = o->attrs;
			Attributes *oa_copy = oa;
			Attributes *xa = x->attrs;
			while(xa != NULL)
			{
				oa->next = coal_alloc(sizeof(Attributes));
				oa->name = strdup(xa->name);
				oa->strong = xa->strong;
				oa->protect = xa->protect;
				oa->value = dup_xvar(xa->value);
				oa_copy = oa;
				oa = oa->next;
				xa = xa->next;
				i++;
			}
			
			if(i)
			{
				free(oa_copy->next);
				oa_copy->next = NULL;
			}
			
			else
			{
				free(o->attrs);
				o->attrs = NULL;
			}
			
			return object_xvar(o);
		}
		
		else
		{
			printf("Unrecognized Value Type %i\n", n->s);
			exit(1);
		}
	}
	
	/*
		Variable Lookup
		Variables, Object Properties, and Dictionary Elements
	*/
	
	else if(n->p == VAR_NODE)
	{
		//Property
		if(n->s == PROPERTY_NODE)
		{
			Attributes *a = lookupObjProp(n, 1);
			return dup_xvar(a->value);
		}
		//Hash Lookup
		else if(n->s == HASH_LOOKUP_NODE)
			return dup_xvar(lookupDictVal(n, 1));
		//Variable Lookup
		else
			return dup_xvar(lookupVar(n, 1));
	}
	
	/*
		Assignment
		Variable, Object Property, List/String Element, Dictionary Element
	*/
	
	else if(n->p == ASSIGN_NODE)
	{
		//Property Assign
		if(n->s == PROPERTY_ASSIGN_NODE)
		{
			property_assign_node *pn = (property_assign_node *) n->sn;
			
			Attributes *a = lookupObjProp(pn->prop, 1);
			xvar *value = evaluate(pn->value);
			
			//References
			xvar_inc_refs(value);
			
			if(a->strong != 'a' && a->strong != value->type)
			{
				printf("Error: Cannot assign value of type %c to a strongly typed object property of type %c.\n", value->type, a->strong);
				exit(1);
			}
			
			//Read Only Property
			if(a->protect == 'R' && strcmp(a->object, "$self"))
			{
				printf("Error: Cannot assign a value to a read-only object property.\n");
				exit(1);
			}
			
			free_xvar(a->value);
			a->value = dup_xvar(value);
			return value;
		}
		
		else if(n->s == VAR_ASSIGN_NODE)
		{
			assign_node *an = (assign_node *) n->sn;
			xvar *value = evaluate(an->value);
			node_t *vn = an->var;
			var_node *var = (var_node *) vn->sn;
			char *name = var->name;
			char scope = var->scope;
			char strong = var->strong;
			
			//Strongly Typed Variable
			if(strong != 'a')
			{
				//Wrong Type
				if(strong != value->type)
				{
					printf("Error: Cannot assign a value of type %c to a strongly typed variable of type %c.\n", value->type, strong);
					exit(1);
				}
			}
			
			SET_SCOPE();
			
			//References
			xvar_inc_refs(value);
			
			xvar *dup = dup_xvar(value);
			dup->strong = strong;
			
			add(name, dup);
			
			//Weakly Typed
			value->strong = 'a';
			return value;
		}
		
		else if(n->s == LIST_ASSIGN_NODE)
		{
			//List Assign Node
			list_assign_node *an = (list_assign_node *) n->sn;
			
			//Get the Element and then Free
			xvar *element = evaluate(an->el);
			int el = intVal(element);
			free_xvar(element);
			
			//Value to Assign
			xvar *value = evaluate(an->value);
			
			//References
			xvar_inc_refs(value);
			
			//Name of Variable
			xvar *list = lookupVar(an->name, 1);
			
			//Is it a List or a String?
			
			//List Element Assignment
			if(list->type == 'l')
			{
				List *l = list->val->l;
			
				//Find the Element
				int i = 0;
				while(l != NULL)
				{
					//Add Value
					if(i == el)
					{
						destroy_xvar(l->var);
						l->var = dup_xvar(value);
						break;
					}
					i++;
					l = l->next;
				}
				//Out of Range
				if(l == NULL)
				{
					printf("Error: Out of range assignment on list.\n");
					exit(1);
				}
				return value;
			}
			
			//String Element Assignment
			else if(list->type == 's')
			{
				//Get the String Values and New Character Value
				char *s = list->val->s;
				char *ns = strVal(value);
				char n = *ns;
				free(ns);
				
				//And its Length
				int len = strlen(s);
				
				//Out of bounds?
				if(el >= len)
				{
					printf("Error: Out of range assignment on string.\n");
					exit(1);
				}
				
				s[el] = n;
				
				return value;
			}
			
			//Hash Assignment (Error)
			else if(list->type == 'h')
			{
				printf("Error: When you're assigning to a dictionary you've got to use braces ({}) and not brackets ([]).\n");
				exit(1);
			}
			
			//Error
			else
			{
				printf("Error: Assigning to non-list element as if it were a list.\n");
				exit(1);
			}
		}
		
		//Hash Table Assign
		else if(n->s == HASH_ASSIGN_NODE)
		{
			hash_assign_node *hn = (hash_assign_node *) n->sn;
			xvar *var = evaluate(hn->var);
			
			xvar *tmp = evaluate(hn->key);
			char *key = strVal(tmp);
			free_xvar(tmp);
			
			xvar *value = evaluate(hn->val);
			
			//References
			xvar_inc_refs(value);
			
			//Non Hash Table
			if(var->type != 'h')
			{
				printf("Error: Treating non-dictionary variable as a dictionary.\n");
				exit(1);
			}
			Hash **hash_save = hash_ptr;
			hash_ptr = var->val->h;
			add(key, dup_xvar(value));
			hash_ptr = hash_save;
			
			//Free and Return
			free(key);
			free_xvar(var);
			return value;
		}
	}
	
	//Increment a Variable
	else if(n->p == INCDEC_NODE)
	{
		node_t *var = ((incdec_node *) n->sn)->var;
		var_node *vn = (var_node *) var->sn;
		char *name = vn->name;
		char scope = vn->scope;
		
		SET_SCOPE();
		
		xvar *value = lookup(name, 1);
		
		//Increment
		if(n->s == INC_NODE)
		{
			//var++
			if(n->t == INCA_NODE)
				return inca_xvar(value);
			//++var
			else
				return incb_xvar(value);
		}
		
		//Decrement
		else
		{
			//var--
			if(n->t == INCA_NODE)
				return deca_xvar(value);
			//--var
			else
				return decb_xvar(value);
		}
	}
	
	//Print a Value
	else if(n->p == PRINT_NODE)
	{
		if(prints == 0)
		{
			prints++;
			printf("\n");
		}
		print_node *pn = (print_node *) n->sn;
		
		xvar *exp = evaluate(pn->value);
		print_xvar(exp);
		free_xvar(exp);
		return NULL;
	}
	
	//exp IN exp (returns 0 or 1)
	else if(n->p == IN_NODE)
	{
		in_node *in = (in_node *) n->sn;
		
		//The List and Element
		xvar *el = evaluate(in->el);
		xvar *list = evaluate(in->list);
		
		//Is the element in the list?
		xvar *return_val = in_xvar(el, list);
		
		//Free
		free_xvar(el);
		free_xvar(list);
		
		//Return
		return return_val;
	}
	
	//Control Structures (if, while)
	else if(n->p == CONTROL_NODE)
	{
		if(n->s == IF_NODE)
		{
			if_node *in = (if_node *) n->sn;
			xvar *exp = evaluate(in->condition);
			if(if_xvar(exp))
			{
				free_xvar(evaluate_all(in->block));
				return NULL;
			}
			
			else
			{
				node_t *n2 = in->opt_elseifs;
				while(n2 != NULL)
				{
					elseif_node *ein = (elseif_node *) n2->sn;
					exp = evaluate(ein->condition);
					if(if_xvar(exp))
					{
						 free_xvar(evaluate_all(ein->block));
						return NULL;				}
					n2 = n2->next;
				}
				
				//Else
				if(in->opt_else != NULL)
				{
					free_xvar(evaluate_all(in->opt_else));
					return NULL;
				}
			}
			return NULL;
		}
		
		else if(n->s == WHILE_NODE)
		{
			while_node *wn = (while_node *) n->sn;
			while(if_xvar(evaluate(wn->condition)))
			{
				free_xvar(evaluate_all(wn->block));
			}
		}
		
		/*
			for(element IN (list|string))
			{
				...
			}
			
			The FOR IN loop takes a variable name as well as a list or string (and later a hash table or maybe a MySQL result set as well) and loops through each element or character and assigns it to the variable before running the block of code. The variable that holds each element or character can be specified as local or global, but it will default to global out of functions and local inside of functions. After finishing, the variable will have the value of the last element seen.
		*/
		else if(n->s == FORIN_NODE)
		{
			//How do I free the list variable?
			forin_node *fn = (forin_node *) n->sn;
			
			node_t *inn = fn->in;
			in_node *in = (in_node *) inn->sn;
			
			
			//Get the Variable
			node_t *var = in->el;
			if(var->p != VAR_NODE)
			{
				printf("Error: For-In loops require left side of exp IN exp to be a variable.\n");
				exit(1);
			}
			var_node *vn = (var_node *) in->el->sn;
			xvar *list = evaluate(in->list);
			char *name = vn->name;
			char scope = vn->scope;
			
			//List
			if(list->type == 'l')
			{
				List *l = list->val->l;
			
				while(l != NULL)
				{
					SET_SCOPE();
					add(name, dup_xvar(l->var));
					free_xvar(evaluate_all(fn->block));
					l = l->next;
				}
			}
			
			//String
			else if(list->type == 's')
			{
				char *s = list->val->s;
				while(*s)
				{
					char *c = malloc(2);
					c[0] = *s;
					c[1] = '\0';
					SET_SCOPE();
					add(name, string_xvar(c));
					free_xvar(evaluate_all(fn->block));
					s++;
				}
			}
			
			//MySQL Row
			else if(list->type == 'r')
			{
				xvar *row = mysql_row_to_list(list);
				while(row->type != 'i')
				{
					SET_SCOPE();
					add(name, dup_xvar(row));
					free_xvar(evaluate_all(fn->block));
					row = mysql_row_to_list(list);
				}
				
				//Free Result?
				mysql_free_result(list->val->m->res);
			}
			
			//Free the List
			free_xvar(list);
		}
	
		return NULL;
	}
	
	//Ternary Operator
	else if(n->p == TERNARY_NODE)
	{
		ternary_node *tn = (ternary_node *) n->sn;
		
		xvar *cond = evaluate(tn->condition);
		
		double d = if_xvar(cond);
		
		if(d)
		{
			return evaluate(tn->if_exp);
		}
		
		else
		{
			return evaluate(tn->else_exp);
		}
	}
	
	//Type Cast Node
	else if(n->p == CAST_NODE)
	{
		cast_node *cn = (cast_node*) n->sn;
		char type = cn->type;
		xvar *exp = evaluate(cn->exp);
		
		if(type == exp->type)
			return exp;
			
		else
		{
			xvar *return_val;
			if(type == 's')
				return_val = string_xvar(strVal(exp));
			else if(type == 'i')
				return_val = integer_xvar(intVal(exp));
			else if(type == 'd')
				return_val = double_xvar(dubVal(exp));
			else if(type == 'l')
				return_val = list_xvar(listVal(exp));
			else if(type == 'b')
				return integer_xvar(if_xvar(exp));
			else
				return exp;
				
			free_xvar(exp);
			return return_val;
		}
	}
	
	//Object Nodes
	else if(n->p == OBJECT_NODE)
	{
		int i;
		Object *o = coal_alloc(sizeof(Object));
		Attributes *a = coal_alloc(sizeof(Attributes));
		Methods *m = coal_alloc(sizeof(Methods));
		
		object_node *on = (object_node *) n->sn;
		char *name = on->name;
		o->name = strdup(name);
		o->attrs = a;
		o->methods = m;
		
		node_t *params = on->attrs;
		node_t *methods = on->methods;
		
		//Add the Attributes
		i = 0;
		Attributes *a_copy = NULL;
		while(params != NULL)
		{
			a->next = coal_alloc(sizeof(Attributes));
			
			param_node *pn = (param_node *) params->sn;
			var_node *vn = (var_node *) pn->param->sn;
			a->name = strdup(vn->name);
			
			//Strongly Typed
			a->strong = vn->strong;
			
			//Protection
			a->protect = vn->scope;
			
			//Problematic! How will this work with references, freeing, duplicating, etc.
			if(pn->def == NULL)
				a->value = NULL;
				
			else
			{
				xvar *value = evaluate(pn->def);
				
				if(a->strong != 'a' && a->strong != value->type)
				{
					printf("Error: Cannot assign value of type %c to a strongly typed object property of type %c.\n", value->type, a->strong);
					exit(1);
				}
			
				a->value = value;
			}
			
			a_copy = a;
			a = a->next;
			params = params->next;
			i++;
		}
		
		//Add the Methods
		i = 0;
		Methods *m_copy = NULL;
		while(methods != NULL)
		{
			m->next = coal_alloc(sizeof(Methods));
			
			method_node *vn = (method_node *) methods->sn;
			m->name = strdup(vn->name);
			m->params = vn->params;
			m->block = vn->block;
			m->strong = vn->strong;
			m->protect = vn->protect;
			
			m_copy = m;
			m = m->next;
			methods = methods->next;
			i++;
		}
		
		//Add Parent Stuff
		node_t *parents = on->parents;
		while(parents != NULL)
		{
			//Lookup the Object
			id_node *in = (id_node *) parents->sn;
			char *name = in->name;
			GLOBAL_SCOPE();
			xvar *object = lookup(name, 1);
			Object *obj = object->val->o;
			
			//Add Attributes
			Attributes *attrs = obj->attrs;
			while(attrs != NULL)
			{
				if(attrs->protect == 'y' || attrs->protect == 'w')
				{
					attrs = attrs->next;
					continue;
				}
				a->next = coal_alloc(sizeof(Attributes));
				a->name = strdup(attrs->name);
				a->value = dup_xvar(attrs->value);
				a->strong = attrs->strong;
				a->protect = attrs->protect;
				
				a_copy = a;
				a = a->next;
				attrs = attrs->next;
			}
			
			//Add Methods
			Methods *meths = obj->methods;
			while(meths != NULL)
			{
				if(meths->protect == 'y' || meths->protect == 'w')
				{
					meths = meths->next;
					continue;
				}
				m->next = coal_alloc(sizeof(Methods));
				m->name = strdup(meths->name);
				m->params = meths->params;
				m->block = meths->block;
				m->strong = meths->strong;
				m->protect = meths->protect;
				
				m_copy = m;
				m = m->next;
				meths = meths->next;
			}
			
			parents = parents->next;
		}
		
		//Pretend that we had at least one attribute
		if(a_copy == NULL)
		{
			free(o->attrs);
			o->attrs = NULL;
		}
		
		else
		{
			free(a_copy->next);
			a_copy->next = NULL;
		}
		
		//Pretend that we had at least one method
		if(m_copy == NULL)
		{
			free(o->methods);
			o->methods = NULL;
		}
		
		else
		{
			free(m_copy->next);
			m_copy->next = NULL;
		}
		
		GLOBAL_SCOPE();
		add(name, object_xvar(o));
		return NULL;
	}
	
	//Skip Empty Nodes
	else if(n->p == -1)
	{
		//Skip
		return NULL;
	}
	
	//Define a Function
	else if(n->p == FUNCTION_NODE)
	{
		int i = 1;
		addF((function_node *) n->sn, functions);
		return NULL;
	}
	
	//Call a Function
	else if(n->p == CALL_NODE)
	{
		//Functio Name
		char *name;
		
		//Parameter Name
		char *pname;
		
		//Arguments
		node_t *args;
		
		//Possible Method and Object
		method_call_node *mn;
		xvar *object;
		var_node *var;
		
		//Method Call
		if(n->s == METHOD_NODE)
		{
			mn = (method_call_node *) n->sn;
			object = evaluate(mn->obj);
			var = (var_node *) mn->obj->sn;
			name = mn->method;
			args = mn->args;
		}
		
		//Function Call
		else
		{
			call_node *cn = (call_node *) n->sn;
			args = cn->args;
			name = cn->name;
		}
		
		//Built-In Functions
		#include "functions/built-in.c"
		
		//Non-Built-In
		else
		{
			//Initializing Stuff
			param_node *param;
			node_t *params;
			node_t *block;
			
			//Strongly Typed
			char strong = 'a';
			

			Hash **locals_save = locals_ptr;
			Hash *locals_new[HASHSIZE];
			hash_ptr = locals_ptr = locals_new;
			hash_init();
		
			//Method Call
			if(n->s == METHOD_NODE)
			{
				//Self Reference
				hash_ptr = locals_new;
				object->refs++;
				
				//Self Reference Security
				ALLOW_SELF = 1;
				add("$self", object);
				ALLOW_SELF = 0;
				
				Object *o = object->val->o;
				
				Methods *m = o->methods;
				int i = 0;
				while(m != NULL)
				{
					if(!strcmp(m->name, name))
					{
						params = m->params;
						block = m->block;
						strong = m->strong;
						
						//Encapsulation
						if(m->protect != 'x' && m->protect != 'w' && strcmp(var->name, "$self"))
						{
							printf("Error: Object %s does not have a method %s.\n", o->name, name);
							exit(1);
						}
						i = 1;
						break;
					}
					m = m->next;
				}
				
				if(i == 0)
				{
					printf("Error: Object %s does not have a method %s.\n", o->name, name);
					exit(1);
				}
				hash_ptr = locals_ptr = locals_new;
			}
			
			//Function Call
			else
			{
				funcs *func = lookupF(name, functions);
				params = func->params;
				block = func->block;
				strong = func->strong;
			}
			
			//Arguments
			xvar *value;
			while(params != NULL)
			{
				locals_ptr = locals_save;
				param = (param_node *) params->sn;
				var_node *parameter = (var_node *) param->param->sn;
				pname = parameter->name;
				if(args == NULL)
				{
					if(param->def == NULL)
					{
						printf("Error: Too few arguments for function %s.\n", name);
						exit(1);
					}
					
					else
					{
						value = evaluate(param->def);
					}
				}
				
				else
				{
					value = evaluate(args);
				}
				hash_ptr = locals_ptr = locals_new;
				
				//References
				xvar_inc_refs(value);
				
				//Strongly Typed
				if(parameter->strong != 'a' && value->type != parameter->strong)
				{
					printf("Error: Cannot assign a value of type %c to a strongly typed parameter (%s) of type %c.\n", value->type,  parameter->name, parameter->strong);
					exit(1);
				}
				
				add(pname, value);
				if(args != NULL)
					args = args->next;
				params = params->next;
			}
			
			//Self Variable
			
			function_depth++;
			xvar *return_val = evaluate_all(block);
		
			//Don't free what we're returning!
			return_val->refs++;
			
			//Free Local Variables
			hash_free(locals_new);
		
			//What we're returning may not have anything pointing to it.
			return_val->refs--;
			
			locals_ptr = locals_save;
			function_depth--;
			if(return_depth)
				return_depth--;

			//Strongly Typed
			if(strong != 'a' && return_val->type != strong)
			{
				printf("Error: Cannot return a value of type %c from a strongly typed function (%s) of type %c.\n", return_val->type,  name, strong);
				exit(1);
			}
			
			return return_val;
		}
	}
	
	//Return Value
	else if(n->p == RETURN_NODE)
	{
		n->next = NULL;
		xvar *return_val = evaluate(((return_node *) n->sn)->value);
		
		return_depth++;
		return return_val;
	}
	
	//Mathematical and Boolean
	else if(n->p == OP_NODE)
	{
		op_node *on = (op_node *) n->sn;
		
		xvar *left = evaluate(on->left);
		xvar *right = evaluate(on->right);
		char *op = on->op;
		
		xvar *retval;
		
		if(n->s == MOP_NODE)
		{
			if(!strcmp(op, "+"))
				retval = add_xvar(left, right);
			else if(!strcmp(op, "-"))
				retval = sub_xvar(left, right);
			else if(!strcmp(op, "*"))
				retval = mul_xvar(left, right);
			else if(!strcmp(op, "/"))
				retval = div_xvar(left, right);
			else if(!strcmp(op, "%"))
				retval = mod_xvar(left, right);
			else if(!strcmp(op, "#"))
				retval = cat_xvar(left, right);
			else
				retval = NULL;
		}
		
		else if(n->s == COP_NODE)
		{
			if(!strcmp(op, "=="))
				retval = eq_xvar(left, right);
			else if(!strcmp(op, "!="))
				retval = neq_xvar(left, right);
			else if(!strcmp(op, "==="))
				retval = ceq_xvar(left, right);
			else if(!strcmp(op, "!=="))
				retval = cneq_xvar(left, right);
			else if(!strcmp(op, ">="))
				retval = gte_xvar(left, right);
			else if(!strcmp(op, "<="))
				retval = lte_xvar(left, right);
			else if(!strcmp(op, ">"))
				retval = gt_xvar(left, right);
			else if(!strcmp(op, "<"))
				retval = lt_xvar(left, right);
			else if(!strcmp(op, "AND"))
				retval = and_xvar(left, right);
			else if(!strcmp(op, "OR"))
				retval = or_xvar(left, right);
		}
		
		free_xvar(left);
		free_xvar(right);
		
		return retval;
	}
	
	else
		return NULL;
}

//Evaluate All Nodes
xvar *evaluate_all(node_t *ast)
{
	xvar *last_val = NULL;
	while(ast != NULL)
	{
		free_xvar(last_val);
		last_val = evaluate(ast);
		ast = ast->next;
		
		if(return_depth > 0)
		{
			return last_val;
		}
	}
	return last_val;
}