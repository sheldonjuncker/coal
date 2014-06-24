//Get the Length of a List or String
int xvar_length(xvar *v)
{
	int return_val = 0;
			
	if(v->type == 's')
		return_val = strlen(v->val->s);
	else if(v->type == 'l')
	{
		List *l = v->val->l;
		int i = 0;
		for(l;l != NULL;l = l->next)
			i++;
		return_val = i;
	}
	else
		return_val = 0;
		
	return return_val;
}

//Join List
char *join_list(List *l, char *j)
{
	//Joined String
	char *joined = strdup("");
			
	//Join!
	while(l != NULL)
	{
		char *jc = joined;
		char *ls = strVal(l->var);
		joined = concat(joined, ls);
			
		free(jc);
		free(ls);
		
		if(l->next != NULL)
		{
			jc = joined;
			joined = concat(joined, j);
		
			free(jc);
		}
		l = l->next;
	}
	return joined;
}