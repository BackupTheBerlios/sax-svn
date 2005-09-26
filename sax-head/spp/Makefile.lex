#/.../
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <ms@suse.de>, 2000
# Generate plog objects used for libsysp
#
all:spp

swig:spp.o spp.tab.o lex.yy.o

spp:spp.tab.o lex.yy.o spp.o example.o
	g++ -Wall -O2 -o example example.o spp.o spp.tab.o lex.yy.o \
		-L/usr/lib/qt3/lib -I/usr/lib/qt3/include -lfl -lreadline -lqt-mt

example.o:example.cpp
	g++ -Wall -O2 -fpic -c example.cpp -I. -I/usr/lib/qt3/include

spp.o:spp.cpp spp.h
	g++ -Wall -O2 -fpic -c spp.cpp -I. -I/usr/lib/qt3/include

spp.tab.o: spp.tab.cc lex.yy.c
	g++ -Wall -O2 -fpic -c spp.tab.cc -I. -I/usr/lib/qt3/include

lex.yy.o: lex.yy.c
	g++ -Wall -O2 -fpic -c lex.yy.c -I. -I/usr/lib/qt3/include

spp.tab.cc: spp.yc
	bison -d spp.yc
	cat spp.tab.cc | sed -e s@yyerrlab1:@@ > spp.tab.cc.new
	mv spp.tab.cc.new spp.tab.cc

lex.yy.c: spp.l spp.tab.cc
	flex -i spp.l

clean:
	rm -f lex.yy.c
	rm -f spp.tab.hc
	rm -f spp.tab.cc
	rm -f *.o
	rm -f example

