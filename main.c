
#include "token.h"
#include "stream.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>


struct IOStream stream;


int iostream_init(struct IOStream *stream, const char *fileName)
{
	int size;
	struct stat fs;
	FILE *f = fopen(fileName, "r");
	if (!f)
		return ENFILE;

	fstat(_fileno(f), &fs);
	size = fs.st_size;

	if (size < 0) {
		goto STREAM_INIT_ERROR;
	}

	stream->buffer = malloc(size);
	memset(stream->buffer, 0, size);

	stream->size = fread(stream->buffer, size, 1, f);
	stream->pos = 0;

	stream->capacity = size;

STREAM_INIT_ERROR:
	fclose(f);
}

void iostream_release(struct IOStream *stream)
{
	if (stream->buffer)
		free(stream->buffer);
}

void getch()
{
	if (stream.pos < stream.capacity)
	{
		ch = stream.buffer[stream.pos++];
	}
	else
	{
		ch = TOK_EOF;
	}
}

void ungetch()
{
	if (stream.pos > 0)
		--stream.pos;
}

int type_specifier() {
	int type_found = 0;

	get_token();
	
	switch (token)
	{
	case KW_INT:
	{
				   type_found = 1;
				   get_token();
				   break;
	}
	case KW_CHAR:
	{
					type_found = 1;
					get_token();
					break;
	}
	case KW_SHORT:
	{
					 type_found = 1;
					 get_token();
					 break;
	}
	case KW_VOID:
	{
					type_found = 1;
					get_token();
					break;
	}
	}

	return type_found;
}

void external_declaration() {
	if (!type_specifier()) {
		error("expect type special");
	}

	if (token == TOK_SEMICOLON) {
		get_token();
		return;
	}

	while (1) {

	}
}

void translation_unit() {
	while (token != TOK_EOF)
	{
		external_declaration();
	}
}

int main(int argc, char *argv) {
	init_lex();
	iostream_init(&stream, "D:\\test.c");

	getch();
	translation_unit();

	return 0;
}
