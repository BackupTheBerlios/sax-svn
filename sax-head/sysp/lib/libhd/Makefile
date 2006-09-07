TOPDIR		= $(CURDIR)
SUBDIRS		= hd ids x86emu int10
TARGETS		= hwinfo.pc
CLEANFILES	= hwinfo.pc libhd.a
LIBDIR		= /usr/lib
ULIBDIR		= $(LIBDIR)
LIBS		= -lhd
SLIBS		= -lhd -ldbus-1 -lhal
TLIBS		= -lhd_tiny -ldbus-1 -lhal
SO_LIBS		= -ldbus-1 -lhal
TSO_LIBS	= -ldbus-1 -lhal

export SO_LIBS

include Makefile.common

SHARED_FLAGS	=
OBJS_NO_TINY	= names.o parallel.o modem.o

.PHONY:	fullstatic static shared tiny doc diet tinydiet uc tinyuc

hwinfo.pc: hwinfo.pc.in
	VERSION=`cat VERSION`; \
	sed -e "s,@VERSION@,$${VERSION},g" -e 's,@LIBDIR@,$(ULIBDIR),g;s,@LIBS@,$(LIBS),g' $< > $@.tmp && mv $@.tmp $@
