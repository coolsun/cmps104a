
//Rahul Patel - rpate3 && Sejal Chadha - schadha2


#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astree.h"
#include "stringset.h"
#include "lyutils.h"
#include "symtable.h"

SymbolTable* struct_table = new SymbolTable(NULL);

astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo) {

   astree* tree = new astree();
   tree->symbol = symbol;
   tree->filenr = filenr;
   tree->linenr = linenr;
   tree->offset = offset;
   tree->lexinfo = intern_stringset (lexinfo);
   DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
           tree, tree->filenr, tree->linenr, tree->offset,
           get_yytname (tree->symbol), tree->lexinfo->c_str());
   return tree;
}
astree* adopt1 (astree* root, astree* child1) {
   root->children.push_back (child1);
   DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
           root, root->lexinfo->c_str(),
           child1, child1->lexinfo->c_str());
   return root;
}
astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}
astree* adopt1sym (astree* root, astree* child1, int symbol) {
   root = adopt1 (root, child1);
   root->symbol = symbol;
   return root;
}
astree* change_sys(astree* root, int symbol){
   root->symbol = symbol;
   return root;
}
static void dump_node (FILE* outfile, astree* node) {
   fprintf (outfile, "%3ld  %ld.%03ld  %-3d  %-15s (%s)  ",
            node->filenr, node->linenr, node->offset, node->symbol,
            get_yytname (node->symbol), node->lexinfo->c_str()); 
   bool need_space = false;
   for (size_t child1 = 0; child1 < node->children.size();
        ++child1) {
      if (need_space) fprintf (outfile, " ");
      need_space = true;
      fprintf (outfile, "%p", node->children.at(child1));
   }
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   dump_node (outfile, root);
   fprintf (outfile, "\n");
   for (size_t child1 = 0; child1 < root->children.size();
        ++child1) {
      dump_astree_rec (outfile, root->children[child1],
                       depth + 1);
   }
}

static void dump_astree_rec_new (FILE* outfile, astree* root, 
			int depth) {
   if (root == NULL) return;
   int i = 0;
      while (i < depth){
         fprintf(outfile, "|   ");
         i++;
      }
      fprintf(outfile, "%s", get_yytname(root->symbol));
      if(strcmp(root->lexinfo->c_str(), "") != 0)
         fprintf(outfile, " (%s)", root->lexinfo->c_str());
      fprintf(outfile, "\n");
   for (size_t child1 = 0; child1 < root->children.size(); ++child1){
      dump_astree_rec_new (outfile, root->children[child1], depth +1);
   }
}
void dump_astree (FILE* outfile, astree* root) {
   dump_astree_rec (outfile, root, 0);
   fflush (NULL);
}
void dump_astree_new(FILE* outfile, astree*root) {
   dump_astree_rec_new (outfile, root, 0);
   fflush (NULL);
}
void scan_tree(astree* root, SymbolTable* symtab){
   if(root == NULL) return;
   for(size_t child1 = 0; child1 < root->children.size(); ++child1){
      const char* cursym = get_yytname(root->children[child1]->symbol);
if(strcmp(cursym, "vardecl") != 0 && strcmp(cursym, "block") != 0 && 
strcmp(cursym, "function") != 0 && strcmp(cursym, "struct_") != 0)
scan_tree(root->children[child1], symtab);
else if(strcmp(cursym, "vardecl") == 0){                         
if(root->children[child1]->children[0]->children[1] == NULL){
symtab->addSymbol(root->children[child1]->children[1]->lexinfo->c_str(),
root->children[child1]->children[0]->children[0]->children[0]->
lexinfo->c_str(),
	root->children[child1]->children[1]->filenr,
	root->children[child1]->children[1]->linenr,
	root->children[child1]->children[1]->offset); 
}else{
string var = root->children[child1]->children[0]->children[0]->
children[0]->lexinfo->c_str();
            var = var + "[]";
            symtab->addSymbol(root->children[child1]->
children[1]->lexinfo->c_str(),
                              var,
      root->children[child1]->children[1]->filenr,
      root->children[child1]->children[1]->linenr,
      root->children[child1]->children[1]->offset);

         }
      }else if (strcmp(cursym, "struct_") == 0){
         string table_id;
         for(size_t child2 = 0; 
child2 < root->children[child1]->children.size(); ++child2){
  string curchild = get_yytname(root->children[child1]->
		children[child2]->symbol);
            if(curchild == "TOK_IDENT"){
               table_id = root->children[child1]->
		children[child2]->lexinfo->c_str();
            }
         }
         for(size_t child2 = 0; 
child2 < root->children[child1]->children.size(); ++child2){
            string curchild = get_yytname(root->
		children[child1]->children[child2]->symbol);
            if(curchild == "decl"){
               struct_table->addSymbol(table_id, 
root->children[child1]->children[child2]->children[1]->lexinfo->c_str(),
     root->children[child1]->children[child2]->children[1]->filenr,
     root->children[child1]->children[child2]->children[1]->linenr,
     root->children[child1]->children[child2]->children[1]->offset);
            }
         }
      }else if(strcmp(cursym, "block") == 0){     
         scan_tree(root->children[child1], 
root->children[child1]->blockpt = symtab->inBlock()); 
      }else if(strcmp(cursym, "function") == 0){     
         char ident[50];
         char typedecl[100];
         int num_blk;
         int numdecl = 0;
         int funcfilenr = 0;
         int funclinenr = 0;
         int funcoffset = 0;
         for(size_t child2 = 0; 
child2 < root->children[child1]->children.size(); ++child2){
            const char* curinsym = 
get_yytname(root->children[child1]->children[child2]->symbol);
            if(strcmp(curinsym, "TOK_IDENT") == 0){   
	funcfilenr = root->children[child1]->children[child2]->filenr;
	funclinenr = root->children[child1]->children[child2]->linenr;
	funcoffset = root->children[child1]->children[child2]->offset;
	strcpy(ident, root->children[child1]->
		children[child2]->lexinfo->c_str());
            }
            if(strcmp(curinsym, "type") == 0){    
strcpy(typedecl, root->children[child1]->children[child2]->
		children[0]->children[0]->lexinfo->c_str());
               strcat(typedecl, "(");
            }
            if(strcmp(curinsym, "block") == 0)
               num_blk = child2;
         }
         for(size_t child2 = 0; 
child2 < root->children[child1]->children.size(); ++child2){  
if(strcmp(get_yytname(root->children[child1]->
   children[child2]->symbol), "decl") == 0){
               if(numdecl != 0)
                  strcat(typedecl, ",");
strcat(typedecl,root->children[child1]->children[child2]->children[0]->
		children[0]->children[0]->lexinfo->c_str());
               ++numdecl;
            }
         }
         strcat(typedecl, ")");
SymbolTable* funcsym = symtab->inFunction(ident, typedecl, 
				funcfilenr, funclinenr, funcoffset);
         for(size_t child2 = 0; 
child2 < root->children[child1]->children.size(); ++child2){ 
if(strcmp(get_yytname(root->children[child1]->
	children[child2]->symbol), "decl") == 0)
funcsym->addSymbol(root->children[child1]->children[child2]->
	children[1]->lexinfo->c_str(),
root->children[child1]->children[child2]->children[0]->
	children[0]->children[0]->lexinfo->c_str(),
root->children[child1]->children[child2]->children[1]->filenr,
root->children[child1]->children[child2]->children[1]->linenr,
root->children[child1]->children[child2]->children[1]->offset);
         }
scan_tree(root->children[child1]->children[num_blk], 
root->children[child1]->children[num_blk]->blockpt = funcsym);
    }
   }

}

astree* func_lookup(astree* root){
  if(root == NULL) return NULL;
  astree* returned;
  for(size_t child = 0; child < root->children.size(); ++child){
     string cur_sym = get_yytname(root->children[child]->symbol);
     if(cur_sym == "return_"){

       return root->children[child];
     }
  }
  for(size_t child = 0; child < root->children.size(); ++child){
     if((returned = func_lookup(root->children[child])) != NULL){
        return returned; 
     } 
  }
  return NULL;

}



void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
   if (is_defined_token (toknum)) {
      dump_node (outfile, yyvaluep);
   }else {
      fprintf (outfile, "%s(%d)\n",
               get_yytname (toknum), toknum);
   }
   fflush (NULL);
}


void free_ast (astree* root) {
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
           root, root->filenr, root->linenr, root->offset,
           get_yytname (root->symbol), root->lexinfo->c_str());
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}


