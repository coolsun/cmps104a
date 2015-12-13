// $Id: cppstrtok.cpp,v 1.3 2014-10-07 18:09:11-07 - - $
//
//
// //Rahul Patel - rpate3 && Sejal Chadha - schadha2


#define __OCLIB_OH__ 

#include <string>
using namespace std;
/*#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>*/

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
#include "lyutils.h"
#include "auxlib.h"
#include "stringset.h"
#include "astree.h"


const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;

// Chomp the last character from a buffer if it is delim.
void chomp(char *string, char delim){
    size_t len = strlen(string);
    if(len == 0) return;
    char *nlpos = string + len -1;
    if(*nlpos == delim) *nlpos = '\0';
}

// Run cpp against the lines of the file.
void cpplines(FILE *pipe, char *filename){
    int linenr = 1;
    char inputname[LINESIZE];
    strcpy(inputname, filename);
    for(;;){
        char buffer[LINESIZE];
        char* fgets_rc = fgets(buffer, LINESIZE, pipe);
        if(fgets_rc == NULL) break;
        chomp (buffer, '\n');
        int sscanf_rc = sscanf(buffer, "# %d \"%[^\"]\"",
                               &linenr, filename);
        if(sscanf_rc == 2){ 
//printf ("DIRECTIVE: line %d file \"%s\"\n", linenr, filename);
         continue;
}
        char *savepos = NULL;
        char *bufptr = buffer;
        for(int tokenct = 1;; ++tokenct){
            char *token = strtok_r (bufptr, " \t\n", &savepos);
            bufptr = NULL;
            if(token == NULL) break;
            intern_stringset(token);
        }
        ++linenr;
    }
}


int main(int argc, char **argv){
    
    set_execname(argv[0]);
    bool flag = false;
    string str;
    extern int yydebug; 
     yydebug = 0; 
    extern int  yy_flex_debug;
    yy_flex_debug = 0;
    extern FILE* tokfile;
    int option = 0;
/* for(;;)*/ 

while((option = getopt(argc, argv, "yl:@:D:")) != -1){
//int  option = getopt(argc, argv, "yl:@:D:");
//if(option == EOF) break;
      switch(option){
case'@':
                set_debugflags(optarg);
                break;
case'D':
flag = true;
str = optind - 1;
//str =  " -D" + string(optarg);
 break;
case'l':
                yy_flex_debug = 1;
     //   DEBUGF('o', "option set to l");
                break;
            case'y':
	yydebug = 1;
// DEBUGF('o', "option set to y");
                break;
            case '?': errprintf("%: invalid option (%c)\n", optopt);
                break;
 default: 
 fprintf(stderr, "invalid file\n");
                set_exitstatus(1);
break;
        }
   }
    
    for(int argi = 1; argi < argc; ++argi){
        char *filename = argv[argi];
        char output[520];
        char strout[512];
	char tokout[512];
	char astout[512];
        strcpy(output, basename(filename));
        char *dot = strrchr(output, '.');
	 if(dot == NULL){
          //  fprintf(stderr, "invalid file type\n");
            set_exitstatus(1);
            continue;
         }
         int last = dot - output;
         if(dot != NULL && 
            strcmp(&output[last], ".oc\0") != 0){
         //   fprintf(stderr,"invalid file type\n");
            set_exitstatus(1);
            continue;
         }
       // int last = dot - output;
        output[last] = '\0';
        strcpy(strout, output);
        strcpy(tokout, output);
	strcpy(astout, output);
        strcat(strout, ".str"); 
        strcat(tokout, ".tok");
        strcat(astout, ".ast");
	string command;

if(flag == false){
command = CPP + " " + filename;
}
if(flag == true){
command = CPP + str + " " + filename; 
} 
        yyin = popen(command.c_str(), "r");
        FILE *strfile = fopen(strout, "w");
        tokfile = fopen(tokout, "w");
if(yyin == NULL){
            syserrprintf(command.c_str());
        }else{
            cpplines (yyin, filename);
            int pclose_rc = pclose(yyin);
            eprint_status(command.c_str(), pclose_rc);
if(pclose_rc != 0) set_exitstatus(1);
        }
        dump_stringset(strfile); 
        fclose(strfile);
        yyin = popen(command.c_str(), "r");
        if(yyin == NULL){
            syserrprintf(command.c_str());
        }else{
            for(;;){
                int yylex_rt = yylex();
                if(yylex_rt == YYEOF)   break;
            }
           int pclose_rc = pclose(yyin);
            eprint_status(command.c_str(), pclose_rc);
if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);
}
	fclose(tokfile); 
	FILE* astoutfile = fopen(astout, "w");
	dump_astree_new (astoutfile, yyparse_astree); 
	fclose(astoutfile); 
	

    }
    return get_exitstatus();
    
}


