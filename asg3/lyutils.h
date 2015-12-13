//Rahul Patel - rpate3 && Sejal Chadha - schadha2



#ifndef __LYUTILS_H__
#define __LYUTILS_H__

// Lex and Yacc interface utility.

#include <stdio.h>

#include "astree.h"
#include "auxlib.h"
//#include "yyparse.h"
#define YYEOF 0

extern FILE* yyin;
extern FILE* tokfile; 
extern astree* yyparse_astree;
extern int yyin_linenr;
extern char* yytext;
extern int yy_flex_debug;
extern int yydebug;
extern int yyleng;

int yylex (void);
//void yyrestart(FILE* file);
int yyparse (void);
void yyerror (const char* message);
int yylex_destroy (void);
const char* get_yytname (int symbol);
bool is_defined_token (int symbol);

const string* scanner_filename (int filenr);
void scanner_newfilename (const char* filename);
void scanner_badchar (unsigned char bad);
void scanner_badident (char* lexeme);
void scanner_badstring(char* lexeme);
void scanner_badtoken (char* lexeme);
extern void scanner_newline (void);
void scanner_setecho (bool echoflag);
void scanner_useraction (void);

astree* new_parseroot (void);
int yylval_token (int symbol);

extern void scanner_include (void);

typedef astree* astree_pointer;
#define YYSTYPE astree_pointer
#include "yyparse.h"

//RCSH("$Id: lyutils.h,v 1.3 2015-04-09 17:45:26-07 - - $")
#endif
