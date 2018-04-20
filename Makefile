GCC=g++
CXX_FLAGS=--std=c++17 -I./ -I./cdif -L./files/lib -L./parsing/lib
LD_FLAGS=-pthread -lfactories -lfiles -lstdc++fs

all: libfiles libparsing main

lib%: ./%
	$(MAKE) -C $<

cleanlib%: ./%
	$(MAKE) clean -C $<

clean: cleanlibfiles cleanlibparsing
	rm -f main

main: main.cc libfiles libparsing
	$(GCC) ${CXX_FLAGS} -o $@ $< ${LD_FLAGS}

