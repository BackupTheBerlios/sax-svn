# /.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany. All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# Makefile for SaX2
# X-Server configuration program
#
# Version 7.1
# Status: Up-to-date
#
# Build init
buildroot      = /
mandir         = /usr/share/man

#============================================
# Prefixs...
#--------------------------------------------
usr_prefix     = ${buildroot}/usr
sax_prefix     = ${buildroot}/usr/share/sax
xfi_prefix     = ${buildroot}/usr/share/xfine
var_prefix     = ${buildroot}/var/cache
lib_prefix     = ${buildroot}/usr/lib
doc_prefix     = ${buildroot}/usr/share/doc/packages
man_prefix     = ${buildroot}/usr/share/man

#============================================
# Variables... 
#--------------------------------------------
MANVZ          = ${man_prefix}/man1
PACKDOCVZ      = ${doc_prefix}/sax2
SBINVZ         = ${buildroot}/sbin
SAXBINVZ       = ${sax_prefix}
APIVZ          = ${sax_prefix}/api
APIDATAVZ      = ${sax_prefix}/api/data
CDBVZ          = ${sax_prefix}/api/data/cdb
PIXVZ          = ${sax_prefix}/api/pixmaps
LANGVZ         = ${sax_prefix}/api/lang
DOCVZ          = ${sax_prefix}/doc
MODULEVZ       = ${sax_prefix}/modules
DETECTVZ       = ${sax_prefix}/modules/detect
CREATEVZ       = ${sax_prefix}/modules/create
SYSPVZ         = ${sax_prefix}/sysp
MAPVZ          = ${sax_prefix}/sysp/maps
PROFILE        = ${sax_prefix}/profile
SAXCACHEVZ     = ${var_prefix}/sax
XFICACHEVZ     = ${var_prefix}/xfine
RDBMS          = ${var_prefix}/sax/sysp/rdbms
FILES          = ${var_prefix}/sax/files
SCRIPTVZ       = ${sax_prefix}/sysp/script
SAXTOOLS       = ${sax_prefix}/tools
APITOOLS       = ${sax_prefix}/api/tools
USRLIBVZ       = ${lib_prefix}
INCVZ          = ${usr_prefix}/include/sax
LOCALEVZ       = ${usr_prefix}/share/locale
XFINEBINVZ     = ${xfi_prefix}
XFINEPIXMAPS   = ${xfi_prefix}/pixmaps
LIBDOCDIR      = ${usr_prefix}/share/doc/packages/libsax
WMVZ           = ${usr_prefix}/X11R6/share/fvwm

#============================================
# Helpers
#--------------------------------------------
ARCH           = `arch`
PVERSION       = `rpm -q perl --queryformat '%{VERSION}' | tr -d .`
LC             = LC_MESSAGES

all:
	#============================================
	# building config parser and system profiler
	#--------------------------------------------
	${MAKE} -C ./parse -f Makefile.Linux all
	${MAKE} -C ./parse all
	${MAKE} -C ./sysp  all

	#============================================
	# building libsax and bindings...
	#--------------------------------------------
	( cd libsax && doxygen sax.dox )
	${MAKE} -C ./libsax all
	( cd libsax && \
	for i in `ls -1 ./bindings`; do \
		if [ -d ./bindings/$$i ];then \
			( cd ./bindings/$$i && make -f Makefile.swig && make ) ;\
		fi \
	done )

	#============================================
	# building GUI and tools...
	#--------------------------------------------
	( cd api && ./.make )
	${MAKE} -C ./api all
	${MAKE} -C ./tools all

	#============================================
	# building XFine2...
	#--------------------------------------------
	( cd xfine && ./.make )
	${MAKE} -C ./xfine all

install:
	#============================================
	# Install base directories
	#--------------------------------------------
	install -d -m 755 ${APIVZ} ${APIDATAVZ} ${USRLIBVZ}
	install -d -m 755 ${CDBVZ} ${LANGVZ} ${PIXVZ} ${DOCVZ} ${INCVZ}
	install -d -m 755 ${MODULEVZ} ${SYSPVZ} ${MAPVZ} ${FILES} ${DETECTVZ}
	install -d -m 755 ${RDBMS} ${SCRIPTVZ} ${SAXTOOLS} ${LIBVZ}
	install -d -m 755 ${PROFILE} ${SBINVZ} ${SAXCACHEVZ} ${XFICACHEVZ}
	install -d -m 755 ${CREATEVZ} ${APITOOLS} ${XFINEBINVZ} ${XFINEPIXMAPS}
	install -d -m 755 ${LIBDOCDIR} ${WMVZ}

	#============================================
	# install X11 parser...
	#--------------------------------------------
	( cd parse && make DESTDIR=${buildroot} install_vendor )

	#============================================
	# install language bindings...
	#--------------------------------------------
	( cd libsax && \
	for i in `ls -1 ./bindings`; do \
		if [ -d ./bindings/$$i ];then \
			( cd ./bindings/$$i && make DESTDIR=${buildroot} install_vendor ) ;\
		fi \
	done )

	#============================================
	# install NLS support (translations)...
	#--------------------------------------------
	( cd ./locale && ./.locale )
	for i in `ls -1 ./locale`; do \
		if [ -d ./locale/$$i ];then \
		if [ ! "$$i" = "sax-help" ] && [ ! "$$i" == "sax-template" ];then \
		install -d -m 755 ${LOCALEVZ}/$$i/${LC} ;\
		( cd ./locale/$$i/${LC}  && msgfmt -o sax.mo sax.po ) ;\
		install -m 644 ./locale/$$i/${LC}/sax.mo   ${LOCALEVZ}/$$i/${LC} ;\
		fi \
		fi \
	done

	#============================================
	# install core files...
	#--------------------------------------------
	for i in `find ./profile -type f`; do \
		echo $$i | grep -q .sh$			&& \
		install -m 755 $$i ${PROFILE}	|| \
		install -m 644 $$i ${PROFILE}	;\
	done
	install -m 755 ./init.pl                                ${SAXBINVZ}
	install -m 644 ./svnbuild                               ${SAXBINVZ}
	install -m 755 ./xc.pl                                  ${SAXBINVZ}
	install -m 755 ./tools/xwrapper/xw                      ${SBINVZ}
	install -m 755 ./api/xapi                               ${SBINVZ}
	install -m 755 ./api/tools/*                            ${APITOOLS}
	install -m 755 ./api/xrun.pl                            ${SAXBINVZ}
	install -m 755 ./startup/pci.pl                         ${SAXBINVZ}
	install -m 755 ./sysp/sysp                              ${SBINVZ}
	install -m 644 ./doc/database/config                    ${DOCVZ}
	install -m 755 ./doc/database/guitest                   ${DOCVZ}
	install -m 644 ./doc/database/README                    ${DOCVZ}
	install -m 644 ./modules/*.pm                           ${MODULEVZ}
	install -m 644 ./modules/detect/*                       ${DETECTVZ}
	install -m 644 ./modules/create/*                       ${CREATEVZ}
	install -m 644 ./modules/export/*                       ${MODULEVZ}
	install -m 644 ./parse/XFree.pm                         ${MODULEVZ}
	install -m 644 ./api/data/cdb/*                         ${CDBVZ}
	install -m 644 ./api/pixmaps/*                          ${PIXVZ}
	install -m 755 ./startup/sax.sh                         ${SBINVZ}
	install -m 755 ./startup/sax2-vesa                      ${SBINVZ}
	install -m 755 ./startup/SaX2                           ${SBINVZ}
	install -m 644 ./sysp/maps/Identity.map                 ${MAPVZ}
	install -m 644 ./sysp/maps/Keyboard.map                 ${MAPVZ}
	install -m 644 ./sysp/maps/Vendor.map                   ${MAPVZ}
	install -m 644 ./sysp/maps/Input.map                    ${MAPVZ}
	install -m 644 ./sysp/maps/Driver.map                   ${MAPVZ}
	install -m 755 ./sysp/script/*                          ${SCRIPTVZ}
	install -m 644 ./api/data/fvwmrc.sax                    ${WMVZ}

	#============================================
	# install library files...
	#--------------------------------------------
	cp -a ./libsax/doc/doxygen/html ${LIBDOCDIR}
	cp ./libsax/libsax.so*  ${USRLIBVZ}
	cp ./libsax/*.h         ${INCVZ}

	#============================================
	# install API data files...
	#--------------------------------------------
	for i in `find ./api/data -maxdepth 1 -name "*"`; do \
	if [ -f $$i ] && [ ! $$i = "./api/data/.testgtx" ];then \
		install -m 644 $$i  ${APIDATAVZ} ;\
	fi \
	done

	#============================================
	# install tools...
	#--------------------------------------------
	${MAKE} -C ./tools install \
		buildroot=${buildroot} sax_prefix=${sax_prefix} DESTDIR=${buildroot}

	#============================================
	# install XFine2...
	#--------------------------------------------
	install -m 644 ./xfine/pixmaps/*                        ${XFINEPIXMAPS}
	install -m 755 ./xfine/xfine                            ${SBINVZ}
	install -m 644 ./xfine/xfine.gtx                        ${XFINEBINVZ}

	@echo "remember to have fun..."
	@echo "your SuSE team :-)"

install-docs:
	#============================================
	# install documentation...
	#--------------------------------------------
	install -d -m 755 ${PACKDOCVZ} ${MANVZ}

	#============================================
	# SaX2 system draft and LICENSE
	#--------------------------------------------
	install -m 644 ./LICENSE           ${PACKDOCVZ}
	install -m 644 ./doc/README        ${PACKDOCVZ}
	install -m 644 ./doc/en/compiled/* ${PACKDOCVZ}
	install -m 644 ./doc/de/compiled/* ${PACKDOCVZ}

	#============================================
	# SaX2 tools manual pages
	#--------------------------------------------
	for i in `ls -1 ./doc/man`;do \
		install -m 644 ./doc/man/$$i ${MANVZ}/$$i.1 ;\
	done
