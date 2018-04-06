GCC=g++
LIBDIR=./lib
CXX_FLAGS=--std=c++17 -L${LIBDIR} -Wl,-rpath=${LIBDIR}

all: ${LIBDIR} main

clean:
	rm -Rf main ${LIBDIR}

${LIBDIR}:
	if [ ! -e ${LIBDIR} ]; then mkdir -p ${LIBDIR}; fi

lib%.so: %.cc %.h
	$(GCC) -fPIC -c $<
	$(GCC) -shared -o ${LIBDIR}/$@.1 funcs.o
	ln -s $@.1 ${LIBDIR}/$@

main: main.cc libfuncs.so
	$(GCC) ${CXX_FLAGS} -o $@ main.cc -lfuncs
