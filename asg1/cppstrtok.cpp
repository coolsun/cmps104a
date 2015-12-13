// $Id: cppstrtok.cpp,v 1.3 2014-10-07 18:09:11-07 - - $

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

//Rahul Patel - rpate3 && Sejal Chadha - schadha2


#define __OCLIB_OH__ 

#include <string>
using namespace std;
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include<fstream>
#include <wait.h>
#include "stringset.h"
#include "auxlib.h"

const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}

// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, filename);
      if (sscanf_rc == 2) {
         printf ("DIRECTIVE: line %d file \"%s\"\n", linenr, filename);
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
        intern_stringset(token);
      }
      ++linenr;
   }
}



int main (int argc, char** argv) {

 set_execname (argv[0]);
   bool flag = false;
   string str;
    for(;;){
        int option = getopt(argc, argv, "D:@:l:y");
        if(option == EOF) break;
        switch(option){
            case '@': set_debugflags(optarg) ;
                break;
            case 'D':
		flag = true; 
		str = " -D" + string(optarg);
                break;
           case 'l':
              //  DEBUGF('o', "option set to l");
                break;
            case 'y':
              // DEBUGF('o', "option set to y");
                break;
            case '?': errprintf("%: invalid option (%c)\n", optopt);
                break;
 	   default: break;
                
        }
    }
    
   for (int argi = 1; argi < argc; ++argi) {
      char *filename = argv[argi];
       set_execname(filename);
       string basename = get_execname();
       string out = basename + "str";
       string command;
	if(flag == false){
     	  command = CPP + " " + filename;
	}
	if(flag == true){
	   command = CPP + str + " " + filename; 
	} 
      FILE *pipe = popen (command.c_str(), "r");
      if (pipe == NULL) {
         syserrprintf (command.c_str());
      }else {
         cpplines (pipe, filename);
         int pclose_rc = pclose (pipe);
         eprint_status (command.c_str(), pclose_rc);
         if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);
	}
	size_t d = out.find_first_of('.'); 
	out.erase(d+1,2);	
	FILE *outfile = fopen(out.c_str(), "w"); 
	dump_stringset(outfile);	
        fclose(outfile);	
   }
   return get_exitstatus();
}

