#
# spec file for package sax2 (Version 7.1)
#
# Copyright (c) 2004 SUSE LINUX AG, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://www.suse.de/feedback/
#

# neededforbuild  flex hwinfo hwinfo-devel qt3-devel-packages swig udev update-desktop-files x-devel-packages xorg-x11-driver-options xorg-x11-server yacc fbset mono-devel-packages glib2 java2-devel-packages python-devel python wireless-tools graphviz doxygen xorg-x11-fonts-100dpi xorg-x11-fonts-75dpi ghostscript-fonts-std sysfsutils hal dbus-1

BuildRequires: aaa_base acl attr bash bind-utils bison bzip2 coreutils cpio cpp cracklib cvs cyrus-sasl db devs diffutils e2fsprogs file filesystem fillup findutils flex gawk gdbm-devel glibc glibc-devel glibc-locale gpm grep groff gzip info insserv klogd less libacl libattr libgcc libnscd libselinux libstdc++ libxcrypt libzio m4 make man mktemp module-init-tools ncurses ncurses-devel net-tools netcfg openldap2-client openssl pam pam-modules patch permissions popt procinfo procps psmisc pwdutils rcs readline sed strace syslogd sysvinit tar tcpd texinfo timezone unzip util-linux vim zlib zlib-devel autoconf automake binutils expat fbset fontconfig fontconfig-devel freeglut freeglut-devel freetype2 freetype2-devel gcc gcc-c++ gdbm gettext hwinfo hwinfo-devel libjpeg libjpeg-devel liblcms liblcms-devel libmng libmng-devel libpng libpng-devel libstdc++-devel libtool perl qt3 qt3-devel rpm swig udev update-desktop-files wireless-tools xorg-x11-Mesa xorg-x11-Mesa-devel xorg-x11-devel xorg-x11-driver-options xorg-x11-libs xorg-x11-server

#=================================================
# Description sax2
#-------------------------------------------------
Name:         sax2
Requires:     perl perl-gettext fbset perl-TermReadLine-Gnu
Requires:     sax2-ident sax2-tools
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
Version:      7.1
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

#=================================================
# Description sax2-tools
#-------------------------------------------------
%package -n sax2-tools
Version:      2.7
Release:      11
Summary:      X Window System tools for SaX2
Group:        System/X11/Utilities
Requires:     sax2-ident coreutils
Provides:     saxtools
Obsoletes:    saxtools

%description -n sax2-tools
Some small X Window System tools to handle input devices, for example,
mouse and keyboard.

Authors:
--------
    Marcus Schäfer <ms@suse.de>

#=================================================
# Description sax2-ident
#-------------------------------------------------
%package -n sax2-ident
Version:      1.7
Release:      11
Summary:      SaX2 identity and profile information
Group:        System/X11/Utilities
Provides:     sax2:/usr/share/sax/sysp/maps/Identity.map
Provides:     sax2-tools:/usr/share/sax/sysp/maps/Identity.map
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

#=================================================
# Description sax2-gui
#-------------------------------------------------
%package -n sax2-gui
Version:      1.7
Release:      11
Requires:     ImageMagick xbanner sax2-tools qt3 fvwm2 tightvnc sax2
Summary:      SuSE advanced X Window System-configuration GUI
Group:        System/X11/Utilities
Provides:     sax2:/usr/sbin/xapi

%description -n sax2-gui
This package contains the GUI for the SuSE Advanced X-Configuration

Authors:
--------
    Marcus Schäfer <ms@suse.de>

#=================================================
# Description sax2-libsax
#-------------------------------------------------
%package -n sax2-libsax
Version:      7.1
Release:      11
Requires:     xbanner sax2-tools qt3 fvwm2 tightvnc sax2
Summary:      SuSE advanced X Window System-configuration library
Group:        System/X11/Libraries
Provides:     sax2:/usr/%{_lib}/libsax.so

%description -n sax2-libsax
This package contains the core library for the SuSE Advanced X-Configuration

Authors:
--------
    Marcus Schäfer <ms@suse.de>

#=================================================
# Description sax2-libsax-devel
#-------------------------------------------------
%package -n sax2-libsax-devel
Version:      7.1
Release:      11
Requires:     sax2-libsax
Summary:      SuSE advanced X Window System-configuration library header files
Group:        System/X11/Libraries

%description -n sax2-libsax-devel
This package contains the core library headers for the
SuSE Advanced X-Configuration development

Authors:
--------
    Marcus Schäfer <ms@suse.de>

#=================================================
# Description sax2-libsax-perl
#-------------------------------------------------
%package -n sax2-libsax-perl
Version:      7.1
Release:      11
Requires:     sax2-libsax
Summary:      SuSE advanced X Window System-configuration Perl binding
Group:        System/X11/Libraries

%description -n sax2-libsax-perl
This package contains the SaX2 binding to be able to use
libsax with Perl

Authors:
--------
    Marcus Schäfer <ms@suse.de>

#=================================================
# Description sax2-libsax-python
#-------------------------------------------------
%package -n sax2-libsax-python
Version:      7.1 
Release:      11 
Requires:     sax2-libsax
%{py_requires}
Summary:      SuSE advanced X Window System-configuration Python binding
Group:        System/X11/Libraries
    
%description -n sax2-libsax-python
This package contains the SaX2 binding to be able to use
libsax with Python

Authors: 
--------
    Marcus Schäfer <ms@suse.de>

#=================================================
# Description sax2-libsax-java
#-------------------------------------------------
%package -n sax2-libsax-java
Version:      7.1
Release:      11
Requires:     sax2-libsax jre1.2.x
Summary:      SuSE advanced X Window System-configuration Java binding
Group:        System/X11/Libraries
    
%description -n sax2-libsax-java
This package contains the SaX2 binding to be able to use
libsax with Java

Authors:      
--------
    Marcus Schäfer <ms@suse.de>


#=================================================
# Description sax2-libsax-csharp
#-------------------------------------------------
%ifnarch ia64 ppc64
%package -n sax2-libsax-csharp
Version:      7.1
Release:      11
Requires:     sax2-libsax mono
Summary:      SuSE advanced X Window System-configuration Java binding
Group:        System/X11/Libraries

%description -n sax2-libsax-csharp
This package contains the SaX2 binding to be able to use
libsax with CSharp

Authors:
--------
    Marcus Schäfer <ms@suse.de>
%endif

#=================================================
# Preparation...
#-------------------------------------------------
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

%build
test -e /.buildenv && . /.buildenv
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
make bindlib=%{_lib}

#=================================================
# install sources
#-------------------------------------------------
%install
rm -rf $RPM_BUILD_ROOT
export ASK_FOR_CHANGELOG=no
export TEMPLATE_CHECK=no
export ARCH=`/bin/arch`
make buildroot=$RPM_BUILD_ROOT \
	 bindlib=%{_lib} lib_prefix=$RPM_BUILD_ROOT/usr/%{_lib} install
make doc_prefix=$RPM_BUILD_ROOT/%{_defaultdocdir} \
	 man_prefix=$RPM_BUILD_ROOT/%{_mandir} install-docs
#=================================================
# create root startup link
#-------------------------------------------------
( 
	rm -f $RPM_BUILD_ROOT/usr/sbin/sax2 && \
	cd $RPM_BUILD_ROOT/usr/sbin && ln -s SaX2 sax2
)
#=================================================
# create user startup link
#-------------------------------------------------
(
	mkdir -p $RPM_BUILD_ROOT/usr/X11R6/bin
	rm -f $RPM_BUILD_ROOT/usr/X11R6/bin/sax2 && \
	cd $RPM_BUILD_ROOT/usr/X11R6/bin && ln -s /usr/sbin/SaX2 sax2
)
#=================================================
# check for options file at /etc/X11/CardModules
#-------------------------------------------------
%ifarch %ix86
if [ -f "/usr/X11R6/bin/xsload" ];then
	MD=/usr/X11R6/lib/modules/drivers
	for i in ati_drv.o radeon_drv.o atimisc_drv.o r128_drv.o;do
		test -f $MD/$i && mv $MD/$i /tmp
	done
	/usr/X11R6/bin/xsload > /tmp/CardModules
	cat /etc/X11/CardModules.addon >> /tmp/CardModules
	cp /tmp/CardModules \
		$RPM_BUILD_ROOT/usr/share/sax/api/data/CardModules
	for i in ati_drv.o radeon_drv.o atimisc_drv.o r128_drv.o;do
		test -f /tmp/$i && mv /tmp/$i $MD
	done
fi
%endif
#=================================================
# install desktop icon...
#-------------------------------------------------
install -d -m 755 $RPM_BUILD_ROOT/usr/share/pixmaps
install -m 644 api/figures/sax2.png \
	$RPM_BUILD_ROOT/usr/share/pixmaps/
#=================================================
# check perl .packlist...
#-------------------------------------------------
%if %{suse_version} > 820
%perl_process_packlist
%endif
#=================================================
# remove unpacked sources...
#-------------------------------------------------
rm -f $RPM_BUILD_ROOT/%{perl_vendorarch}/*.pl

#=================================================
# update desktop file 
#-------------------------------------------------
%suse_update_desktop_file -i %name System SystemSetup

#=================================================
# uninstall script stage:[previous]
#-------------------------------------------------
%preun
chroot . rm -f /var/cache/xfine/*
if [ ! -d /var/cache/xfine ];then
	mkdir -p /var/cache/xfine
fi

#=================================================
# SaX files...      
#-------------------------------------------------
%files
%defattr(-,root,root)
%dir /usr/share/sax/api
%dir /usr/share/sax/api/data
%dir /usr/share/doc/packages/sax2
%dir /usr/X11R6/share/fvwm
%dir /usr/share/sax
%dir /usr/share/sax/sysp/script
%dir /usr/share/sax/sysp
%dir /var/cache/sax/sysp
%dir /var/cache/sax/sysp/rdbms
%dir /var/cache/sax/files
%dir /var/cache/sax
%dir /var/cache/xfine
/usr/share/pixmaps/sax2.png
/usr/X11R6/share/fvwm/fvwmrc.sax
/usr/share/sax/svnbuild
/usr/share/sax/init.pl
/usr/share/sax/xc.pl
/usr/share/sax/pci.pl
/usr/share/sax/modules
/usr/sbin/sax.sh
/usr/sbin/sax2-vesa
/usr/sbin/SaX2
/usr/X11R6/bin/sax2
/usr/sbin/sax2
/usr/sbin/sysp
/usr/sbin/xcmd
/usr/share/sax/api/data/CardModules
/usr/share/sax/api/data/LangCodes
/usr/share/sax/api/data/PointerDevice
/usr/share/sax/api/data/LangFirstPath
/usr/share/sax/api/data/StaticFontPathList
/usr/share/sax/api/data/MonitorColors
/usr/share/sax/api/data/TabletModules
/usr/share/sax/api/data/MonitorRatio
/usr/share/sax/api/data/MonitorResolution
/usr/share/sax/api/data/MonitorTraversal
/usr/share/sax/sysp/script/installed.pl
/usr/share/sax/sysp/script/killdot.pl
/usr/share/sax/sysp/script/vbios.pl
/usr/share/sax/sysp/script/machine.pl
/usr/share/sax/sysp/script/preparelog.pl
/usr/share/sax/sysp/script/checkmap.pl
/usr/share/sax/sysp/script/profilecount.pl
/usr/share/sax/sysp/script/psection.pl
/usr/share/sax/sysp/script/profiledriver.pl
/usr/share/sax/sysp/script/vendor.pl
%{perl_vendorarch}/XFree.pm
%{perl_vendorarch}/auto/XFree
%{perl_vendorarch}/PLog.pm
%{perl_vendorarch}/auto/PLog
/var/adm/perl-modules/sax2
%doc %{_defaultdocdir}/sax2/LICENSE
%doc %{_defaultdocdir}/sax2/README
%doc %{_defaultdocdir}/sax2/sax.pdf

#=================================================
# SaX-GUI file list...  
# ------------------------------------------------
%files -n sax2-gui
%defattr(-,root,root)
%dir /usr/share/sax/api
%dir /usr/share/xfine
/usr/share/sax/api/macros
/usr/share/sax/api/figures
/usr/share/xfine/figures
/usr/share/xfine/xfine.gtx
/usr/share/sax/api/data/xapi.gtx
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
/usr/share/locale/uk/LC_MESSAGES/sax.mo
/usr/sbin/xapi
/usr/sbin/xfine
%if %{suse_version} > 820
/usr/share/applications/sax2.desktop
%endif

#=================================================
# SaX-Tools file list...  
# ------------------------------------------------
%files -n sax2-tools
%defattr(-,root,root)
%doc %{_mandir}/man1/sax2.1.gz
%doc %{_mandir}/man1/xkbctrl.1.gz
%doc %{_mandir}/man1/xkbset.1.gz
%doc %{_mandir}/man1/xmode.1.gz
%doc %{_mandir}/man1/xmset.1.gz
%doc %{_mandir}/man1/xquery.1.gz
/usr/sbin/catch
/usr/sbin/corner
/usr/sbin/demo
/usr/sbin/demo.sh
/usr/sbin/dots
/usr/sbin/fake
/usr/sbin/hwupdate
/usr/sbin/isax
/usr/sbin/screen
/usr/sbin/testX
/usr/sbin/whois
/usr/sbin/wmstart
/usr/sbin/wrap
/usr/sbin/xbounce
/usr/sbin/xbound
/usr/sbin/xidle
/usr/sbin/xkbctrl
/usr/sbin/xkbset
/usr/sbin/xlook
/usr/sbin/xmirror
/usr/sbin/xmode
/usr/sbin/xmset
/usr/sbin/xquery
/usr/sbin/xupdate
/usr/sbin/xw

#=================================================
# SaX-Ident file list...  
# ------------------------------------------------
%files -n sax2-ident
%defattr(-,root,root)
%dir /usr/share/sax
%dir /usr/share/sax/api
%dir /usr/share/sax/api/data/cdb
%dir /usr/share/sax/api/data
%dir /usr/share/sax/sysp/maps
%dir /usr/share/sax/sysp
/usr/share/sax/sysp/maps/Identity.map
/usr/share/sax/sysp/maps/Keyboard.map
/usr/share/sax/sysp/maps/Vendor.map
/usr/share/sax/sysp/maps/Input.map
/usr/share/sax/sysp/maps/Driver.map
/usr/share/sax/api/data/cdb/*
/usr/share/sax/profile

#=================================================
# SaX-libsax file list...  
# ------------------------------------------------
%files -n sax2-libsax
%defattr(-,root,root)
%dir %{_defaultdocdir}/libsax
%doc %{_defaultdocdir}/libsax/html
/usr/%{_lib}/libsax.so*
/var/lib/sax

#=================================================
# SaX-libsax-devel file list...  
# ------------------------------------------------
%files -n sax2-libsax-devel
%defattr(-,root,root)
%dir /usr/include/sax
/usr/include/sax/*

#=================================================
# SaX-libsax-perl file list...  
# ------------------------------------------------
%files -n sax2-libsax-perl
%defattr(-,root,root)
%{perl_vendorarch}/SaX.pm
%{perl_vendorarch}/auto/SaX

#=================================================
# SaX-libsax-python file list...  
# ------------------------------------------------
%files -n sax2-libsax-python
%defattr(-,root,root)
%dir %{py_sitedir}/SaX
%{py_sitedir}/SaX.pth
%{py_sitedir}/SaX/*

#=================================================
# SaX-libsax-java file list...  
# ------------------------------------------------
%files -n sax2-libsax-java
%defattr(-,root,root)
%dir /usr/%{_lib}/sax
%dir /usr/%{_lib}/sax/plugins
/usr/share/java/SaX.jar
/usr/%{_lib}/sax/plugins/SaX.so

#=================================================
# SaX-libsax-csharp file list...  
# ------------------------------------------------
%ifnarch ia64 ppc64
%files -n sax2-libsax-csharp
%defattr(-,root,root)
%dir /usr/%{_lib}/mono
%dir /usr/%{_lib}/mono/gac
%dir /usr/%{_lib}/mono/gac/SaX
/usr/%{_lib}/mono/gac/SaX/*
/usr/%{_lib}/libSaX.so
%endif
