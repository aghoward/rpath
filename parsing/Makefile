GCC=g++
AR=ar
FLAGS=--std=c++17 -I../ -I../cdif -L../files/lib
AR_FLAGS=rcs

objdir=./obj
libdir=./lib
objects=${objdir}/elfheaderfactory.o ${objdir}/programheaderfactory.o ${objdir}/sectionheaderfactory.o ${objdir}/elfparser.o ${objdir}/factoriesmodule.o
libs=${libdir}/libfactories.a

all: ${objdir} ${objects} ${libdir} ${libs}

${objdir}:
	if [[ ! -d ${objdir} ]]; then mkdir ${objdir}; fi;

${libdir}:
	if [[ ! -d ${libdir} ]]; then mkdir ${libdir}; fi;

clean:
	rm -f ${objects}
	rm -Rf ${objdir} ${libdir}

${objdir}/%.o: %.cc %.h ${objdir}
	$(GCC) ${FLAGS} -c $< -o $@ ${externallinks}

${libdir}/lib%.a: ${objects} 
	$(AR) ${AR_FLAGS} $@ $?
