/*
	Lookup.c -- Lookup Variables, Object Properties, and Dictionary Elements
*/

//Lookup Variable
xvar *lookupVar(node_t *n, int nn)
{
	var_node *vn = (var_node *) n->sn;
	char *name = vn->name;
	char scope = vn->scope;

	SET_SCOPE();

	return lookup(name, nn);
}

//Lookup Object Property
Attributes *lookupObjProp(node_t *n, int nn)
{
	property_node *pn = (property_node *) n->sn;
	
	var_node *vn = (var_node *) pn->obj->sn;
	char *name = vn->name;
	xvar *object = evaluate(pn->obj);

	char *prop = pn->prop;
	
	Object *o = object->val->o;
	
	Attributes *a = o->attrs;
	
	while(a != NULL)
	{
		if(!strcmp(a->name, prop))
		{
			if(a->protect != 'x' && a->protect != 'w' && a->protect != 'R' && strcmp(name, "$self"))
			{
				break;
			}
			
			//The Parent Variable
			a->object = name;
			return a;
		}
		a = a->next;
	}
	
	if(nn)
	{
		printf("Error: Object %s does not have property %s.\n", o->name, prop);
		exit(1);
	}
	
	else
	{
		return NULL;
	}
}

//Lookup Dictionary Value
xvar *lookupDictVal(node_t *n, int nn)
{
	hash_lookup_node *hn = (hash_lookup_node *) n->sn;
			
	//The Key
	xvar *xkey = evaluate(hn->key);
	char *key = strVal(xkey);
	free_xvar(xkey);
	
	//The Variable
	xvar *var = evaluate(hn->var);
	
	Hash **hash_save = hash_ptr;
	
	//Must be a valid Dicitionary
	if(var->type != 'h')
	{
		printf("Error: A non-dictionary variable is being treated as a dictionary.");
		exit(1);
	}
			
	hash_ptr = var->val->h;
	xvar *val = lookup(key, nn);
	hash_ptr = hash_save;
			
	free(key);
	free_xvar(var);
	return val;
}