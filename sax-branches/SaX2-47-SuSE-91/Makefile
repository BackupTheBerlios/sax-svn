# /.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany. All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# Makefile for SaX2
# X-Server configuration program
#
# Version 4.8
# Status: Up-to-date
#
# Build init
buildroot      = /
mandir         = /usr/share/man

prefix         = ${buildroot}/usr
usr_prefix     = ${buildroot}/usr/X11R6
var_prefix     = ${buildroot}/var/cache
lib_prefix     = lib

# Variables 
SBINVZ       = ${prefix}/sbin
BINVZ        = ${usr_prefix}/bin
APIVZ        = ${usr_prefix}/${lib_prefix}/sax/api
SAXBINVZ     = ${usr_prefix}/${lib_prefix}/sax
DOCVZ        = ${usr_prefix}/${lib_prefix}/sax/doc
APIDATAVZ    = ${usr_prefix}/${lib_prefix}/sax/api/data
CDBVZ        = ${usr_prefix}/${lib_prefix}/sax/api/data/cdb
PIXVZ        = ${usr_prefix}/${lib_prefix}/sax/api/pixmaps
LANGVZ       = ${usr_prefix}/${lib_prefix}/sax/api/lang
MODULEVZ     = ${usr_prefix}/${lib_prefix}/sax/modules
DETECTVZ     = ${usr_prefix}/${lib_prefix}/sax/modules/detect
CREATEVZ     = ${usr_prefix}/${lib_prefix}/sax/modules/create
SYSPVZ       = ${usr_prefix}/${lib_prefix}/sax/sysp
MAPVZ        = ${usr_prefix}/${lib_prefix}/sax/sysp/maps
RDBMS        = ${var_prefix}/sax/sysp/rdbms
SCRIPTVZ     = ${usr_prefix}/${lib_prefix}/sax/sysp/script
SAXTOOLS     = ${usr_prefix}/${lib_prefix}/sax/tools
APITOOLS     = ${usr_prefix}/${lib_prefix}/sax/api/tools
PERLVZ       = ${usr_prefix}/${lib_prefix}/sax/perl
FILES        = ${var_prefix}/sax/files
LIBVZ        = ${usr_prefix}/${lib_prefix}
VARVZ        = ${var_prefix}/sax
LOCALEVZ     = ${prefix}/share/locale
LC           = LC_MESSAGES
PROFILE      = ${usr_prefix}/${lib_prefix}/sax/profile
FONTVZ       = ${usr_prefix}/${lib_prefix}/X11/fonts/truetype
XFINEBINVZ   = ${usr_prefix}/${lib_prefix}/xfine
XFINEPIXMAPS = ${usr_prefix}/${lib_prefix}/xfine/pixmaps

ARCH         = `arch`
PVERSION     = `rpm -q perl --queryformat '%{VERSION}' | tr -d .`

all:
	# /.../
	# building config parser and system profiler...
	# ---------------------------------------------
	${MAKE} -C ./parse all
	${MAKE} -C ./sysp  all

	# building GUI and tools...
	# -------------------------
	( cd api && ./.make )
	${MAKE} -C ./api all
	${MAKE} -C ./tools all

	# building XFine2...
	# ------------------
	( cd xfine && ./.make )
	${MAKE} -C ./xfine all

	# building perl stuff...
	# ----------------------
	for i in `ls -1 ./perl | grep "tar.gz" | sed -e s@.tar.gz@@`; do \
		( cd perl && pkgmake extract $$i && cd $$i && perl Makefile.PL ) ;\
	done

	
install:
	install -d -m 755 ${BINVZ} ${APIVZ} ${APIDATAVZ}
	install -d -m 755 ${CDBVZ} ${LANGVZ} ${PIXVZ} ${DOCVZ}
	install -d -m 755 ${MODULEVZ} ${SYSPVZ} ${MAPVZ} ${FILES} ${DETECTVZ}
	install -d -m 755 ${RDBMS} ${SCRIPTVZ} ${SAXTOOLS} ${LIBVZ} ${PERLVZ}
	install -d -m 755 ${PROFILE} ${SBINVZ} ${FONTVZ} ${VARVZ} ${CREATEVZ}
	install -d -m 755 ${APITOOLS} ${XFINEBINVZ} ${XFINEPIXMAPS}

	# install perl stuff...
	# ---------------------
	for i in `ls -1 ./perl | grep "tar.gz" | sed -e s@.tar.gz@@`; do \
		if [ "${PVERSION}" -ge 581 ];then \
		( \
        cd ./perl/$$i && make DESTDIR=${buildroot} install_vendor \
        ) ;\
		else \
		( \
		cd ./perl/$$i && make PREFIX=${buildroot}/usr \
		BUILDROOTPREFIX=${buildroot} \
		INSTALLMAN1DIR=${buildroot}/${mandir}/man1 \
		INSTALLMAN3DIR=${buildroot}/${mandir}/man3 install \
		) ;\
		fi \
	done

	# install NLS support...
	# -----------------------
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

	# install files...
	# -----------------
	for i in `find ./profile -type f`; do \
		echo $$i | grep -q .sh$			&& \
		install -m 755 $$i ${PROFILE}	|| \
		install -m 644 $$i ${PROFILE}	;\
	done
	install -m 755 ./init.pl                                ${SAXBINVZ}
	install -m 644 ./svnbuild                               ${SAXBINVZ}
	install -m 755 ./xc.pl                                  ${SAXBINVZ}
	install -m 755 ./tools/xwrapper/xw                      ${SAXBINVZ}/xw.pl
	install -m 755 ./api/xapi                               ${SAXBINVZ}
	install -m 755 ./api/tools/*                            ${APITOOLS}
	install -m 755 ./api/xrun.pl                            ${SAXBINVZ}
	install -m 755 ./startup/pci.pl                         ${SAXBINVZ}
	install -m 755 ./sysp/sysp.pl                           ${SAXBINVZ}
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
	install -m 755 ./startup/sax.sh                         ${BINVZ}
	install -m 755 ./startup/sax2-vesa                      ${BINVZ}
	install -m 755 ./startup/SaX2                           ${BINVZ}
	install -m 644 ./sysp/maps/Identity.map                 ${MAPVZ}
	install -m 644 ./sysp/maps/Keyboard.map                 ${MAPVZ}
	install -m 644 ./sysp/maps/Vendor.map                   ${MAPVZ}
	install -m 755 ./sysp/script/*                          ${SCRIPTVZ}
	install -m 755 ./parse/blib/arch/auto/XFree/XFree.so    ${LIBVZ}
	install -m 755 ./tools/plog/blib/arch/auto/PLog/PLog.so ${LIBVZ}

	# install API data files...
	# --------------------------
	for i in `find ./api/data -maxdepth 1 -name "*"`; do \
	if [ -f $$i ];then \
		install -m 644 $$i  ${APIDATAVZ} ;\
	fi \
	done

	# install tools...
	# ------------------
	${MAKE} -C ./tools install \
		buildroot=${buildroot} lib_prefix=${lib_prefix}

	# install XFine2...
	# -------------------
	install -m 644 ./xfine/pixmaps/*                        ${XFINEPIXMAPS}
	install -m 755 ./xfine/xfine                            ${XFINEBINVZ}
	install -m 644 ./xfine/xfine.gtx                        ${XFINEBINVZ}

	@echo "remember to have fun..."
	@echo "your SuSE team :-)"
