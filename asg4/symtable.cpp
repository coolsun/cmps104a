//Rahul Patel - rpate3 && Sejal Chadha - schadha2 

#include "auxlib.h"
#include "symtable.h"

SymbolTable::SymbolTable(SymbolTable* parent) {
  this->parent = parent;
  this->num = SymbolTable::N++;
}

SymbolTable* SymbolTable::inBlock() {
  SymbolTable* child = new SymbolTable(this);
  char buff[10];
  sprintf(&buff[0], "%d", child->num);
  this->subscopes[buff] = child;
  return child;
}

SymbolTable* SymbolTable::inFunction(string id, string signature,
	int filenr, int linenr, int offset ) {
  this->addSymbol(id, signature, filenr, linenr, offset);
  SymbolTable* child = new SymbolTable(this);
  this->subscopes[id] = child;
  return child;
}

void SymbolTable::addSymbol(string id, string type, 
int filenr, int linenr, int offset) {
  this->mapping[id] = type;
  this->filenrs[id] = filenr;
  this->linenrs[id] = linenr;
  this->offsets[id] = offset;
}


string SymbolTable::lookup(string id) {
  if (this->mapping.count(id) > 0) {
    return this->mapping[id];
  }
  if (this->parent != NULL) {
    return this->parent->lookup(id);
  } else {
    errprintf("Invalid: %s\n", id.c_str());
    return "";
  }
}



void SymbolTable::dumpsym(FILE* symout, int depth) {
  std::map<string,string>::iterator i;
  for (i = this->mapping.begin(); i != this->mapping.end(); ++i) {
    const char* id = i->first.c_str();
    const char* type = i->second.c_str();
    fprintf(symout, "%*s%s (%d.%d.%d) {%d} %s\n", 3*depth, "", 
	id,this->filenrs[id],this->linenrs[id],this->offsets[id], 
	this->num, type);
    if (this->subscopes.count(id) > 0) {
      this->subscopes[id]->dumpsym(symout, depth + 1);
    }
  }
  std::map<string,SymbolTable*>::iterator i1;
  for (i1 = this->subscopes.begin(); i1 != this->subscopes.end(); 
	++i1) {
    if (this->mapping.count(i1->first) < 1) {
      i1->second->dumpsym(symout, depth + 1);
    }
  }
}


string SymbolTable::parent_func(SymbolTable* inner) {
  std::map<string,SymbolTable*>::iterator i;
  for (i = this->subscopes.begin(); i != this->subscopes.end(); ++i) {
    if (i->second == inner && this->mapping.count(i->first) > 0) {
      return this->mapping[i->first];
    }
  }
  if (this->parent != NULL) {
    return this->parent->parent_func(this);
  }
  errprintf("missing function\n");
  return "";
}

int SymbolTable::N(0);

vector<string> SymbolTable::sig(string str) {
  vector<string> out;
  size_t left = str.find_first_of('(');
  if (left == string::npos) {
    errprintf("%s invalid\n", str.c_str());
    return out;
  }
  out.push_back(str.substr(0, left));
  for (size_t i = left + 1; 
i < str.length()-1; i = str.find_first_of(",)", i) + 1) {
    out.push_back(str.substr(i, str.find_first_of(",)", i) - i));
  }
  return out;
}

