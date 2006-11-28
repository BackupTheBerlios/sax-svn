#
# spec file for package sax2 (Version 8.1)
#
# Copyright (c) 2006 SUSE LINUX Products GmbH, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           sax2
URL:            http://sax.berlios.de
%define build_java 0
%if %{?suse_version:1}0
# SuSE Build Requires...
%if %{suse_version} > 1010
BuildRequires:  doxygen ghostscript-fonts-std graphviz hal-devel java-1_4_2-gcj-compat-devel python-devel qt3-devel readline-devel swig sysfsutils update-desktop-files xorg-x11-server-sdk antlr
%else
BuildRequires:  doxygen ghostscript-fonts-std graphviz hal-devel java2-devel-packages python-devel qt3-devel readline-devel swig sysfsutils update-desktop-files
%endif
%else
# FC5 Build Requires...
BuildRequires:  doxygen gcc-c++ graphviz hal-devel java-gcj-compat-devel libxkbfile-devel python-devel qt-devel readline readline-devel swig sysfsutils sysfsutils-devel xorg-x11-server-sdk
%define __perl_requires %{nil}
%define py_sitedir /usr/%{_lib}/python2.4/site-packages
%endif
%if %{?suse_version:1}0
# SuSE Requires...
Requires:       perl readline ncurses hal dbus-1 binutils
Requires:       sax2-ident sax2-tools
Requires:       xorg-x11-server
%else
# FC5 Requires...
Requires:       perl readline ncurses hal dbus binutils
Requires:       sax2-ident sax2-tools
Requires:       xorg-x11-server-Xorg
%endif
%if %{?suse_version:1}0
# SuSE pre Requires...
PreReq:         /bin/rm /bin/mkdir /usr/bin/chroot %fillup_prereq %insserv_prereq
%else
# FC5 pre Requires...
%endif
Summary:        SuSE advanced X Window System-configuration
Version:        8.1
Release:        9
Group:          System/X11/Utilities
License:        Other License(s), see package, GPL
Source:         sax2.tar.bz2
%if %{?suse_version:1}0
# SuSE Source...
Source1:        sax2.desktop
%endif
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
ExcludeArch:  s390

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



Authors:
--------
    Marcus Schäfer <ms@suse.de>

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



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-gui
%if %{?suse_version:1}0
# SuSE Requires...
Requires:       netpbm sax2-tools qt3 fvwm2 sax2
%else
# FC5 Requires...
Requires:       netpbm sax2-tools qt sax2
%endif
Summary:        SuSE advanced X Window System-configuration GUI
Group:          System/X11/Utilities
Provides:       sax2:/usr/sbin/xapi

%description -n sax2-gui
This package contains the GUI for the SuSE Advanced X-Configuration



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-libsax
%if %{?suse_version:1}0
# SuSE Requires...
Requires:       sax2-tools qt3 fvwm2 sax2
%ifarch       %ix86 x86_64
Requires:       915resolution
%endif
%else
# FC5 Requires...
Requires:       sax2-tools qt sax2
%endif
Summary:        SaX management library for X Window System-configuration
Group:          Development/Libraries/X11
Provides:       sax2:/usr/%{_lib}/libsax.so

%description -n sax2-libsax
libsax provides a C++ written library to manage X11 configurations



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-libsax-devel
Requires:       sax2-libsax
Summary:        SaX management library header files for X Window System-configuration development
Group:          Development/Libraries/X11

%description -n sax2-libsax-devel
The devel package of libsax provides all header files needed to use
libsax in your own development environment



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-libsax-perl
Requires:       sax2-libsax
Summary:        Language binding to use libsax with perl
Group:          Development/Libraries/X11

%description -n sax2-libsax-perl
This package provides a wrapper to be able to use libsax in perl
written programs



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%package -n sax2-libsax-python
Requires:       sax2-libsax
%if %{?suse_version:1}0
# SuSE Requires...
%{py_requires}
%endif
Summary:        Language binding to use libsax with python
Group:          Development/Libraries/X11
    

%description -n sax2-libsax-python
This package provides a wrapper to be able to use libsax in python
written programs



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%if %{build_java}
%package -n sax2-libsax-java
Requires:       sax2-libsax jre1.2.x
Summary:        Language binding to use libsax with java
Group:          Development/Libraries/X11
    

%description -n sax2-libsax-java
This package provides a wrapper to be able to use libsax in java
written programs



Authors:
--------
    Marcus Schäfer <ms@suse.de>

%endif
%prep
%setup -n sax

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
rm -rf $RPM_BUILD_ROOT
make buildroot=$RPM_BUILD_ROOT \
	 bindlib=%{_lib} \
	 lib_prefix=$RPM_BUILD_ROOT/usr/%{_lib} \
	 doc_prefix=$RPM_BUILD_ROOT/%{_defaultdocdir} \
	 man_prefix=$RPM_BUILD_ROOT/%{_mandir} \
	 install
%find_lang sax
%if %{?suse_version:1}0
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
%else
	# FC5 specific build instructions...
	#=================================================
	# remove unpacked sources... 
	#-------------------------------------------------
	rm -f $RPM_BUILD_ROOT/%{perl_vendorarch}/*.pl
	rm -f $RPM_BUILD_ROOT/%{perl_archlib}/perllocal.pod
%endif
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
%dir %{_datadir}/sax/api
%dir %{_datadir}/sax/api/data
%dir %{_defaultdocdir}/sax2
%dir %{_datadir}/sax
%dir %{_datadir}/sax/sysp/script
%dir %{_datadir}/sax/sysp
%dir /var/cache/sax/sysp
%dir /var/cache/sax/sysp/rdbms
%dir /var/cache/sax/files
%dir /var/cache/sax
%dir /var/cache/xfine
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
%if %{?suse_version:1}0
# SuSE files...
/var/adm/perl-modules/sax2
%endif
%doc %{_defaultdocdir}/sax2/LICENSE
%doc %{_defaultdocdir}/sax2/README
%doc %{_defaultdocdir}/sax2/sax.pdf
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
%if %{?suse_version:1}0
# SuSE files...
%{_datadir}/applications/sax2.desktop
%endif
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
%{_datadir}/sax/api/data/cdb/*
%{_datadir}/sax/profile
#=================================================
# SaX-libsax file list...  
# ------------------------------------------------

%files -n sax2-libsax
%defattr(-,root,root)
%dir %{_datadir}/sax/libsax
%dir %{_var}/lib/sax
%dir %{_defaultdocdir}/libsax
%doc %{_defaultdocdir}/libsax/html
%{_prefix}/%{_lib}/libsax.so*
%{_datadir}/sax/libsax/*
#=================================================
# SaX-libsax-devel file list...  
# ------------------------------------------------

%files -n sax2-libsax-devel
%defattr(-,root,root)
%dir %{_includedir}/sax
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
