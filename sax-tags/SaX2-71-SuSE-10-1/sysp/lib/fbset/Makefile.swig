all: fbset.o

example: example.o fbset.o
	g++ -Wall -o example example.o fbset.o

fbset.o: fbset.cpp
	g++ -Wall -c -fpic -O2 fbset.cpp

example.o: example.cpp
	g++ -Wall -c -fpic -O2 example.cpp

swig:
	swig -Wall -c++ -perl5 FBSet.i
	perl Makefile.PL

clean:
	rm -f *.o
	rm -rf blib
	rm -f  Makefile
	rm -f  Makefile.old pm_to_blib
	rm -f FBSet.pm FBSet.bs
	rm -f FBSet_*
