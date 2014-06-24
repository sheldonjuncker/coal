		//Standard Functions
		
		//Sum Function (list, string, arguments)
		if(!strcmp(name, "sum"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The sum() function requires at least one argument.\n");
			}
			
			xvar *first = evaluate(args);
			
			//String
			if(first->type == 's')
			{
				char *s = first->val->s;
				int i = 0;
				while(*s)
				{
					i += (int) *s++;
				}
				free_xvar(first);
				return integer_xvar(i);
			}
			
			//List
			else if(first->type == 'l')
			{
				double d = 0;
				List *l = first->val->l;
				
				while(l != NULL)
				{
					d += dubVal(l->var);
					l = l->next;
				}
				free_xvar(first);
				return double_xvar(d);
			}
			
			//Arguments
			else
			{
				double d = dubVal(first);
				free_xvar(first);
				args = args->next;
				
				while(args != NULL)
				{
					xvar *v = evaluate(args);
					d += dubVal(v);
					free_xvar(v);
					args = args->next;
				}
				return double_xvar(d);
			}
		}
		
		//Avg Function (list, string, arguments)
		else if(!strcmp(name, "avg"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The avg() function requires at least one argument.\n");
			}
			
			xvar *first = evaluate(args);
			
			//String
			if(first->type == 's')
			{
				int total = 0;
				char *s = first->val->s;
				int i = 0;
				while(*s)
				{
					i += (int) *s++;
					total++;
				}
				free_xvar(first);
				double di = (double) i;
				double dtotal = (double) total;
				return double_xvar(di / dtotal);
			}
			
			//List
			else if(first->type == 'l')
			{
				double d = 0;
				double total = 0;
				List *l = first->val->l;
				
				while(l != NULL)
				{
					d += dubVal(l->var);
					l = l->next;
					total += 1;
				}
				free_xvar(first);
				if(total == 0)
					return integer_xvar(0);
				return double_xvar(d / total);
			}
			
			//Arguments
			else
			{
				double total = 1;
				double d = dubVal(first);
				free_xvar(first);
				args = args->next;
				
				while(args != NULL)
				{
					xvar *v = evaluate(args);
					d += dubVal(v);
					free_xvar(v);
					args = args->next;
					total += 1;
				}
				return double_xvar(d / total);
			}
		}
		
		//Max Function (list, string, arguments)
		else if(!strcmp(name, "max"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The max() function requires at least one argument.\n");
			}
			
			xvar *first = evaluate(args);
			
			//String
			if(first->type == 's')
			{
				char *s = first->val->s;
				char cmax = *s;
				while(*s)
				{
					if(*s > cmax)
						cmax = *s;
					s++;
				}
				free_xvar(first);
				char *m = coal_alloc(2);
				m[0] = cmax;
				m[1] = '\0';
				return string_xvar(m);
			}
			
			//List
			else if(first->type == 'l')
			{
				List *l = first->val->l;
				
				if(l == NULL)
					return first;
				
				xvar *xmax = l->var;
				l = l->next;
				
				while(l != NULL)
				{
					xvar *gt = gt_xvar(l->var, xmax);
					if(if_xvar(gt))
					{
						xmax = l->var;
					}
					l = l->next;
				}
				xvar *return_val = dup_xvar(xmax);
				free_xvar(first);
				return return_val;
			}
			
			//Arguments
			else
			{
				xvar *xmax = first;
				args = args->next;
				
				while(args != NULL)
				{
					xvar *v = evaluate(args);
					xvar *gt = gt_xvar(v, xmax);
					if(if_xvar(gt))
					{
						free_xvar(xmax);
						xmax = v;
					}
					
					else
					{
						free_xvar(v);
					}
					args = args->next;
				}
				return xmax;
			}
		}
		
		//Min Function (list, string, arguments)
		else if(!strcmp(name, "min"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The min() function requires at least one argument.\n");
			}
			
			xvar *first = evaluate(args);
			
			//String
			if(first->type == 's')
			{
				char *s = first->val->s;
				char cmax = *s;
				while(*s)
				{
					if(*s < cmax)
						cmax = *s;
					s++;
				}
				free_xvar(first);
				char *m = coal_alloc(2);
				m[0] = cmax;
				m[1] = '\0';
				return string_xvar(m);
			}
			
			//List
			else if(first->type == 'l')
			{
				List *l = first->val->l;
				
				if(l == NULL)
					return first;
				
				xvar *xmax = l->var;
				l = l->next;
				
				while(l != NULL)
				{
					xvar *gt = lt_xvar(l->var, xmax);
					if(if_xvar(gt))
					{
						xmax = l->var;
					}
					l = l->next;
				}
				xvar *return_val = dup_xvar(xmax);
				free_xvar(first);
				return return_val;
			}
			
			//Arguments
			else
			{
				xvar *xmax = first;
				args = args->next;
				
				while(args != NULL)
				{
					xvar *v = evaluate(args);
					xvar *gt = lt_xvar(v, xmax);
					if(if_xvar(gt))
					{
						free_xvar(xmax);
						xmax = v;
					}
					
					else
					{
						free_xvar(v);
					}
					args = args->next;
				}
				return xmax;
			}
		}
		
		//Pow Function
		else if(!strcmp(name, "pow"))
		{
			//Two Arguments
			if(!(args != NULL && args->next != NULL))
			{
				fe("Error: The pow() function requires two arguments.\n");
			}
			
			//Number and Power
			xvar *n = evaluate(args);
			xvar *p = evaluate(args->next);
			
			double num = dubVal(n);
			double power = dubVal(p);
			
			free_xvar(n);
			free_xvar(p);
			
			double raise = pow(num, power);
			return double_xvar(raise);
		}
		
		//Sqrt Function
		else if(!strcmp(name, "sqrt"))
		{
			//Two Arguments
			if(args == NULL)
			{
				fe("Error: The sqrt() function requires one argument.\n");
			}
			
			//Number
			xvar *n = evaluate(args);
			double num = dubVal(n);
			free_xvar(n);
			
			double root = sqrt(num);
			return double_xvar(root);
		}
		
		//Substring Function (it'll work for lists too later)
		else if(!strcmp(name, "sub"))
		{
			//At Least One Argument
			if(args == NULL)
			{
				fe("Error: The sub() function requires at least one argument.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			args = args->next;
			
			//The Starting Position
			if(args == NULL)
				return string_xvar(s);
				
			xvar *start = evaluate(args);
			int p = intVal(start);
			free_xvar(start);
			
			args = args->next;
			
			//Number of Characters
			int n;
			if(args == NULL)
				n = -1;
			else
			{
				xvar *num = evaluate(args);
				n = intVal(num);
				free_xvar(num);
			}
			
			char *sub = substr(s, p, n);
			
			return string_xvar(sub);
		}
		
		//String Replace
		else if(!strcmp(name, "replace"))
		{
			//Two Arguments
			if(!(args != NULL && args->next != NULL))
			{
				fe("Error: The replace() function requires at least two arguments.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			args = args = args->next;
			
			//The Substring to Replace
			xvar *substr = evaluate(args);
			char *sub = strVal(substr);
			free_xvar(substr);
			args = args->next;
			
			//The Replacement Text (Defaults to "")
			char *rep = "";
			if(args != NULL)
			{
				xvar *replace = evaluate(args);
				rep = strVal(replace);
				free_xvar(replace);
				args = args->next;
			}
			
			//Number of Occurrences to Replace (Defaults to Unlimited)
			int num = 0;
			if(args != NULL)
			{
				xvar *number = evaluate(args);
				num = intVal(number);
				free_xvar(number);
			}
			
			//Replace, Free, Return!
			char *replaced = str_replace(s, sub, rep, num);
			free(s);
			return string_xvar(replaced);
		}
		
		//String to Uppercase
		else if(!strcmp(name, "upper"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The upper() function requires at least one argument.\n");
			}
			
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *copy = s;
			while(*s)
			{
				if(*s >= 'a' && *s <= 'z')
					*s -= 32;
				s++;
			}
			
			return string_xvar(copy);
		}
		
		/*
			Case Conversion Functions
			
			upper()		--	upper case
			lower()		--	lower case
			upperw()	--	upper case words
			lowerw()	--	lower case words
			upperf()	--	upper case first
			lowerf()	--	lower case first
		*/
		
		//String to Lowercase
		else if(!strcmp(name, "lower"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The lower() function requires at least one argument.\n");
			}
			
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *copy = s;
			while(*s)
			{
				if(*s >= 'A' && *s <= 'Z')
					*s += 32;
				s++;
			}
			
			return string_xvar(copy);
		}
		
		//Upper Case Words
		else if(!strcmp(name, "upperw"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The upperw() function requires at least one argument.\n");
			}
			
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *copy = s;
			
			if(*s >= 'a' && *s <= 'z')
				*s++ -= 32;
			
			while(*s)
			{
				if(*s >= 'a' && *s <= 'z' && (*(s-1)) == ' ')
					*s -= 32;
				s++;
			}
			
			return string_xvar(copy);
		}
		
		//Lower Case Words
		else if(!strcmp(name, "lowerw"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The lowerw() function requires at least one argument.\n");
			}
			
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *copy = s;
			
			if(*s >= 'A' && *s <= 'Z')
				*s++ += 32;
			
			while(*s)
			{
				if(*s >= 'A' && *s <= 'Z' && (*(s-1)) == ' ')
					*s += 32;
				s++;
			}
			
			return string_xvar(copy);
		}
		
		//Upper Case First
		else if(!strcmp(name, "upperf"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The upperf() function requires at least one argument.\n");
			}
			
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *copy = s;
			
			if(*s >= 'a' && *s <= 'z')
				*s -= 32;
			
			return string_xvar(copy);
		}
		
		//Lower Case First
		else if(!strcmp(name, "lowerf"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The lowerf() function requires at least one argument.\n");
			}
			
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *copy = s;
			
			if(*s >= 'A' && *s <= 'Z')
				*s += 32;
			
			return string_xvar(copy);
		}
		
		//String Split (Explode)
		else if(!strcmp(name, "split"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The split() function requires at least one argument.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			args = args->next;
			
			//Split On
			char *on = " ";
			if(args != NULL)
			{
				xvar *spliton = evaluate(args);
				on = strVal(spliton);
				free_xvar(spliton);
			}
			
			//Create the List
			link_str *ls = str_split(s, on);
			List *l = coal_alloc(sizeof(List));
			List *lc = l;
			List *lp = l;
			
			int i = 0;
			while(ls != NULL)
			{
				lc->next = coal_alloc(sizeof(List));
				lc->var = string_xvar(ls->str);
				
				lp = lc;
				lc = lc->next;
				ls = ls->next;
				i++;
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
		
		//Join Lists
		else if(!strcmp(name, "join"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The join() function requires at least one argument.\n");
			}
			
			xvar *list = evaluate(args);
			if(list->type != 'l')
				return list;

			List *l = list->val->l;
			
			if(l == NULL)
				return list;
				
			args = args->next;
			
			//Join On
			char *j = strdup("");
			if(args != NULL)
			{
				xvar *join = evaluate(args);
				j = strVal(join);
				free_xvar(join);
			}
			
			char *joined = join_list(l, j);
			free_xvar(list);
			free(j);
			
			return string_xvar(joined);
		}
		
		//String Position
		else if(!strcmp(name, "pos"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The pos() function requires at least one argument.\n");
			}
			
			//Haystack
			xvar *string = evaluate(args);
			char *h = strVal(string);
			free_xvar(string);
			args = args->next;
			
			if(args == NULL)
				return integer_xvar(0);
			
			//Needle
			xvar *needle = evaluate(args);
			char *n = strVal(needle);
			free_xvar(needle);
			
			//Position
			char *pos = strstr(h, n);
			
			if(pos == NULL)
				return integer_xvar(-1);
				
			return integer_xvar(pos - h);
		}
		
		/*
			Regular Expression Functions
			
			list match(string, regex);
			
			list match_all(string, regex);
			
			list reg_split(string, regex, [splits : unlimited]);
			
			string reg_replace(string, regex, [replacement : "", replacements : unlimited]);
		*/
		
		//Regex Match
		else if(!strcmp(name, "match"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The match() function requires two arguments.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			char *sptr = s;
			free_xvar(string);
			
			//The Regular Expression
			xvar *regex = evaluate(args->next);
			char *r = strVal(regex);
			free_xvar(regex);
			
			//The RE Object
			pcre *re;
			
			//Number of Matches
			int re_matches;
			
			//Error Stuff
			const char *re_error;
			int re_error_offset;
			
			//Compile the Regular Expression
			re = pcre_compile(r, 0, &re_error, &re_error_offset, NULL);
			
			//Did it compile?
			if(re == NULL)
			{
				printf("Error: The regular expressio n (%s) failed to compile at offset %i.\n Message: %s\n", r, re_error_offset, re_error);
				exit(1);
			}
			
			//Number of Matches
			int ovector_size;
			pcre_fullinfo(re, NULL, PCRE_INFO_CAPTURECOUNT , &ovector_size);
			ovector_size = (ovector_size + 1) * 3;
			int ovector[ovector_size];
			
			//Execute the Match
			re_matches = pcre_exec(re, NULL, s, strlen(s), 0, 0, ovector, ovector_size);
			
			//No Matches
			if(re_matches == PCRE_ERROR_NOMATCH)
			{
				pcre_free(re);
				free(s);
				free(r);
				return list_xvar(NULL);
			}
			
			//An Error Occurred
			else if(re_matches < 0)
			{
				printf("Error: An unknown error occurred while executing the regular expression\n");
				exit(1);
			}
			
			//All Good
			if(re_matches == 0)
				re_matches = ovector_size / 3;
			
			
			//The List
			List *l = coal_alloc(sizeof(List));
			List *lc = l;
			List *lp = NULL;
			
			
			//Add all Matches to the List
			int i;
			const char *match;
			for(i=0; i < re_matches; i++)
			{
				pcre_get_substring(s, ovector, re_matches, i, &match);
				
				lc->var = string_xvar(strdup(match));
				lc->next = coal_alloc(sizeof(List));
				lp = lc;
				lc = lc->next;
				
				//Free Substring
				pcre_free_substring(match);
			}
			
			//Free Everything
			pcre_free(re);
			free(s);
			free(r);
			
			free(lp->next);
			lp->next = NULL;
			return list_xvar(l);
		}
		
		//Match All
		else if(!strcmp(name, "match_all"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The match_all() function requires two arguments.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			char *sptr = s;
			free_xvar(string);
			
			//The Regular Expression
			xvar *regex = evaluate(args->next);
			char *r = strVal(regex);
			free_xvar(regex);
			
			//The RE Object
			pcre *re;
			
			//The Studied Extra
			pcre_extra *extra;
			
			//Number of Matches
			int re_matches;
			
			//Error Stuff
			const char *re_error;
			int re_error_offset;
			
			//Compile the Regular Expression
			re = pcre_compile(r, 0, &re_error, &re_error_offset, NULL);
			
			//Did it compile?
			if(re == NULL)
			{
				printf("Error: The regular expressio n (%s) failed to compile at offset %i.\n Message: %s\n", r, re_error_offset, re_error);
				exit(1);
			}
			
			//Since we're matching multiple times, optimize the regex
			extra = pcre_study(re, 0, &re_error);
			
			//Did it optimize the regex?
			if(re_error != NULL)
			{
				printf("Error: The regular expression (%s) failed to optimize.\n Message: %s\n", r, re_error);
				exit(1);
			}
			
			//Number of Matches
			int ovector_size;
			pcre_fullinfo(re, extra, PCRE_INFO_CAPTURECOUNT , &ovector_size);
			ovector_size = (ovector_size + 1) * 3;
			int ovector[ovector_size];
			
			//The Entire List
			List *l1 = coal_alloc(sizeof(List));
			List *l1c = l1;
			List *l1p = NULL;
			
			//Execute All Matches
			while((re_matches = pcre_exec(re, extra, s, strlen(s), 0, 0, ovector, ovector_size)) >= 0)
			{
				l1c->next = coal_alloc(sizeof(List));
				
				//Too many matches
				if(re_matches == 0)
					re_matches = ovector_size / 3;
					
				//All Good
				//The Second List
				List *l2 = coal_alloc(sizeof(List));
				List *l2c = l2;
				List *l2p = NULL;
				
			
				//Add all Matches to the List
				int i;
				const char *match;
				for(i=0; i < re_matches; i++)
				{
					pcre_get_substring(s, ovector, re_matches, i, &match);
					
					l2c->var = string_xvar(strdup(match));
					l2c->next = coal_alloc(sizeof(List));
					l2p = l2c;
					l2c = l2c->next;
					
					//Free Substring
					pcre_free_substring(match);
				}
				
				//Update Source String
				s = s + ovector[1];
			
				//Free the Match
				pcre_free_substring(match);
				
				//Free Extra List Stuff
				free(l2p->next);
				l2p->next = NULL;
				
				//Add sublist to the main list
				l1c->var = list_xvar(l2);
				l1p = l1c;
				l1c = l1c->next;
			}
			
			//Free Extra List Stuff
			free(l1p->next);
			l1p->next = NULL;
			
			//Free the String and Regex
			free(sptr);
			free(r);
			
			//Free the Regex and the Extra
			pcre_free(re);
			
			if(extra != NULL)
				pcre_free(extra);
				
			return list_xvar(l1);
		}
		
		//Match All
		else if(!strcmp(name, "reg_split"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The match_all() function requires two arguments.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			char *sptr = s;
			free_xvar(string);
			
			//The Regular Expression
			xvar *regex = evaluate(args->next);
			char *r = strVal(regex);
			free_xvar(regex);
			
			//Optional Number of Splits
			
			int num = -1;
			if(args->next->next != NULL)
			{
				xvar *number = evaluate(args->next->next);
				num = intVal(number);
				if(num == 0)
					num = -1;
				free_xvar(number);
			}
			
			//The RE Object
			pcre *re;
			
			//The Studied Extra
			pcre_extra *extra;
			
			//Number of Matches
			int re_matches;
			
			//Error Stuff
			const char *re_error;
			int re_error_offset;
			
			//Compile the Regular Expression
			re = pcre_compile(r, 0, &re_error, &re_error_offset, NULL);
			
			//Did it compile?
			if(re == NULL)
			{
				printf("Error: The regular expressio n (%s) failed to compile at offset %i.\n Message: %s\n", r, re_error_offset, re_error);
				exit(1);
			}
			
			//Since we're matching multiple times, optimize the regex
			extra = pcre_study(re, 0, &re_error);
			
			//Did it optimize the regex?
			if(re_error != NULL)
			{
				printf("Error: The regular expression (%s) failed to optimize.\n Message: %s\n", r, re_error);
				exit(1);
			}
			
			//Number of Matches
			int ovector_size;
			pcre_fullinfo(re, extra, PCRE_INFO_CAPTURECOUNT , &ovector_size);
			ovector_size = (ovector_size + 1) * 3;
			int ovector[ovector_size];
			
			//The Entire List
			List *l = coal_alloc(sizeof(List));
			List *lc = l;
			List *lp = NULL;
			
			//Execute All Matches
			while((re_matches = pcre_exec(re, extra, s, strlen(s), 0, 0, ovector, ovector_size)) >= 0 && num != 0)
			{
				lc->next = coal_alloc(sizeof(List));
				
				//Too many matches
				if(re_matches == 0)
					re_matches = ovector_size / 3;

				//Update Source String
				s[ovector[0]] = '\0';
				
				//The String
				char *part = strdup(s);
				
				s = s + ovector[1];
				
				//Add substring to list
				lc->var = string_xvar(part);
				lp = lc;
				lc = lc->next;
				num--;
			}
			
			//Free Extra List Stuff
			lc->next = NULL;
			lc->var = string_xvar(strdup(s));
			
			//Free the String and Regex
			free(sptr);
			free(r);
			
			//Free the Regex and the Extra
			pcre_free(re);
			
			if(extra != NULL)
				pcre_free(extra);
				
			return list_xvar(l);
		}
		
		//Match All
		else if(!strcmp(name, "reg_replace"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The reg_replace() function requires two arguments.\n");
			}
			
			//The String
			xvar *string = evaluate(args);
			char *s = strVal(string);
			char *sptr = s;
			free_xvar(string);
			args = args->next;
			
			//The Regular Expression
			xvar *regex = evaluate(args);
			char *r = strVal(regex);
			free_xvar(regex);
			args = args->next;
			
			//The Replacement Text
			char *rep = strdup("");
			if(args != NULL)
			{
				xvar *replace = evaluate(args);
				rep = strVal(replace);
				free_xvar(replace);
				args = args->next;
			}
			
			
			//Optional Number of Replacements
			int num = -1;
			if(args != NULL)
			{
				xvar *number = evaluate(args);
				num = intVal(number);
				if(num == 0)
					num = -1;
				free_xvar(number);
			}
			
			//The RE Object
			pcre *re;
			
			//The Studied Extra
			pcre_extra *extra;
			
			//Number of Matches
			int re_matches;
			
			//Error Stuff
			const char *re_error;
			int re_error_offset;
			
			//Compile the Regular Expression
			re = pcre_compile(r, 0, &re_error, &re_error_offset, NULL);
			
			//Did it compile?
			if(re == NULL)
			{
				printf("Error: The regular expressio n (%s) failed to compile at offset %i.\n Message: %s\n", r, re_error_offset, re_error);
				exit(1);
			}
			
			//Since we're matching multiple times, optimize the regex
			extra = pcre_study(re, 0, &re_error);
			
			//Did it optimize the regex?
			if(re_error != NULL)
			{
				printf("Error: The regular expression (%s) failed to optimize.\n Message: %s\n", r, re_error);
				exit(1);
			}
			
			//Number of Matches
			int ovector_size;
			pcre_fullinfo(re, extra, PCRE_INFO_CAPTURECOUNT , &ovector_size);
			ovector_size = (ovector_size + 1) * 3;
			int ovector[ovector_size];
			
			//Result
			char *result = strdup("");
			
			//Execute All Matches
			while((re_matches = pcre_exec(re, extra, s, strlen(s), 0, 0, ovector, ovector_size)) >= 0 && num != 0)
			{
				char *p1 = substr(s, 0, ovector[0]);
				
				char *result_ptr = result;
				result = concat(result, p1);
				free(result_ptr);
				
				result_ptr = result;
				result = concat(result, rep);
				free(result_ptr);
				
				free(p1);
				
				s = s + ovector[1];
				num--;
			}
			
			//Free the String and Regex
			free(r);
			free(rep);
			
			//Free the Regex and the Extra
			pcre_free(re);
			
			if(extra != NULL)
				pcre_free(extra);
			
			char *result_ptr = result;
			result = concat(result, s);
			free(result_ptr);
			
			free(sptr);
			
			return string_xvar(result);
		}
		
		else if(!strcmp(name, "delete"))
		{
			while(args != NULL)
			{
				if(args->p == VAR_NODE)
				{
					var_node *vn = (var_node *) args->sn;
					char *name = vn->name;
					char scope = vn->scope;
				
					SET_SCOPE();
	
					unset(name);
				}
			
				else if(args->s == ELEMENT_NODE)
				{
					element_node *en = (	element_node *) args->sn;
					node_t *value = en->value;
					if(value->p != VAR_NODE)
					{
						fe("Error The delete() function annot unset a value.\n");
					}
					var_node *vn = (var_node *) 	value->sn;
				
					char *name = vn->name;
					char scope = vn->scope;
					xvar *element = evaluate(en->el);
					int el = intVal(element);
					free_xvar(element);
				
					SET_SCOPE();
				
					delete_element(lookup(name, 1), el);
				}
			
				else
				{
					fe("Error: The delete() function cannot unset anything other than variables and 	elements.\n");
				}
				args = args->next;
				}
			return NULL;
		}
		
		else if(!strcmp(name, "length"))
		{
			if(args == NULL)
			{
				return integer_xvar(0);
			}
			
			xvar *arg = evaluate(args);
			int length = xvar_length(arg);
			free_xvar(arg);
			return integer_xvar(length);
		}
		
		else if(!strcmp(name, "include"))
		{
			node_t *save = root;
			
			xvar *file = evaluate(args);
			char *file_name = strVal(file);
			free_xvar(file);
			
			
			mmgr_node *save_b = mmgr_b;
			mmgr_node *save_c = mmgr_c;
			
			coal_parse(file_name);
			
			mmgr_b = save_b;
			mmgr_c = save_c;
			
			free(file_name);
			
			root = save;
			return NULL;
		}
		
		else if(!strcmp(name, "exit"))
		{
			print_xvar(evaluate(args));
			exit(1);
		}
		
		//Random Integer
		else if(!strcmp(name, "rand"))
		{
			srand(time(NULL));
			return integer_xvar(rand());
		}
		
		//Special Function for Chad
		//Returns a Random Element from an Array or String
		else if(!strcmp(name, "chad"))
		{
			xvar *v = evaluate(args);
			srand(time(NULL));
			int i, length;
			
			if(v->type == 's')
				length = strlen(v->val->s);
			else if(v->type == 'l')
			{
				List *l = v->val->l;
				i = 0;
				for(l;l != NULL;l = l->next)
					i++;
				length = i;
			}
			else
				return integer_xvar(0);
			
			int random = rand() % (length);
			
			if(v->type == 's')
			{
				char *n = malloc(2);
				n[0] = (v->val->s)[random];
				n[1] = '\0';
				return string_xvar(n);
			}
			else if(v->type == 'l')
			{
				List *l = v->val->l;
				i = 0;
				for(l;l != NULL;l = l->next,i++)
					if(i == random)
						return dup_xvar(l->var);
			}
			else
				return integer_xvar(0);
			return NULL;
		}
		
		//Css Function
		else if(!strcmp(name, "css"))
		{
			int flag = 0;
			char *file = strVal(evaluate(args));
			if(args->next != NULL)
				flag = intVal(evaluate(args->next));
			
			if(flag)
			{
				char *open_style = "<style type=\"text/css\">\n";
				char *contents = file_get_contents(file);
				char *close_style = "\n</style>";
				
				char *step_one = concat(open_style, contents);
				char *style = concat(step_one, close_style);
				
				free(step_one);
				free(contents);
				free(file);
				
				return string_xvar(style);
			}
			
			else
			{
				char *link_open = "<link rel=\"stylesheet\" type=\"text/css\" href=\"";
				char *link_close = "\" />";
				
				char *step_one = concat(link_open, file);
				char *link = concat(step_one, link_close);
				
				free(step_one);
				free(file);
				
				return string_xvar(link);
			}
		}
		
		//JS Function
		else if(!strcmp(name, "js"))
		{
			int flag = 0;
			char *file = strVal(evaluate(args));
			if(args->next != NULL)
				flag = intVal(evaluate(args->next));
			
			if(flag)
			{
				char *open_style = "<script type=\"text/javascript\">\n";
				char *contents = file_get_contents(file);
				char *close_style = "\n</script>";
				
				char *step_one = concat(open_style, contents);
				char *style = concat(step_one, close_style);
				
				free(step_one);
				free(contents);
				free(file);
				
				return string_xvar(style);
			}
			
			else
			{
				char *link_open = "<srcipt type=\"text/javascript\" src=\"";
				char *link_close = "\"></script>";
				
				char *step_one = concat(link_open, file);
				char *link = concat(step_one, link_close);
				
				free(step_one);
				free(file);
				
				return string_xvar(link);
			}
		}
		
		/*
			MySQL Functions -- Being Implemented Now
			
			mysql_connect();
			mysql_query();
			mysql_close();
			mysql_free_result();
			mysql_affected();
			mysql_data_seek();
			mysql_fetch_row();
			mysql_select_db();
			mysql_insert_id();
			mysql_errno();
			mysql_error();
		*/
		
		//MySQL Connect (Memory Leaks!!!)
		else if(!strcmp(name, "mysql_connect"))
		{
			MYSQL *con = mysql_init(NULL);
			if(con == NULL)
			{
				fe("Error: The mysql_connect() function could not initialize a MySQL connection!\n");
			}
			char *host;
			char *user;
			char *pass;
			char *db;
			int port;
			char *socket;
			int flag;
			
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				host = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				host = NULL;
			
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				user = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				user = NULL;
			
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				pass = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				pass = NULL;
				
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				db = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				db = NULL;
			
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				port = intVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				port = 0;
			
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				socket = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				socket = NULL;
			
			if(args != NULL)
			{
				xvar *tmp = evaluate(args);
				flag = intVal(tmp);
				free_xvar(tmp);
				args = args->next;
			}
			
			else
				flag = 0;
			
			//Connect
			xvar *return_val;
			
			//This is probably redundant
			MYSQL *connection;
			if((connection = mysql_real_connect(con, host, user, pass, db, port, socket, flag)) == NULL)
				return_val = integer_xvar(0);
			
			else
				return_val = mysql_con_xvar(connection);
			
			
			free(host);
			free(user);
			free(pass);
			free(db);
			free(socket);
			
			return return_val;
		}
		
		//Query the Database
		else if(!strcmp(name, "mysql_query"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The mysql_query() function requires at least two parameters.\n");
			}
			
			xvar *mysql = evaluate(args);
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_query() function requires a valid MySQL connection\n");
			}
			MYSQL *con = mysql->val->m->con;
			free_xvar(mysql);
			args = args->next;
			
			
			//The Query
			xvar *query = evaluate(args);
			char *q = strVal(query);
			free_xvar(query);
			args = args->next;
			
			//Optional: Store or Use Result?
			int store = 1;
			if(args != NULL)
			{
				xvar *store_tmp = evaluate(args);
				store = intVal(store_tmp);
				free_xvar(store_tmp);
			}
			
			//Run the Query
			int return_val = mysql_query(con, q);
			
			free(q);
			
			//Error
			if(return_val)
				return integer_xvar(0);
				
			//Use Result
			MYSQL_RES *res;
			
			if(store)
				res = mysql_store_result(con);
			else
				res = mysql_use_result(con);
			
			//Error
			if(res == NULL)
				return integer_xvar(0);
				
			//Success
			return mysql_res_xvar(res);
		}
		
		//MySQL Data Seek
		else if(!strcmp(name, "mysql_data_seek"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The mysql_data_seek function requires two arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'r')
			{
				fe("Error: The mysql_data_seek() function requires a valid MySQL result set.\n");
			}
			
			//The Offset
			xvar *offset = evaluate(args->next);
			int o = intVal(offset);
			free_xvar(offset);
			
			//The Connection
			MYSQL_RES *r = result->val->m->res;
			free_xvar(result);
			
			//Number of Rows
			int rows = mysql_num_rows(r);
			
			if(o >= rows)
				o = rows - 1;
				
			//Seek
			mysql_data_seek(r, o);
			
			//Return
			return integer_xvar(o);
		}
		
		//MySQL Field Seek
		else if(!strcmp(name, "mysql_field_seek"))
		{
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The mysql_field_seek function requires two arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'r')
			{
				fe("Error: The mysql_field_seek() function requires a valid MySQL result set.\n");
			}
			
			//The Offset
			xvar *offset = evaluate(args->next);
			int o = intVal(offset);
			free_xvar(offset);
			
			//The Connection
			MYSQL_RES *r = result->val->m->res;
			free_xvar(result);
			
			//Out of Bounds
			int fields = mysql_num_fields(r);
			if(o >= fields)
				o = fields - 1;
			
			//Return
			return integer_xvar(mysql_field_seek(r, o));
		}
		
		//MySQL Field Lengths
		else if(!strcmp(name, "mysql_fetch_lengths"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_fetch_lengths function requires one arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'r')
			{
				fe("Error: The mysql_fetch_lengths function requires a valid MySQL result set.\n");
			}
			
			//The Connection
			MYSQL_RES *r = result->val->m->res;
			free_xvar(result);
			
			//Number of Rows
			unsigned long *lengths;
			lengths = mysql_fetch_lengths(r);
			int fields = mysql_num_fields(r);
			
			//No Fields
			if(fields == 0)
				return list_xvar(NULL);
			
			List *l = coal_alloc(sizeof(List));
			List *lc = l;
			List *lp = NULL;
			
			int i;
			for(i = 0; i < fields; i++)
			{
				lc->next = coal_alloc(sizeof(List));
				int length = lengths[i];
				lc->var = integer_xvar(length);
				
				lp = lc;
				lc = lc->next;
			}
			free(lp->next);
			lp->next = NULL;
			
			return list_xvar(l);
		}
		
		//MySQL Fetch Field
		else if(!strcmp(name, "mysql_fetch_field"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_fetch_field function requires one arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'r')
			{
				fe("Error: The mysql_fetch_field function requires a valid MySQL result set.\n");
			}
			
			//The Connection
			MYSQL_RES *r = result->val->m->res;
			free_xvar(result);
			args = args->next;
			
			MYSQL_FIELD *field;
			//Optional Offset
			if(args != NULL)
			{
				xvar *offset = evaluate(args);
				int o = intVal(offset);
				free_xvar(offset);
				
				int fields = mysql_num_fields(r);
				if(o >= fields)
					o = fields - 1;
				
				field = mysql_fetch_field_direct(r, o);
			}
			
			else
			{
				field = mysql_fetch_field(r);
			}
			
			//No Fields Left
			if(field == NULL)
				return integer_xvar(0);
				
			//All Good
			Hash **h = coal_alloc(sizeof(Hash) * HASHSIZE);
			Hash **hash_save = hash_ptr;
			hash_ptr = h;
			hash_init();
			//Add Everything to the Dictionary
			
			add("name", string_xvar(strdup(field->name)));
			add("org_name", string_xvar(strdup(field->org_name)));
			add("table", string_xvar(strdup(field->table)));
			add("org_table", string_xvar(strdup(field->org_table)));
			add("db", string_xvar(strdup(field->db)));
			add("catalog", string_xvar(strdup(field->catalog)));
			add("length", integer_xvar(field->length));
			add("max_length", integer_xvar(field->max_length));
			add("charsetnr", integer_xvar(field->charsetnr));
			unsigned int flags = field->flags;
			enum enum_field_types type = field->type;
			
			//Not Null
			add("nn", integer_xvar(IS_NOT_NULL(flags)));
			
			//Primary Key
			add("pk", integer_xvar(IS_PRI_KEY(flags)));
			
			//Auto Increment
			add("ai", integer_xvar(flags & AUTO_INCREMENT_FLAG));
			
			//Unique Key
			add("uk", integer_xvar(flags & UNIQUE_KEY_FLAG));
			
			//Multiple Key
			add("mk", integer_xvar(flags & MULTIPLE_KEY_FLAG));
			
			//Unsigned
			add("us", integer_xvar(flags & UNSIGNED_FLAG));
			
			//Zero Fill
			add("zf", integer_xvar(flags & ZEROFILL_FLAG));
			
			//Binary
			add("bin", integer_xvar(flags & BINARY_FLAG));
			
			//Numeric
			add("num", integer_xvar(flags & NUM_FLAG));
			
			//No Default Value
			add("ndv", integer_xvar(flags & NO_DEFAULT_VALUE_FLAG));
			
			//Data Type
			char *d;
			switch(type)
			{
				case MYSQL_TYPE_TINY:
					d = "tiny"; break;
				case MYSQL_TYPE_SHORT:
					d = "smallint"; break;
				case MYSQL_TYPE_LONG:
					d = "int"; break;
				case MYSQL_TYPE_INT24:
					d = "mediumint"; break;
				case MYSQL_TYPE_LONGLONG:
					d = "bigint"; break;
				case MYSQL_TYPE_DECIMAL:
					d = "decimal"; break;
				case MYSQL_TYPE_NEWDECIMAL:
					d = "ndecimal"; break;
				case MYSQL_TYPE_FLOAT:
					d = "float"; break;
				case MYSQL_TYPE_DOUBLE:
					d = "double"; break;
				case MYSQL_TYPE_BIT:
					d = "bit"; break;
				case MYSQL_TYPE_TIMESTAMP:
					d = "timestamp"; break;
				case MYSQL_TYPE_DATE:
					d = "date"; break;
				case MYSQL_TYPE_TIME:
					d = "time"; break;
				case MYSQL_TYPE_DATETIME:
					d = "datetime"; break;
				case MYSQL_TYPE_YEAR:
					d = "year"; break;
				case MYSQL_TYPE_STRING:
					d = "char"; break;
				case MYSQL_TYPE_VAR_STRING:
					d = "varchar"; break;
				case MYSQL_TYPE_BLOB:
					d = "blob"; break;
				case MYSQL_TYPE_SET:
					d = "set"; break;
				case MYSQL_TYPE_ENUM:
					d = "enum"; break;
				case MYSQL_TYPE_GEOMETRY:
					d = "geometry"; break;
				case MYSQL_TYPE_NULL:
					d = "null"; break;
				default:
					d = "";
			}
			
			add("type", string_xvar(strdup(d)));
			
			hash_ptr = hash_save;
			return hash_xvar(h);
		}
		
		//MySQL Num Rows
		else if(!strcmp(name, "mysql_num_rows"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_num_rows function requires one arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'r')
			{
				fe("Error: The mysql_num_rows function requires a valid MySQL result set.\n");
			}
			
			//The Connection
			MYSQL_RES *r = result->val->m->res;
			free_xvar(result);
			
			//Number of Rows
			int rows = mysql_num_rows(r);

			return integer_xvar(rows);
		}
		
		//MySQL Num Fields
		else if(!strcmp(name, "mysql_num_fields"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_num_fields function requires one arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'r')
			{
				fe("Error: The mysql_num_fields function requires a valid MySQL result set.\n");
			}
			
			//The Connection
			MYSQL_RES *r = result->val->m->res;
			free_xvar(result);
			
			//Number of Rows
			int rows = mysql_num_fields(r);

			return integer_xvar(rows);
		}
		
		//MySQL Field Count
		else if(!strcmp(name, "mysql_field_count"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_field_count function requires one arguments.");
			}
			
			xvar *result = evaluate(args);
			
			if(result->type != 'm')
			{
				fe("Error: The mysql_field_count function requires a valid MySQL connection.\n");
			}
			
			//The Connection
			MYSQL *m = result->val->m->con;
			free_xvar(result);
			
			//Number of Rows
			int count = mysql_field_count(m);

			return integer_xvar(count);
		}
		
		//MySQL Charset Name
		else if(!strcmp(name, "mysql_character_set_name"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_character_set_name function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_character_set_name function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return string_xvar(strdup(mysql_character_set_name(m)));
		}
		
		//MySQL Escape String
		else if(!strcmp(name, "mysql_escape_string"))
		{
			if(args == NULL && args->next != NULL)
			{
				fe("Error: The mysql_escape_string function requires two argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_escape_string function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			//The String to Escape
			xvar *string = evaluate(args->next);
			char *s = strVal(string);
			free_xvar(string);
			
			//Result String
			char *result = coal_alloc(strlen(s) * 2 + 1);
			
			mysql_real_escape_string(m, result, s, strlen(s));
			
			free(s);
			return string_xvar(result);
		}
		
		//MySQL Set Charset
		else if(!strcmp(name, "mysql_set_charset"))
		{
			if(args == NULL && args->next != NULL)
			{
				fe("Error: The mysql_set_charset function requires two argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_set_charset function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			//Character Set
			xvar *string = evaluate(args->next);
			char *s = strVal(string);
			free_xvar(string);
			
			int result = mysql_set_character_set(m, s);
			
			free(s);
			return integer_xvar(result == 0);
		}
		
		//MySQL Server Status
		else if(!strcmp(name, "mysql_stat"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_stat function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_stat function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);

			const char *result = mysql_stat(m);
			
			if(result != NULL && *result)
				return string_xvar(strdup(result));
			else
				return string_xvar(strdup(""));
		}
		
		//MySQL Info
		else if(!strcmp(name, "mysql_info"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_info function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_info function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);

			const char *result = mysql_info(m);
			
			if(result != NULL)
				return string_xvar(strdup(result));
			else
				return string_xvar(strdup(""));
		}
		
		//MySQL Client Info
		else if(!strcmp(name, "mysql_get_client_info"))
		{
			return string_xvar(strdup(mysql_get_client_info()));
		}
		
		//MySQL Client Version
		else if(!strcmp(name, "mysql_get_client_version"))
		{
			return integer_xvar(mysql_get_client_version());
		}
		
		//MySQL Host Info
		else if(!strcmp(name, "mysql_get_host_info"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_get_host_info function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_get_host_info function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);

			const char *result = mysql_get_host_info(m);
			
			return string_xvar(strdup(result));
		}
		
		//MySQL Protocol Info
		else if(!strcmp(name, "mysql_get_proto_info"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_get_proto_info function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_get_proto_info function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return integer_xvar(mysql_get_proto_info(m));
		}
		
		//MySQL Server Info
		else if(!strcmp(name, "mysql_get_server_info"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_get_server_info function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_get_server_info function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return string_xvar(strdup(mysql_get_server_info(m)));
		}
		
		//MySQL Server Version
		else if(!strcmp(name, "mysql_get_server_version"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_get_server_version function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_get_server_version function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return integer_xvar(mysql_get_server_version(m));
		}
		
		//MySQL Ping
		else if(!strcmp(name, "mysql_ping"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_ping function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_ping function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return integer_xvar(mysql_ping(m) == 0);
		}
		
		//MySQL Thread ID
		else if(!strcmp(name, "mysql_thread"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_thread function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_thread function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return integer_xvar(mysql_thread_id(m));
		}
		
		//MySQL Error Number
		else if(!strcmp(name, "mysql_errno"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_errno function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_errno function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return integer_xvar(mysql_errno(m));
		}
		
		//MySQL Error Message
		else if(!strcmp(name, "mysql_error"))
		{
			if(args == NULL)
			{
				fe("Error: The mysql_error function requires one argument.\n");
			}
			
			xvar *mysql = evaluate(args);
			
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_error function requires a valid MySQL connection.\n");
			}
			
			MYSQL *m = mysql->val->m->con;
			free_xvar(mysql);
			
			return string_xvar(strdup(mysql_error(m)));
		}
		
		//MySQL Fetch Row
		else if(!strcmp(name, "mysql_fetch_row"))
		{
			xvar *mysql = evaluate(args);
			return mysql_row_to_list(mysql);
		}
		
		//MySQL Affected Rows
		else if(!strcmp(name, "mysql_affected"))
		{
			xvar *mysql = evaluate(args);
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_affected() function requires a valid MySQL connection.\n");
			}
			
			int affected = mysql_affected_rows(mysql->val->m->con);
			free_xvar(mysql);
			return integer_xvar(affected);
		}
		
		//Select a Database
		else if(!strcmp(name, "mysql_select_db"))
		{
			//If this function is not passed a valid MySQL object, it doesn't hurt anything just to ignore the error and move on.
			if(args == NULL || args->next == NULL)
				return integer_xvar(0);
			
			//Get the MySQL object and Database Name
			xvar *mysql = evaluate(args);
			xvar *database = evaluate(args->next);
			char *db = strVal(database);
			free_xvar(database);
			
			if(mysql->type != 'm')
			{
				free_xvar(mysql);
				return integer_xvar(0);
			}
			
			//Can I free db?
			int result = mysql_select_db(mysql->val->m->con, db);
			free(db);
			free_xvar(mysql);
			return integer_xvar(result == 0);
		}
		
		//MySQL Insert ID
		else if(!strcmp(name, "mysql_insert_id"))
		{
			//Stupid Programmer
			if(args == NULL)
			{
				fe("Error: The mysql_insert_id() function requires a valid MySQL connection.\n");
			}
			
			//Get the MySQL object and Database Name
			xvar *mysql = evaluate(args);
			if(mysql->type != 'm')
			{
				fe("Error: The mysql_insert_id() function  requires a valid MySQL connection.\n");
			}
			
			int result = mysql_insert_id(mysql->val->m->con);
			free_xvar(mysql);
			return integer_xvar(result);
		}
		
		//Free a Result
		else if(!strcmp(name, "mysql_free_result"))
		{
			//If this function is not passed a valid MySQL object, it doesn't hurt anything just to ignore the error and move on.
			if(args == NULL)
				return integer_xvar(0);
			xvar *mysql = evaluate(args);
			if(mysql->type != 'r')
			{
				free_xvar(mysql);
				return integer_xvar(0);
			}
			
			mysql_free_result(mysql->val->m->res);
			free_xvar(mysql);
			return integer_xvar(1);
		}
		
		//Close a Connection
		else if(!strcmp(name, "mysql_close"))
		{
			//If this function is not passed a valid MySQL object, it doesn't hurt anything just to ignore the error and move on.
			if(args == NULL)
				return integer_xvar(0);
			xvar *mysql = evaluate(args);
			if(mysql->type != 'm')
			{
				free_xvar(mysql);
				return integer_xvar(0);
			}
			
			mysql_close(mysql->val->m->con);
			free_xvar(mysql);
			return integer_xvar(1);
		}
		
		//Does a variable, list element, string element, or dictionary element exist?
		else if(!strcmp(name, "set"))
		{
			while(args != NULL)
			{
				//Variables
				if(args->p == VAR_NODE && args->s != HASH_LOOKUP_NODE)
				{
					//Look Up Variable
					var_node *vn = (var_node *) args->sn;
					char *name = vn->name;
					char scope = vn->scope;
				
					SET_SCOPE();
	
					xvar *v = lookup(name, 0);
					
					//If NULL, the variable doesn't exist
					if(v == NULL)
						return integer_xvar(0);
				}
				
				//String and Lists
				else if(args->s == ELEMENT_NODE)
				{
					//Get the List and Element
					element_node *en = (element_node *) args->sn;
					
					xvar *list = evaluate(en->value);
					xvar *element = evaluate(en->el);
					int el = intVal(element);
					free_xvar(element);
					
					//Get Length
					int length = xvar_length(list);
					free_xvar(list);
					
					if(el >= length)
						return integer_xvar(0);
				}
				
				//Dictionary Element
				else if(args->s == HASH_LOOKUP_NODE)
				{
					//Get the Key
					hash_lookup_node *hn = (hash_lookup_node *) args->sn;
					xvar *k = evaluate(hn->key);
					char *key = strVal(k);
					free_xvar(k);
					
					//And the Variable
					xvar *d = evaluate(hn->var);
					
					Hash **hash_save = hash_ptr;
					hash_ptr = d->val->h;
					xvar *val = lookup(key, 0);
					hash_ptr = hash_save;
					
					//Doesn't exist!
					if(val == NULL)
						return integer_xvar(0);
				}
				args = args->next;
			}
			
			return integer_xvar(1);
		}
		
		//Typeof Value
		else if(!strcmp(name, "typeof"))
		{
			extern int line_number;
			xvar *t = evaluate(args);
			char *type = malloc(2);
			type[0] = t->type;
			type[1] = '\0';
			free_xvar(t);
			return string_xvar(type);
		}
		
		//Set a Session Cookie
		else if(!strcmp(name, "cookie"))
		{
			char *format = "Set-Cookie: %s=%s; %s %s %s %s %s\n";
			if(args != NULL && args->next != NULL)
			{
				xvar *tmp;
				tmp = evaluate(args);
				char *name = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
				
				tmp = evaluate(args);
				char *value = strVal(tmp);
				free_xvar(tmp);
				args = args->next;
				
				char *max_age = strdup("");
				if(args != NULL)
				{
					tmp = evaluate(args);
					max_age = strVal(tmp);
					
					char *ptr = max_age;
					max_age = concat(max_age, ";");
					free(ptr);
					
					ptr = max_age;
					max_age = concat("Max-Age=", max_age);
					
					free(ptr);
					free_xvar(tmp);
					args = args->next;
				}
				
				char *path = strdup("");
				if(args != NULL)
				{
					tmp = evaluate(args);
					path = strVal(tmp);
					
					char *ptr = path;
					path = concat(path, ";");
					free(ptr);
					
					ptr = path;
					path = concat("Path=", path);
					free(ptr);
					
					free_xvar(tmp);
					args = args->next;
				}
				
				char *domain = strdup("");
				if(args != NULL)
				{
					tmp = evaluate(args);
					domain = strVal(tmp);
					
					char *ptr = domain;
					domain = concat(domain, ";");
					free(ptr);
					
					ptr = domain;
					domain = concat("Domain=", domain);
					free(ptr);
					
					free_xvar(tmp);
					args = args->next;
				}
				
				char *secure = strdup("");
				if(args != NULL)
				{
					tmp = evaluate(args);
					if(if_xvar(tmp))
						secure = strdup("Secure;");
					args = args->next;
				}
				
				char *http_only = strdup("");
				if(args != NULL)
				{
					tmp = evaluate(args);
					if(if_xvar(tmp))
						http_only = strdup("HttpOnly");
					args = args->next;
				}

				//Print the Cookie
				printf(format, name, value, max_age, path, domain, secure, http_only);
				
				free(name);
				free(value);
				free(max_age);
				free(path);
				free(domain);
				free(secure);
				free(http_only);
			}
			
			else
			{
				fe("Error: The cookie() function requires at least two arguments.\n");
			}
			
			return integer_xvar(1);
		}
		
		//File Handling Functions
		
		//Open File
		else if(!strcmp(name, "fo"))
		{
			//Two Arguments
			if(args == NULL)
			{
				fe("Error: The fo() requires requires at least arguments.\n");
			}
			
			xvar *filename = evaluate(args);
			
			char *f = strVal(filename);
			free_xvar(filename);
			args = args->next;
			
			char *m = "r+";
			if(args != NULL)
			{
				xvar *mode = evaluate(args->next);
				m = strVal(mode);
				free_xvar(mode);
			}
			
			FILE *fp = fopen(f, m);
			if(fp == NULL)
				return integer_xvar(0);
			return file_xvar(fp);
		}
		
		//Read From File
		else if(!strcmp(name, "fr"))
		{
			//One Arguments
			if(args == NULL)
			{
				fe("Error: The  fr() function requires at least one argument.\n");
			}
			
			//File and length
			xvar *file = evaluate(args);
			xvar *length = NULL;
			
			if(args->next != NULL)
			{
				length = evaluate(args->next);
			}
			
			//Valid file object
			if(file->type != 'f')
			{
				fe("Error: The fr() function requries a valid file object.\n");
			}
			
			//The File
			FILE *fp = file->val->f;
			
			//Read the whole file
			int size;
			if(length == NULL)
			{
				int before = ftell(fp);
				fseek(fp, 0, SEEK_END);
				size = ftell(fp) - before;
				fseek(fp, before, SEEK_SET);
			}
			//Only part of the file
			else
			{
				size = intVal(length);
			}
			
			free_xvar(length);
			free_xvar(file);
			
			//The Return String
			char *s = malloc(size + 1);
			char *copy = s;
			
			char c;
			int i = 0;
			while(i < size && (c = fgetc(fp)) != EOF)
			{
				*copy++ = c;
				i++;
			}
			*copy = '\0';
			
			return string_xvar(s);
		}
		
		//Write to File
		else if(!strcmp(name, "fw"))
		{
			//Two Arguments
			if(!(args != NULL && args->next != NULL))
			{
				fe("Error: The fw() function requires two arguments.\n");
			}
			
			//File and Characters to be Writen
			xvar *file = evaluate(args);
			xvar *string = evaluate(args->next);
			
			char *str = strVal(string);
			free_xvar(string);
			FILE *fp = file->val->f;
			free_xvar(file);
			
			//Write Each Character
			int i = 0;
			while(*str)
			{
				//Return Error
				if(fputc(*str++, fp) == EOF)
					return integer_xvar(0);
				i++;
			}
			//Return Number of Characters Written
			return integer_xvar(i);
		}
		
		//Close a File
		else if(!strcmp(name, "fc"))
		{
			//We don't really care what we're given
			if(args == NULL)
			{
				return integer_xvar(0);
			}
			
			xvar *file = evaluate(args);
			
			if(file->type != 'f')
				return integer_xvar(0);
				
			fclose(file->val->f);
			return integer_xvar(1);
		}
		
		//Remove a File
		else if(!strcmp(name, "fd"))
		{
			//One Argument
			if(args == NULL)
			{
				fe("Error: The fd() function requires at least one argument.\n");
			}
			
			//Evaluate, Get String Value, and Remove
			xvar *f = evaluate(args);
			char *fn = strVal(f);
			int i = remove(fn);
			
			//Free, Free, and Return
			free(fn);
			free_xvar(f);
			return integer_xvar(i == 0);
		}
		
		//Environment Variables ($_SERVER in PHP)
		else if(!strcmp(name, "getenv"))
		{
			if(args == NULL)
			{
				printf("Error: The getenv() function requires one argument.\n");
				exit(1);
			}
			xvar *string = evaluate(args);
			char *s = strVal(string);
			free_xvar(string);
			
			char *ge = getenv(s);
			
			free(s);
			
			if(ge == NULL)
				return string_xvar(strdup(""));
				
			return string_xvar(strdup(ge));
		}
		
		//Add a Property to an Object
		else if(!strcmp(name, "add_prop"))
		{
			//Two Arguments
			if(args == NULL || args->next == NULL)
			{
				fe("Error: The add_prop() function requires two arguments.");
			}
			
			//Object
			xvar *object = evaluate(args);
			if(object->type != 'o')
				return integer_xvar(0);
			
			Object *o = object->val->o;
			free_xvar(object);
			args = args->next;
			
			//Property Name
			xvar *name = evaluate(args);
			char *n = strVal(name);
			free_xvar(name);
			args = args->next;
			
			//Optional Value
			xvar *value = NULL;
			if(args != NULL)
				value = evaluate(args);
				
			//The Attribute to Add
			Attributes *a = coal_alloc(sizeof(Attributes));
			a->name = n;
			a->value = value;
			a->protect = 'x';
			a->strong = 'a';
			a->next = NULL;
			
			//No Properties
			if(o->attrs == NULL)
			{
				o->attrs = a;
				return integer_xvar(1);
			}
			
			Attributes *an = o->attrs;
			while(an->next != NULL)
				an = an->next;
			an->next = a;
			return integer_xvar(1);
		}
		
		//Copy (Objects, Dictionaries (Works with other stuff as well))
		else if(!strcmp(name, "copy"))
		{
			if(args == NULL)
			{
				fe("Error: The copy() function requires at least one argument.\n");
			}
			
			xvar *arg = evaluate(args);
			//Non Objects and Dictionaries
			if(arg->type != 'h' && arg->type != 'o')
			{
				xvar *dup = dup_xvar(arg);
				free_xvar(arg);
				return dup;
			}
			
			//Dictionary
			else if(arg->type == 'h')
			{
				Hash **h = arg->val->h;
				Hash **copy = coal_alloc(sizeof(Hash *) * HASHSIZE);
				
				int i;
				for(i=0;i<HASHSIZE;i++)
				{
					copy[i] = coal_alloc(sizeof(Hash));
					Hash *h2;
					Hash *c2 = copy[i];
					Hash *c2_copy = c2;
					Hash *p = c2;
					int j = 0;
					for(h2 = h[i];h2 != NULL; h2 = h2->next, p = c2_copy, c2_copy = c2_copy->next, j++)
					{
						c2_copy->next = coal_alloc(sizeof(Hash));
						c2_copy->key = strdup(h2->key);
						c2_copy->value = dup_xvar(h2->value);
					}
					
					if(j)
					{
						free(c2_copy);
						p->next = NULL;
					}
					
					else
					{
						free(c2);
						c2 = NULL;
					}
				}
				free_xvar(arg);
				return hash_xvar(copy);
			}
			
			//Object
			else
			{
				Object *x = arg->val->o;
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
					oa->value = NULL;
					oa->protect = xa->protect;
					oa->strong = xa->strong;
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
			
				free_xvar(arg);
				return object_xvar(o);
			}
		}