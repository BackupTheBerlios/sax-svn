/**************
FILE          : config.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : Global definitions and configuration   
              : options for sysp
              :
STATUS        : Status: Up-to-date
**************/

#ifndef CONFIG_H

//==============================
// machine types...
//------------------------------ 
#define CONFIG_H          1
#define ARCH_PPC          "ppc"
#define ARCH_SPARC        "sparc"

//==============================
// paths...
//------------------------------
#define RC_CONFIG         "/etc/rc.config"
#define RC_SYSCONFIG      "/etc/sysconfig/"
#define INPUT_MAP         "/usr/X11R6/lib/sax/sysp/maps/Input.map"
#define DRIVER_MAP        "/usr/X11R6/lib/sax/sysp/maps/Driver.map"
#define KBD_MAP           "/usr/X11R6/lib/sax/sysp/maps/Keyboard.map"
#define VENDOR_MAP        "/usr/X11R6/lib/sax/sysp/maps/Vendor.map"
#define IDENTITY          "/usr/X11R6/lib/sax/sysp/maps/Identity.map"
#define PROFILEDIR        "/usr/X11R6/lib/sax/profile/"

//==============================
// Expressions...
//------------------------------
#define FLAG_DEFAULT      "DEFAULT"
#define FLAG_3D           "3D"
#define TEXT_3D           "Do you want to enable 3D for this card ?"
#define TMP_CONFIG        "/tmp/sysdata"
#define XW_LOG            "/tmp/xwlog"
#define XWRAPPER          "/usr/X11R6/lib/sax/xw.pl"
#define PROBE             "-probeonly"
#define CONFIG            "-xf86config"
#define BLANK             "-blank"
#define NO_FLAG_QUESTION  99
#define FB_DEV            "/dev/fb0"
#define KILL              "/usr/bin/killall"
#define QTASK             "/usr/X11R6/lib/sax/xapi"
#define STDOUT            0
#define STDERR            1
#define STDNONE           2
#define UNCVID            0x1234
#define UNCDID            0x5678

//==============================
// RDBMS data files... 
//------------------------------
#define MOUSE_DATA        "/var/cache/sax/sysp/rdbms/scanmouse"
#define KEYBOARD_DATA     "/var/cache/sax/sysp/rdbms/scankeyboard"
#define SERVER_DATA       "/var/cache/sax/sysp/rdbms/scanserver"
#define SERVER_STUFF_DATA "/var/cache/sax/sysp/rdbms/scanserverstuff"
#define STUFF_DATA        "/var/cache/sax/sysp/rdbms/scanstuff"
#define D3_DATA           "/var/cache/sax/sysp/rdbms/scan3d"

//==============================
// Scripts...
//------------------------------
#define MACHINE           "/usr/X11R6/lib/sax/sysp/script/machine.pl"
#define INSTALLED         "/usr/X11R6/lib/sax/sysp/script/installed.pl"
#define KILLDOT           "/usr/X11R6/lib/sax/sysp/script/killdot.pl"
#define PREPARELOG        "/usr/X11R6/lib/sax/sysp/script/preparelog.pl"
#define CHECKMAP          "/usr/X11R6/lib/sax/sysp/script/checkmap.pl"
#define PROFILECOUNT      "/usr/X11R6/lib/sax/sysp/script/profilecount.pl"
#define PSECTION          "/usr/X11R6/lib/sax/sysp/script/psection.pl"
#define DIAG              "/usr/X11R6/lib/sax/sysp/script/diag.pl"
#define GETVENDOR         "/usr/X11R6/lib/sax/sysp/script/vendor.pl"
#define VBIOS             "/usr/X11R6/lib/sax/sysp/script/vbios.pl"

#ifdef XORG_LOADER
#define  LOADER_NAME      "Xorg"
#define  MODULE_VENDOR    "X.Org Foundation"
#else
#define  LOADER_NAME      "XFree86"
#define  MODULE_VENDOR    "The XFree86 Project"
#endif

#endif

