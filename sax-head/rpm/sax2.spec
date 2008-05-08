#
# spec file for package sax2 (Version 8.1)
#
# Copyright (c) 2007 SUSE LINUX Products GmbH, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           sax2
URL:            http://sax.berlios.de
%define build_java 0
%if %{suse_version} > 1010
BuildRequires:  antlr bison doxygen flex gettext-devel ghostscript-fonts-std
BuildRequires:  graphviz hal-devel python-devel
BuildRequires:  libqt4 libqt4-qt3support libqt4-devel libqt4-x11
BuildRequires:  readline-devel swig sysfsutils update-desktop-files
BuildRequires:  xorg-x11-server-sdk hwinfo-devel
%if %{build_java}
BuildRequires: java-1_4_2-gcj-compat-devel
%endif
%endif
%if %{suse_version} <= 1010
BuildRequires:  doxygen ghostscript-fonts-std graphviz hal-devel python-devel
BuildRequires:  qt qt-devel qt-x11 qt-qt3support
BuildRequires:  readline-devel swig sysfsutils update-desktop-files gcc-c++
BuildRequires:  libpng-devel freetype2-devel
%if %{build_java}
BuildRequires: java2-devel-packages
%endif
%endif
Requires:       perl = %{perl_version}
Requires:       readline ncurses hal dbus-1 sax2-libsax
Requires:       sax2-ident sax2-tools
Requires:       xorg-x11-server
PreReq:         /bin/rm /bin/mkdir /usr/bin/chroot %fillup_prereq %insserv_prereq
Summary:        SuSE advanced X Window System-configuration
Version:        8.1
Release:        126
Group:          System/X11/Utilities
License:        GNU General Public License (GPL)
Source:         sax2.tar.bz2
Source1:        sax2.desktop
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
ExcludeArch:  s390
%if %{suse_version} > 1020
Patch:          sax2.dif
%endif

%description
This package contains the SuSE Advanced X-Configuration

Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-tools
Summary:        X Window System tools for SaX2
Group:          System/X11/Utilities
Requires:       coreutils
Provides:       saxtools 
Obsoletes:      saxtools
%ifarch s390x 
Provides:       sax2
Obsoletes:      sax2
Provides:       sax2-gui
Obsoletes:      sax2-gui
Provides:       sax2-libsax
Obsoletes:      sax2-libsax
Provides:       sax2-libsax-perl
Obsoletes:      sax2-libsax-perl
Provides:       sax2-ident
Obsoletes:      sax2-ident
%endif

%description -n sax2-tools
Some small X Window System tools to handle input devices, for example,
mouse and keyboard.

%package -n sax2-ident
Summary:        SaX2 identity and profile information
Group:          System/X11/Utilities
Provides:       sax2:/usr/share/sax/sysp/maps/Identity.map
Provides:       sax2-tools:/usr/share/sax/sysp/maps/Identity.map
Provides:       saxident
Obsoletes:      saxident

%description -n sax2-ident
This package contains information about the supported graphics hardware
and its special parameters. For some graphics cards a profile is needed
to describe configuration parameters outside the ordinary way of
setting up the card with SaX2.

%package -n sax2-gui
Requires:       netpbm sax2-tools icewm-bin sax2
Summary:        SuSE advanced X Window System-configuration GUI
Group:          System/X11/Utilities
Provides:       sax2:/usr/sbin/xapi

%description -n sax2-gui
This package contains the GUI for the SuSE Advanced X-Configuration

%package -n sax2-libsax
Requires:       perl = %{perl_version}
Requires:       sax2-tools sax2-libsax-perl icewm-bin
%if %{suse_version} <= 1020
%ifarch       %ix86 x86_64
Requires:       915resolution
%endif
%endif
Summary:        SaX library
Group:          Development/Libraries/X11
Provides:       sax2:/usr/%{_lib}/libsax.so

%description -n sax2-libsax
libsax provides a C++ written library to manage X11 configurations

%package -n sax2-libsax-devel
Requires:       sax2-libsax = %version
Summary:        SaX management library header files for X Window System-configuration development
Group:          Development/Libraries/X11

%description -n sax2-libsax-devel
The devel package of libsax provides all header files needed to use
libsax in your own development environment

%package -n sax2-libsax-perl
Requires:       perl = %{perl_version}
Summary:        Language binding to use libsax with perl
Group:          Development/Libraries/X11

%description -n sax2-libsax-perl
This package provides a wrapper to be able to use libsax in perl
written programs

%package -n sax2-libsax-python
Requires:       sax2-libsax
Summary:        Language binding to use libsax with python
Group:          Development/Libraries/X11
%py_requires

%description -n sax2-libsax-python
This package provides a wrapper to be able to use libsax in python
written programs

%if %{build_java}
%package -n sax2-libsax-java
Requires:       sax2-libsax jre1.2.x
Summary:        Language binding to use libsax with java
Group:          Development/Libraries/X11

%description -n sax2-libsax-java
This package provides a wrapper to be able to use libsax in java
written programs
%endif

%prep
%setup -n sax
%if %{suse_version} > 1020
%patch
%endif

%build
test -e /.buildenv && . /.buildenv
test -z "$QTDIR" && export QTDIR=/usr/lib/qt-3.3
test -z "$JAVA_BINDIR" && export JAVA_BINDIR=/usr/bin
test -z "$BUILD_DISTRIBUTION_NAME" && export BUILD_DISTRIBUTION_NAME=OTHER
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
make bindlib=%{_lib} buildroot=$RPM_BUILD_ROOT
#=================================================
# install sources
#-------------------------------------------------

%install
mkdir -p $RPM_BUILD_ROOT/var/log
touch $RPM_BUILD_ROOT/var/log/SaX.log
make buildroot=$RPM_BUILD_ROOT \
	 bindlib=%{_lib} \
	 lib_prefix=$RPM_BUILD_ROOT/usr/%{_lib} \
	 doc_prefix=$RPM_BUILD_ROOT/%{_defaultdocdir} \
	 man_prefix=$RPM_BUILD_ROOT/%{_mandir} \
	 install
find $RPM_BUILD_ROOT -name "*.bs" | xargs rm -f
%find_lang sax
# SuSE specific build instructions...
#=================================================
# check perl .packlist...
#-------------------------------------------------
%perl_process_packlist
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

%post -n sax2-libsax
%run_ldconfig

%postun -n sax2-libsax
%run_ldconfig

#=================================================
# SaX files...      
#-------------------------------------------------

%files
%defattr(-,root,root)
%ghost %config(noreplace) /var/log/SaX.log
%dir %{_datadir}/sax/api
%dir %{_datadir}/sax/api/data
%dir %{_defaultdocdir}/sax2
%dir %{_datadir}/sax
%dir %{_datadir}/sax/sysp/script
%dir %{_datadir}/sax/sysp
%dir %{_datadir}/sax/libsax
%dir %{_var}/lib/sax
%dir /var/cache/sax/sysp
%dir /var/cache/sax/sysp/rdbms
%dir /var/cache/sax/files
%dir /var/cache/sax
%dir /var/cache/xfine
%{_datadir}/sax/libsax/*
%{_datadir}/pixmaps/sax2.png
%{_datadir}/sax/svnbuild
%{_datadir}/sax/init.pl
%{_datadir}/sax/xc.pl
%{_datadir}/sax/pci.pl
%{_datadir}/sax/modules
%{_sbindir}/sax.sh
%{_sbindir}/sax2-vesa
%{_sbindir}/SaX2
%{_sbindir}/sax2
%{_sbindir}/sysp
%{_sbindir}/xcmd
%{_datadir}/sax/api/data/fvwmrc.sax
%{_datadir}/sax/api/data/twmrc.sax
%{_datadir}/sax/api/data/icerc.sax
%{_datadir}/sax/api/data/CardModules
%{_datadir}/sax/api/data/LangCodes
%{_datadir}/sax/api/data/PointerDevice
%{_datadir}/sax/api/data/LangFirstPath
%{_datadir}/sax/api/data/StaticFontPathList
%{_datadir}/sax/api/data/MonitorColors
%{_datadir}/sax/api/data/TabletModules
%{_datadir}/sax/api/data/MonitorRatio
%{_datadir}/sax/api/data/MonitorResolution
%{_datadir}/sax/api/data/MonitorTraversal
%{_datadir}/sax/sysp/script/installed.pl
%{_datadir}/sax/sysp/script/killdot.pl
%{_datadir}/sax/sysp/script/memory.pl
%{_datadir}/sax/sysp/script/machine.pl
%{_datadir}/sax/sysp/script/preparelog.pl
%{_datadir}/sax/sysp/script/checkmap.pl
%{_datadir}/sax/sysp/script/profilecount.pl
%{_datadir}/sax/sysp/script/psection.pl
%{_datadir}/sax/sysp/script/profiledriver.pl
%{_datadir}/sax/sysp/script/vendor.pl
%doc %{_defaultdocdir}/sax2/LICENSE
%doc %{_defaultdocdir}/sax2/README
#=================================================
# SaX-GUI file list...  
# ------------------------------------------------

%files -n sax2-gui -f sax.lang
%defattr(-,root,root)
%dir %{_datadir}/sax/api
%dir %{_datadir}/xfine
%{_datadir}/sax/api/macros
%{_datadir}/sax/api/figures
%{_datadir}/xfine/figures
%{_datadir}/xfine/xfine.gtx
%{_datadir}/sax/api/data/xapi.gtx
%{_sbindir}/xapi
%{_sbindir}/xfine
%{_datadir}/applications/sax2.desktop
#=================================================
# SaX-Tools file list...  
# ------------------------------------------------

%files -n sax2-tools
%defattr(-,root,root)
%doc %{_mandir}/man1/sax2.1.gz
%doc %{_mandir}/man1/xkbctrl.1.gz
%doc %{_mandir}/man1/xmode.1.gz
%doc %{_mandir}/man1/xquery.1.gz
%{_sbindir}/corner
%{_sbindir}/dots
%{_sbindir}/isax
%{_sbindir}/testX
%{_sbindir}/whereiam
%{_sbindir}/wmstart
%{_sbindir}/ximage
%{_sbindir}/xidle
%{_sbindir}/xkbctrl
%{_sbindir}/xlook
%{_sbindir}/xmode
%{_sbindir}/xquery
%{_sbindir}/vncp
%{_sbindir}/xw
#=================================================
# SaX-Ident file list...  
# ------------------------------------------------

%files -n sax2-ident
%defattr(-,root,root)
%dir %{_datadir}/sax
%dir %{_datadir}/sax/api
%dir %{_datadir}/sax/api/data/cdb
%dir %{_datadir}/sax/api/data
%dir %{_datadir}/sax/sysp/maps
%dir %{_datadir}/sax/sysp
%{_datadir}/sax/sysp/maps/Identity.map
%{_datadir}/sax/sysp/maps/Keyboard.map
%{_datadir}/sax/sysp/maps/Vendor.map
%{_datadir}/sax/sysp/maps/Input.map
%{_datadir}/sax/sysp/maps/Driver.map
%{_datadir}/sax/sysp/maps/IntelPatch.map
%{_datadir}/sax/api/data/cdb/Cards
%{_datadir}/sax/api/data/cdb/Pads
%{_datadir}/sax/api/data/cdb/Pens
%{_datadir}/sax/api/data/cdb/Pointers
%{_datadir}/sax/api/data/cdb/Tablets
%{_datadir}/sax/api/data/cdb/Touchscreens
%config %{_datadir}/sax/api/data/cdb/Monitors
%{_datadir}/sax/profile
#=================================================
# SaX-libsax file list...  
# ------------------------------------------------

%files -n sax2-libsax
%defattr(-,root,root)
%{_prefix}/%{_lib}/libsax.so.*
%{perl_vendorarch}/XFree.pm
%{perl_vendorarch}/auto/XFree
%{perl_vendorarch}/PLog.pm
%{perl_vendorarch}/auto/PLog
%{perl_vendorarch}/SPP.pm
%{perl_vendorarch}/auto/SPP
%{perl_vendorarch}/FBSet.pm
%{perl_vendorarch}/auto/FBSet
%{perl_vendorarch}/CVT.pm
%{perl_vendorarch}/auto/CVT
/var/adm/perl-modules/sax2
#=================================================
# SaX-libsax-devel file list...  
# ------------------------------------------------

%files -n sax2-libsax-devel
%defattr(-,root,root)
%dir %{_includedir}/sax
%dir %{_defaultdocdir}/libsax
%doc %{_defaultdocdir}/libsax/html
%doc %{_defaultdocdir}/sax2/sax.pdf
%{_prefix}/%{_lib}/libsax.so
%{_includedir}/sax/*
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
%if %{build_java}

%files -n sax2-libsax-java
%defattr(-,root,root)
%dir %{_prefix}/%{_lib}/sax
%dir %{_prefix}/%{_lib}/sax/plugins
%{_datadir}/java/SaX.jar
%{_prefix}/%{_lib}/sax/plugins/SaX.so
%endif
