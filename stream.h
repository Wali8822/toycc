


#ifndef __STREAM_H__
#define __STREAM_H__


struct IOStream {
	char *buffer;

	int pos;
	int size;
	int capacity;
};


int iostream_init(struct IOStream *stream, const char *fileName);
void iostream_release(struct IOStream *stream);

void getch();
void ungetch();

extern struct IOStream stream;

#endif

