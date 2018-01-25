


#ifndef __DYNSTRING_H__
#define __DYNSTRING_H__


struct DynString {
	int count;
	int capicity;
	char *data;
};


int dynString_init(struct DynString *str, int initSize);
int dynString_realloc(struct DynString *str);
void dynString_free(struct DynString *str);
int dynString_cat(struct DynString *str, char c);
void dynString_reset(struct DynString *str);

#endif
