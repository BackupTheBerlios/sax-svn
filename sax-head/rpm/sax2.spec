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
%if %{?suse_version:1}0
# SuSE Build Requires...
%if %{suse_version} > 1010
BuildRequires:  antlr bison doxygen flex gettext-devel ghostscript-fonts-std graphviz hal-devel java-1_4_2-gcj-compat-devel python-devel libqt4 libqt4-qt3support libqt4-devel libqt4-x11 readline-devel swig sysfsutils update-desktop-files xorg-x11-server-sdk hwinfo-devel
%else
BuildRequires:  doxygen ghostscript-fonts-std graphviz hal-devel java2-devel-packages python-devel qt qt-devel qt-x11 qt-qt3support readline-devel swig sysfsutils update-desktop-files gcc-c++ libpng-devel freetype2-devel qt3-devel
%endif
%else
# FC5 Build Requires...
BuildRequires:  doxygen gcc-c++ graphviz hal-devel java-gcj-compat-devel libxkbfile-devel python-devel libqt4 libqt4-qt3support libqt4-devel libqt4-x11 readline readline-devel swig sysfsutils sysfsutils-devel xorg-x11-server-sdk
%define __perl_requires %{nil}
%define py_sitedir /usr/%{_lib}/python2.4/site-packages
%endif
%if %{?suse_version:1}0
# SuSE Requires...
Requires:       perl readline ncurses hal dbus-1 sax2-libsax
Requires:       sax2-ident sax2-tools
Requires:       xorg-x11-server
%else
# FC5 Requires...
Requires:       perl readline ncurses hal dbus sax2-libsax
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
Release:        126
Group:          System/X11/Utilities
License:        GNU General Public License (GPL)
Source:         sax2.tar.bz2
%if %{?suse_version:1}0
# SuSE Source...
Source1:        sax2.desktop
%endif
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
ExcludeArch:  s390
%if %{suse_version} > 1020
Patch:          sax2.dif
%endif

%description
This package contains the SuSE Advanced X-Configuration



Authors:
--------
    Marcus Sch‰fer <ms@suse.de>

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
    Marcus Sch‰fer <ms@suse.de>

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
    Marcus Sch‰fer <ms@suse.de>

%package -n sax2-gui
%if %{?suse_version:1}0
# SuSE Requires...
Requires:       netpbm sax2-tools fvwm2 sax2
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
    Marcus Sch‰fer <ms@suse.de>

%package -n sax2-libsax
%if %{?suse_version:1}0
# SuSE Requires...
Requires:       sax2-tools sax2-libsax-perl fvwm2
%if %{suse_version} <= 1020
%ifarch       %ix86 x86_64
Requires:       915resolution
%endif
%endif
%else
# FC5 Requires...
Requires:       sax2-tools qt sax2
%endif
Summary:        SaX library
Group:          Development/Libraries/X11
Provides:       sax2:/usr/%{_lib}/libsax.so

%description -n sax2-libsax
libsax provides a C++ written library to manage X11 configurations



Authors:
--------
    Marcus Sch‰fer <ms@suse.de>

%package -n sax2-libsax-devel
Requires:       sax2-libsax
Summary:        SaX management library header files for X Window System-configuration development
Group:          Development/Libraries/X11

%description -n sax2-libsax-devel
The devel package of libsax provides all header files needed to use
libsax in your own development environment



Authors:
--------
    Marcus Sch‰fer <ms@suse.de>

%package -n sax2-libsax-perl
Summary:        Language binding to use libsax with perl
Group:          Development/Libraries/X11

%description -n sax2-libsax-perl
This package provides a wrapper to be able to use libsax in perl
written programs



Authors:
--------
    Marcus Sch‰fer <ms@suse.de>

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
    Marcus Sch‰fer <ms@suse.de>

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
    Marcus Sch‰fer <ms@suse.de>

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
rm -rf $RPM_BUILD_ROOT
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
%{_prefix}/%{_lib}/libsax.so.*
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

%changelog -n sax2
* Thu Jan 24 2008 - ms@suse.de
- use nobus setup for FireGL profile (#355681)
* Thu Jan 24 2008 - ms@suse.de
- added xen-mouse evdev map (FATE: #302948)
* Wed Jan 23 2008 - ms@suse.de
- added fate resolution feature: (FATE: #302987)
* Tue Jan 15 2008 - ms@suse.de
- added Identity sort patch (#353865)
* Mon Jan 14 2008 - ms@suse.de
- added microsoft-IntelliMouse-Explorer profile. Patch by
  Stefan Dirsch <sndirsch@novell.com> (#353185)
* Tue Jan 01 2008 - ms@suse.de
- fixed modeline timings for Samsumg SyncMister 213t (#350919)
* Wed Dec 19 2007 - ms@suse.de
- added CDB tablet patches by Danny Kukawka <dkukawka@novell.com> (#323541)
* Wed Dec 12 2007 - ms@suse.de
- added Xen:Virtual Pointer to cdb/Pointers (FATE: #302948)
* Fri Dec 07 2007 - ms@suse.de
- fixed cut call, fields are are numbered from 1 (#346740)
- update Identity maps for all architectures (#346740)
* Thu Dec 06 2007 - ms@suse.de
- fixed setup of inverting pointer Y-axis (#343795)
* Tue Nov 27 2007 - ms@suse.de
- fixed typographic error 1440*900 WXGA is 1440*900 WXGA+ (#344243)
* Mon Nov 26 2007 - ms@suse.de
- update FireGL profile
* Sun Nov 18 2007 - ms@suse.de
- fixed resolution list setup for fbdev and vesa driver (#342047)
* Wed Nov 14 2007 - ms@suse.de
- added Identity patch by Stefan Dirsch <sndirsch@novell.com>
  which activates fbdev for RN50 on ppc (#341406)
* Mon Nov 12 2007 - ms@suse.de
- fixed build for new gcc compiler (beta)
* Mon Nov 05 2007 - ms@suse.de
- activate evdev profiles for logitech-* profiles (#337822)
- Added spp patch and new logitech evdev profiles
  by Stefan Dirsch <sndirsch@novell.com> (#337822)
* Wed Oct 31 2007 - ms@suse.de
- fixed variant setup (#337513)
* Tue Oct 30 2007 - ms@suse.de
- disabled profiles logitech-Gaming, logitech-MediaPlay
  logitech-mxlaser and logitech-optical (#337822)
* Tue Oct 30 2007 - ms@suse.de
- don't add xfree86 as variant. use basic if no variant is set (#337513)
* Wed Oct 17 2007 - ms@suse.de
- really added HorizScrollDelta to synaptics profile (#223827)
* Tue Oct 16 2007 - ms@suse.de
- added NoModelines profile for nvidia driver (#285003)
* Mon Oct 15 2007 - ms@suse.de
- fixed intel dualhead position setup (#330348)
* Tue Oct 02 2007 - ms@suse.de
- added logitech-optical profile for mouse 0xc040 (#329666)
* Fri Sep 28 2007 - ms@suse.de
- fixed NoModelines profile remove SpecialModeline as well (#328939)
* Fri Sep 28 2007 - ms@suse.de
- fixed modeline trigger in GUI (#328939)
* Tue Sep 25 2007 - ms@suse.de
- allow "Logitech USB R*" evdev matching (#327290)
* Mon Sep 24 2007 - ms@suse.de
- fixed microsoft-natural keyboard profile (#327017)
* Mon Sep 24 2007 - ms@suse.de
- added CDB record for SGX GDM-5411 monitor (#327171)
- update Identity maps for all architectures (#326503)
* Mon Sep 24 2007 - ms@suse.de
- update Identity maps for all architectures (#327568)
* Thu Sep 20 2007 - ms@suse.de
- update Identity maps for all architectures (#309546)
* Thu Sep 20 2007 - ms@suse.de
- update hwinfo source
* Tue Sep 18 2007 - ms@suse.de
- update Identity maps for all architectures
* Mon Sep 17 2007 - ms@suse.de
- update translations
* Mon Sep 17 2007 - ms@suse.de
- fixed logitech evdev profiles. Thanks to
  Stefan Dirsch <sndirsch@suse.de> who provided the patch (#310305)
* Mon Sep 17 2007 - ms@suse.de
- fixed null pointer dereference in xapi (#320480)
* Thu Sep 13 2007 - ms@suse.de
- added patch from Michal Kubeƒçek <bh-novell@mk-sys.cz>
  fixing a buffer overflow in parse.yc,parse.l (#309009)
* Thu Sep 13 2007 - ms@suse.de
- added a check for the mode string of the bootloader update (#310115)
* Wed Sep 12 2007 - ms@suse.de
- added evdev logitech patch, adding vendor ID's (#262317)
- removed use of evdev for 0x046d:0xc041 (#262317)
* Wed Sep 12 2007 - ms@suse.de
- update hwinfo source
* Mon Sep 10 2007 - ms@suse.de
- sax2-libsax-perl is required by sax2-libsax (#309125)
* Fri Sep 07 2007 - ms@suse.de
- update hwinfo source
* Tue Sep 04 2007 - ms@suse.de
- update Identity maps for all architectures (#307218)
- update hwinfo source
* Fri Aug 31 2007 - ms@suse.de
- fixed build for stable
* Thu Aug 30 2007 - ms@suse.de
- update hwinfo source
- fixed broken modeline handling in ImportAPI.pm (#305890)
- added option DontZap *** against my wishes *** (#59038)
* Wed Aug 29 2007 - ms@suse.de
- fixed syntax bug in Xinerama.pl (#305051)
* Tue Aug 28 2007 - ms@suse.de
- added check for init stage Monitor entries (#304597)
* Mon Aug 27 2007 - ms@suse.de
- update hwinfo source
* Mon Aug 27 2007 - ms@suse.de
- fixed sax2 card selection (option -c)
* Fri Aug 24 2007 - ms@suse.de
- fixed monitor names. The " sign is not allowed here (#304142)
- update translations and hwinfo monitor edid block changes
* Thu Aug 23 2007 - ms@suse.de
- added EDID first modeline block (#287093)
* Thu Aug 23 2007 - ms@suse.de
- fixed keyboard model/type setup (#302107)
* Wed Aug 22 2007 - ms@suse.de
- fixed intel position setup (#270846)
* Tue Aug 21 2007 - ms@suse.de
- Include hwinfo changes
- fixed data record for USB gaming mouse (#262317)
- update Identity maps for all architectures (#300604)
- fixed randr plugin setup (#270846)
* Mon Aug 20 2007 - ms@suse.de
- fixed xkb options reset (#301040)
- fixed randr plugin detection (#270846)
* Sun Aug 19 2007 - ms@suse.de
- update translations
- update Identity maps for all architectures (#199376)
- added apply information, related to (#285603)
- added ibm-trackpoint profile (#301713)
* Mon Aug 13 2007 - ms@suse.de
- added dualhead support for new intel driver (#270846)
* Fri Aug 10 2007 - ms@suse.de
- fixed modeline validation (#287093)
- update Identity maps for all architectures (##299265)
- update translations and hwinfo source
* Sun Aug 05 2007 - ms@suse.de
- Include hwinfo changes
- update Identity maps for all architectures (#296006)
- Include libhd patch (#296964)
* Fri Jul 27 2007 - ms@suse.de
- Include hwinfo changes:
  fixed framebuffer color depth calculation (#294334)
* Fri Jul 20 2007 - ms@suse.de
- fixed broken X11 connection if framebuffer is in use (#292951)
- including missing XKB keys (#292951)
* Thu Jul 19 2007 - ms@suse.de
- moved documentation into devel package (#293020)
* Thu Jul 19 2007 - ms@suse.de
- added french/belgium Keyboard.map patch (#290355)
* Mon Jul 16 2007 - ms@suse.de
- fixed grub updater to change vga value to correct hex numbers
* Mon Jul 09 2007 - ms@suse.de
- update Identity maps for all architectures
- update translations
* Fri Jun 22 2007 - ms@suse.de
- fixed DDC resolution validation in libsax (#285232)
- include hwinfo changes
* Fri Jun 22 2007 - ms@suse.de
- fixed compiler warnings, cleanup code
- fixed some not all rpmlint warnings and errors
* Fri Jun 22 2007 - ms@suse.de
- added Samsung SyncMaster 940BW (#286595)
* Wed Jun 20 2007 - ms@suse.de
- reset xkb options before setting new one (#285603)
* Tue Jun 19 2007 - ms@suse.de
- added Samsung SyncMaster 226BW to CDB (#285111)
* Mon Jun 18 2007 - ms@suse.de
- more robust vga parameter handling in createGRB.pl
* Thu Jun 14 2007 - ms@suse.de
- removed 915resolution requirement for suse > 10.2 (#266362)
* Tue Jun 05 2007 - ms@suse.de
- fixed segfault during DDC/FBDEV resolution validation (#279951)
- fixed build (#280462)
* Mon Jun 04 2007 - ms@suse.de
- added /var/log/SaX.log as %%ghost config to spec file (#280462)
- fixed monitor detection for FSC AMILO Pro V3205 (#279920)
* Fri May 25 2007 - ms@suse.de
- disable resolution check in testX app (#273147)
* Thu May 24 2007 - ms@suse.de
- added grub updater (#277325)
* Mon May 21 2007 - ms@suse.de
- update x11 DB for use of intel driver (#214943)
- added Samsung 931C monitor into the CDB (#275595)
* Thu May 17 2007 - ms@suse.de
- include hwinfo changes
* Wed May 16 2007 - ms@suse.de
- do not calculate modelines if fbdev driver is used (#265498)
* Fri May 11 2007 - ms@suse.de
- include hwinfo changes
* Fri May 11 2007 - ms@suse.de
- don't use MouseManPlusPS/2 protocol (#271256)
- removed strings usage, replaced by grep -a and therefore
  removed binutils requirement (#273595)
* Fri May 11 2007 - ms@suse.de
- update Identity maps for all architectures (#271741)
* Tue May 08 2007 - schwab@suse.de
- Fix uninitialized data.
* Fri May 04 2007 - ms@suse.de
- include hwinfo changes
* Thu May 03 2007 - ms@suse.de
- update translations
- added support for reverse layout (right-to-left languages).
  The sax translation file offers a key named "I18N" which could
  be translated as "RTL". In this case the application will use
  the reverse layout for displaying the data (#268608)
* Thu May 03 2007 - ms@suse.de
- fixed info messages not to be showed if specific dialog is
  requested by option --dialog | -O (#269787)
* Tue May 01 2007 - ms@suse.de
- added opendir patch by Dieter J√§ger <dieter@linuxjaeger.de>
  libsax bug which causes an endless loop if there is no xfine
  cache directory
* Thu Apr 26 2007 - ms@suse.de
- disabled modeline calculation for nv driver (#230961)
- fixed monitor detection for Belinea 105075 (#261155)
* Fri Apr 20 2007 - ms@suse.de
- added MonitorLayout setup for radeon driver in xapi (#263199)
* Thu Apr 19 2007 - ms@suse.de
- fixed sax2 manual page (#266190)
* Tue Apr 17 2007 - ms@suse.de
- removed help button, not needed (#215213)
* Tue Apr 17 2007 - ms@suse.de
- removed X11 font server support (#262444)
- added radeon MonitorLayout patch by Matthias Hopf (#263199)
- update translations
* Mon Apr 16 2007 - ms@suse.de
- resolving symlinks in sysfs did not work properly
- updated X11 data
* Sat Apr 07 2007 - ms@suse.de
- update Identity maps for all architectures
* Fri Mar 30 2007 - rguenther@suse.de
- add flex and bison BuildRequires
* Fri Mar 30 2007 - ms@suse.de
- added evdev support for Logitech Bluetooth Mouse (mxlaser)
- fixed synaptics default configuration
* Wed Mar 28 2007 - ms@suse.de
- changed FingerHigh from 15 to 17 for alps touchpads (#179966)
* Wed Mar 28 2007 - ms@suse.de
- fixed invalid resolution lookup (#257323)
* Mon Mar 26 2007 - ms@suse.de
- added Intel driver options (#237676)
* Wed Mar 21 2007 - ms@suse.de
- add intel to checkForIntelBoard() (#250133)
* Mon Mar 19 2007 - ms@suse.de
- add eurosign: group to the XKB options setup (#181116)
* Fri Mar 16 2007 - ms@suse.de
- include libhd changed v13.29
* Tue Mar 13 2007 - ms@suse.de
- fixed WACOM:ISDV4 TABLET PC setup (#254161)
* Tue Mar 13 2007 - ms@suse.de
- fixed reading monitor disk/CD/DVD (#239608)
* Fri Mar 09 2007 - ms@suse.de
- changed driver map i810beta is now intel
* Thu Mar 08 2007 - ms@suse.de
- update identity maps for all architectures
* Tue Mar 06 2007 - ms@suse.de
- disabled loading of the v4l module (#229228)
* Fri Mar 02 2007 - ms@suse.de
- removed the PenabledTabletPC entry from the touchscreen
  section. This is in fact a tablet represented by the entry
  ISDV4 Tablet PC (Serial) within the Tablet section (#250587)
- fixed segfault when clicking in empty options box (#250564)
- added options for the fpit driver (#250564)
* Fri Mar 02 2007 - ms@suse.de
- include hwinfo changes v13.27
- added checkForIntelBoard() function to init sequence (#250133)
- by default 3D is enabled now for all cards (#250129)
- added FinePoint tablet support (#250151)
* Thu Mar 01 2007 - ms@suse.de
- changed X -> Xorg in Profile.pm
* Wed Feb 28 2007 - ms@suse.de
- include hwinfo changes v13.25
* Thu Feb 22 2007 - ms@suse.de
- include hwinfo changes v13.23
* Thu Feb 22 2007 - ms@suse.de
- update translations [hu,lt,ru,el,cs,et]
* Wed Feb 21 2007 - ms@suse.de
- added GARTSize option to Radeon driver profile (#247242)
* Tue Feb 20 2007 - ms@suse.de
- append Xaa options to getCardOptions() method (#245661)
* Mon Feb 19 2007 - ms@suse.de
- fixed overbound array access (#246655)
* Thu Feb 15 2007 - ms@suse.de
- include hwinfo changes v13.22
* Wed Feb 14 2007 - ms@suse.de
- fixed use of uninitialized bootDisplay pointer (#245396)
* Mon Feb 12 2007 - ms@suse.de
- added 3D warning message in case of unsupported graphics cards
  or unsupported multihead extensions like Xorg Xinerama (#242062)
* Mon Feb 12 2007 - ms@suse.de
- fixed build on beta
* Fri Feb 09 2007 - ms@suse.de
- added 1440x900 into LCD and VESA section (#242062)
* Thu Feb 08 2007 - ms@suse.de
- fixed monitor record for Fujitsu 21P4 (#237683)
* Thu Feb 01 2007 - ms@suse.de
- fixed sysp detection (#240693)
* Wed Jan 31 2007 - ms@suse.de
- fixed compiler warning
- prevent domain busid on probeonly call in sysp for ppc
  and don't set busid in case of fbdev
* Wed Jan 31 2007 - ms@suse.de
- ask X-Server if colordepth value is unknown, related to (#236670)
- added fbdev yast2 bootloader information to the logfile (#234142)
* Tue Jan 30 2007 - ms@suse.de
- fixed GUI crash if no default color depth is set (#233847)
- don't set default color depth on fbdev devices (#233847)
- don't set bus ID on fbdev devices (#233847)
* Mon Jan 29 2007 - ms@suse.de
- update Identity maps for all architectures (#236670)
- added patch from schwab@suse.de fixing usage of alps driver
  for Apple Internal Touchpad [ALPS]
* Mon Jan 29 2007 - ms@suse.de
- there are usb tapes (#222978)
- apple vs. pc kbd layout issue solved for ppc (#233968)
* Wed Jan 24 2007 - ms@suse.de
- added LinearAlloc profile for the i810 driver profile (#237674)
* Tue Jan 23 2007 - ms@suse.de
- added full set of options for i810 driver (#237674)
- added partial support for fbdev setup. YaST bootloader setup is
  missing [ not yet finished ] (#234142)
* Wed Jan 10 2007 - ms@suse.de
- update manual page [sax2]
* Tue Jan 09 2007 - ms@suse.de
- activated evdev support for Logitech-Gaming and Logitech-MediaPlay
  mouse. These devices have been tested successfully. General support
  for evdev instead of mouse is planed as soon as evdev is stable for
  all common devices (#231189)
* Mon Jan 08 2007 - ms@suse.de
- added ButtonMapping to support left hand mouse (#231081)
* Mon Jan 08 2007 - ms@suse.de
- added Bridge BM10A monitor to the CDB
- added 10.0 traversal support
* Thu Dec 21 2006 - ms@suse.de
- fixed tablet config and added wacom pad support (#229886)
* Tue Dec 19 2006 - ms@suse.de
- changed system of intel patch map, use black list of devices
  instead of a white list. For details see bug: (#201338)
* Mon Dec 18 2006 - ms@suse.de
- added BIOS patch for Dell Inspiron 510m (#223745)
- added BIOS patch for Dell Inspiron 640m (#223745)
- added BIOS patch for Intel 865 G / BenQ FP91GX (#229241)
* Wed Dec 13 2006 - ms@suse.de
- added support for TwinViewXineramaInfoOrder (#227592)
- fixed keyboard navigation and tablet import() flag (#228156)
- fixed WinINF monitor disk parser (#227393)
* Tue Dec 12 2006 - ms@suse.de
- fixed size information for MultiMedia Display (#227111)
* Mon Dec 11 2006 - ms@suse.de
- activate microsoftpro for all pc104/pc105 keyboards (#210881)
* Fri Dec 08 2006 - ms@suse.de
- added Samsung Syncmaster 940T into the CDB (#226746)
- added BIOS patch for Samsung X20 (#226756)
* Wed Dec 06 2006 - ms@suse.de
- added BIOS patch for Dell D620 device (#223745)
* Tue Dec 05 2006 - ms@suse.de
- Added DisplaySize setup for horizontal and vertical layout (#219468)
- If DDC data available show only DDC available resolutions plus two
  default resolutions which are 1024x768 and 800x600 (#223651)
- added BIOS patch for HP nc2400/nc6400/nx7400 (#226023)
* Thu Nov 30 2006 - ms@suse.de
- fixed internationalization of display tab naming (#218433)
* Wed Nov 29 2006 - ms@suse.de
- added Dell D820 to the IntelPatch.map (#224528)
* Tue Nov 28 2006 - ms@suse.de
- update libsax version to v7.3
- added libsax method to obtain the DDC resolution list
- activate memory detection again, XServer has been fixed (#215522)
- add missing XKB keys
* Tue Nov 28 2006 - ms@suse.de
- deactivate memory detection, workaround for bug (#220197)
- update i810 patch map for Thinpad R52 and GW MX6923b (#223745)
- deactivate HorizScrollDelta in synaptics driver (#223827)
* Mon Nov 27 2006 - ms@suse.de
- fixed keyboard selection (#223799)
* Fri Nov 24 2006 - ms@suse.de
- update Identity maps. Using i810beta for Intel 965G chipset
* Fri Nov 24 2006 - ms@suse.de
- update Identity maps. This change will activate 3D for some
  ATI graphics cards. 3D is activated using the opensource
  driver
* Thu Nov 23 2006 - ms@suse.de
- fixed .inf search procedure in WinINF.pm (#223449)
* Wed Nov 22 2006 - ms@suse.de
- fixed CDB data for Monitor Proview 713s (#223134)
* Tue Nov 21 2006 - ms@suse.de
- sometimes usb mice were missed (#216091)
* Mon Nov 20 2006 - ms@suse.de
- added tseng driver profile (#221987)
- update language translations
* Mon Nov 20 2006 - ms@suse.de
- added 18.0 Inch display diagonal value  (#222480)
- added HP LP2065 monitor to the database (#222500)
- update language translations
* Fri Nov 17 2006 - ms@suse.de
- fixed monitor specs for Samsung V550 (#221500)
* Wed Nov 15 2006 - ms@suse.de
- fixed monitor specs for DELL 2007FP (#219389)
- fixed monitor specs for DELL 1907FP (#220389)
* Wed Nov 15 2006 - ms@suse.de
- fixed intel patch map detection (#220725)
- fixed memory detection when multiple cards are installed
- prevent multiple probeonly calls if one is enough
* Tue Nov 14 2006 - ms@suse.de
- added patch map for Fujitsu Siemens Lifebook (#220725)
* Mon Nov 13 2006 - ms@suse.de
- changed xkb layout from microsoft to microsoftpro (#210881)
- removed dual head display size magic. It's wrong under certain
  circumstances like clone config or vertical layout (#219468)
- removed evdev driver support, driver is broken (#220237)
- added sanity check for the display size in sysp (#219801)
- fixed Intel (i810) clone setup (#219772)
- fixed X11 setup on ThinkPad T60p (#220171)
* Sat Nov 11 2006 - ms@suse.de
- remove Release/Version tag from subpackages
- fixed build on powerPC this fixes bugs: (#219400,#219405)
* Wed Nov 08 2006 - ms@suse.de
- fixed 3D support for Radeon R300 cards (#213029)
- fixed NoDDC profile (#218115)
* Tue Nov 07 2006 - ms@suse.de
- fixed NoDDC profile: do not include the same option twice
  even if called twice (#218115)
- allow up to four serial ports for tablets (#218730)
- changed dual-head setup of intel driver due to new options
- fixed sysp memory detection. sysp needs to create a temporary
  config file to be able to call X with probeonly. This was done
  for multiple cards only but it is now as well needed for a device
  count of 1. Additionally vt swtich is done for the complete
  duration of detection
- added missing XKB descriptions
- update translations
* Mon Nov 06 2006 - ms@suse.de
- added patch map for Intel Mac Mini's internal display (#218352)
- exchanged libhd framebuffer check to determine the videoram
  with a X11 probeonly call for the following reasons:
  1) framebuffer size != videoram
  2) framebuffer check could cause broken X11 screen which
  needs a repaint by chvt(1) -> chvt(7)
* Mon Nov 06 2006 - ms@suse.de
- fixed missing provides/obsoletes (#218235)
* Thu Nov 02 2006 - ms@suse.de
- update translations
* Mon Oct 30 2006 - ms@suse.de
- added plausibility check for ddc bandwidth data (#210923)
- fixed uninitialized bandwidth data if &MsgDetect == 0 (#195630)
* Mon Oct 30 2006 - ms@suse.de
- fixed desktop file (#216040)
* Fri Oct 27 2006 - ms@suse.de
- sometimes it's not really an usb mouse (#208745)
* Thu Oct 26 2006 - ms@suse.de
- FHS fix: changed location of dynamically changed profiles
* Mon Oct 23 2006 - ms@suse.de
- added plausibility check for monitor display size DDC data
* Sat Oct 21 2006 - ms@suse.de
- fixed --lowres mode, use 800x600 instead of 640x480. This
  seems to be save enough at a level of 60Hz (#214238)
* Fri Oct 20 2006 - ms@suse.de
- update hwinfo DDC detection, additional data evaluation code
* Mon Oct 16 2006 - ms@suse.de
- update CDB data for Apple 9C5E and Philips 200W
- fixed round error while calculating traversal length
* Mon Oct 16 2006 - ms@suse.de
- added Intel bios patch code for Mac Mini (#212547)
- use microsoft inet keyboard type as default for latin types (#210881)
- use libsax for creating automatic configuration
* Mon Oct 16 2006 - thoenig@suse.de
- add patch sax2-do-not-close-shared-connection-thoenig-01.patch:
  Do not close shared connection to the D-Bus system bus.
* Fri Oct 13 2006 - ms@suse.de
- update language translations
* Thu Oct 12 2006 - ms@suse.de
- update hwinfo patches (v13.7)
* Tue Oct 10 2006 - ms@suse.de
- changed patch bios requirement to 915resolution (#201338)
* Tue Oct 10 2006 - ms@suse.de
- added support for Multimedia Keyboards (#210881)
* Wed Sep 27 2006 - ms@suse.de
- include libhd fixes:
  - revert TIOCGDEV removal accidentally introduced in v13.0 (#201741)
  - remove adb code (#206648)
  - provide bogo mips value (#206649)
  - no edd unless x86 or x86_64 (#206654)
  - always use cpu emulation for BIOS calls (#207112)
  - on dell notebooks, scan 3 ports for monitor data (#162973)
  - include ACPI dump in log (#143090)
* Tue Sep 26 2006 - ms@suse.de
- fixed server detection in case of multiple devices.
  Several problems like uninitialized variables plus unmatched
  flex tokens caused this (#205371)
* Mon Sep 25 2006 - ms@suse.de
- fixed CD/DVD detection for utility disk feature (#177476)
* Mon Sep 25 2006 - ms@suse.de
- fixed location of pci.ids in libpci.a (#207615)
- changed root mouse cursor from watch to arrow (#207649)
* Fri Sep 22 2006 - ms@suse.de
- use libsax to check for IntelPatch.map (#201338)
- fixed security issue which allows a user to call
  any command from within a password entry field (#206669)
- fixed password setup in vnc setup (#206669)
* Thu Sep 21 2006 - ms@suse.de
- added support for patching bios using a white list of
  devices. The IntelPatch.map contains the command and
  the parameters to patch the bios (#201338)
* Tue Sep 12 2006 - ms@suse.de
- added DEL1000 monitor profile (#199011)
* Mon Sep 11 2006 - ms@suse.de
- use RPM_OPT_FLAGS for building
* Fri Sep 08 2006 - ms@suse.de
- added support for evdev driver. As this driver is very new
  enablement has done for one device only. Details see bug: (#159405)
* Thu Sep 07 2006 - ms@suse.de
- update to hwinfo 13.2, better modalias matching (#199112)
- update to hwinfo 13.2, more device files (#159405)
* Wed Sep 06 2006 - ms@suse.de
- update Identity maps for all archs (#199376)
* Mon Sep 04 2006 - ms@suse.de
- fixed .pro project file according to last include path
  fix from Stefan. Added -Iquote ...
* Sat Sep 02 2006 - sndirsch@suse.de
- sax2.diff:
  * finally removed issues when xf86Parser.h/xf86Optrec.h is
  installed to /usr/include/xorg
* Fri Sep 01 2006 - ms@suse.de
- changed VNC module naming and messages to prevent
  people from being confused about what VNC does in that case
* Fri Sep 01 2006 - ms@suse.de
- fixed dependency tree (cyclic dependency)
* Wed Aug 30 2006 - ms@suse.de
- added 2560x1600 QSXGA resolution (#202624)
- added DELL 3007 WFP Display into CDB (#202624)
* Wed Aug 30 2006 - ms@suse.de
- include hwinfo patch: missing x86 emulation (#188839)
- removed usage of libsysfs from sysp
* Mon Aug 28 2006 - ms@suse.de
- update monitor database (#143286)
- update monitor database (#199011)
- added better information if probing failed (#200146)
- fixed CloneRefresh setup for Intel driver (#201252)
- update translations from new lcn repository
* Sun Aug 27 2006 - ms@suse.de
- update CDB entries for Xorg v7 (#176265)
- added IgnoreABI option to FireGL profile
- fixed fontpath changes between Xorg v6 and v7 (.fontpath)
- update to hwinfo head branch code, took care for new dbus
  interface
* Thu Aug 24 2006 - thoenig@suse.de
- revert duplication of dbus_connection_unref as libdbus is now
  fixed
* Wed Aug 23 2006 - ro@suse.de
- work around dbus breakage
* Thu Aug 17 2006 - olh@suse.de
- quote variable with unpredictable content in build section
- build as unprivileged user
- disable sax2-libsax-java for everyone to fix build
* Sat Aug 12 2006 - thoenig@suse.de
- Add patch sax2-dbus-api-fix-thoenig-01.patch: Use
  dbus_connection_close, not dbus_connection_disconnect
* Fri Aug 11 2006 - sndirsch@suse.de
- sax2-xorg-fontpath.patch:
  * fixes wrong fontpath (Bug #198653)
* Wed Aug 09 2006 - ms@suse.de
- fixed fglrx driver option (Bug #191839)
- fixed include path to be able to build for older
  distributions too
* Mon Aug 07 2006 - aj@suse.de
- Fix build failure with xorg7 include files.
* Thu Jul 27 2006 - ms@suse.de
- fixed include path for build within xorg7
- fixed bracket quoting in profile scripts
* Mon Jul 10 2006 - ms@suse.de
- fixed locale dependant XKB options setup (#184796)
- fixed Intel profile patch from mhopf (#190678)
* Thu Jul 06 2006 - ms@suse.de
- adapt build requires for xorg-x11-server-sdk package (#190391)
- added fontpath check for Xorg v6/v7 basic font path  (#190391)
- disabled java binding from being build (#190391)
* Fri Jun 30 2006 - ms@suse.de
- update source code documentation from libhd head
- adapt build requires for new xorg-x11-server-sdk package (#190391)
- added fontpath check for Xorg v6/v7 basic font path (#190391)
- disabled java binding from being build (#190391)
* Mon Jun 26 2006 - ms@suse.de
- hd/monitor.c add sanity check to ddc parser (#186096)
* Fri Jun 23 2006 - ms@suse.de
- fixed CDB entry for 800x600@75Hz (#137878)
* Wed Jun 21 2006 - ms@suse.de
- changed testX to search the environment
* Mon Jun 19 2006 - ms@suse.de
- fixed alps default values (#164747)
* Mon Jun 19 2006 - ms@suse.de
- added new NVidia devices (#186055)
* Mon Jun 19 2006 - ms@suse.de
- fixed options setup while merging the config (#184540)
* Sun Jun 11 2006 - sndirsch@suse.de
- using multix variant for multilinugual canadian keyboard mapping
  by now; problems with non-existing ISO_Level5_* characters has
  been resolved in xorg-x11/xkeyboard-config (Bug #158951,
  X.Org Bug #4411)
* Fri Jun 09 2006 - sndirsch@suse.de
- added multilinugual canadian keyboard mapping (Bug #158951)
* Wed May 31 2006 - ms@suse.de
- update list of supported wacom devices (#173408)
- added popup message when fbdev driver is used (#168693)
- added support for detecting DDC modeline timings (#159071)
- added support for reduced timing modelines (#159071)
- added IQT L70S monitor to the monitor CDB (#179733)
- disable DAMAGE extension for fglrx based setup (#173180)
- added standard ViewSonic 15/17 Inch displays (#181660)
- fixed NoDDC profile to not overwrite current options (#167367)
- added support for SiS DualHead; Xinerama and MergedFB
- fixed display size detection for HP nc6000 laptop
- fixed spec file to use find_lang and other macros
* Tue May 30 2006 - ms@suse.de
- fixed nvidia binary check (#179934)
* Tue May 23 2006 - ms@suse.de
- added missing display size values (#176328)
- added CVT modeline calculation module (#159071)
- reverted patch for French-Canadian keyboard support. This was done
  because the related and needed patch for Xorg will not become
  applied, so it makes no sense to still have it in sax (#158951)
- added MBO e-Tron 998 monitor into the CDB
* Mon May 22 2006 - ms@suse.de
- fixed Identity entries for DELL ATI devices (#177561)
- fixed perl syntax using defined (#177476)
* Wed May 17 2006 - ms@suse.de
- fixed handling of "KernelModuleParm" option (#170698)
* Wed May 17 2006 - ms@suse.de
- added automatic detection of touchscreen for P1510 FSC laptop
- added display size data for LifeBook T4210
- added DefaultFbBpp support for 32 bit framebuffer
- changed size unit in sysp from cm -> mm
* Fri May 12 2006 - ms@suse.de
- new version sax2 v8 for xorg v7 (X11R7)
- added chvt support to monitor.c, change before calling int10
- added support for automatic tablet detection. Currently only one
  wacom device has been tested (Input.map -> 0x056a:0x0013).
  this is related to bug: (#163785)
- added cn-latin1 mapping for French Canadian Keyboard (#158951)
- enhancement: add wacom rotate option to be configured (#151448)
- enhancement: disable mouse device before probing (#157415)
- enhancement: moved libsax scripts (#157475)
- enhancement: added support for prefixed Identifiers usable within
  the sections Device, Modes, Monitor and Screen. If an Identifier
  contains a prefix like "prefix-name[id]" this section is preferred
  compared to the non prefixed "name[id]" section. SaX2 itself will
  not make use of prefixed Identifiers but other config tools which
  are changing the existing config file can include their own section
  with a prefixed Identifier (#164793)
- enhancement: added combined DisplaySize check
- enhancement: added xfs FontPath for default setup (#166113)
- enhancement: added support for translated xkb country names (#145477)
- enhancement: added support for monitor bandwidth detection (#159071)
- enhancement: added new modeline calculation scheme (#159071)
- moved XSetErrorHandler() to be active on first misc extensions
  call XF86MiscGetMouseSettings() in hw/mouse.c
- include libhd fixes: (#115845, #117639, #162209, #155357)
- added check for vnc password longer than eight chars (#168683)
- deactivate code to limit framebuffer mode to 60Hz (#167305)
- fixed mnemonic key for OK button (#168784)
- fixed Targa 3867-1 monitor data  (#170896)
* Mon May 08 2006 - ms@suse.de
- enhancement: added update directory for Identity.map files. A user
  can put a file named *Identity.map* file into the directory
  /usr/share/sax/sysp/maps/update. The map there is searched for
  graphics cards first. It is possible to add support for new
  cards or overwrite an old entry with this file (#163803)
* Tue May 02 2006 - ms@suse.de
- added support for Radeon XPRESS 200M (1002:5955) (#171118)
* Thu Apr 27 2006 - ms@suse.de
- update Identity maps for all archs supporting new
  NVidia CPUs (#166065, #159731, #168618, #167367)
* Tue Apr 25 2006 - ms@suse.de
- added obsoletes for sax2-tools packages on s390x (#145823)
* Tue Apr 25 2006 - ms@suse.de
- update monitor database (#167857)
* Mon Apr 24 2006 - ms@suse.de
- fixed Khmer keyboard layout setup (#164381)
- fixed DEL7002 monitor setup (#164901)
* Wed Apr 12 2006 - ms@suse.de
- update Identity map for all archs (#165387)
* Tue Apr 11 2006 - ms@suse.de
- update Identity map for ppc (#159510)
* Mon Apr 10 2006 - ms@suse.de
- update Identity maps for all archs (#163877)
- fixed quoting bug for secondary monitor name on laptops
* Wed Apr 05 2006 - ms@suse.de
- fixed display detection on Xgl server (#163027)
- added monitor Eizo Flexiscan L795 into the CDB (#163656)
- update Identity maps for all archs (#159815,#163016,#163507,#163483,#152336)
- added wacom options: TPCButton,ForceDevice and Button2
- fixed batch mode to allow equal sign in values using __EQUALS__
- fixed batch mode documentation
* Mon Apr 03 2006 - ms@suse.de
- update Identity maps for all archs (#160812,#156015)
- add option KernelModuleParm to FireGL profile (#162495)
* Mon Mar 27 2006 - ms@suse.de
- update Identity maps for all archs (#147070,#160833)
* Mon Mar 27 2006 - ms@suse.de
- revert to old bus ID scheme (#139477)
* Mon Mar 27 2006 - ms@suse.de
- update Identity maps for all archs (#156015)
- update translations from Dublin [fi,pl_PL]
* Fri Mar 24 2006 - ms@suse.de
- update translations from Dublin [pt,km]
* Wed Mar 22 2006 - ms@suse.de
- fixed MonitorLayout setup for radeon X700 chip (#159391)
* Wed Mar 22 2006 - ms@suse.de
- update monitor CDB, added Apple Cinema Display 20
* Tue Mar 21 2006 - ms@suse.de
- removed EnablePrivateBackZ from FireGL profile (#159343)
- update translations hu from Dublin
* Mon Mar 20 2006 - ms@suse.de
- added support for korean keyboard detection (#87443)
- include libhd fixes (#117053,#154725,#104300)
* Fri Mar 17 2006 - ms@suse.de
- fixed default value of RealDevice in lib/hw/mouse.c (#157415)
- fixed build for ppc64, csharp binding builds there now
* Thu Mar 16 2006 - ms@suse.de
- update Identity maps for all archs (#157399, #147070)
- fixed sax2-tools spec file requirements (#145823)
- fixed csharp language binding
* Tue Mar 14 2006 - ms@suse.de
- fixed Identity Raw handling in Options.pm (#157995)
- limit monitor detection to 2 ports (#155018)
- assigning to HAL udi could go wrong (#155018)
* Tue Mar 14 2006 - ms@suse.de
- fixed calling sax2 shell if option -a is set and no GUI
  package (sax2-gui) is installed (#157342)
* Mon Mar 13 2006 - ms@suse.de
- update Identity maps for all architectures (#146261,#155323)
- fixed default value for DisplaySize 15' 4/3 (#156462)
- fixed display detection for ASUS MV6 laptop (#157587)
- fixed parsing of MetaModes/ConnectedMonitor in ParseConfig.pm
- fixed MetaModes setup for Nvidia based Laptops
- set highest priority of colordepth setup to GUI value
* Thu Mar 09 2006 - ms@suse.de
- fixed sysp compiler flags -DXORG_LOADER (#157019)
- removed 32 bit from being used in Screen sections (#136363)
- fixed Intel_DualHead Xinerama based setup. Intel Xinerama based
  setups needs including the MonitorLayout too (#136363)
- remove remaining perror()s so we don't write to stderr (#155132)
- don't map video bios ram; not really neccessary (#155132)
* Wed Mar 08 2006 - ms@suse.de
- update Identity maps for all architectures (#156015)
- added libhd sanity check for display size data (#155096)
- fixed parsing of MetaModes/ConnectedMonitor in ParseConfig.pm
- fixed 855resolution requirement, for ix86 and x86_64 only
* Mon Mar 06 2006 - ms@suse.de
- fixed sysp detection if /dev/fb0 doesn't exist (#154101)
- fixed display size detection on Toshiba Satellite M30X (#155096)
- fixed sysp detection if no max h/vsync's provided by DDC (#155317)
- fixed sysp detection on vmware workstation v5.5 (#154775)
- update Identity maps for all architectures (#154101)
- update translations fi,ja,km from Dublin
* Mon Feb 27 2006 - ms@suse.de
- fixed libhd bug detecting mice on older 2.6 kernels
- removed check-build.sh, no longer used
* Mon Feb 27 2006 - ms@suse.de
- fixed vmware X11 configuration setup (#148271)
- removed Modelines from being used with nvidia driver (#153596)
- fixed CDB entry for monitor HighScreen MS 1795P (#153596)
- fixed libsax / ParseConfig.pm MetaModes option handling
- update translations fi,hu,nb from Dublin
* Thu Feb 23 2006 - ms@suse.de
- fixed setup for Hyundai L50S monitor (#152336)
- fixed sysp detection if there are multiple cards installed
* Wed Feb 22 2006 - ms@suse.de
- fixed setup for Sony SDM-P234 (ANALOG) monitor (#152769)
- fixed monitor setup to include CDB modelines if set (#152769)
* Mon Feb 20 2006 - ms@suse.de
- fixed monitor detection for IVM 451 (#149038)
- added bios data for LifeBook S 7020 display (#151867)
- added bios data for Samsung SX 20 S display (#151867)
- reinitialize cache if called in tty console mode
- adapt Xinerama profiles to make use of the DDC[2] data
- adapt MergedFB profiles to make use of the DDC[2] data
- added support for DDC[2] dual head information
* Thu Feb 16 2006 - ms@suse.de
- added support for Philips 200W 1680x1050 widescreen TFT (#152466)
* Tue Feb 14 2006 - ms@suse.de
- removed initial Modelines from fglrx setup
- fixed 64bit binary driver detection problems
* Mon Feb 13 2006 - ms@suse.de
- fixed wrong graphics card detection (#150246)
- update Identity maps for all architectures
- improved DDC detection [snwint]
- update translations from Dublin
* Thu Feb 09 2006 - ms@suse.de
- fixed kill call in sax.sh killProc() function (#149114)
- activate VIDEOBIOS_PATCH if VIDEOBIOS_PARAMETERS are set (#105898)
- fixed and activated build for s390x (#145823)
- improved DisplaySize detection for NoteBooks
* Tue Feb 07 2006 - ms@suse.de
- fixed RawDef options handling for comments (#148292)
- fixed sort order of color depth values (#148697)
- added libsax method getResolutionFromServer(). This allows
  asking the server for its currently used resolution. If the
  resolution isn't set within xorg.conf the SaX2 GUI will call
  the new method to ask the server for the resolution. ATI's
  binary driver required that method. (#148737)
- added Fujitsu LifeBook P-Series Touchscreen configuration data
- added LCD 1024x600@60Hz entry
- update translations from Dublin
* Tue Feb 07 2006 - ms@suse.de
- fixed GUI listbox keyboard handling (#148340)
- fixed CDB monitor entry: Nokia:445XiPlus (#145245)
- disable Composite extension. This extension is broken on
  ati,nv,nvidia,fglrx and has therefore no business case. It is
  only creating tons of bugs (#118580) (#148475)
- added khmer keyboard configuration (#143348)
* Fri Feb 03 2006 - ms@suse.de
- adapt XKB options due to changes in libxkbfile
* Wed Feb 01 2006 - ms@suse.de
- fixed mapping table for nvidia driver (#147097)
- fixed sysp detection on vmware machines (#147566)
- fixed sysp DisplaySize detection for incomplete DDC records (#148228)
- update Identity maps for all architectures
- update translations for hu,km,cs,fi and nb locales
* Tue Jan 31 2006 - ms@suse.de
- fixed build
* Sat Jan 28 2006 - ms@suse.de
- added support for Wacom PenabledTabletPC (#145974)
- added CDB entry for Monitor: PHILIPS 200P4 (#146063)
- fixed xgettext locale setup, unterminated strings (#145842)
- prepare build on s390x (#145823)
- fixed ChipId and ChipRev setup (#146455)
- update translations, added new translation (sax.km.po)
- update Identity maps for all architectures
* Fri Jan 27 2006 - mls@suse.de
- converted neededforbuild to BuildRequires
* Tue Jan 24 2006 - ms@suse.de
- added 1920x1440 resolution to the list of resolutions (#144801)
- added check for broken vidmode extensions (#144996)
* Mon Jan 23 2006 - ms@suse.de
- update Identity maps for all architectures
* Fri Jan 20 2006 - ms@suse.de
- fixed NVidia TwinView MetaModes/ConnectedMonitor setup
* Tue Jan 17 2006 - ms@suse.de
- added special case handling for the Radeon X700 card (#120424)
- added support for inverting mouse axis (#142569)
- added hwinfo patch from Steffen Winterfeldt <snwint@suse.de>
  which fixes DDC detection on mulihead cards, great job Steffen !
- fixed RawDef option handling from Identity map RAW entries
- fixed Profile.pm to call X -probeonly only if needed
- update Identity maps for all architectures
* Mon Jan 16 2006 - ms@suse.de
- prepared sax2 to be able to parse/create multiseat configurations.
  This feature is up to now not implemented completely (#142657)
- fixed NVidia MetaModes setup according to information from
  Andy Ritger <aritger@nvidia.com>
* Fri Jan 13 2006 - ms@suse.de
- added timestamp check for binary only drivers
- added 1280x800@60Hz to the LCD section of the CDB
- fixed sysp detection -> better check for laptop hardware
- fixed sysp detection -> use framebuffer timing if display.vesacount==0
- added support for external VGA on nvidia laptops, Thanks to
  Matthias Hopf <mhopf@suse.de> providing the profile code
- update all translations from Dublin
* Thu Jan 12 2006 - ms@suse.de
- changed sysp mouse detection to know about the real device (#142657)
- changed sysp keyboard detection to detect all keyboards (#142657)
- enable calculation of modelines for the nv driver
- fixed MetaModes setup for laptop users
* Mon Jan 09 2006 - ms@suse.de
- removed runlevel check on startup (#135605)
- added DELL monitors to the database (#141720,#141721)
- renamed vnc password program used for sax (#142466)
- exchanged /usr/X11R6/include with /usr/include/X11 where possible.
  First step to be prepared for xorg7 (X11R7)
- allow commandline help to be showed without root privileges
- update translations from Dublin [ nb,nl,hu,cs ]
- update CardModules to the current option table of all drivers using
  the X -showopts feature
- removed post-arch fixes from qmake generated Makefile. qmake
  handles it correctly now
* Thu Jan 05 2006 - ms@suse.de
- removed usage of xsload and setup a static options file again
* Wed Jan 04 2006 - ms@suse.de
- removed vmware profile to be applied automatically (#140139)
* Tue Jan 03 2006 - ms@suse.de
- fixed example section in the libsax documentation
* Tue Dec 27 2005 - ms@suse.de
- fixed PCI detection for multiple cards (#135729)
* Wed Dec 21 2005 - ms@suse.de
- fixed monitor specs for VISIONMASTER PRO 452
* Wed Dec 14 2005 - ms@suse.de
- added experimental support for Acecad flair tablets (#137970)
- added experimental XKB Options setup (#137109)
* Tue Dec 13 2005 - ms@suse.de
- removed static size setup from wacom configuration (#138297)
* Mon Dec 12 2005 - ms@suse.de
- added workaround for xsload bug (#137374)
* Fri Dec 09 2005 - ms@suse.de
- removed InputDevices setup from Files section (#136674)
* Thu Dec 08 2005 - ms@suse.de
- update Identity maps for all architectures (#127510)
- include hwinfo changes to detect the correct input device
  names. This was broken due to changes in sysfs (#134032)
* Wed Dec 07 2005 - ms@suse.de
- update german translation, fixed fuzzy in topic line
* Tue Dec 06 2005 - ms@suse.de
- added setDisplayRatioAndTraversal() getDisplayRatio() and
  getDisplayTraversal() methods to libsax for easy DisplaySize
  setup implemented in yast2-x11 (#136330)
* Fri Dec 02 2005 - ms@suse.de
- added option --complete to the cards.pl helper, this option will
  include the PCI vendor/subvendor ID's if specified
- removed old code refering to rc.config handling
- update FireGL driver profile
* Thu Dec 01 2005 - ms@suse.de
- fixed Profile.pm module for Intel DualHead setup (#136363)
- added ProfileCreatePreliminaryConfig() function to Profile.pm
  to be able to run probeonly servers. This is needed to check
  for the MonitorLayout on Intel and NVidia devices (#136363)
* Thu Dec 01 2005 - ms@suse.de
- added SiS driver profile to support the control protocol
- fixed vendor check in new Profile.pm module
- fixed Driver.map for radeon devices
* Wed Nov 30 2005 - ms@suse.de
- fixed vbios.c not to crash if hd_list returns null (#136070)
* Wed Nov 30 2005 - ms@suse.de
- added EnablePrivateBackZ for FireGL devices (#135708)
- disable Composite extension for FireGL and NVidia (#135708,#118580)
- fixed Radeon_DualHead_DriverOptions profile to know about the
  FGLRX_DualHead_DriverOptions profile when needed (#135708)
- fixed bug including an option multiple times
- applied sax2-rpath.dif from Michael Schroeder <mls@suse.de>
* Mon Nov 28 2005 - ms@suse.de
- remove FGLRX_SingleHead profile (#135708)
- added basic support for Relative layout setup (#135539)
- include more buildin comfort into Xi DualHead profiles
- create preliminary registry before calling profiles
* Tue Nov 22 2005 - ms@suse.de
- removed old nvidia profile and added the information to the new
  driver profile NVidia/NVidia.pl. Update CDB as well (#127510)
- add support for Matrox merged framebuffer (G450/G550) (#135423)
- fixed virtual screen resolution setup (#135423)
- use QTDIR environment for build process
* Sun Nov 20 2005 - ms@suse.de
- fixed MonitorLayout setup for intel devices (#134378)
- fixed SaX2 init not to enter the batch mode accidently (#134623)
- added Schwab SGI monitors 1920x1200 from SONY (#134496)
- add a copy of libhd to the repository to support non SuSE dists
- removed Modes Option from the Intel MergedFB option set.
  This option is no longer available within the intel driver
- libsax version 7.2
* Thu Nov 17 2005 - ms@suse.de
- improved modeline calculation in ImportAPI.pm
- update Identity maps for all architectures
- fixed widget layout to display card and monitor correctly
- fixed driver options profiles to know about laptop hardware.
  If not applied to a laptop the first monitor data is copied
  to the second head. If applied on a laptop the second channel
  will be set to 1024x768@60Hz which is the default resolution
  used for a projector
* Wed Nov 16 2005 - ms@suse.de
- added Intel_DualHead profile for Xinerama configuration (#133828)
- added Samtron 94v to the monitor CDB (#132640)
- removed exception in xc.pl which causes the modules glx,dri NOT
  to be included into the temporary X configuration used by SaX
  to start its own X-Server. There are configurations which requires
  the modules to be loaded otherwise the server will not start at all
- added ia64 devices for testing purposes
* Tue Nov 15 2005 - ms@suse.de
- simplify manual build process:
	- update doc README including better compile instructions
	- removed static nvidia binary due to license issues
	- include SVN release number to SaX.log file
	- removed additional pixmap source, one tarball only
* Mon Nov 14 2005 - ms@suse.de
- added colon to be allowed letters for BusID values (#133236)
- added domain information to BusID setup (#133236)
* Fri Nov 11 2005 - ms@suse.de
- fixed build environment; usage of autoconf and automake
- added hwinfo requirement to sax2 base package
* Thu Nov 10 2005 - ms@suse.de
- fixed RawData option handling, bug in removeRawItem of libsax
* Thu Nov 10 2005 - ms@suse.de
- fixed value for AllowMouseOpenFail setup (#132055)
- update header information to include sax-user mailing list
* Wed Nov 09 2005 - ms@suse.de
- update monitor database (#132640) (#131959)
- fixed typo in startup/sax.sh and sax-man/sax2 (#132882)
* Mon Nov 07 2005 - ms@suse.de
- change wheel default value in lib/hw/mouse.c to always provide
  a wheel even if the currently connected mouse doesn't have a
  wheel. This is because to support mice connected later to the
  system providing a wheel (#118745)
- fixed build of doxygen docu. A plugin configuration for dot
  has to be created first. This is done by calling dot -c
- removed sax2-libsax-mono binding from build because code
  generated by swig doesn't compile with new compiler
* Fri Oct 28 2005 - ms@suse.de
- fixed button mapping for logitech devices; freedesktop Bug (#4915)
* Fri Oct 28 2005 - ms@suse.de
- added LCD TV resolution 1366x768 to the list (#130964)
* Thu Oct 27 2005 - ms@suse.de
- added DELL Ultrasharp 1905FP to the CDB (#130724)
- update translations from dublin
* Wed Oct 26 2005 - ms@suse.de
- fixed framebuffer configuration using new FbSet module
* Tue Oct 25 2005 - ms@suse.de
- enabled HeaderCheck to not read files not written by SaX
- added helpfull startup messages
* Mon Oct 24 2005 - ms@suse.de
- fixed broken libsax removeCardOption() method
- fixed regexp quoting bug (#130152)
* Fri Oct 21 2005 - ms@suse.de
- fixed HP P720 entry, wrong DDC ID (#129921)
* Fri Oct 21 2005 - ms@suse.de
- fixed enable3D function in libsax, do not set wrong colordepth
- fixed md5sum check which prevent writing the configuration
  because someone changed the output format of md5sum
* Fri Oct 21 2005 - ms@suse.de
- changed NVidia_DualHead_DriverOptions from bash to perl
- fixed profile name NoDCC to NoDDC
* Mon Oct 17 2005 - ms@suse.de
- fixed positioning of windows to the middle if clone mode is active
- activate framebuffer timing patch again to use the framebuffer
  timings but decrease to a 60Hz mode
* Mon Oct 17 2005 - ms@suse.de
- increased startup speed a lot
* Thu Oct 13 2005 - ms@suse.de
- added busy cursor handling to xw.c
- added monitor VideoSeven L19B to the database
- added resolution 1024x600
* Wed Oct 12 2005 - ms@suse.de
- added global exception (saxGlobalException) to libsax, to be
  called in case of exception no matter what exception it was
* Mon Oct 10 2005 - ms@suse.de
- added LCD 1280x600 entry to the MonitorResolution [#115271]
- added support for automatic configuration of ATI binary driver (fglrx)
- changed DriverOptions profiles to use 1024x768@60Hz by default
- removed tightvnc requirement, use my own vncpasswd program
- removed fbset requirement, use my own ioctl implementation
- removed perl-gettext requirement, no longer needed
- moved xmset and xupdate to new package (xmset)
* Wed Oct 05 2005 - ms@suse.de
- fixed tty check in SaX2 startup script (#117439)
- fixed minValue for mouse emulation (#119950)
- fixed default resolution setup in xstuff.c (#118361)
- added Compaq MV920 monitor to the CDB (#119797)
- fixed LCD sections in the database (CDB)
- fixed perl binding typemap for hash types
* Fri Sep 30 2005 - stbinner@suse.de
- fixed capitalization in .desktop file
* Mon Sep 26 2005 - ms@suse.de
- activate Composite extension by default (#118580)
- fixed hsync range of 1024x768@85Hz entry (#118360)
- added SaX Profile parser, new system based on lex&yacc
  This one has been implemented according to the feature database
- disabled code for graphical 3D question from sysp
* Thu Sep 15 2005 - ms@suse.de
- added Dell 2405 widescreen TFT to the CDB (#117848)
- implement VBIOS patch support (#105898)
- implement enhancement part 4) from Bug: (#113344)
- implement enhancement part 3) from Bug: (#113344)
- fixed name clash for whois (#117228)
- fixed monitor entry for Dell 2001FP (#117290)
- added Nokia 920C monitor data to the CDB (#117638)
- added Samsung SyncMaster 406 monitor to the CDB (#117657)
- fixed getDualHeadProfile() method to recognize if the driver
  has changed by the user. The returned profile additionally depends
  on the driver changed by the user. Related to bug: (#113344)
- implement enhancement part 1) and 2) from Bug: (#113344)
- did some code cleanup, removed old tools fixed warnings
- allow multiple profiles to be set using the option -b
- update Identity maps for alle architectures
- removed similarity check in detect/Monitor.pm. According to
  some Bugs like for example #117657 this check will lead to a
  misdetected monitor.
* Tue Sep 13 2005 - ms@suse.de
- added support for another IBook G4 display (#116594)
- fixed PPC keyboard detection to use pc105 as default model (#116946)
- switched off using xmset for enabled/disable mouse in init.pl (#116946)
- fixed monitor display bug crashing sax if an unknown resolution
  key is used. This happens for example on 720x400 displays (#116946)
* Mon Sep 12 2005 - ms@suse.de
- added support for another IBook G4 display (#116481)
- fixed keyboard map for swiss and french people (#116019)
- fixed firegl dualhead setup (#116573)
* Mon Sep 12 2005 - ms@suse.de
- added Samsung SyncMaster 793 MB to the CDB (#116024)
* Fri Sep 09 2005 - ms@suse.de
- fixed default depth setup on Power-PC (#115673)
- added support for IBook G4 display (#116097)
- fixed wrong message text adjustment code (#114872)
* Fri Sep 09 2005 - ro@suse.de
- added check-build.sh
  (require 2.6.13 or newer on buildhost, otherwise mono may hang)
* Wed Sep 07 2005 - ms@suse.de
- fixed VBIOS detection due to hwinfo semantic changes (#115567)
- fixed libsax using SYSP import information (#115236)
- added support for Sony-Vaio GRT995MP display (#53635)
* Mon Sep 05 2005 - ms@suse.de
- update Identity maps according to bug (#114794)
* Mon Sep 05 2005 - ms@suse.de
- fixed APPLE color LCD CDB settings (#115151)
* Mon Sep 05 2005 - ms@suse.de
- fixed segfault while clicking in empty options box [#115162]
- added LCD 1280x600 section to the CDB [#115271]
- fixed APPLE LCD entry to support 1024x768 [#115151]
- update translations from dublin
* Thu Sep 01 2005 - ms@suse.de
- added support for SYNCMASTER 910T (#113795)
- added support for APPLE Color LCD (#113795)
- removed radiobutton for traditional mode while setting up
  MergedFB. The traditional mode is not supported by the drivers
  implementing MergedFB [#113344]
- update translations from dublin [uk,bg,pa,pl_PL,nl,lt,cs,sl_SI]
- update Identity maps for all architectures [#102976]
* Wed Aug 31 2005 - ro@suse.de
- remove config-dist.sh completely
- add ExcludeArch s390 s390x to specfile (last function from above)
* Wed Aug 31 2005 - ro@suse.de
- ia64 has mono, remove extra hacks for ia64
* Thu Aug 25 2005 - ms@suse.de
- fixed slovene keyboard layout in Keyboard.map (#112614)
- fixed keyboard toggle setup (#105420)
- update Identity maps for all architectures
* Tue Aug 23 2005 - ms@suse.de
- added Modes2 handling for fglrx driver (#65350)
- fixed FGLRX_DualHead_DriverOptions profile (#65350)
- added Intel_DualHead_DriverOptions.sh dynamic part (#65350)
- fixed button spacing for fixed-width widgets
- fixed I18n support for libsax error messages
* Wed Aug 17 2005 - ms@suse.de
- fixed keyboard toggle setup (#105420)
- fixed appearance of scrolling pixmaps in mouse test field (#105781)
- fixed CDB entries for various SAMSUNG monitors (#105577) (#105565)
- include information box if VNC has been activated to setup the
  needed RFB and HTTP ports within the firewall setup as well (#105628)
- fixed FireGL profile to use internal mode pool only
- fixed mouse detection on ppc64, button detection with libhd
  is failing (#102743)
- update translations from dublin
* Mon Aug 15 2005 - ms@suse.de
- fixed MetaModes handling according to bug (#104728)
- fixed resolution setup while importing primary resolution (#104728)
- fixed modeline check and RAM-dac sort order (#105208)
- fixed CDB resolution setup for ADI:MICROSCAN G710 (#105156)
- fixed hwupdate id handling (#105207)
* Mon Aug 15 2005 - ms@suse.de
- update Identity maps for all architectures
- update translations [fi,pa]
* Fri Aug 12 2005 - ms@suse.de
- enabled full featured MetaModes handling (#104069)
- update Identity maps for all architectures
- update translations [pl_PL,el_GR,pt]
* Thu Aug 11 2005 - ms@suse.de
- fixed libsax segfault in pointers.cpp
- fixed libsax import code for addID() calls
- fixed wacom tablet configuration, add Graphire-3 support
- fixed Portuguese keyboard setup (removed nodeadkeys) (#103716)
* Wed Aug 10 2005 - ms@suse.de
- fixed vmware X11 configuration using SaX/libsax (#99358)
- fixed emulate wheel handling (#102656)
- fixed alps touchpad default settings (AccelFactor,MaxSpeed)
- according to seife we set the default display size to 300x230
  this enables all people to at least see the fonts in an
  acceptable size. This size applies only if the diplay size
  wasn't detected automatically
* Tue Aug 09 2005 - ms@suse.de
- update ident maps for all architectures
- update translations from dublin: nb hu lt cs
- fixed Keyboard.map to use _qwerty layouts as XKB variant (#102657)
- fixed Keyboard.map use gr instead of el (#102657)
- fixed sysp 3D check (xcmd) to work xorg.conf independently (#99694)
- fixed modeline calculation on second head
- fixed option list; Don't show options handled by dualhead profile
- added default mouse if needed (#102656)
- fixed build for ppc64
* Fri Jul 29 2005 - ms@suse.de
- added language nl and pl_PL from Dublin
- remove mystic XKB options tab, there is no real documenation
  about all the options and the translators as well as the users
  don't know what to do with the information provided there.
* Thu Jul 28 2005 - ms@suse.de
- fixed sysp path in synaptics.sh profile script (#99019)
- removed ImageMagick requirement use netpbm instead
* Tue Jul 26 2005 - ms@suse.de
- fixed FGLRX dualhead driver options profile
- added new methods for set/add/remove special modelines in libsax
- prepared GUI for MetaModes handling including multiple resolutions
- added modeline calculation for special modelines needed in
  combination with dualhead device driver options
- update Identitiy maps, 3D support for i915GM chips
- changed default dual-head mode to clone, to avoid missing windows
  if the user didn't connect both monitors
- fixed DesktopSetup Option of FGLRX dualhead configuration
- added uk and pt translations from Dublin
- fixed battery check in libsax
* Mon Jul 25 2005 - ms@suse.de
- added fi -> Finnish translations from Dublin
- fixed manual driver setup. If the option -m is used to set
  the driver name it is not allowed to reset that driver by
  libsax. libsax will now check the CDB driver name with the
  optional driver name set manually and prefer the manually set
  one in any case. An exception is thrown in the case where the
  currently used driver does not fit the CDB driver(s).
* Fri Jul 22 2005 - ms@suse.de
- update Identity maps for [ix86,x86_64,ppc]
* Wed Jul 20 2005 - ms@suse.de
- added icons for dualhead arrangement from Ken <wimer@suse.de>
* Tue Jul 19 2005 - ms@suse.de
- added dynamic profile support to createPRO libsax script
* Tue Jul 19 2005 - ms@suse.de
- adding translations from dublin for [bg,hu,cs]
- fixed build for ia64 and ppc64
* Mon Jul 18 2005 - ms@suse.de
- added new icons from Kenneth Wimer <wimer@suse.de>
- fixed pixmap path for tooltip help
* Fri Jul 15 2005 - ms@suse.de
- update icons from Kenneth Wimer <wimer@suse.de>
- fixed environment for doInit() call within libsax
- fixed mouse detection if libhd reports a mouse with buttons=0
* Thu Jul 14 2005 - ms@suse.de
- added automatic answer to the 3D question for the sysp
  profile call in pci.pl
* Thu Jul 14 2005 - ms@suse.de
- fixed nvidia TwinView Orientation options
* Fri Jul 08 2005 - ms@suse.de
- fixed build
* Thu Jul 07 2005 - ms@suse.de
- deactivate libhd hardware flags check in needInit()
- add /usr/X11R6/bin/sax2 link to run sax2 as "x11"user
- enable xapi commandline support -- not finished yet
* Tue Jul 05 2005 - ms@suse.de
- activate new GUI system and library management (libsax)
* Wed Jun 29 2005 - ms@suse.de
- update ident maps for all architectures
- update translation key file, modified by rwalter@suse.de
- introduce new GUI system based on libsax
- introduce new libsax library system
* Tue Jun 14 2005 - ro@suse.de
- added sysfsutils to nfb
* Wed Apr 20 2005 - ms@suse.de
- added fbdev rgba color check to detect/Resolution.pm
- fixed Portuguese keyboard setup
* Mon Apr 18 2005 - ms@suse.de
- added widescreen resolutions.
  details: http://www.i386.info/vidres.htm
* Wed Apr 13 2005 - ms@suse.de
- added FGLRX single head profile (#76890)
* Mon Apr 11 2005 - ms@suse.de
- fixed variant of greek keyboard layout (#75911)
- removed mem-check in Resolution.pm
* Fri Apr 08 2005 - ms@suse.de
- added extended mouse devices (#63632)
* Thu Apr 07 2005 - ms@suse.de
- call checkDetected() only if useHwData is true
- added 5/4 aspect/ratio (#76702)
* Wed Apr 06 2005 - ms@suse.de
- fixed InputDevice setting for special device names (#74239)
- changed default mouse device in xmset (#66989)
* Mon Apr 04 2005 - ms@suse.de
- added DELL 2001FP to the CDB (#74541)
* Wed Mar 23 2005 - ms@suse.de
- fixed SendCoreEvents setting for ttySX devices (#74239)
* Tue Mar 22 2005 - ms@suse.de
- added support for XGI Volari cards (#74130)
* Mon Mar 21 2005 - ms@suse.de
- update Identity maps for [i386,x86_64] (#74022)
* Sat Mar 19 2005 - ms@suse.de
- added NoDDC driver profile to i810 driver (#71198)
* Fri Mar 18 2005 - ms@suse.de
- added Hitachi Widescreen display (#73830)
* Fri Mar 18 2005 - ms@suse.de
- update Identity maps for [i386,x86_64]
* Wed Mar 16 2005 - ms@suse.de
- added language translations for: it,ja,zh_CN,zh_TW
- update Identity maps for [i386,x86_64]
* Tue Mar 15 2005 - ms@suse.de
- added LCD check again to not overwrite hwinfo LCD data (#67214)
- removed linking against GL library for xapi
* Mon Mar 14 2005 - ms@suse.de
- update monitor database
- added translations for languages: nl,hu
* Fri Mar 11 2005 - ms@suse.de
- added new language translation [uk]
* Fri Mar 11 2005 - ms@suse.de
- fixed acceleration factor for ALPS based touchpads (#72046)
* Fri Mar 11 2005 - ms@suse.de
- added translations for the following languages:
  CS,BG,DE,EL_GR,ES,SK
* Thu Mar 10 2005 - ms@suse.de
- added HP zv5000 monitor to the database (#71797)
* Wed Mar 09 2005 - ms@suse.de
- fixed image file format of sax2.xpm (#71793)
* Mon Mar 07 2005 - ms@suse.de
- don't use Bitstream fonts in documentation
* Mon Mar 07 2005 - ms@suse.de
- update Identity maps for [i386,x86_64]
* Fri Mar 04 2005 - ms@suse.de
- added [lt] translations
* Thu Mar 03 2005 - ms@suse.de
- update Identity maps for [i386,x86_64]
* Mon Feb 28 2005 - ms@suse.de
- fixed Dell D600 panel size setting (#66679)
* Mon Feb 28 2005 - ms@suse.de
- fixed mouse wheel detection (#65722)
* Fri Feb 25 2005 - ms@suse.de
- fixed monitor detection, framebuffer settings aren't used
  if hwinfo --monitor reports noting
* Tue Feb 22 2005 - ms@suse.de
- From <ro@suse.de>: added wireless-tools to nfb (for libhd)
* Fri Feb 18 2005 - ms@suse.de
- fixed libhd version check
* Thu Feb 17 2005 - ms@suse.de
- fixed mouse detection for multiple multiplexed devices on
  the same input device but with different properties (#50722)
* Thu Feb 17 2005 - ms@suse.de
- fixed typo in translations (#50987)
* Thu Feb 17 2005 - ms@suse.de
- fixed state of ratio/traversal widgets if no DisplaySize
  has been detected
* Mon Feb 14 2005 - ms@suse.de
- update CDB monitor database
- update CDB card database (fglrx update)
* Mon Jan 31 2005 - ms@suse.de
- fixed dynamic profile invocation for InputDevice's (#50323)
* Wed Jan 26 2005 - ms@suse.de
- update firegl profile (#49989)
- fixed incorrect path for fbset (#50196)
- fixed croat kbd layout mapping and added lt.baltic map (#50197)
- added cz-lat2-us kbd layout mapping (#46829)
- update Identity maps for all architectures
* Mon Jan 24 2005 - ms@suse.de
- update monitor database (IVM481A)
* Tue Jan 18 2005 - ms@suse.de
- fixed ExtraKeyboard and ExtraPointer handling in ImportAPI.pm
* Fri Jan 14 2005 - ms@suse.de
- fixed keyboard model <-> layout validation
* Wed Jan 12 2005 - ms@suse.de
- fixed xmset showMonitorConfig() function
* Wed Jan 12 2005 - ms@suse.de
- fixed portuguese keyboard setup, use pc105 instead of pc102
  Thanks to <paulo.granadas@netvisao.pt>
* Mon Jan 03 2005 - ms@suse.de
- removed high-resolution framebuffer settings (#49406)
- added monitor with information from bug (#49042)
* Thu Dec 16 2004 - ms@suse.de
- added hu translation changes   (#49270)
- fixed typo in hotkey asignment (#49151)
* Mon Dec 13 2004 - ms@suse.de
- update monitor CDB (include feedback data)
* Thu Dec 09 2004 - ms@suse.de
- remove monitor profile patch. Include the profile information
  into the CDB now, Jens Oberender you rock :-)
* Fri Dec 03 2004 - ms@suse.de
- added white list for 3D high-color cards (#44341)
* Mon Nov 15 2004 - ms@suse.de
- switched to read and write /etc/X11/xorg.conf (#48235)
- removed unneeded patches from head branch
- removed /etc/sysconfig/sax, the toshiba bounce problem has
  been fixed within the kernel
* Fri Oct 22 2004 - ms@suse.de
- added sync value check to xquery PrepareModeline() (#47232)
- provide possibility to select Traversal plus Aspect/Ratio
  for setting up the DisplaySize. A user can select for example
  17 Zoll -> 4/3. This is usefull if the DisplaySize wasn't
  detected via DDC (#46592)
* Thu Oct 21 2004 - ms@suse.de
- added DisplaySize entry to X11 config file used for
  installation if patched via xupdate (#46632)
* Fri Oct 15 2004 - ms@suse.de
- fixed map check in keyboardhandler.cpp (#46891)
* Wed Oct 13 2004 - ms@suse.de
- fixed hwupdate to take care about hw_mouse (#44834)
* Thu Oct 07 2004 - ms@suse.de
- added input device profile handling based on Vendor/Device ID's
  for special input devices like synaptics touchpads (#46649)
* Thu Oct 07 2004 - ms@suse.de
- update nvidia binary module
- remove perl module source -> require perl-TermReadLine-Gnu
- update XFree86 -> Xorg naming in translations
- changed X link to point to /usr/X11R/bin/Xorg
* Wed Oct 06 2004 - ms@suse.de
- fixed automatic resolution selection to avoid virtual pixels (#46841)
* Tue Oct 05 2004 - ms@suse.de
- update monitor database, fixed VIEWMODE and LCD entries (#46766)
- fixed sysp detection module (xstuff) (#46766)
* Fri Oct 01 2004 - ms@suse.de
- added polish (pl_PL) translation
* Thu Sep 30 2004 - ms@suse.de
- update Identity [ ix86 ] (#45870)
* Wed Sep 29 2004 - ms@suse.de
- include HW mouse name to configuration
- fixed import of extensions section on older versions
- remove multiple InputDevice sections which are bound to the
  same device and driver. related to (#44815)
* Tue Sep 28 2004 - ms@suse.de
- fixed detect/Mouse and ImportAPI modules: Do not include
  SendCoreEvents for input devices which already send core events
  refering to their driver and device specifications (#44815)
* Mon Sep 27 2004 - ms@suse.de
- include readline patch from Michael Schroeder <mls@suse.de>
  module FileHandle expects PerlIO is blessed to IO::Handle
  This will stop segmentation fault on Perl 5.8.0 w/PerlIO
- added translation for the languages: pt_BR,zh_CN and zh_TW
* Mon Sep 27 2004 - ms@suse.de
- update Identity: updated ix86 entries
* Mon Sep 27 2004 - ms@suse.de
- added WXGA 1440x900 resolution (#46210)
* Thu Sep 23 2004 - ms@suse.de
- added DisplaySize check to dynamic NVidia_Twinview profile
- fixed device check to xmset; no device -> no mouse (#45596)
* Wed Sep 22 2004 - ms@suse.de
- disabled hwcursor for all Matrox profiles (#45599)
- update Identity: enabled 3D support for i915G (#45752)
- update Identity: updated AMD64 entries
* Tue Sep 21 2004 - ms@suse.de
- fixed dvorak keyboard setup (#45144)
* Mon Sep 20 2004 - ms@suse.de
- added 1280x768 WXGA entry (#45566)
- added device check to xmset; no device -> no mouse (#45596)
- added japanese translation fixes from Takashi Iwai
* Fri Sep 17 2004 - ms@suse.de
- added WUXGA resolution and CDB entries (#45516)
* Thu Sep 16 2004 - ms@suse.de
- removed sizerGrip if GUI is in fullscreen mode (#45468)
* Wed Sep 15 2004 - ms@suse.de
- update monitor database (#45323)
- fixed use of value added option and Twinview profile (#45376)
- activate serial mouse detection again
* Mon Sep 13 2004 - ms@suse.de
- update Identity (#45023)
- added fullscreen option for lower resolution (#45197)
- added missing show() call to xmouse.cpp (#45120)
* Thu Sep 09 2004 - ms@suse.de
- fixed wheel detection (#44849)
* Wed Sep 08 2004 - ms@suse.de
- allow ISaX to list section(s) without root permissions
* Mon Sep 06 2004 - ms@suse.de
- added translations for various languages :-)
* Mon Sep 06 2004 - ms@suse.de
- added syntax check for --module option (#44686)
- fixed NVidia_DualHead.sh script header (#44687)
- remove AccessX Keys from X0-config.keyboard file (#44133)
- added es,cs and bg translations
* Thu Sep 02 2004 - ms@suse.de
- fixed sysp detection, return DEFAULT entry if 3D does not
  exist but was requested with "-A yes" (#44327)
* Wed Sep 01 2004 - ms@suse.de
- added ALPS configuration specifications
* Mon Aug 30 2004 - ms@suse.de
- implement read/write handler for Extensions section (#44092)
- removed usage of psaux device use /dev/input/mice instead (#38655)
* Mon Aug 30 2004 - ms@suse.de
- removed speedo module, it doesn't exist anymore
- adapt firegl profile (add dri module)
- update Identity [i386]
* Mon Aug 23 2004 - ms@suse.de
- added Acer-1400x1050 display (IDT0002)
- prepared parser and isax for Extensions section (#44092)
- fixed DELL D600 specifications (#44005)
- update japanese translation
* Thu Aug 19 2004 - ms@suse.de
- added 1280x800 LCD entry to the CDB (#43976)
* Thu Aug 19 2004 - ms@suse.de
- fixed version check for libhd to support DDC and mouse wheel
- update card database (CDB)
* Wed Aug 18 2004 - ms@suse.de
- changed to new "kbd" keyboard driver (#43902)
- changed testX to use fvwmrc.yast2, remove fvwmrc.yast2 from
  package sax2 -> moved to YaST2 (#43796)
- update monitor database
* Fri Aug 06 2004 - meissner@suse.de
- %%_lib fix to make it build on lib64 archs.
* Thu Jul 29 2004 - ms@suse.de
- added profile for i915G chipset, update card database
* Wed Jul 28 2004 - ms@suse.de
- adapt manual pages
* Tue Jul 27 2004 - ms@suse.de
- added GOLDSTAR:CS788C monitor specifications
- completed specifications for VIEWSONIC:E70 monitor
- fixed regexp in checkmap.pl not to interpret input \Q...\E
  Thanks to Jan Engelhardt <jengelh@linux01.gwdg.de>
* Thu Jul 22 2004 - ms@suse.de
- added BENQ FP783 specifications (#43233)
- added DELL LATITUDE D600 specification
* Fri Jul 16 2004 - ms@suse.de
- added HTTP port validation code to ensure unique port names
* Tue Jul 13 2004 - ms@suse.de
- add option --fast to the testX utility
* Tue Jul 13 2004 - ms@suse.de
- fixed handling of profile paramter: VideoRam (#42831)
- added SAMSUNG SYNCMASTER 213T to CDB
- added option --ignoreprofile to be able to ignore
  automatically applied profiles
* Fri Jul 09 2004 - ms@suse.de
- added support for HTTP access via VNC
* Thu Jul 08 2004 - ms@suse.de
- fixed Matrox_G550 profile (#42831)
* Thu Jul 08 2004 - ms@suse.de
- implement full featured mode not only for the core pointer
  during installation (#37346)
* Mon Jul 05 2004 - ms@suse.de
- fixed LICENSE confusion: SaX2 is GPL !
* Fri Jul 02 2004 - ms@suse.de
- fixed RawOption handling used if 3D is selected/deselected (#42186)
* Fri Jun 25 2004 - ms@suse.de
- fixed polish mapping table (#42348)
* Tue Jun 22 2004 - ms@suse.de
- fixed VIEWSONIC 17PS-2 entry within the CDB (#42308)
* Wed Jun 16 2004 - ms@suse.de
- added DUMMY graphics entry for manual dummy driver configuration
- Include monitor: IIYAMA PRO LITE E481S into CDB
* Tue Jun 08 2004 - ms@suse.de
- fixed default configuration for greek kbd-layout (#41727)
* Tue Jun 01 2004 - ms@suse.de
- enable wheel emulation if there are exactly 4 btns and no wheel
- added configuration data for Tablet: Aiptek HyperPen 6000
* Mon May 24 2004 - ms@suse.de
- fixed TrackMan Marble FX (PS/2) data record (#40443)
- fixed use of ininitialized structure variable (#38660)
- added 1280x800 WXGA resolution (#41271)
* Tue May 18 2004 - ms@suse.de
- include SGI monitors (#40748)
- fixed width of mouse test field (#40780)
- fixed naming of mouse devices in USB-mouse section (#40779)
- fixed WinINF module function mountMedia() (#40716)
- removed fvwmrc.yast2 from package list (#37480)
- add profile patch for handling monitor profiles
- update monitor database
* Thu May 13 2004 - ms@suse.de
- do not allow removing major entities, for example the
  monitor entity which is the only one existing (#40477)
* Thu Apr 29 2004 - ms@suse.de
- lots of xorg-x11 fixes which will break SaX2 to work correctly
- fixed building lex&yacc based sources with g++ compiler
* Tue Apr 27 2004 - ms@suse.de
- moved fvwmrc.notitle to fvwmrc.yast2 and include the changes
  from Olaf <od@suse.de> (#36488)
- changed XFree86 license naming in all source files
- adapt documentation according to XFree86 -> Xorg changes
* Tue Apr 27 2004 - sndirsch@suse.de
- use xorg-x11 packages (neededforbuild/Requires)
* Thu Apr 22 2004 - ms@suse.de
- call checkDetected() only for the primary (first) devices
- added hint to enter the DisplaySize if it couldn't be detected
- update Identity for IA64 (#39282)
* Wed Apr 21 2004 - ms@suse.de
- fixed INF parser to know about [...] sections not starting at
  the beginning of a line, which is a bug in the .inf file but there
  is no way out you know
* Tue Apr 20 2004 - ms@suse.de
- SaX2 split to provide packages for seperating the engine and GUI
  as well as providing a unique name scheme and assign the package
  requirements to GUI and engine:
  /.../
  sax2     -> sax2-gui   |  sax2
  saxtools -> sax2-tools
  saxident -> sax2-ident
* Tue Apr 20 2004 - ms@suse.de
- fixed spanish keyboard mapping (#39003)
* Fri Apr 16 2004 - ms@suse.de
- added SaX2 VNC configuration (#26492)
- changed xmset to use the device reported from libhd instead of
  linking it to /dev/mouse
* Sun Apr 11 2004 - ms@suse.de
- check for adjustable flag in saveMode function (#38640)
- create CDB record if ID is provided but not yet included (#38785)
- fixed CDB data for Belinea 106055 (#38704)
- removed keyboard repeat/delay dialog, AutoRepeat no longer
  supported to be set within the config file (#38713)
- fixed availability of parsed XkbOptions settings (#38714)
- added TV resolutions to monitor resolution list (#32134)
- make the SAVE button to be the default
* Tue Apr 06 2004 - ms@suse.de
- include sax2_nvidia-msg.dif (#38438)
- fixed sv-latin1 keyboard configuration (#38440)
- added [ ja ] translation from Novell
* Tue Apr 06 2004 - ms@suse.de
- fixed mouse setup if a Tablet has been connected. The tablet
  shouldn't be handled as normal pointer device
* Tue Apr 06 2004 - sndirsch@suse.de
- sax2_nvidia-msg.dif:
  * fixed size of 3D nvidia warning window (Bug #38438)
* Thu Apr 01 2004 - ms@suse.de
- disable <Save> button in test screen if mode tuning is not
  available. It may lead to broken modelines (#35949)
- added 1680x1050 TFT wide screen mode to CDB (#37465)
- added Dell Latitude D600 display to CDB (#37787)
- added EIZO:L565 (ANALOG) monitor to CDB
- update Identities
* Wed Mar 31 2004 - ms@suse.de
- fixed monitor CDB entries for Gateway EV500 series (#37560)
- added translations [ it,fr ]
- added LGP monitor LGP0557
* Tue Mar 30 2004 - ms@suse.de
- added translation [ sv ]
* Mon Mar 29 2004 - ms@suse.de
- fixed sax.sh startup script to transfer the --dialog option
  for later xapi call (#35504)
* Mon Mar 29 2004 - ms@suse.de
- added translation [ sl_SI ]
- added monitor [ SAMSUNG SEC 1024 ] to CDB
* Fri Mar 26 2004 - ms@suse.de
- update [ ja ] translation
* Fri Mar 26 2004 - ms@suse.de
- disabled mouse device linking code (#36944)
- changed testX to set the windowmanager as the first X client
  started, this will prevent testX to remain open as separate
  client consuming memory (#37035)
* Wed Mar 24 2004 - ms@suse.de
- fixed possible loop at installation. set different exit codes
  according to the possible exceptions. Handle the exit codes in
  installation/scripts/YaST2 (#36794)
- added [ el_GR ] translation
* Tue Mar 23 2004 - ms@suse.de
- provide additional fvwmrc.notitle configuration file (#36488)
- added translations [ hu,bg,ru ]
* Thu Mar 18 2004 - ms@suse.de
- added errorHandler() function to xquery.c
- added translation [ ja ]
* Thu Mar 18 2004 - ms@suse.de
- fixed USB tablet port detection (#36115)
* Tue Mar 16 2004 - ms@suse.de
- added translation [ es ]
- added translation [ nb ]
* Mon Mar 15 2004 - ms@suse.de
- update Identities for all architectures (#35883)
- added nodeadkeys option to all pt based keyboards (#36001)
- added translation [ cs ]
- added translation [ sv ]
* Thu Mar 11 2004 - ms@suse.de
- extend testX to check for dimension and colordepth (#35693)
- removed mousedev check (#35642)
* Wed Mar 10 2004 - ms@suse.de
- added -br option to the loader. This will prevent the
  moiree pattern to be displayed
* Wed Mar 10 2004 - ms@suse.de
- add CTX monitor VGA/DVI specs (#35573)
* Wed Mar 10 2004 - ms@suse.de
- added option to start SaX2 with a specific dialog (#35504)
* Mon Mar 08 2004 - ms@suse.de
- update monitor database
* Fri Mar 05 2004 - ms@suse.de
- added unknown monitor IQT217A (#35384)
- include Identity fixes for AMD64 and I386
* Tue Mar 02 2004 - adrian@suse.de
- add udev to neededforbuild, hwinfo needs libsysfs now
* Fri Feb 27 2004 - ms@suse.de
- update Identity [ix86]
* Tue Feb 17 2004 - ms@suse.de
- added Emulate3Buttons to Synaptics default configuration
- add EIZO L557-A display to database (CDB)
- fixed keyboard option handling for evaluation of the
  XkbOptions string (#34563)
* Thu Feb 12 2004 - ms@suse.de
- fixed AccessX status evaluation code
- fixed russian keyboard configuration, and XkbVariant
  evaluation including empty variant settings
* Wed Feb 11 2004 - ms@suse.de
- update monitor CDB new IIYAMA LCD/CRT models
- implement reading of .inf files from CD and Disk (#34362)
- fixed isax bug while exporting YCP code
- fixed Identity for x86_64 (#34446)
* Tue Feb 10 2004 - ms@suse.de
- removed gunze and pacebook driver specifications (#34394)
- fixed resolution compare bug (#33086)
* Mon Feb 09 2004 - ms@suse.de
- changed radeon8500 profile to be more general -> Radeon_DualHead
* Tue Jan 27 2004 - ms@suse.de
- implement improvements for tablet configuration
- fixed greek mapping from gr to el
- update monitor database with new LIKOM models
- new version 4.8
* Wed Jan 21 2004 - ms@suse.de
- fixed WinINF.pm module to work with .inf files which
  handles keys as .AddReg and .Addreg in the same way
* Thu Jan 08 2004 - ms@suse.de
- fixed build according to new yacc/bison version
* Wed Jan 07 2004 - ms@suse.de
- added firegl UseInternalAGPGART option
- fixed synaptics options according to new driver 0.12.1 (#33765)
* Mon Jan 05 2004 - ms@suse.de
- update no-source package to current nvidia driver
  version: 1.0-5328
