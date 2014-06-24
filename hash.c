#include "hash.h"
#define HASHSIZE 256
int ALLOW_SELF = 0;

//A Hash Table Implementation for C

//Hash Table
Hash *globals[HASHSIZE];
Hash **locals_ptr;
Hash **hash_ptr;

//Init
void hash_init()
{
	int i;
	for(i=0;i<HASHSIZE;i++)
		hash_ptr[i] = NULL;
}

void hash_init_all()
{
	hash_ptr = globals;
	hash_init();
}

//Free Hash Element
void hash_el_free(Hash *ptr)
{
	while(ptr != NULL)
	{
		Hash *copy = ptr;
		free(ptr->key);
		destroy_xvar(ptr->value);
		ptr = ptr->next;
		//?
		free(copy);
	}
}

//Free Hash Table
void hash_free(Hash **ptr)
{
	int i;
	for(i=0;i<HASHSIZE;i++)
	{
		hash_el_free(ptr[i]);
		//Double Free
		//free(ptr[i]);
	}
}

//Hash Function
int hash(char *s)
{
	int val = HASHSIZE;
	while(*s)
	{
		val += (*s * 13);
		s++;
	}
	return val % HASHSIZE;
}

//New Hash Node
Hash *new_hash(char *key, xvar *val)
{
	Hash *h = coal_alloc(sizeof(Hash));
	h->key = strdup(key);
	h->value = val;
	h->next = NULL;
	return h;
}

xvar *lookup(char *s, int not_null)
{
	//Hash the String
	Hash *h;
	int i = hash(s);
	
	//Lookup Key in Hash Table
	for(h = hash_ptr[i]; h != NULL; h = h->next)
		if(!strcmp(h->key, s))
			return h->value;
			
	//For the isset function, it is not necessary that the key was found.
	if(not_null)
	{
		printf("Error: Could not find variable %s\n", s);
		exit(1);
	}
	
	else
	{
		return NULL;
	}
}

xvar *add(char *key, xvar *value)
{
	//Security Self Referencing
	if(ALLOW_SELF == 0 && !strcmp(key, "$self"))
	{
		printf("Error: Invalid use of the $self variable.\n");
		exit(1);
	}
	int i = hash(key);

	//First
	if(hash_ptr[i] == NULL)
	{
		hash_ptr[i] = new_hash(key, value);
		return value;
	}
	Hash *h;
	Hash *end;
	for(h = hash_ptr[i]; h != NULL; h = h->next)
	{
		//Replace
		if(!strcmp(h->key, key))
		{
			//Strongly Typing
			if(h->value->strong != 'a')
			{
				if(h->value->strong != value->type)
				{
					printf("Error: Cannot assign a value of type %c to a strongly typed variable of type %c.\n", value->type, h->value->strong);
					exit(1);
				}
				
				else
				{
					//Make sure that it stays strongly typed
					value->strong = h->value->strong;
				}
			}
			destroy_xvar(h->value);
			h->value = value;
			return value;
		}
		end = h;
	}
	//Last
	end->next = new_hash(key, value);
	return value;
}

xvar *unset(char *key)
{
	int i = hash(key);
	
	if(hash_ptr[i] == NULL)
		return NULL;
	
	Hash *c;
	Hash *p;
	Hash *n;
	
	int counter = 0;
	
	for(c = hash_ptr[i]; c != NULL; c = c->next)
	{
		if(!strcmp(c->key, key))
		{
			//Last
			if(c->next == NULL)
			{
				destroy_xvar(c->value);
				free(c->key);
				free(c);
				if(counter)
					p->next = NULL;
				else
					hash_ptr[i] = NULL;
				return NULL;
			}
			
			
			//First
			if(counter == 0)
			{
				hash_ptr[i] = c->next;
				destroy_xvar(c->value);
				free(c->key);
				free(c);
				return NULL;
			}
			
			n = c->next;
			p->next = n;
			destroy_xvar(c->value);
			free(c->key);
			free(c);
			return NULL;
		}
		p = c;
		counter++;
	}
	return NULL;
}