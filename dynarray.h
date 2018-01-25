


#ifndef __DYNARRAY_H__
#define __DYNARRAY_H__

struct DynArray {
	int count;
	int capcity;
	void **data;
};

int dynArray_init(struct DynArray *arr, int initSize);
int dynArray_realloc(struct DynArray *arr);
int dynArray_add(struct DynArray *arr, void *data);
int dynArray_free(struct DynArray *arr);

#endif
