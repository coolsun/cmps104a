//Rahul Patel - rpate3 && Sejal Chadha - schadha2

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdio.h>

#include <string>
#include <vector>
#include <map>

using namespace std;

class SymbolTable {

  int num;

  SymbolTable* parent;

  map<string,string> mapping;
  map<string,int> filenrs;
  map<string,int> linenrs;
  map<string,int> offsets;
  map<string,string> structs;
  map<string,SymbolTable*> subscopes;

public:
  SymbolTable(SymbolTable* parent);

  SymbolTable* inBlock();

  SymbolTable* inFunction(string name,
                             string signature, int filenr, int linenr, int offset);

  void addSymbol(string name, string type, int filenr, int linenr, int offset);

  void dumpsym(FILE* symfile, int depth);
  string lookup(string name);

  string parent_func(SymbolTable* innerScope);

  static int N;

  static vector<string> sig(string signature);
};

#endif
