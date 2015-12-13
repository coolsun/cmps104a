#Rahul Patel - rpate3 && Sejal Chadha - schadha2


GCC        = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP      = g++ -MM -std=gnu++11
VALGRIND   = valgrind --leak-check=full --show-reachable=yes

MKFILE     = Makefile
DEPFILE    = Makefile.dep
NOINCL     = ci clean spotless
NEEDINCL   = ${filter ${NOINCL}, ${MAKECMDGOALS}}
CSOURCES   = astree.cpp auxlib.cpp lyutils.cpp main.cpp stringset.cpp 
SOURCES    = ${CSOURCES} ${CLGEN} ${CYGEN} 
HEADERS    = astree.h auxlib.h lyutils.h stringset.h
OBJECTS    = ${SOURCES:.cpp=.o}
EXECBIN    = oc
SRCFILES   = ${HEADERS} ${SOURCES} ${MKFILE}
SMALLFILES = ${DEPFILE} foo.oc foo1.oh foo2.oh
LSOURCES   = scanner.l
YSOURCES   = parser.y
CLGEN      = yylex.cpp
HYGEN      = yyparse.h
CYGEN      = yyparse.cpp
LREPORT    = yylex.output
YREPORT    = yyparse.output
CHECKINS   = ${SRCFILES} ${SMALLFILES}
LISTING    = Listing.ps
CLASS      = cmps104a-wm.f15
PROJECT    = asg2

#all : ${EXECBIN}

all : ${CLGEN} ${CYGEN} ${HYGEN} ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${GCC} -c $<

${CLGEN} : ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES} 2>${LREPORT}
	
${CYGEN} ${HYGEN} : ${YSOURCES}
	bison --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}



clean :
	rm *.o *.str *.tok yylex.cc yyparse.* *.out *.err *.oc *.pdf \
	*.ps *.output *.html *.lexyacctrace *.oh oc *.ast

spotless : clean
	- rm ${EXECBIN} 

deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

