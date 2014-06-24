/*
	A List of Coal's Standard Library Functions
	-----
	coal_alloc();
		Memory Allocator
		
	print_s();
		Print String
	print_sn();
		(with newline)
	
	print_d();
	print_dn();
		Print Double
	
	print_i();
	print_in();
		Print Integer
		
	concat();
		Concatenate Two Strings
	split()
		Split String by Delimiter
	plus2space()
		Convert Plusses to Spaces (for URLS)
	nullify()
		Add a Nul Terminator to a String to Make it Usable (for MySQL)
	substr()
		Finds a string within a string
	str_replace()
		Replaces occurrences of a substring with another string in a source string
		
	i2s();
		Integer To String
	f2s();
		Double To String
	
	filesize();
		Length of a File
	file_get_contents();
		File to String
*/

//Coal's Memory Allocator
void *coal_alloc(int i)
{
	void *mem = malloc(i);
	if(mem == NULL)
	{
		printf("Error: Ran out of memory.\n");
		exit(1);
	}
	return mem;
}

//Print a String
void *print_s(char *s)
{
	printf("%s", s);
}
	
//Print a String (With Newline)
void *print_sn(char *s)
{
	printf("%s\n", s);
}

//Print an Integer
void print_i(int i)
{
	printf("%i", i);
}
	
//Print an Integer (With Newline)
void print_in(int i)
{
	printf("%i\n", i);
}

//Print a Double
void print_d(double d)
{
	printf("%f", d);
}

//Print a Double (With Newline)
void print_dn(double d)
{
	printf("%f\n", d);
}

//Concatenation
char *concat(char *s1, char *s2)
{
	char *result = coal_alloc(strlen(s1) + strlen(s2) + 1);
	char *copy = result;
	
	while(*s1)
		*copy++ = *s1++;
	while(*s2)
		*copy++ = *s2++;
	*copy = '\0';
	
	return result;
}

//Nullify
char *nullify(char *s, int len)
{
	char *o = coal_alloc(len + 1);
	int i;
	for(i=0; i<len; i++)
	{
		o[i] = s[i];
	}
	o[i] = '\0';
	return o;
}

//Linked Strings
typedef struct link_str link_str;
struct link_str{
	char *str;
	link_str *next;
};

link_str *split(char *s, char d, int n)
{
	s = strdup(s);
	char *copy = s;
	link_str *ls = malloc(sizeof(link_str));
	link_str *ls_copy = ls;
	int i = 0;
	
	while(*copy)
	{
		if(*copy == d)
		{
			*copy = '\0';
			char *one = strdup(s);
			s = (copy+1);
			ls_copy->next = malloc(sizeof(link_str));
			ls_copy->str = one;
			ls_copy = ls_copy->next;
			i++;
			if(n>0 && n==i)
				break;
		}
		copy++;
	}

	ls_copy->str = strdup(s);
	ls_copy->next = NULL;
	
	free(s);
	return ls;
}

//Other String Split
link_str *str_split(char *s, char *by)
{
	//Length
	int by_len = strlen(by);

	//Duplicate and Pointer Copy
    s = strdup(s);
    char *cp = s;
	
	//Link Strings
	link_str *ls = coal_alloc(sizeof(link_str));
	link_str *ls_copy = ls;
	link_str *ls_prev = ls;

	int i = 0;
    while(*cp)
	{
		//Next String
		ls_copy->next = coal_alloc(sizeof(link_str));
		
		//Next Substring
        char *next = strstr(cp, by);
		
		//No more!
        if(next == NULL)
		{
			ls_copy->str = strdup(cp);
			free(ls_copy->next);
			ls_copy->next = NULL;
			i++;
			break;
		}
		
		//More!
        *next = '\0';
		ls_copy->str = strdup(cp);
		
		//Increment Stuff
        cp = next;
        cp += by_len;
		ls_prev = ls_copy;
		ls_copy = ls_copy->next;
		i++;
	}
	
	//Finish it Off
	if(!i)
	{
		ls->str = strdup(s);
		ls->next = NULL;
	}
	
	free(s);
	return ls;
}

//Replace Plus Signs with Spaces
void plus2space(char *s)
{
	while(*s)
	{
		if(*s == '+')
			*s = ' ';
		s++;
	}
}

//Urldecode
char *urldecode(const char *src)
{
	char *decoded = coal_alloc(strlen(src) + 1);
	char *dst = decoded;
	char a, b;
	while (*src)
	{
		if((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b)))
		{
			if (a >= 'a')
				a -= 'a'-'A';
			if (a >= 'A')
				a -= ('A' - 10);
			else
				a -= '0';
			if (b >= 'a')
				b -= 'a'-'A';
			if (b >= 'A')
				b -= ('A' - 10);
			else
				b -= '0';
			*dst++ = 16*a+b;
			src+=3;
		}
		
		else
		{
			*dst++ = *src++;
		}
	}
	*dst++ = '\0';
	return decoded;
}

//Substring Function
char *substr(char *str, int start, int num)
{
    //find string length
    int len = strlen(str);

    char *result;

    //Return "" if start is longer than str
    if(start > len || (start*-1) > len)
    {
        result = strdup("");
        return result;
    }

    //Setting pointer to appropriate position
    if(start >= 0)
    {
        str += start;
    }
    else
    {
        str += (len+start);
    }

    //Allocate space for returned substring
    result = malloc(num+1);
    char *ptr = result;

	//If no real "num" was given, get the whole string.
	if(num < 0)
	{
		num = len;
	}
	
    //Adding characters to the result
    while(num > 0 && *str)
    {
        *ptr++ = *str++;
        num--;
    }

    //Terminating character
    *ptr = '\0';

    return result;
}

//String Replace Function (Thanks to jmucchiello from SO)
// You must free the result if result is non-NULL.
char *str_replace(char *orig, char *rep, char *with, int times) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);


	ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count) {
		ins = tmp + len_rep;
	}
	
	if(times > 0)
	{
		count = min(count, times);
	}
	

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

//Integer to String
char *i2s(int i)
{
	char *s = coal_alloc(32);
	sprintf(s, "%i", i);
	return s;
}

//Float To String
char *f2s(double d)
{
	char *s = coal_alloc(64);
	sprintf(s, "%f", d);
	return s;
}

int filesize(char *name)
{
	FILE *fp = fopen(name,"rb");
	if(fp == NULL)
	{
		printf("Error: Could not open file %s\n", name);
		exit(1);
	}
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fclose(fp);
	return size;
}

char *file_get_contents(char *name)
{
	int size = filesize(name);
	FILE *fp;
	fp = fopen(name,"r");
	char *contents = coal_alloc(size);
	char c = getc(fp);
	int i;
	for(i=0;c != EOF;i++)
	{
		contents[i] = c;
		c = getc(fp);
	}
	contents[i] = '\0';
	fclose(fp);
	return contents;
}