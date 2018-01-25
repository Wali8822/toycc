


#include "token.h"
#include "dynstring.h"
#include "dynarray.h"
#include "hash.h"
#include "stream.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char ch;
int token;
struct TkWord *tk_hashtable[MAXKEY];
struct DynArray tktable;
struct DynString tkstr;

struct TkWord *tkword_direct_insert(struct TkWord *tp) {
	int keyno;

	dynArray_add(&tktable, tp);
	keyno = elf_hash(tp->spelling);
	tp->next = tk_hashtable[keyno];
	tk_hashtable[keyno] = tp;
	return tp;
}

struct TkWord *tkword_find(char *spelling, int keyno) {
	struct TkWord *itr = NULL;

	for (itr = tk_hashtable[keyno]; itr; itr = itr->next){
		if (strcmp(spelling, itr->spelling) == 0) {
			return itr;
		}
	}

	return NULL;
}

struct TkWord *tkword_insert(char *p) {
	struct TkWord *tp;
	int keyno;
	int length;

	keyno = elf_hash(p);
	tp = tkword_find(p, keyno);
	if (!tp) {
		length = strlen(p);
		tp = calloc(1, sizeof(struct TkWord) + length + 1);
		tp->next = tk_hashtable[keyno];
		tk_hashtable[keyno] = tp;
		dynArray_add(&tktable, tp);
		tp->tkcode = tktable.count - 1;
		tp->spelling = ((char*)tp) + sizeof(struct TkWord);
		strncpy(tp->spelling, p, length);
	}

	return tp;
}

void init_lex()
{
	struct TkWord *tp;
	static struct TkWord keywords[] = {
		{ TOK_PLUS,		NULL,	"+",	NULL,	NULL},
		{ TOK_MINUS,	 NULL, "-", NULL, NULL },
		{ TOK_MULTI,	 NULL, "*", NULL, NULL },
		{ TOK_DIV,		NULL, "/", NULL, NULL },
		{ TOK_MOD,		NULL, "%", NULL, NULL },
		{ TOK_EQ,			NULL, "==", NULL, NULL },
		{ TOK_NEQ,		NULL, "!=", NULL, NULL },
		{ TOK_LT,			NULL, "<", NULL, NULL },
		{ TOK_LEQ,		NULL, "<=", NULL, NULL },
		{ TOK_GT,			NULL, ">", NULL, NULL },
		{ TOK_GEQ,		NULL, ">=", NULL, NULL },
		{ TOK_ASSIGN,	NULL, "=", NULL, NULL },
		{ TOK_POINTSTO, NULL, "->", NULL, NULL },
		{ TOK_DOT,		NULL, ".", NULL, NULL },
		{ TOK_AND,		NULL, "&", NULL, NULL },
		{ TOK_OR, NULL, "|", NULL, NULL },
		{ TOK_CROSS_OR, NULL, "||", NULL, NULL },
		{ TOK_OPENPA,	NULL, "(", NULL, NULL },
		{ TOK_CLOSEPA,NULL, ")", NULL, NULL },
		{ TOK_OPENBR,	NULL, "[", NULL, NULL },
		{ TOK_CLOSEBR,NULL, "]", NULL, NULL },
		{ TOK_BEGIN,	NULL, "{", NULL, NULL },
		{ TOK_END,		NULL, "}", NULL, NULL },
		{ TOK_SEMICOLON, NULL, ";", NULL, NULL },
		{ TOK_COMMA,	NULL, ",", NULL, NULL },
		{ TOK_ELLIPSIS, NULL, "...", NULL, NULL },
		{ TOK_EOF,		NULL, "End_Of_File", NULL, NULL },
		{ TK_CINT,		NULL, "ConstInteger", NULL, NULL },
		{ TK_CCHAR,		NULL, "ConstChar", NULL, NULL },
		{ TK_CSTR,		NULL, "ConstString", NULL, NULL },
		
		{ KW_CHAR,		NULL, "char", NULL, NULL },
		{ KW_SHORT,		NULL, "short", NULL, NULL },
		{ KW_INT,			NULL, "int", NULL, NULL },
		{ KW_VOID,		NULL, "void", NULL, NULL },
		{ KW_STRUCT,	NULL, "struct", NULL, NULL },
		{ KW_IF,      NULL, "if", NULL, NULL },
		{ KW_ELSE,    NULL, "else", NULL, NULL },
		{ KW_FOR,     NULL, "for", NULL, NULL },
		{ KW_CONTINUE,NULL, "continue", NULL, NULL },
		{ KW_BREAK,   NULL, "break", NULL, NULL },
		{ KW_RETURN,  NULL, "return", NULL, NULL },
		{ KW_SIZEOF,  NULL, "sizeof", NULL, NULL },
		
		{ KW_ALIGN,   NULL, "__align", NULL, NULL },
		{ KW_CDECL,   NULL, "__cdecl", NULL, NULL },
		{ KW_STDCALL, NULL, "__stdcall", NULL, NULL },
		
		{NULL, NULL, NULL, NULL, NULL}
	};

	dynString_init(&tkstr, 8);
	dynArray_init(&tktable, 8);

	for (tp = keywords; tp->spelling != NULL; ++tp)
		tkword_direct_insert(tp);
}

void preprocess() {
	while (1)
	{
		while (isspace(ch)){
			getch();
		}

		if (ch != '/') { break; }

		getch();

		if (ch == '/') {
			while (ch != '\n'){
				getch();
			}
		} else if (ch == '*') {
			getch();
			while (1) {
				if (ch == '*') {
					getch();
					if (ch == '/') {
						getch();
						break;
					}
					else {
						continue;
					}
				}

				getch();
			}
		} else {
			ungetch();
			ch = '/';
			break;
		}
	}
}

void parse_identifier() {
	dynString_reset(&tkstr);

	while (isalnum(ch) || ch == '_') {
		dynString_cat(&tkstr, ch);

		getch();
	}

	tkstr.data[tkstr.count] = '\0';
}

void parse_number() {
	dynString_reset(&tkstr);

	while (ch >= '0' && ch <= '9')
	{
		dynString_cat(&tkstr, ch);
		getch();
	}

	tkstr.data[tkstr.count] = '\0';
}

void parse_string(char sep) {
	dynString_reset(&tkstr);

	dynString_cat(&tkstr, sep);

	for (;;) {
		getch();

		if (ch == sep)
			break;

		dynString_cat(&tkstr, ch);
	}

	dynString_cat(&tkstr, sep);
	dynString_cat(&tkstr, '\0');

	getch();
}

void get_token()
{
	preprocess();

	switch (ch)
	{
	case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':
	case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':
	case 'w':case 'x':case 'y':case 'z':case 'A':case 'B':case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
	case 'L':case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W':case 'X': case 'Y': case 'Z':case '_':
	{
				 struct TkWord *tp;
				 parse_identifier();
				 tp = tkword_insert(tkstr.data);
				 token = tp->tkcode;
				 break;
	}
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
	{
				 parse_number();
				 token = TK_CINT;
				 break;
	}
	case '+':
	{
				getch();
				token = TOK_PLUS;
				break;
	}
	case '-':
	{
				getch();
				if (ch == '>') {
					getch();
					token = TOK_POINTSTO;
				} else {
					token = TOK_MINUS;
				}
				break;
	}
	case '*':
	{
				getch();
				token = TOK_MULTI;
				break;
	}
	case '/':
	{
				getch();
				token = TOK_DIV;
				break;
	}
	case '%':
	{
				getch();
				token = TOK_MOD;
				break;
	}
	case '=':
	{
				getch();
				if (ch == '=') {
					getch();
					token = TOK_EQ;
				} else {
					token = TOK_ASSIGN;
				}
				break;
	}
	case '!':
	{
				getch();
				if (ch == '=') {
					getch();
					token == TOK_NEQ;
				} else {
					error("! not support now\n");
				}
				break;
	}
	case '<':
	{
				getch();
				if (ch == '=') {
					getch();
					token = TOK_LEQ;
				} else {
					token = TOK_LT;
				}
				break;
	}
	case '>':
	{
				getch();
				if (ch == '=') {
					getch();
					token = TOK_GEQ;
				} else {
					token = TOK_GT;
				}
				break;
	}
	case '.':
	{
				getch();
				if (ch == '.')
				{
					getch();
					if (ch == '.') {
						token = TOK_ELLIPSIS;
					} else {
						error("...");
					}
					getch();
				} else {
					token = TOK_DOT;
				}
	}
	case '&':
	{
				getch();
				token = TOK_AND;
				break;
	}
	case '|':
	{
				getch();
				if (ch == '|') {
					token = TOK_CROSS_OR;
					getch();
				} else {
					token = TOK_OR;
				}
				break;
	}
	case ';':
	{
				getch();
				token = TOK_SEMICOLON;
				break;
	}
	case ']':
	{
				getch();
				token = TOK_CLOSEBR;
				break;
	}
	case '[':
	{
				getch();
				token = TOK_OPENBR;
				break;
	}
	case ')':
	{
				getch();
				token = TOK_CLOSEPA;
				break;
	}
	case '(':
	{
				getch();
				token = TOK_OPENPA;
				break;
	}
	case '{':
	{
				getch();
				token = TOK_BEGIN;
				break;
	}
	case '}':
	{
				getch();
				token = TOK_END;
				break;
	}
	case ',':
	{
				getch();
				token = TOK_COMMA;
				break;
	}
	case '\'':
	{
				 parse_string(ch);
				 token = TK_CCHAR;
				 break;
	}
	case '\"':
	{
				 parse_string(ch);
				 token = TK_CSTR;
				 break;
	}
	case TOK_EOF:
	{
				token = TOK_EOF;
				break;
	}
	default:
	{
			   getch();
			   error("unknow character");
	}
	}
}
