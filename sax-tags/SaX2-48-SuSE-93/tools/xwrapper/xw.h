/**************
FILE          : xw.h
***************
PROJECT       : SaX2 ( SuSE advanced X11 configuration )
              :
DESCRIPTION   : xw.c is used to work as X-Server wrapper.
              : It will start the X11 loader where the first
              : is used as file for the X-log.
              :
              : if the server is ready the fvwm windowmanager is started 
              : and the background,corner marks are set as well as 
              : xaccess is enabled. If the process receives the 
              : ready [SIGUSR1] we will fork into the background 
              : and exit
              :
              : 
STATUS        : Status: Up-to-date
**************/
#ifndef XW_H
#define XW_H 1

#include <locale.h>
#include <libintl.h>

//===========================
// Defines...
//---------------------------
#define  XFTFONT     "serif-30"
#define  XBannerFile "/usr/X11R6/lib/sax/tools/demo"
#define  XScreenFile "/usr/X11R6/lib/sax/tools/screen"
#define  XBanner     "/usr/X11R6/bin/xbanner"
#define  XLook       "/usr/X11R6/lib/sax/tools/xlook"
#define  TWM         "/usr/X11R6/bin/twm"
#define  FVWM        "/usr/X11R6/lib/sax/tools/wmstart"
#define  XIDLE       "/usr/X11R6/lib/sax/xapi"
#define  TWMRC       "/usr/X11R6/lib/sax/api/data/twmrc"
#define  FVWMRC      "/usr/X11R6/share/fvwm/fvwmrc.sax"
#define  TDOMAIN     "/usr/share/locale"
#define  DISPSIZE    255

#ifdef XORG_LOADER
#define  XLOADER     "/usr/X11R6/bin/Xorg"
#else
#define  XLOADER     "/usr/X11R6/bin/XFree86"
#endif

struct mirror {
	int x1,y1,width,height;
	Pixmap bg;
};

//===========================
// Function protottypes
//---------------------------
void ready (int s);
void prepare (void);
void trapper (int s);
void child (int s);
int  tty (void);
int  is_a_console(int fd);
int  open_a_console(char *fnam);
int  getfd(void);
int  chvt (int num);
void CountScreens (Display *dpy);
Bool IsXinerama (Display *dpy);
void closeClients (Display *dpy);
void RedrawWindow (void);
int  catchWindowErrors (Display *dpy, XErrorEvent *ev);
int XTestRender (Display* dpy);
XImage* setText (Display* dpy,int screen,char* text, int* x, int* y);

#endif
