#
# Linux Frame Buffer Device Configuration
#

CC =		g++ -Wall -Wno-unused -fpic -O2 -I.
YACC =		yacc -d
FLEX =		flex
INSTALL =	install
RM =		rm -f

all:		fbset.o

fbset.o:	fbset.c fbset.h fb.h

clean:
		$(RM) *.o

