# /.../
# Copyright (c) 2002 SuSE GmbH Nuernberg, Germany. All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2002
#
# Makefile for SaX2 tools area
# X-Server configuration program
#
# Version 7.1
# Status: Up-to-date
#
#====================================
# Variable setup...
#------------------------------------
buildroot   = /
sax_tools   = ${buildroot}/sbin

CFLAGS      = -Wall
VERSION     = 7.1

export

#====================================
# Build
#------------------------------------
all:configure build

configure:
	echo "SUBDIRS=" > Makefile.am
	for i in `ls -1`; do \
	if ([ -d ./$$i ] && [ -f ./$$i/Makefile.am ]);then \
		echo "$$i " >> Makefile.am ;\
	fi \
	done
	echo "AC_INIT()" > configure.in
	echo "AM_INIT_AUTOMAKE(tools,${VERSION})" >> configure.in
	echo "AC_PROG_CXX" >> configure.in
	echo "AC_PROG_CC"  >> configure.in
	echo "AM_CONFIG_HEADER(configure.h)" >> configure.in
	echo "AC_OUTPUT( \\" >> configure.in
	for i in `ls -1`; do \
	if ([ -d ./$$i ] && [ -f ./$$i/Makefile.am ]);then \
		echo -n "$$i/Makefile " >> configure.in ;\
	fi \
	done
	echo ")" >> configure.in

build: configure
	aclocal
	autoconf
	autoheader
	automake --add-missing
	./configure
	automake
	${MAKE} -C ./plog -f Makefile.Linux
	for i in `ls -1`; do \
	if ([ -d ./$$i ] && [ $$i != ".svn" ] && [ $$i != "autom4te.cache" ]);then \
		${MAKE} -C ./$$i all ;\
	fi \
	done
	${MAKE} -C ./xkbset xkbctrl
	${MAKE} -C ./xmset xupdate

#====================================
# Clean sweep
#------------------------------------
clean:
	for i in `ls -1`; do \
	if [ -d ./$$i ];then \
		${MAKE} -C ./$$i clean ;\
		if [ $$i != "plog" ];then \
			rm -f ./$$i/Makefile ; rm -f ./$$i/Makefile.in ;\
			rm -rf ./$$i/.deps ;\
		fi \
	fi \
	done
	${MAKE} -C ./plog -f Makefile.Linux clean
	rm -f whois/whois
	rm -f xkbset/xkbctrl
	rm -f xkbset/xkbctrl.c
	rm -f xmset/xupdate
	rm -f xmset/xupdate.c
	rm -f configure.in
	rm -f Makefile.am
	rm -f aclocal.m4
	rm -f config.log
	rm -f config.status
	rm -f configure
	rm -f configure.h
	rm -f configure.h.in
	rm -f depcomp install-sh missing mkinstalldirs
	rm -rf autom4te.cache
	rm -f compile
	rm -f stamp*

#====================================
# Install
#------------------------------------
install:
	install -d -m 755 ${sax_tools}
	for i in `find -type f -perm 755 | grep -v libX | grep -v plog.pl`;do \
		install -m 755 $$i ${sax_tools} ;\
	done
	install -m 644 ./xbanner/demo   ${sax_tools}
	install -m 644 ./xbanner/screen ${sax_tools}
	mv ${sax_tools}/isax.pl ${sax_tools}/isax
	mv ${sax_tools}/xkbctrl.pl ${sax_tools}/xkbctrl
	mv ${sax_tools}/xupdate.pl ${sax_tools}/xupdate
	${MAKE} -C ./plog DESTDIR=${buildroot} install_vendor
	rm -f ${sax_tools}/config.status
	rm -f ${sax_tools}/configure
	rm -f ${sax_tools}/xw
	rm -f ${sax_tools}/prepare
	rm -f ${sax_tools}/PLog.so
	rm -f ${sax_tools}/check_ddc.sh
	rm -f ${sax_tools}/check_loader.sh
	rm -f ${sax_tools}/check_wheel.sh
