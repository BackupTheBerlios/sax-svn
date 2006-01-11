#
# spec file for package sax2 (Version 7.1)
#
# Copyright (c) 2005 Marcus Schaefer, Isny, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via
# http://developer.berlios.de/projects/sax
#

BuildRequires: acl attr bash bind-utils bison bzip2 coreutils cpio cpp cracklib diffutils e2fsprogs file filesystem findutils flex gawk gdbm-devel glibc glibc-devel grep groff gzip info less libacl libattr libgcc libselinux libstdc++ m4 make man mktemp module-init-tools ncurses ncurses-devel net-tools openssl pam patch popt procps psmisc rcs readline sed strace tar texinfo unzip util-linux zlib zlib-devel autoconf automake binutils doxygen expat fontconfig fontconfig-devel freeglut gcc gcc-c++ gdbm gettext glib2 graphviz hal jpackage-utils libjpeg libjpeg-devel libmng libmng-devel libpng libpng-devel libstdc++-devel libtool perl python python-devel rpm swig sysfsutils udev wireless-tools xorg-x11-devel xorg-x11-libs hal-devel

#=================================================
# Description sax2
#-------------------------------------------------
Name:         sax2
Requires:     perl readline ncurses hal dbus binutils
Requires:     sax2-ident sax2-tools
PreReq:       /bin/rm /bin/mkdir /usr/sbin/chroot
Requires:     xorg-x11
Summary:      SuSE advanced X Window System-configuration
Version:      7.1
Release:      18
Group:        System/X11/Utilities
License:      Other License(s), see package, GPL
Source:       sax2.tar.bz2
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
ExcludeArch:  s390 s390x

%define sub_package 0
%define __perl_requires %{nil}


%description
This package contains the SuSE Advanced X-Configuration



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-tools
#-------------------------------------------------
%package -n sax2-tools
Version:      2.7
Release:      18
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
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-ident
#-------------------------------------------------
%package -n sax2-ident
Version:      1.7
Release:      18
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
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-gui
#-------------------------------------------------
%package -n sax2-gui
Version:      1.7
Release:      18
Requires:     netpbm sax2-tools qt fvwm2 sax2
Summary:      SuSE advanced X Window System-configuration GUI
Group:        System/X11/Utilities
Provides:     sax2:/usr/sbin/xapi

%description -n sax2-gui
This package contains the GUI for the SuSE Advanced X-Configuration



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-libsax
#-------------------------------------------------
%package -n sax2-libsax
Version:      7.1
Release:      18
Requires:     sax2-tools qt fvwm2 sax2 855resolution
Summary:      SaX management library for X Window System-configuration
Group:        Development/Libraries/X11
Provides:     sax2:/usr/%{_lib}/libsax.so

%description -n sax2-libsax
libsax provides a C++ written library to manage X11 configurations



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-libsax-devel
#-------------------------------------------------
%package -n sax2-libsax-devel
Version:      7.1
Release:      18
Requires:     sax2-libsax
Summary:      SaX management library header files for X Window System-configuration development
Group:        Development/Libraries/X11

%description -n sax2-libsax-devel
The devel package of libsax provides all header files needed to use
libsax in your own development environment



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-libsax-perl
#-------------------------------------------------
%package -n sax2-libsax-perl
Version:      7.1
Release:      18
Requires:     sax2-libsax
Summary:      Language binding to use libsax with perl
Group:        Development/Libraries/X11

%description -n sax2-libsax-perl
This package provides a wrapper to be able to use libsax in perl
written programs



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-libsax-python
#-------------------------------------------------
%package -n sax2-libsax-python
Version:      7.1 
Release:      18
Requires:     sax2-libsax
Summary:      Language binding to use libsax with python
Group:        Development/Libraries/X11
    

%description -n sax2-libsax-python
This package provides a wrapper to be able to use libsax in python
written programs



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

#=================================================
# Description sax2-libsax-java
#-------------------------------------------------
%if%sub_package
%ifnarch ppc64
%package -n sax2-libsax-java
Version:      7.1
Release:      18
Requires:     sax2-libsax jre1.2.x
Summary:      Language binding to use libsax with java
Group:        Development/Libraries/X11
    

%description -n sax2-libsax-java
This package provides a wrapper to be able to use libsax in java
written programs



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

%endif
%endif
#=================================================
# Description sax2-libsax-csharp
#-------------------------------------------------
%if%sub_package
%ifnarch ppc64
%package -n sax2-libsax-csharp
Version:      7.1
Release:      18
Requires:     sax2-libsax mono
Summary:      Language binding to use libsax with C#
Group:        Development/Libraries/X11

%description -n sax2-libsax-csharp
This package provides a wrapper to be able to use libsax in C# written
programs



Authors:
--------
    Marcus Schäfer <marcus.schaefer@gmail.com>

%endif
%endif
%prep
%setup -n sax
# %patch

%build
test -e /.buildenv && . /.buildenv
#=================================================
# add version to sax.sh script...
#-------------------------------------------------
cat ./startup/sax.sh | \
	sed -e s@SuSE-Linux@"$BUILD_DISTRIBUTION_NAME"@ \
> /tmp/sax.sh
cp /tmp/sax.sh ./startup/sax.sh
#=================================================
# build sources
#-------------------------------------------------
make bindlib=%{_lib} buildroot=$RPM_BUILD_ROOT
#=================================================
# install sources
#-------------------------------------------------
%install
rm -rf $RPM_BUILD_ROOT
make buildroot=$RPM_BUILD_ROOT \
	 bindlib=%{_lib} \
	 lib_prefix=$RPM_BUILD_ROOT/usr/%{_lib} \
	 doc_prefix=$RPM_BUILD_ROOT/%{_defaultdocdir} \
	 man_prefix=$RPM_BUILD_ROOT/%{_mandir} \
	 install
#=================================================
# remove unpacked sources...
#-------------------------------------------------
rm -f $RPM_BUILD_ROOT/%{perl_vendorarch}/*.pl
rm -f $RPM_BUILD_ROOT/%{perl_archlib}/perllocal.pod
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
%dir %{_defaultdocdir}/sax2
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
%{perl_vendorarch}/SPP.pm
%{perl_vendorarch}/auto/SPP
%{perl_vendorarch}/FBSet.pm
%{perl_vendorarch}/auto/FBSet
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
/usr/share/locale/fi/LC_MESSAGES/sax.mo
/usr/share/locale/pa/LC_MESSAGES/sax.mo
/usr/sbin/xapi
/usr/sbin/xfine
#=================================================
# SaX-Tools file list...  
# ------------------------------------------------

%files -n sax2-tools
%defattr(-,root,root)
%doc %{_mandir}/man1/sax2.1.gz
%doc %{_mandir}/man1/xkbctrl.1.gz
%doc %{_mandir}/man1/xkbset.1.gz
%doc %{_mandir}/man1/xmode.1.gz
%doc %{_mandir}/man1/xquery.1.gz
/usr/sbin/corner
/usr/sbin/dots
/usr/sbin/isax
/usr/sbin/testX
/usr/sbin/whereiam
/usr/sbin/wmstart
/usr/sbin/ximage
/usr/sbin/xidle
/usr/sbin/xkbctrl
/usr/sbin/xlook
/usr/sbin/xmode
/usr/sbin/xquery
/usr/sbin/vncp
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
%dir /usr/lib/python2.4/site-packages/SaX
/usr/lib/python2.4/site-packages/SaX.pth
/usr/lib/python2.4/site-packages/SaX/*
#=================================================
# SaX-libsax-java file list...  
# ------------------------------------------------
%if%sub_package
%ifnarch ppc64

%files -n sax2-libsax-java
%defattr(-,root,root)
%dir /usr/%{_lib}/sax
%dir /usr/%{_lib}/sax/plugins
/usr/share/java/SaX.jar
/usr/%{_lib}/sax/plugins/SaX.so
%endif
%endif
#=================================================
# SaX-libsax-csharp file list...  
# ------------------------------------------------
%if%sub_package
%ifnarch ppc64

%files -n sax2-libsax-csharp
%defattr(-,root,root)
%dir /usr/%{_lib}/mono
%dir /usr/%{_lib}/mono/gac
%dir /usr/%{_lib}/mono/gac/SaX
/usr/%{_lib}/mono/gac/SaX/*
/usr/%{_lib}/libSaX.so
%endif
%endif
