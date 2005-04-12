#/.../
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <ms@suse.de>, 2000
# Generate plog objects used for libsysp
#

CC       = g++
CFLAGS   = -O2 -Wall -fpic
LIBS     = -lfl
YACC     = yacc
FLEX     = flex -i

SRC      = plog.cc parse.cc
OBJ      = y.tab.o plog.o parse.o 

# build...
# ---------
all:${OBJ}

parse.cc:
	@echo "parse.c is not a target for me... skip"

y.tab.o: y.tab.c lex.yy.c
	${CC} -c -fpic y.tab.c -I.

y.tab.c:  parse.y
	$(YACC) parse.y

lex.yy.c: parse.l
	$(FLEX) parse.l

plog.o:plog.cc ../syslib.h
	${CC} ${CFLAGS} -c plog.cc

parse.o:parse.cc ../syslib.h
	${CC} ${CFLAGS} -c parse.cc


clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f *.o

