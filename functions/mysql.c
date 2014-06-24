/*
	This function takes a current MySQL row from a query result set and converts it into a list.
*/
xvar *mysql_row_to_list(xvar *mysql)
{
	if(mysql->type != 'r')
	{
		printf("Error: Argument 1 of mysql_fetch_row() must be a valid MySQL result set\n");
		exit(1);
	}

	//Returns 0 on failure, and a MySQL result set on success
			
	MYSQL_RES *res = mysql->val->m->res;
	MYSQL_ROW row;	
			
	//Get the Row
	if((row = mysql_fetch_row(res)))
	{
		
		//Process Row
		unsigned int i;
		unsigned int num_fields = mysql_num_fields(res);
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(res);
				
		//The Result List
		List *l = coal_alloc(sizeof(List));
		List *lc = l;
		List *lp = l;
		
		for(i = 0; i < num_fields; i++)
		{
			char s[lengths[i]];
			int len;
			char *r;
			if(row[i])
			{
				len = lengths[i];
				r = row[i];
			}
					
			else
			{
				r = "";
				len = 0;
			}
			sprintf(s, "%.*s" , len, r);
			
			//Add a nul terminator to the string so that it becomes usable.
			char *n = nullify(s, len);
			
			//Add String to List
			lc->next = coal_alloc(sizeof(List));
			lc->var = string_xvar(n);
			lp = lc;
			lc = lc->next;
		}
				
		if(i)
		{
			free(lp->next);
			lp->next = NULL;
		}
		
		else
		{
			free(l);
			l = NULL;
		}
				
		return list_xvar(l);
	}
			
	else
	{
		//Return False
		return false;
	}
}