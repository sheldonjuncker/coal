void cgi_get()
{
	char *get = getenv("QUERY_STRING");
	Hash **get_hash = coal_alloc(sizeof(Hash *) * HASHSIZE);
	Hash **hash_save = hash_ptr;
	hash_ptr = get_hash;
	hash_init();
	
	if(get != NULL && *get)	
	{
		plus2space(get);
		get = urldecode(get);
		//As many get variables as we want
		link_str *ls = split(get, '&', 0);
		link_str *ls_eq;
		for(ls; ls != NULL; ls = ls->next)
		{
			//Only one equal sign
			ls_eq = split(ls->str, '=', 1);
			char *key = ls_eq->str;
			
			char *value;
			if(ls_eq->next == NULL)
				value = "";
			else
				value = ls_eq->next->str;
			add(key, string_xvar(value));
		}
	}
	
	hash_ptr = hash_save;
	add("$get", hash_xvar(get_hash));
	return;
}

void cgi_post()
{
	Hash **post_hash = coal_alloc(sizeof(Hash *) * HASHSIZE);
	Hash **hash_save = hash_ptr;
	hash_ptr = post_hash;
	hash_init();
	char *rm = getenv("REQUEST_METHOD");
	if(rm != NULL && !strcmp(rm, "POST"))
	{
		int len = atoi(getenv("CONTENT_LENGTH"));
		char *post = coal_alloc(len+1);
		char *copy = post;
		char c;
		while((c = getc(stdin)) != EOF)
		{
			*copy++ = c;
		}
		*copy = '\0';
		
		plus2space(post);
		post = urldecode(post);
		
		//As many get variables as we want
		link_str *ls = split(post, '&', 0);
		link_str *ls_eq;
		for(ls; ls != NULL; ls = ls->next)
		{
			//Only one equal sign
			ls_eq = split(ls->str, '=', 1);
			char *key = ls_eq->str;
			
			char *value;
			if(ls_eq->next == NULL)
				value = "";
			else
				value = ls_eq->next->str;
			add(key, string_xvar(value));
		}
	}
	
	hash_ptr = hash_save;
	add("$post", hash_xvar(post_hash));
	return;
}

void cgi_cookie()
{
	char *cookie = getenv("HTTP_COOKIE");
	Hash **get_hash = coal_alloc(sizeof(Hash *) * HASHSIZE);
	Hash **hash_save = hash_ptr;
	hash_ptr = get_hash;
	hash_init();
	
	if(cookie != NULL && *cookie)
	{
		link_str *ls = split(cookie, ';', 0);
		link_str *ls_eq;
		for(ls; ls != NULL; ls = ls->next)
		{
			//Only one equal sign
			ls_eq = split(ls->str, '=', 1);
			char *key = ls_eq->str;
			if(*key == ' ')
				key++;
			
			char *value;
			if(ls_eq->next == NULL)
				value = "";
			else
				value = ls_eq->next->str;
			add(key, string_xvar(value));
		}
	}
	
	hash_ptr = hash_save;
	add("$cookie", hash_xvar(get_hash));
	return;
}

void cgi_init()
{
	cgi_get();
	cgi_post();
	cgi_cookie();
	printf("Content-type: text/html\n");
}