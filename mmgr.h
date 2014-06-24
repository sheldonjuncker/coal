#include <stdio.h>
#include <stdlib.h>

/*
	A Little Memory Manager for Coal
	
	It maintains a linked list of pointers to all nodes and other parts of the AST that need to be freed. Then, when an AST is no longer in use, the entire tree can be freed one pointer at a time.
	
	This is probably crazy, but I'm not an expert C programmer and it's the only thing that I can come up with.
*/

//Bytes of Memory Allocated
long mmgr_bytes = 0;

//Length of Linked List
long mmgr_length = 0;

//Linked List
typedef struct mmgr_node mmgr_node;

struct mmgr_node{
	void *ptr;
	mmgr_node *next;
};

//mmgr_b Node
mmgr_node *mmgr_b;

//mmgr_c Node
mmgr_node *mmgr_c;

//New Node
mmgr_node *mmgr_new()
{
	mmgr_node *n = coal_alloc(sizeof(mmgr_node));
	n->ptr = NULL;
	n->next = NULL;
	mmgr_bytes += sizeof(mmgr_node);
	mmgr_length++;
	return n;
}

//Initialize
void mmgr_init()
{
	mmgr_b = mmgr_new();
	mmgr_c = mmgr_b;
}

//Add a Node
void mmgr_add(void *ptr)
{
	mmgr_c->next = mmgr_new();
	mmgr_c->ptr = ptr;
	mmgr_c = mmgr_c->next;
}

//Memory Allocator
void *mmgr_alloc(size_t i)
{
	void *ptr = coal_alloc(i);
	mmgr_add(ptr);
	return ptr;
}

//Mmgr Strdup
char *mmgr_strdup(const char *s)
{
	char *r = strdup(s);
	mmgr_add(r);
	return r;
}

//Free Nodes
void mmgr_free()
{
	while(mmgr_b != NULL)
	{
		free(mmgr_b->ptr);
		mmgr_node *ptr = mmgr_b;
		mmgr_b = mmgr_b->next;
		free(ptr);
	}
}
