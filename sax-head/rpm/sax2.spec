#
# spec file for package sax2 (Version 4.8)
#
# Copyright (c) 2004 SUSE LINUX AG, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://www.suse.de/feedback/
#

# neededforbuild  flex hwinfo hwinfo-devel qt3-devel-packages swig udev update-desktop-files x-devel-packages xorg-x11-driver-options xorg-x11-server yacc

BuildRequires: aaa_base acl attr bash bind-utils bison bzip2 coreutils cpio cpp cracklib cvs cyrus-sasl db devs diffutils e2fsprogs file filesystem fillup findutils flex gawk gdbm-devel glibc glibc-devel glibc-locale gpm grep groff gzip info insserv less libacl libattr libgcc libselinux libstdc++ libxcrypt libzio m4 make man mktemp module-init-tools ncurses ncurses-devel net-tools netcfg openldap2-client openssl pam pam-modules patch permissions popt procinfo procps psmisc pwdutils rcs readline sed strace syslogd sysvinit tar tcpd texinfo timezone unzip util-linux vim zlib zlib-devel autoconf automake binutils expat fontconfig fontconfig-devel freeglut freeglut-devel freetype2 freetype2-devel gcc gcc-c++ gdbm gettext hwinfo hwinfo-devel libjpeg liblcms liblcms-devel libmng libmng-devel libpng libpng-devel libstdc++-devel libtool perl qt3 qt3-devel readline-devel rpm swig udev update-desktop-files xorg-x11-Mesa xorg-x11-Mesa-devel xorg-x11-devel xorg-x11-driver-options xorg-x11-libs xorg-x11-server

Name:         sax2
Requires:     perl perl-gettext fbset sax2-ident perl-TermReadLine-Gnu
%if %{suse_version} >= 810
PreReq:       /bin/rm /bin/mkdir /usr/bin/chroot %fillup_prereq %insserv_prereq
%endif
%if %{suse_version} > 810
%if %{suse_version} > 910
Requires:     xorg-x11-server
%else
Requires:     XFree86-server
%endif
%else
Requires:     xloader
%endif
Summary:      SuSE advanced X Window System-configuration
Version:      4.8
Release:      117
Group:        System/X11/Utilities
License:      Other License(s), see package, GPL
Source:       sax2.tar.bz2
Source1:      sax2_pixmaps.tar.bz2
Source2:      sax2_nvidia.tar.bz2
Source3:      sax2.desktop
NoSource:     2
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
%if %{suse_version} > 820
Provides:     sax xfine
Obsoletes:    sax xfine
%endif
%ifarch sparc sparc64
Requires:     sax2_sbus
%endif

%description
This package contains the SuSE Advanced X-Configuration



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-tools
Version:      2.3
Release:      11
Summary:      X Window System tools for SaX2
Group:        System/X11/Utilities
Requires:     sax2-ident
Provides:     saxtools
Obsoletes:    saxtools

%description -n sax2-tools
Some small X Window System tools to handle input devices, for example,
mouse and keyboard.



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-ident
Version:      1.2
Release:      11
Summary:      SaX2 identity and profile information
Group:        System/X11/Utilities
Provides:     sax2:/usr/X11R6/lib/sax/sysp/maps/Identity.map
Provides:     sax2-tools:/usr/X11R6/lib/sax/sysp/maps/Identity.map
Provides:     saxident
Obsoletes:    saxident

%description -n sax2-ident
This package contains information about the supported graphics hardware
and its special parameters. For some graphics cards a profile is needed
to describe configuration parameters outside the ordinary way of
setting up the card with SaX2.



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-gui
Requires:     xbanner sax2-tools qt3 fvwm2 3ddiag tightvnc sax2
Version:      1.2
Release:      11
Requires:     sax2
Summary:      SuSE advanced X Window System-configuration GUI
Group:        System/X11/Utilities
Provides:     sax2:/usr/X11R6/%{_lib}/sax/xapi

%description -n sax2-gui
This package contains the GUI for the SuSE Advanced X-Configuration



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
# %patch
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
test -e /.buildenv && . /.buildenv

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
. modules/create/.patch.sh
if [ "$SECURE_MISC" = "no" ];then
	cat ./modules/create/files.pm | \
	sed -e s@miscExitCode=1@miscExitCode=0@ > ./modules/create/files.pm.new
	mv ./modules/create/files.pm.new ./modules/create/files.pm
fi
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
# install tools for the sax2-tools subpackage
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
%ifarch %ix86
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
install -m 644 ./LICENSE $RPM_BUILD_ROOT%{_defaultdocdir}/sax2
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
# install sax2-tools manual pages...
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
install -m 644 api/data/fvwmrc* \
	$RPM_BUILD_ROOT/usr/X11R6/share/fvwm/
# install desktop icon...
# ------------------------
if [ ! -d $RPM_BUILD_ROOT/share/pixmaps ];then
	install -d -m 755 $RPM_BUILD_ROOT/usr/share/pixmaps
fi
install -m 644 api/pixmaps/sax2.xpm \
	$RPM_BUILD_ROOT/usr/share/pixmaps/
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
#FILLUP_DIR=$RPM_BUILD_ROOT/var/adm/fillup-templates
#mkdir -p $FILLUP_DIR
#install -o root -g root ./startup/sysconfig.sax $FILLUP_DIR
# install script variants of xupdate and xkbctrl...
# --------------------------------------------------
mv $RPM_BUILD_ROOT/usr/X11R6/bin/xkbctrl.pl \
   $RPM_BUILD_ROOT/usr/X11R6/bin/xkbctrl
mv $RPM_BUILD_ROOT/usr/X11R6/bin/xupdate.pl \
   $RPM_BUILD_ROOT/usr/X11R6/bin/xupdate
# check perl .packlist...
# --------------------------
%if %{suse_version} > 820
%perl_process_packlist
%endif
# remove unpacked sources...
# --------------------------
rm -f $RPM_BUILD_ROOT/usr/X11R6/%_lib/sax/api/data/cdb/Monitors.orig
rm -f $RPM_BUILD_ROOT/usr/X11R6/share/fvwm/fvwmrc.yast2
rm -f $RPM_BUILD_ROOT/usr/X11R6/lib/sax/api/data/fvwmrc.yast2
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/check_loader.sh
rm -f $RPM_BUILD_ROOT/var/adm/perl-modules/sax2
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/SecureMode
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/SetMode
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/catch
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/check_wheel.sh
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/corner
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/demo
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/demo.sh
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/dots
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/fake
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/hwupdate
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/isax
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/screen
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/whois
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/wrap
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/xlook
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/wmstart
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/xmirror
rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/check_ddc.sh
rm -f $RPM_BUILD_ROOT/usr/X11R6/%{_lib}/sax/api/data/.testgtx
%suse_update_desktop_file -i %name System SystemSetup

%post
#%{fillup_and_insserv -npY sax}

%postun
#%{insserv_cleanup}
#=================================================
# SaX files...      
#-------------------------------------------------

%files
%defattr(-,root,root)
%dir /usr/X11R6/%{_lib}/sax/api
%dir /usr/X11R6/%{_lib}/sax/api/data
%dir /usr/share/doc/packages/sax2
%dir /usr/X11R6/share/fvwm
%dir /usr/X11R6/%{_lib}/sax
%dir /usr/X11R6/%{_lib}/sax/doc
%dir /usr/X11R6/%{_lib}/sax/sysp/script
%dir /usr/X11R6/%{_lib}/sax/sysp
%dir /var/cache/sax/sysp
%dir /var/cache/sax/sysp/rdbms
%dir /var/cache/sax/files
%dir /var/cache/sax
%dir /var/cache/xfine
#/var/adm/fillup-templates/sysconfig.sax
/usr/share/pixmaps/sax2.xpm
/usr/X11R6/share/fvwm/fvwmrc.sax
/usr/X11R6/%{_lib}/sax/doc/config
/usr/X11R6/%{_lib}/sax/doc/guitest
/usr/X11R6/%{_lib}/sax/doc/README
/usr/X11R6/%{_lib}/sax/svnbuild
/usr/X11R6/%{_lib}/sax/init.pl
/usr/X11R6/%{_lib}/sax/xc.pl
/usr/X11R6/%{_lib}/sax/pci.pl
/usr/X11R6/%{_lib}/sax/sysp.pl
/usr/X11R6/%{_lib}/sax/modules
/usr/X11R6/%{_lib}/sax/tools
/usr/X11R6/bin/sax.sh
/usr/X11R6/bin/sax2-vesa
/usr/X11R6/bin/SaX2
/usr/X11R6/bin/sax2
/usr/X11R6/%{_lib}/sax/api/data/*????
/usr/X11R6/%{_lib}/sax/sysp/script/installed.pl
/usr/X11R6/%{_lib}/sax/sysp/script/killdot.pl
/usr/X11R6/%{_lib}/sax/sysp/script/vbios.pl
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
%doc %{_defaultdocdir}/sax2/LICENSE
%doc %{_defaultdocdir}/sax2/README
%doc %{_defaultdocdir}/sax2/sax.en.ps
%doc %{_defaultdocdir}/sax2/sax.en.dvi
%doc %{_defaultdocdir}/sax2/sax.de.ps
%doc %{_defaultdocdir}/sax2/sax.de.dvi
#=================================================
# SaX-GUI file list...  
# ------------------------------------------------

%files -n sax2-gui
%defattr(-,root,root)
%dir /usr/X11R6/%{_lib}/sax/api
%dir /usr/X11R6/%{_lib}/sax/api/lang
%dir /usr/X11R6/%{_lib}/xfine
/usr/X11R6/%{_lib}/sax/xw.pl
/usr/X11R6/%{_lib}/sax/xapi
/usr/X11R6/%{_lib}/sax/xrun.pl
/usr/X11R6/%{_lib}/sax/api/tools
/usr/X11R6/%{_lib}/sax/api/pixmaps
/usr/X11R6/%{_lib}/xfine/pixmaps
/usr/X11R6/%{_lib}/xfine/xfine
/usr/X11R6/%{_lib}/xfine/xfine.gtx
%if %{suse_version} > 820
/usr/share/applications/sax2.desktop
%endif
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
/usr/share/locale/nb/LC_MESSAGES/sax.mo
/usr/share/locale/ru/LC_MESSAGES/sax.mo
/usr/share/locale/el_GR/LC_MESSAGES/sax.mo
/usr/share/locale/sl_SI/LC_MESSAGES/sax.mo
/usr/share/locale/sv/LC_MESSAGES/sax.mo
/usr/share/locale/ja/LC_MESSAGES/sax.mo
/usr/share/locale/bg/LC_MESSAGES/sax.mo
/usr/share/locale/pt_BR/LC_MESSAGES/sax.mo
/usr/share/locale/pl_PL/LC_MESSAGES/sax.mo
/usr/share/locale/zh_CN/LC_MESSAGES/sax.mo
/usr/share/locale/zh_TW/LC_MESSAGES/sax.mo
#=================================================
# SaX-Tools file list...  
# ------------------------------------------------

%files -n sax2-tools
%defattr(-,root,root)
%doc /usr/X11R6/man/man1/*
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

%files -n sax2-ident
%defattr(-,root,root)
%dir /usr/X11R6/%{_lib}/sax
%dir /usr/X11R6/%{_lib}/sax/api
%dir /usr/X11R6/%{_lib}/sax/api/data/cdb
%dir /usr/X11R6/%{_lib}/sax/api/data
%dir /usr/X11R6/%{_lib}/sax/sysp/maps
%dir /usr/X11R6/%{_lib}/sax/sysp
/usr/X11R6/%{_lib}/sax/sysp/maps/Identity.map
/usr/X11R6/%{_lib}/sax/sysp/maps/Keyboard.map
/usr/X11R6/%{_lib}/sax/sysp/maps/Vendor.map
/usr/X11R6/%{_lib}/sax/sysp/maps/Input.map
/usr/X11R6/%{_lib}/sax/sysp/maps/Driver.map
/usr/X11R6/%{_lib}/sax/api/data/cdb/*
/usr/X11R6/%{_lib}/sax/profile
