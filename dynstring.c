


#include "dynstring.h"

#include <stdlib.h>
#include <errno.h>


int dynString_init(struct DynString *str, int initSize)
{
	if (initSize <= 0) initSize = 8;

	str->data = malloc(initSize);
	if (!str->data)
		return ENOMEM;

	str->count = 0;
	str->capicity = initSize;

	return 0;
}

int dynString_realloc(struct DynString *str)
{
	int newCapcity = str->capicity;
	char *data;

	while (newCapcity <= str->capicity)
	{
		newCapcity = newCapcity * 2;
	}

	data = realloc(str->data, newCapcity);
	if (!data)
		return ENOMEM;

	str->data = data;
	str->capicity = newCapcity;

	return 0;
}

void dynString_free(struct DynString *str)
{
	if (str && str->data) {
		free(str->data);
		str->data = NULL;
		str->capicity = 0;
		str->count = 0;
	}
}

int dynString_cat(struct DynString *str, char c)
{
	if (str->count + 1 > str->capicity) {
		if (dynString_realloc(str))
			return ENOMEM;
	}

	str->data[str->count] = c;
	str->count = str->count + 1;

	return 0;
}

void dynString_reset(struct DynString *str)
{
	str->count = 0;
	memset(str->data, 0, str->capicity);
}

