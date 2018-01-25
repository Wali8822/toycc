


#ifndef __TOKEN_H__
#define __TOKEN_H__


enum Tokens
{
	TOK_PLUS,		//+
	TOK_MINUS,		//-
	TOK_MULTI,		//*
	TOK_DIV,		// /
	TOK_MOD,		// %
	TOK_EQ,			// ==
	TOK_NEQ,		// !=
	TOK_LT,			// <
	TOK_LEQ,		// <=
	TOK_GT,			// >
	TOK_GEQ,		// >=
	TOK_ASSIGN,		// =
	TOK_POINTSTO,	// ->
	TOK_DOT,		// .
	TOK_AND,		// &
	TOK_OR,			// |
	TOK_CROSS_OR,	// ||
	TOK_OPENPA,		// (
	TOK_CLOSEPA,	// )
	TOK_OPENBR,		// [
	TOK_CLOSEBR,	// ]
	TOK_BEGIN,		// {
	TOK_END,		// }
	TOK_SEMICOLON,	// ;
	TOK_COMMA,		// ,
	TOK_ELLIPSIS,	// ...
	TOK_EOF,		// end of file

	TK_CINT,		// const int
	TK_CCHAR,		// const char
	TK_CSTR,		// const string

	KW_CHAR,		// key word char
	KW_SHORT,		// key word short
	KW_INT,			// key word int
	KW_VOID,		// key word void
	KW_STRUCT,		// key word struct 
	KW_IF,
	KW_ELSE,
	KW_FOR,
	KW_CONTINUE,
	KW_BREAK,
	KW_RETURN,
	KW_SIZEOF,

	KW_ALIGN,
	KW_CDECL,
	KW_STDCALL,

	TK_IDENT
};


struct Symbol;

struct TkWord {
	int tkcode;
	struct TkWord *next;
	char *spelling;
	struct Symbol *sym_struct;
	struct Symbol *sym_identifier;
};

#define MAXKEY	1024

extern char ch;
extern int token;


void init_lex();
void get_token();

#endif
