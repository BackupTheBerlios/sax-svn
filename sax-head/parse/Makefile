#/.../
# Copyright (c) 2001 SuSE GmbH Nuernber, Germany.  All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# Makefile to build XF86Config parser
# --
# XFree.pm
# 
# Version 4.3
# Status: Up-to-date
#
LIBDIR = /usr/lib/perl5/5.6.0/i586-linux/auto/XFree
MODDIR = /usr/lib/perl5/5.6.0/XFree

all:
	cp Makefile Makefile.Linux
	@echo "compiling C stuff..."
	gcc -O2 parse.c -o parse -L/usr/X11R6/lib -L/usr/X11R6/lib64 \
		-I/usr/X11R6/include \
		-DX_BYTE_ORDER=X_LITTLE_ENDIAN -DNEW_INPUT -DNDEBUG \
		-lxf86config -lm

	@echo "running swig..."
	swig -perl5 XFree.i

	@echo "generating Makefile..."
	perl Makefile.PL
	touch Makefile

	@echo "compiling XFree perl stuff..."
	make

	rm -f *.o
	rm -f pm_to_blib
	rm -f XFree_wrap.*
	rm -f XFree.bs
	rm -f Makefile
	mv Makefile.Linux Makefile

install:
	install -d -m 755 ${LIBDIR} ${MODDIR}

	install -m 755 ./blib/arch/auto/XFree/XFree.so 		${LIBDIR}
	install -m 644 ./XFree.pm							${MODDIR}

clean:
	rm -rf blib/
