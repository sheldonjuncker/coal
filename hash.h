typedef struct Hash Hash;

struct Hash{
	char *key;
	xvar *value;
	Hash *next;
};

int hash(char *);
Hash *new_hash(char *, xvar *);
xvar *lookup(char *, int);
xvar *add(char *, xvar *);
xvar *unset(char *);