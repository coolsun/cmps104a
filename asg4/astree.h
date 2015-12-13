
//Rahul Patel - rpate3 && Sejal Chadha - schadha2

#ifndef __ASTREE_H__
#define __ASTREE_H__
#include <string>
#include <vector>
using namespace std;
#include "auxlib.h"
#include "symtable.h"

struct astree {
   int symbol;               
   size_t filenr;            
   size_t linenr;            
   size_t offset;            
   SymbolTable* blockpt;
   const string* lexinfo;    
   vector<astree*> children; 
};


astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo);
astree* adopt1 (astree* root, astree* child);
astree* adopt2 (astree* root, astree* left, astree* right);
astree* adopt1sym (astree* root, astree* child, int symbol);


void dump_astree (FILE* outfile, astree* root);
void dump_astree_new (FILE* outfile, astree* root);
void scan_tree (astree* root, SymbolTable* symtab);
string type_check (astree* root, SymbolTable* symtab);
astree* func_lookup(astree* root);
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);
void free_ast (astree* tree);
void free_ast2 (astree* tree1, astree* tree2);

#endif
