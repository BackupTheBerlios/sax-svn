#/.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# Makefile to build XFree86 log file parser
# --
# PLog.pm
# 
# Version 4.8
# Status: Up-to-date
#
# Build init
buildroot      = /
lib_prefix     = lib
usr_prefix     = ${buildroot}/usr/X11R6

# Variables
LIBDIR         = ${usr_prefix}/${lib_prefix}/
MODDIR         = ${usr_prefix}/${lib_prefix}/sax/modules/
CC             = g++
LIB            = ../../sysp/lib/libsysp.a -lfl

all:
	cp Makefile Makefile.Linux
	@echo "compiling C stuff (plog)..."
	${MAKE} -C ../../sysp/lib all
	${CC} -Wall -O2 plog.c -o plog ${LIB}

	@echo "running swig..."
	swig -perl5 PLog.i

	@echo "generating Makefile..."
	perl Makefile.PL CC=${CC}

	@echo "compiling PLog perl stuff..."
	make

	rm -f *.o
	rm -f pm_to_blib
	rm -f PLog_wrap.*
	rm -f PLog.bs
	rm -f Makefile
	mv Makefile.Linux Makefile

install:
	install -d -m 755 ${LIBDIR} ${MODDIR}

	install -m 755 ./blib/arch/auto/PLog/PLog.so        ${LIBDIR}
	install -m 644 ./PLog.pm                            ${MODDIR}
	install -m 644 ./PLogData.pm                        ${MODDIR}

clean:
	rm -f  ./plog
	rm -rf ./blib
