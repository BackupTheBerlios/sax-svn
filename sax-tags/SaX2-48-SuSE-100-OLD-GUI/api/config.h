/**************
FILE          : config.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Sch�fer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : global configuation settings for the API
              : 
              :
STATUS        : Status: Up-to-date
**************/
#ifndef CONFIG_H
#define CONFIG_H 1

//==============================
// Pixmaps and translations...
//------------------------------
#define PIXDIR          "/usr/share/sax/api/pixmaps/"
#define INFDIR          "/usr/share/sax/api/data/"
#define PRODIR          "/usr/share/sax/profile/"
#define TMPDIR          "/tmp/"
#define MD5DIR          "/var/cache/sax/files/"

#define LOGO            PIXDIR "logo.png"
#define PAPER           PIXDIR "marble.xpm"
#define XAPI            INFDIR "xapi.gtx"
#define PIXXCONTROL     PIXDIR "vnc.xpm"
#define PIXDESKTOP      PIXDIR "card.xpm"
#define PIXCARD         PIXDIR "card.xpm"
#define PIXINPUT        PIXDIR "input.xpm"
#define PIXINFO         PIXDIR "tooltip.xpm"
#define PIXMULTI        PIXDIR "multi.xpm"
#define PIXGEO          PIXDIR "geo.xpm"
#define PIXINTROCARD    PIXDIR "card.xpm"
#define PIXINTROMOUSE   PIXDIR "mouse.xpm"
#define PIXINTRO3D      PIXDIR "3d.xpm"
#define PIXINTROPORT    PIXDIR "port.xpm"
#define PIXINTROKBD     PIXDIR "keyboard.xpm"
#define PIXINTROPEN     PIXDIR "pen.xpm"
#define PIXINTROERASER  PIXDIR "eraser.xpm"
#define PIXINTROTABLET  PIXDIR "tablet.xpm"
#define PIXINTROTOUCH   PIXDIR "touch.xpm"
#define FIGURE          PIXDIR "monitor.xpm"
#define PIXDESKTOPCOLOR PIXDIR "card.xpm"
#define PIXDESKTOPRIGHT PIXDIR "arrowright.xpm"
#define PIXDESKTOPLEFT  PIXDIR "arrowleft.xpm"
#define PIXMONITORFRQ   PIXDIR "card.xpm"
#define PIXKBDLAYOUT    PIXDIR "kbdlayout.xpm"
#define PIXKBDDELAY     PIXDIR "delay.xpm"
#define PIXKBDRATE      PIXDIR "rate.xpm"
#define PIXTABLET       PIXDIR "tablet.xpm"
#define PIXTOUCHSCREEN  PIXDIR "touch.xpm"
#define PIXMONITOR      PIXDIR "card.xpm"
#define PIXVIRTUAL      PIXDIR "virtual.xpm"
#define HELP            PIXDIR "help.xpm"
#define ABOUT           PIXDIR "about.xpm"

//==============================
// Profile output files...
//------------------------------
#define LAYOUTPROFILE   TMPDIR "layout"
#define DESKTOPPROFILE  TMPDIR "desktop"
#define CARDPROFILE     TMPDIR "card"

//==============================
// MD5 sum for current config
//------------------------------
#define MD5CONFIG       MD5DIR "xconfig.md5"
#define HWFLAG          MD5DIR "hardware.chg"

//==============================
// XKB constants
//------------------------------
#define XKB_VARIANT     0
#define XKB_LAYOUT      1
#define XKB_MODEL       2
#define XKB_OPTIONS     3

//==============================
// QX paramters...
//------------------------------
#define STDOUT          0
#define STDERR          1
#define STDNONE         2

//==============================
// Uni-Font...
//------------------------------
#define UNIFONT  "-gnu-unifont-medium-r-normal--16-160-75-75-p-80-iso10646-1"

//==============================
// Line length in cache files...
//------------------------------
#define MAX_LINE_LENGTH 8192

//==============================
// Message Box buttons...
//------------------------------
#define TEST            1
#define SAVE            2
#define CANCEL          5

//==============================
// Max args for XCommand...
//------------------------------
#define MAX_ARGS      128

//==============================
// lilocode file...
//------------------------------
#define LILOCODE        "/var/cache/sax/files/lilocode.ycp"

//==============================
// LOGGING definitions...
//------------------------------
#define LOGFILEDIR      "/var/log"

//==============================
// ISaX calls...
//------------------------------
#define ISAX            "/sbin/isax"
#define GETCARD         ISAX " -l Card"
#define GETDESKTOP      ISAX " -l Desktop"
#define GETINPUT        ISAX " -l Mouse"
#define GETKBD          ISAX " -l Keyboard"
#define GETLAYOUT       ISAX " -l Layout"
#define GETPATH         ISAX " -l Path"
#define GETEXTENSIONS   ISAX " -l Extensions"

//==============================
// Binary files...
//------------------------------
#define RPM             "/bin/rpm"
#define XMODE           "/sbin/xmode"
#define WRAP            "/sbin/wrap"
#define FAKE            "/sbin/fake"
#define CATCH           "/sbin/catch" 
#define SYSP            "/sbin/sysp"
#define XMSET           "/sbin/xmset"
#define XKBSET          "/sbin/xkbset"
#define SETXKBMAP       "/usr/X11R6/bin/setxkbmap"
#define DEMO            "/sbin/demo.sh" 
#define XQUERY          "/sbin/xquery"
#define XRUN            "/usr/share/sax/xrun.pl"
#define XVERSION        "/usr/share/sax/api/tools/getversion"
#define GETPROFILE      "/usr/share/sax/api/tools/getprofile"
#define GETCONFIG       "/usr/share/sax/api/tools/getconfig"
#define GETLINKED       "/usr/share/sax/api/tools/getlinked"
#define GETBUS          "/usr/share/sax/api/tools/getbus"
#define GETDDC          "/usr/share/sax/api/tools/getDDC"
#define GETINF          "/usr/share/sax/api/tools/getINF"
#define GETLOG          "/usr/share/sax/api/tools/getlog"
#define GETRAWLIST      "/usr/share/sax/api/tools/getRawList"
#define GET3D           "/usr/share/sax/api/tools/get3D"
#define GETTABLETPORT   "/usr/share/sax/api/tools/getUSBTabletPort"
#define GETACTIVATED    "/usr/share/sax/api/tools/getActivated"
#define GETINSTALLED    "/usr/share/sax/api/tools/getInstalled"
#define GETPACKAGE      "/usr/share/sax/api/tools/getPackage"
#define VALIDATELAYOUT  "/usr/share/sax/api/tools/getLayout"
#define GETVNCPASSWORD  "/usr/share/sax/api/tools/getVNCPassword"
#define MD5             "/usr/bin/md5sum"
#define XSET            "/usr/X11R6/bin/xset"
#define XSETROOT        "/usr/X11R6/bin/xsetroot"
#define NV_INSTALLER    "/usr/bin/nvidia-installer"

#ifdef XORG_LOADER
#define TEST_LOG        "/var/log/Xorg.99.log"
#else
#define TEST_LOG        "/var/log/XFree86.99.log"
#endif

//==============================
// Init files...
//------------------------------
#define IDENTITY        "/usr/share/sax/sysp/maps/Identity.map"
#define APIATA          "/var/cache/sax/files/apidata" 
#define XFINECACHE      "/var/cache/xfine"
#define XFINE           "/usr/X11R6/lib/xfine/xfine"
#define XCONFIG         "/etc/X11/xorg.conf"
#define LASTLOG         "/var/cache/sax/files/lastlog"
#define HWCONFIG        "/var/cache/sax/files/config"
#define TESTCONFIG      "/var/cache/sax/files/xorg.conf"
#define XVT             "/var/log/xvt"
#define FONTS           "/usr/X11R6/lib/X11/fonts"
#define DRIVERS         "/usr/X11R6/lib/modules/drivers"
#define TDOMAIN         "/usr/share/locale"

#define CDBDIR          "/usr/share/sax/api/data/cdb/"
#define CARDS           CDBDIR "Cards"
#define MONITORS        CDBDIR "Monitors"
#define POINTER         CDBDIR "Pointers"
#define PENS            CDBDIR "Pens"
#define TABLETS         CDBDIR "Tablets"
#define TOUCHER         CDBDIR "Touchscreens"

#define DATADIR         "/usr/share/sax/api/data/"
#define PDEVICE         DATADIR "PointerDevice"
#define KLANGUAGE       DATADIR "KbdLanguage"
#define CMODULES        DATADIR "CardModules"
#define TMODULES        DATADIR "TabletModules"
#define C3DMODULES      DATADIR "Card3DModules"
#define MCOLORS         DATADIR "MonitorColors"
#define MVISUAL         DATADIR "MonitorVisual"
#define MRESOLUTION     DATADIR "MonitorResolution"
#define MTRAVERSAL      DATADIR "MonitorTraversal"
#define MRATIO          DATADIR "MonitorRatio"
#define MFBRESOLUTION   DATADIR "MonitorFBResolution"

#define CONFDIR         "/var/cache/sax/files/"
#define ISAXCARD        CONFDIR "card"
#define ISAXDESKTOP     CONFDIR "desktop"
#define ISAXINPUT       CONFDIR "input"
#define ISAXKEYBOARD    CONFDIR "keyboard"
#define ISAXLAYOUT      CONFDIR "layout"
#define ISAXPATH        CONFDIR "path"
#define ISAXEXTENSIONS  CONFDIR "extensions"

#endif
