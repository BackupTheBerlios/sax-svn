#/.../
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <ms@suse.de>, 2000
#
# sysp Makefile to generate the SaX2 system profiler
# used for creating the SaX2 hardware registry
#

CC       = g++
CFLAGS   = -O2 -Wall -L/usr/X11R6/lib
LIBS     = -lfl -lhd -lX11 -lm

SRC      = plog/plog.c plog/parse.c
OBJ      = plog.o parse.o y.tab.o
AIM      = sysp

PLOG     = ./plog

# build...
# ---------
all:${AIM}

sysp:
	${CC} ${CFLAGS} main.c -o $@ ../lib/libsysp.a ${LIBS}

clean:
	rm -f sysp

