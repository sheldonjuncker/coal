
#include "nodes.h"
#include "mmgr.h"

//Blank Node
node_t *blank_node()
{
	node_t *n = mmgr_alloc(sizeof(node_t));
	n->next = NULL;
	return n;
}

//Start Node
node_t *start_node()
{
	node_t *n = blank_node();
	n->p = START_NODE;
}

//End Node
node_t *end_node()
{
	return NULL;
}

//Integer Node
node_t *new_int_node(int i)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = INT_NODE;
	int_node *in = mmgr_alloc(sizeof(int_node));
	in->i = i;
	n->sn = in;
	return n;
}

//Double Node
node_t *new_dub_node(double d)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = DUB_NODE;
	dub_node *dn = mmgr_alloc(sizeof(dub_node));
	dn->d = d;
	n->sn = dn;
	return n;
}

//String Node
node_t *new_str_node(char *s)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = STR_NODE;
	
	str_node *sn = mmgr_alloc(sizeof(str_node));
	sn->s = mmgr_strdup(s);
	free(s);
	n->sn = sn;
	return n;
}

//List Node
node_t *new_list_node(stmt_t *l)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = LIST_NODE;
	
	list_node *ln = mmgr_alloc(sizeof(list_node));
	if(l != NULL)
		ln->l = l->b;
	else
		ln->l = NULL;
	n->sn = ln;
	return n;
}

//Id Node
node_t *new_id_node(char *name)
{
	node_t *n = blank_node();
	
	id_node *in = mmgr_alloc(sizeof(id_node));
	in->name = mmgr_strdup(name);
	free(name);
	
	n->sn = in;
	return n;
}

//Ternary Node
node_t *new_ternary_node(node_t *condition, node_t *if_exp, node_t *else_exp)
{
	node_t *n = blank_node();
	n->p = TERNARY_NODE;
	
	ternary_node *tn = mmgr_alloc(sizeof(ternary_node));
	tn->condition = condition;
	tn->if_exp = if_exp;
	tn->else_exp = else_exp;
	n->sn = tn;
	return n;
}

//Type Cast Node
node_t *new_cast_node(char type, node_t *exp)
{
	node_t *n = blank_node();
	n->p = CAST_NODE;
	
	cast_node *cn = mmgr_alloc(sizeof(cast_node));
	cn->type = type;
	cn->exp = exp;
	n->sn = cn;
	return n;
}

//If Node
node_t *new_if_node(node_t *condition, stmt_t *block, stmt_t *opt_elseifs, stmt_t *opt_else)
{
	node_t *n = blank_node();
	n->p = CONTROL_NODE;
	n->s = IF_NODE;
	if_node *in = mmgr_alloc(sizeof(if_node));
	in->condition = condition;
	in->block = block->b;
	if(opt_elseifs != NULL)
		in->opt_elseifs = opt_elseifs->b;
	else
		in->opt_elseifs = NULL;
	if(opt_else != NULL)
		in->opt_else = opt_else->b;
	else
		in->opt_else = NULL;
	n->sn = in;
	return n;
}

//ElseIf Node
node_t *new_elseif_node(node_t *condition, stmt_t *block)
{
	node_t *n = blank_node();
	n->p = CONTROL_NODE;
	n->s = ELSEIF_NODE;
	elseif_node *in = mmgr_alloc(sizeof(elseif_node));
	in->condition = condition;
	in->block = block->b;
	n->sn = in;
	return n;
}

//Else Node
node_t *new_else_node(stmt_t *block)
{
	node_t *n = blank_node();
	n->p = CONTROL_NODE;
	n->s = ELSE_NODE;
	else_node *in = mmgr_alloc(sizeof(else_node));

	in->block = block->b;
	n->sn = in;
	return n;
}

//While Node
node_t *new_while_node(node_t *condition, stmt_t *block)
{
	node_t *n = blank_node();
	n->p = CONTROL_NODE;
	n->s = WHILE_NODE;
	while_node *wn = mmgr_alloc(sizeof(while_node));
	wn->condition = condition;
	wn->block = block->b;
	n->sn = wn;
	return n;
}

//In Node
node_t *new_in_node(node_t *el, node_t *list)
{
	node_t *n = blank_node();
	n->p = IN_NODE;
	in_node *fn = mmgr_alloc(sizeof(in_node));
	fn->el = el;
	fn->list = list;
	n->sn = fn;
	return n;
}

//For In Node
node_t *new_forin_node(node_t *in, stmt_t *block)
{
	node_t *n = blank_node();
	n->p = CONTROL_NODE;
	n->s = FORIN_NODE;
	forin_node *fn = mmgr_alloc(sizeof(forin_node));
	fn->in = in;
	fn->block = block->b;
	n->sn = fn;
	return n;
}

//Function Node
node_t *new_function_node(char strong, char *name, stmt_t *params, stmt_t *block)
{
	node_t *n = blank_node();
	n->p = FUNCTION_NODE;
	function_node *fn = mmgr_alloc(sizeof(function_node));
	fn->strong = strong;
	fn->name = mmgr_strdup(name);
	free(name);
	fn->block = block->b;
	if(params != NULL)
		fn->params = params->b;
	else
		fn->params = NULL;
	n->sn = fn;
	return n;
}

//Method Node
node_t *new_method_node(char protect, char strong, char *name, stmt_t *params, stmt_t *block)
{
	node_t *n = blank_node();
	n->p = METHOD_NODE;
	method_node *fn = mmgr_alloc(sizeof(method_node));
	fn->protect = protect;
	fn->strong = strong;
	fn->name = mmgr_strdup(name);
	free(name);
	fn->block = block->b;
	if(params != NULL)
		fn->params = params->b;
	else
		fn->params = NULL;
	n->sn = fn;
	return n;
}

//Parameter Node
node_t *new_param_node(node_t *param, node_t *def)
{
	node_t *n = blank_node();
	n->p = -101;
	param_node *pn = mmgr_alloc(sizeof(param_node));
	pn->param = param;
	pn->def = def;
	n->sn = pn;
	return n;
}

//New Object Node
node_t *new_object_node(char *name, stmt_t *parents, stmt_t *attributes, stmt_t *methods)
{
	node_t *n = blank_node();
	n->p = OBJECT_NODE;
	object_node *on = mmgr_alloc(sizeof(object_node));
	on->name = mmgr_strdup(name);
	free(name);
	if(attributes != NULL)
		on->attrs = attributes->b;
	else
		on->attrs = NULL;
		
	if(methods != NULL)
		on->methods = methods->b;
	else
		on->methods = NULL;
	
	if(parents != NULL)
		on->parents = parents->b;
	else
		on->parents = NULL;
	n->sn = on;
	return n;
}

//New Instance Node
node_t *new_instance_node(char *name)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = INSTANCE_NODE;
	instance_node *in = mmgr_alloc(sizeof(instance_node));
	in->name = mmgr_strdup(name);
	free(name);
	n->sn = in;
	return n;
}

//New Property Node
node_t *new_property_node(node_t *obj, char *prop)
{
	node_t *n = blank_node();
	n->p = VAR_NODE;
	n->s = PROPERTY_NODE;
	property_node *in = mmgr_alloc(sizeof(property_node));
	in->obj  = obj;
	in->prop = mmgr_strdup(prop);
	free(prop);
	n->sn = in;
	return n;
}

//New Property Assign Node
node_t *new_property_assign_node(node_t *prop, node_t *value)
{
	node_t *n = blank_node();
	n->p = ASSIGN_NODE;
	n->s = PROPERTY_ASSIGN_NODE;
	property_assign_node *in = mmgr_alloc(sizeof(property_assign_node));
	in->prop = prop;
	in->value = value;
	n->sn = in;
	return n;
}

//Element Node
node_t *new_element_node(node_t *value, node_t *el)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = ELEMENT_NODE;
	element_node *en = mmgr_alloc(sizeof(element_node));
	en->value = value;
	en->el = el;
	n->sn = en;
	return n;
}

//Math Operator Node
node_t *new_op_node(int type, char *op, node_t *left, node_t *right)
{
	node_t *n = blank_node();
	n->p = OP_NODE;
	
	//mop or cop
	n->s = type;
	op_node *on = mmgr_alloc(sizeof(op_node));
	on->left = left;
	on->op = mmgr_strdup(op);
	free(op);
	on->right = right;
	n->sn = on;
	return n;
}

//Assignment Node
node_t *new_assign_node(node_t *var, node_t *value)
{
	node_t *n = blank_node();
	n->p = ASSIGN_NODE;
	n->s = VAR_ASSIGN_NODE;
	
	assign_node *an = mmgr_alloc(sizeof(assign_node));
	an->var = var;
	an->value = value;
	n->sn = an;
	return n;
}

//List Assignment Node
node_t *new_list_assign_node(node_t *name, node_t *el, node_t *value)
{
	node_t *n = blank_node();
	n->p = ASSIGN_NODE;
	n->s = LIST_ASSIGN_NODE;
	
	list_assign_node *an = mmgr_alloc(sizeof(list_assign_node));
	an->name = name;
	an->el = el;
	an->value = value;
	n->sn = an;
	return n;
}

//Hash Assignment Node
node_t *new_hash_assign_node(node_t *var, node_t *key, node_t *val)
{
	node_t *n = blank_node();
	n->p = ASSIGN_NODE;
	n->s = HASH_ASSIGN_NODE;
	
	hash_assign_node *an = mmgr_alloc(sizeof(hash_assign_node));
	an->var = var;
	an->key = key;
	an->val = val;
	n->sn = an;
	return n;
}

//Hash Lookup Node
node_t *new_hash_lookup_node(node_t *var, node_t *key)
{
	node_t *n = blank_node();
	n->p = VAR_NODE;
	n->s = HASH_LOOKUP_NODE;
	
	hash_lookup_node *an = mmgr_alloc(sizeof(hash_lookup_node));
	an->var = var;
	an->key = key;
	n->sn = an;
	return n;
}

//Variable Node
node_t *new_var_node(char *name, char scope, char strong)
{
	
	if(strong == 'b')
		strong = 'i';
	node_t *n = blank_node();
	n->p = VAR_NODE;
	n->s = VAR_NODE;
	var_node *vn = mmgr_alloc(sizeof(var_node));
	//Leak?
	vn->name = mmgr_strdup(name);
	free(name);
	vn->scope = scope;
	vn->strong = strong;
	n->sn = vn;
	return n;
}

//Hash Table Node
node_t *new_hash_node(node_t *key, node_t *val)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = HASH_NODE;
	hash_node *hn = mmgr_alloc(sizeof(hash_node));
	hn->key = key;
	hn->val = val;
	n->sn = hn;
	return n;
}

//Hash Table Node Thingy
node_t *new_hash_thingy(stmt_t *hashes)
{
	node_t *n = blank_node();
	n->p = VALUE_NODE;
	n->s = HASH_NODE;
	if(hashes == NULL)
		n->sn = NULL;
	else
		n->sn = hashes->b;
	return n;
}

//Function Call Node
node_t *new_call_node(char *name, stmt_t *args)
{
	node_t *n = blank_node();
	n->p = CALL_NODE;
	n->s = -1;
	call_node *vn = mmgr_alloc(sizeof(call_node));
	vn->name = mmgr_strdup(name);
	free(name);
	if(args == NULL)
		vn->args = NULL;
	else
		vn->args = args->b;
	n->sn = vn;
	return n;
}

//Method Call Node
node_t *new_method_call_node(node_t *obj, char *method, stmt_t *args)
{
	node_t *n = blank_node();
	n->p = CALL_NODE;
	n->s = METHOD_NODE;
	method_call_node *vn = mmgr_alloc(sizeof(method_call_node));
	vn->obj = obj;
	vn->method = mmgr_strdup(method);
	free(method);
	if(args == NULL)
		vn->args = NULL;
	else
		vn->args = args->b;
	n->sn = vn;
	return n;
}

//Return Node
node_t *new_return_node(node_t *value)
{
	node_t *n = blank_node();
	n->p = RETURN_NODE;
	return_node *rn = mmgr_alloc(sizeof(return_node));
	rn->value = value;
	n->sn = rn;
	return n;
}

//Print Node
node_t *new_print_node(node_t *value)
{
	node_t *n = blank_node();
	n->p = PRINT_NODE;
	print_node *pn = mmgr_alloc(sizeof(print_node));
	pn->value = value;
	n->sn = pn;
	return n;
}

//Increment/Decrement Var
node_t *new_incdec_node(node_t *var, char *op, int after)
{
	node_t *n = blank_node();
	n->p = INCDEC_NODE;
	if(*op == '+')
		n->s = INC_NODE;
	else
		n->s = DEC_NODE;
	if(after)
		n->t = INCA_NODE;
	else
		n->t = INCB_NODE;
	
	incdec_node *in = mmgr_alloc(sizeof(incdec_node));
	in->var = var;
	n->sn = in;
	free(op);
	return n;
}

stmt_t *blank_stmt()
{
	stmt_t *s = mmgr_alloc(sizeof(stmt_t));
	s->b = NULL;
	s->e = NULL;
	return s;
}

stmt_t *node_to_stmt(node_t *n)
{
	stmt_t *s = blank_stmt();
	s->b = n;
	s->e = n;
	return s;
}

//Link Statements
stmt_t *link_stmts(stmt_t *s1, stmt_t *s2)
{
	if(s1 == NULL)
		return s2;
	if(s2 == NULL)
	{
		s1->e->next = NULL;
		return s1;
	}
	s1->e->next = s2->b;
	stmt_t *s = mmgr_alloc(sizeof(stmt_t));
	s->b = s1->b;
	s->e = s1->e->next;
	return s;
}