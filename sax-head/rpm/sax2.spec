#
# spec file for package sax2 (Version 4.7)
#
# Copyright (c) 2002 SuSE Linux AG, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# please send bugfixes or comments to feedback@suse.de.
#

# neededforbuild  flex yacc hwinfo hwinfo-devel mesa-devel-packages mesasoft x-devel-packages XFree86-driver-options qt3-devel-packages swig readline-devel freetype2 freetype2-devel XFree86-server

Name:         sax2
Requires:     perl xbanner perl-gettext XFree86-server fbset saxident saxtools qt3 fvwm2
%if %{suse_version} >= 801
PreReq:       /bin/rm /bin/mkdir /usr/bin/chroot %fillup_prereq %insserv_prereq
%endif
Summary:      SuSE advanced X-Configuration (XFree86 4.x)
Version:      4.7
Release:      60
Group:        System/X11/Utilities
License:      LGPL, Other License(s), see package
Source:       sax2.tar.bz2
Source1:      sax2_pixmaps.tar.bz2
Source2:      sax2_nvidia.tar.bz2
NoSource:     2
%if %{suse_version} < 801
Patch:        sax2_binmode.dif
%endif
Patch1:       sax2_monitor.dif
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
%ifarch ppc sparc sparc64
Provides:     sax
Obsoletes:    sax
%endif
%ifarch sparc sparc64
Requires:     sax2_sbus
%endif

%description
SuSE advanced X-Configuration (XFree86 4.x)

Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n saxtools
Version:      2.2
Release:      569
Summary:      X11 tools for SaX2
Group:        System/X11/Utilities
Requires:     saxident

%description -n saxtools
Some small X11 tools to handle input devices,
for example, mouse and keyboard devices.

Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n saxident
Version:      1.1
Release:      304
Summary:      SaX2 Identitiy and Profile information
Group:        System/X11/Utilities
Provides:     sax2:/usr/X11R6/lib/sax/sysp/maps/Identity.map
Provides:     saxtools:/usr/X11R6/lib/sax/sysp/maps/Identity.map

%description -n saxident
This package hold the information about the supported graphics hardware and
its special parameters. For some graphics card a profile is needed to describe
configuration parameters outside the ordinary way of setting up the card with SaX2

Authors:
--------
    Marcus Schäfer <ms@suse.de>

%prep
%setup -n sax -a 1
if [ -f $RPM_SOURCE_DIR/sax2_nvidia.tar.bz2 ];then
	tar -xvjf $RPM_SOURCE_DIR/sax2_nvidia.tar.bz2
	if [ ! -d /usr/X11R6/%{_lib}/modules/drivers/ ];then
		mkdir -p /usr/X11R6/%{_lib}/modules/drivers/
	fi
	cp nvidia_drv.o /usr/X11R6/%{_lib}/modules/drivers/
fi
%if %{suse_version} < 801
%patch
%endif
%patch1
#=================================================
# adapt lib environment to currently used %_{lib}
#-------------------------------------------------
for i in `find -type f | grep -v "/\."`;do
(
	grep -I "lib\/" $i 2>/dev/null >/dev/null
	if [ $? = 0 ];then
	head -n 1 $i | grep "^#!" >/dev/null 2>/dev/null
	if [ $? = 0 ];then
		cat $i | \
			sed -e 's@/usr/lib\/@/usr/%{_lib}\/@g' \
		> $i.sed; mv $i.sed $i
		cat $i | \
			sed -e 's@/usr/X11R6/lib\/@/usr/X11R6/%{_lib}\/@g' \
		> $i.sed; mv $i.sed $i
		chmod 755 $i
	fi
	fi
)
done
for i in `find -regex ".*\.\(h\|pm\)"`;do
(
	grep -I "lib\/" $i 2>/dev/null >/dev/null
	if [ $? = 0 ];then
		cat $i | \
			sed -e 's@/usr/lib\/@/usr/%{_lib}\/@g' \
		> $i.sed; mv $i.sed $i
		cat $i | \
			sed -e 's@/usr/X11R6/lib\/@/usr/X11R6/%{_lib}\/@g' \
		> $i.sed; mv $i.sed $i
	fi
)
done

%build
test -e /.buildenv && . /.buildenv
#=================================================
# if SuSE 7.0 change parse.c not to use dev_screen
#-------------------------------------------------
%if %{suse_version} <= 700
	cat ./parse/parse.c | \
	sed -e s@"#if 1"@"#if 0"@ > ./parse/parse.c.new
	mv ./parse/parse.c.new ./parse/parse.c
%endif
#=================================================
# patch files.pm according to misc extension
#-------------------------------------------------
%define isSecureMisc %(./modules/create/.patch && echo "0" || echo "1"; exit 0)
%if ! %{isSecureMisc}
	cat ./modules/create/files.pm | \
	sed -e s@miscExitCode=1@miscExitCode=0@ > ./modules/create/files.pm.new
	mv ./modules/create/files.pm.new ./modules/create/files.pm
%endif
#=================================================
# add SuSE version to sax.sh script...
#-------------------------------------------------
cat ./startup/sax.sh | \
	sed -e s@SuSE-Linux@"$BUILD_DISTRIBUTION_NAME"@ \
> /tmp/sax.sh
cp /tmp/sax.sh ./startup/sax.sh
#=================================================
# build sources
#-------------------------------------------------
make

%preun
chroot . rm -f /var/cache/xfine/*
if [ ! -d /var/cache/xfine ];then
 mkdir -p /var/cache/xfine
fi
#=================================================
# install sources
#-------------------------------------------------

%install
rm -rf $RPM_BUILD_ROOT
export ASK_FOR_CHANGELOG=no
export TEMPLATE_CHECK=no
export ARCH=`/bin/arch`
make buildroot=$RPM_BUILD_ROOT lib_prefix=%{_lib} install
# install tools for the saxtools subpackage
# ------------------------------------------
(
 cd $RPM_BUILD_ROOT/usr/X11R6/%{_lib}/sax/tools
 install -m 755 * $RPM_BUILD_ROOT/usr/X11R6/bin/
)
# create startup link
# --------------------
( 
 rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/sax2 && \
    cd $RPM_BUILD_ROOT/usr/X11R6/bin && ln -s SaX2 sax2
)
# create xfine directory
# -----------------------
if [ ! -d $RPM_BUILD_ROOT/var/cache/xfine ];then
 mkdir -p $RPM_BUILD_ROOT/var/cache/xfine
fi
# create sysp link
# -----------------
(
 rm -f $RPM_BUILD_ROOT/usr/sbin/sysp
 cd $RPM_BUILD_ROOT/usr/sbin && \
    ln -s /usr/X11R6/%{_lib}/sax/sysp.pl sysp
)
# check for the xf86 options file at /etc/X11/CardModules...
# ------------------------------------------------------------
%ifarch ix86
if [ -f "/usr/X11R6/bin/xsload" ];then
	mv /usr/X11R6/lib/modules/drivers/ati_drv.o /tmp
	mv /usr/X11R6/lib/modules/drivers/radeon_drv.o /tmp
	mv /usr/X11R6/lib/modules/drivers/atimisc_drv.o /tmp
	mv /usr/X11R6/lib/modules/drivers/r128_drv.o /tmp
	/usr/X11R6/bin/xsload > /tmp/CardModules
	cat /etc/X11/CardModules.addon >> /tmp/CardModules
	cp /tmp/CardModules \
	$RPM_BUILD_ROOT/usr/X11R6/%{_lib}/sax/api/data/CardModules
	mv /tmp/ati_drv.o /usr/X11R6/lib/modules/drivers/
	mv /tmp/radeon_drv.o /usr/X11R6/lib/modules/drivers/
	mv /tmp/atimisc_drv.o /usr/X11R6/lib/modules/drivers/
	mv /tmp/r128_drv.o /usr/X11R6/lib/modules/drivers/
fi
%endif
# install docs
# -------------
if [ ! -d $RPM_BUILD_ROOT%{_defaultdocdir}/sax2 ];then
	mkdir -p $RPM_BUILD_ROOT%{_defaultdocdir}/sax2
fi
install -m 644 ./doc/LICENCE $RPM_BUILD_ROOT%{_defaultdocdir}/sax2
install -m 644 ./doc/README  $RPM_BUILD_ROOT%{_defaultdocdir}/sax2
install -m 644 ./doc/en/compiled/* $RPM_BUILD_ROOT%{_defaultdocdir}/sax2
install -m 644 ./doc/de/compiled/* $RPM_BUILD_ROOT%{_defaultdocdir}/sax2
# install architecture dependant Identity
# ---------------------------------------
if [ -f ./sysp/maps/arch/Identity.map.$ARCH ];then
	install -m 644 ./sysp/maps/arch/Identity.map.$ARCH \
	$RPM_BUILD_ROOT/usr/X11R6/%{_lib}/sax/sysp/maps/Identity.map
fi
# transform Identitiy information to Cards file
# -----------------------------------------------
./.cards.pl -f \
	$RPM_BUILD_ROOT/usr/X11R6/%{_lib}/sax/sysp/maps/Identity.map \
> $RPM_BUILD_ROOT/usr/X11R6/%{_lib}/sax/api/data/cdb/Cards
# install saxtools manual pages...
# ---------------------------------
install -d -m 755 $RPM_BUILD_ROOT/usr/X11R6/man
install -d -m 755 $RPM_BUILD_ROOT/usr/X11R6/man/man1
if [ -d "$RPM_BUILD_ROOT/usr/X11R6/man" ];then
	for i in `ls -1 ./doc/man`;do
	install -m 644 ./doc/man/$i $RPM_BUILD_ROOT/usr/X11R6/man/man1/$i.1
	done
fi
# install fvwm config file...
# ----------------------------
if [ ! -d $RPM_BUILD_ROOT/usr/X11R6/share/fvwm ];then
	install -d -m 755 $RPM_BUILD_ROOT/usr/X11R6/share/fvwm
fi
install -m 644 api/data/fvwmrc \
	$RPM_BUILD_ROOT/usr/X11R6/share/fvwm/fvwmrc
# copy manual pages...
# ---------------------
install -d -m 755 $RPM_BUILD_ROOT/etc
install -d -m 755 $RPM_BUILD_ROOT/etc/X11
install -d -m 755 $RPM_BUILD_ROOT/usr/share
install -d -m 755 $RPM_BUILD_ROOT/usr/share/man
install -d -m 755 $RPM_BUILD_ROOT/usr/share/man/man3
if [ -d "/usr/man/man3" ];then
	for i in `ls -1 /usr/man/man3`;do
	install -m 644 /usr/man/man3/$i $RPM_BUILD_ROOT/usr/share/man/man3
	done
fi
# sysconfig variables...
# ----------------------
FILLUP_DIR=$RPM_BUILD_ROOT/var/adm/fillup-templates
mkdir -p $FILLUP_DIR
install -o root -g root ./startup/sysconfig.sax $FILLUP_DIR

%post
%{fillup_and_insserv -npY sax}

%postun
%{insserv_cleanup}

#=================================================
# SaX files...      
#-------------------------------------------------
%files
%dir /usr/X11R6/%{_lib}/sax
%dir /usr/X11R6/%{_lib}/sax/api
%dir /usr/X11R6/%{_lib}/sax/api/lang
%dir /usr/X11R6/%{_lib}/sax/api/data
%dir /usr/X11R6/%{_lib}/sax/doc
%dir /usr/X11R6/%{_lib}/sax/sysp/script
%dir /usr/X11R6/%{_lib}/sax/sysp
%dir /var/cache/sax/sysp
%dir /var/cache/sax/sysp/rdbms
%dir /var/cache/sax/files
%dir /usr/X11R6/%{_lib}/xfine
%dir /var/cache/sax
%dir /var/cache/xfine
/var/adm/fillup-templates/sysconfig.sax
/usr/X11R6/share/fvwm/fvwmrc
/usr/X11R6/%{_lib}/sax/doc/config
/usr/X11R6/%{_lib}/sax/doc/guitest
/usr/X11R6/%{_lib}/sax/doc/README
/usr/X11R6/%{_lib}/sax/svnbuild
/usr/X11R6/%{_lib}/sax/init.pl
/usr/X11R6/%{_lib}/sax/xc.pl
/usr/X11R6/%{_lib}/sax/xw.pl
/usr/X11R6/%{_lib}/sax/xapi
/usr/X11R6/%{_lib}/sax/xrun.pl
/usr/X11R6/%{_lib}/sax/pci.pl
/usr/X11R6/%{_lib}/sax/sysp.pl
/usr/X11R6/%{_lib}/sax/modules
/usr/X11R6/%{_lib}/sax/tools
/usr/X11R6/%{_lib}/sax/api/tools
/usr/X11R6/%{_lib}/sax/api/data/*????
/usr/X11R6/%{_lib}/sax/api/pixmaps
/usr/X11R6/bin/sax.sh
/usr/X11R6/bin/SaX2
/usr/X11R6/bin/sax2
/usr/X11R6/%{_lib}/xfine/pixmaps
/usr/X11R6/%{_lib}/xfine/xfine
/usr/X11R6/%{_lib}/xfine/xfine.gtx
/usr/X11R6/%{_lib}/sax/sysp/script/installed.pl
/usr/X11R6/%{_lib}/sax/sysp/script/killdot.pl
/usr/X11R6/%{_lib}/sax/sysp/script/machine.pl
/usr/X11R6/%{_lib}/sax/sysp/script/preparelog.pl
/usr/X11R6/%{_lib}/sax/sysp/script/checkmap.pl
/usr/X11R6/%{_lib}/sax/sysp/script/profilecount.pl
/usr/X11R6/%{_lib}/sax/sysp/script/diag.pl
/usr/X11R6/%{_lib}/sax/sysp/script/psection.pl
/usr/X11R6/%{_lib}/sax/sysp/script/vendor.pl
/usr/X11R6/%{_lib}/XFree.so
/usr/X11R6/%{_lib}/PLog.so
/usr/sbin/sysp
/usr/share/locale/de/LC_MESSAGES/sax.mo
/usr/share/locale/en_US/LC_MESSAGES/sax.mo
/usr/share/locale/en_GB/LC_MESSAGES/sax.mo
/usr/share/locale/lt/LC_MESSAGES/sax.mo
/usr/share/locale/hu/LC_MESSAGES/sax.mo
/usr/share/locale/da/LC_MESSAGES/sax.mo
/usr/share/locale/fr/LC_MESSAGES/sax.mo
/usr/share/locale/es/LC_MESSAGES/sax.mo
/usr/share/locale/ca/LC_MESSAGES/sax.mo
/usr/share/locale/pt/LC_MESSAGES/sax.mo
/usr/share/locale/cs/LC_MESSAGES/sax.mo
/usr/share/locale/it/LC_MESSAGES/sax.mo
/usr/share/locale/sk/LC_MESSAGES/sax.mo
/usr/share/locale/nl/LC_MESSAGES/sax.mo
/usr/share/locale/el_GR/LC_MESSAGES/sax.mo
/usr/share/locale/sl_SI/LC_MESSAGES/sax.mo
/usr/share/locale/sv/LC_MESSAGES/sax.mo
/usr/share/locale/ja/LC_MESSAGES/sax.mo
/usr/share/locale/bg/LC_MESSAGES/sax.mo
/usr/lib/perl5/site_perl/*/*/auto/Term/ReadLine/Gnu/.packlist
/usr/lib/perl5/site_perl/*/*/Term/ReadLine/Gnu.pm
/usr/lib/perl5/site_perl/*/*/Term/ReadLine/Gnu/XS.pm
/usr/lib/perl5/site_perl/*/*/auto/Term/ReadLine/Gnu/Gnu.bs
/usr/lib/perl5/site_perl/*/*/auto/Term/ReadLine/Gnu/Gnu.so
/usr/lib/perl5/site_perl/*/*/auto/Term/ReadLine/Gnu/XS/autosplit.ix
%doc %{_defaultdocdir}/sax2/LICENCE
%doc %{_defaultdocdir}/sax2/README
%doc %{_defaultdocdir}/sax2/sax.en.ps
%doc %{_defaultdocdir}/sax2/sax.en.dvi
%doc %{_defaultdocdir}/sax2/sax.de.ps
%doc %{_defaultdocdir}/sax2/sax.de.dvi
%doc %{_mandir}/man3/Term::ReadLine::Gnu.3pm.gz
#=================================================
# SaX-Tools file list...  
# ------------------------------------------------

%files -n saxtools
%doc /usr/X11R6/man/man1
/usr/X11R6/bin/xbounce
/usr/X11R6/bin/xupdate
/usr/X11R6/bin/xmset
/usr/X11R6/bin/xkbset
/usr/X11R6/bin/xkbctrl
/usr/X11R6/bin/xmode
/usr/X11R6/bin/xquery
/usr/X11R6/bin/xidle
/usr/X11R6/bin/xbound
/usr/X11R6/bin/testX
#=================================================
# SaX-Ident file list...  
# ------------------------------------------------

%files -n saxident
%dir /usr/X11R6/%{_lib}/sax
%dir /usr/X11R6/%{_lib}/sax/api
%dir /usr/X11R6/%{_lib}/sax/api/data/cdb
%dir /usr/X11R6/%{_lib}/sax/api/data
%dir /usr/X11R6/%{_lib}/sax/sysp/maps
%dir /usr/X11R6/%{_lib}/sax/sysp
/usr/X11R6/%{_lib}/sax/sysp/maps/Identity.map
/usr/X11R6/%{_lib}/sax/sysp/maps/Keyboard.map
/usr/X11R6/%{_lib}/sax/sysp/maps/Vendor.map
/usr/X11R6/%{_lib}/sax/api/data/cdb/*
/usr/X11R6/%{_lib}/sax/profile
