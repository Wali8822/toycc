

#include "dynarray.h"

#include <stdlib.h>

int dynArray_init(struct DynArray *arr, int initSize)
{
	if (initSize <= 0)
		initSize = 8;

	if (arr) {
		arr->data = malloc(sizeof(void*)* initSize);
		arr->capcity = initSize;
		arr->count = 0;
	}

	return 0;
}

int dynArray_realloc(struct DynArray *arr)
{
	int newSize = arr->capcity;
	void *data;

	while (newSize <= arr->capcity)
	{
		newSize = newSize * 2;
	}

	data = realloc(arr->data, newSize * sizeof(void*));
	if (data) {
		arr->data = data;
		arr->capcity = newSize;
	}

	return 0;
}

int dynArray_add(struct DynArray *arr, void *data)
{
	if (arr->count >= arr->capcity)
	{
		dynArray_realloc(arr);
	}

	arr->data[arr->count++] = data;

	return 0;
}

int dynArray_free(struct DynArray *arr)
{
	int i = 0;

	for (i = 0; i < arr->count; ++i) {
		if (arr->data[i])
			free(arr->data[i]);
	}

	free(arr->data);

	return 0;
}
