/**************
FILE          : xmirror.c
***************
PROJECT       : SaX2 ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool XFree86 version 4.x
              :  
DESCRIPTION   : xmirror covers an area of the root window
              : to be saved in a simple window. This will
              : prevent us from cleaning this area without
              : redrawing
              : 
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

void usage(void);

int main(int argc,char *argv[]) {
 XSetWindowAttributes xswa;
 XWindowAttributes wattr;
 char *displayname = NULL;
 unsigned long mask = 0;
 Display *dpy;
 Window root;
 Window win;
 GC mgc;
 Visual vis;
 int c;
 int x1     = 0;
 int y1     = 0;
 int depth;
 int width  = 0;
 int height = 0;
 Pixmap bg;

 // get options...
 // ---------------
 while (1) {
  int option_index = 0;
  static struct option long_options[] =
  {
   {"display"    , 1 , 0 , 'd'},
   {"x1"         , 1 , 0 , 'x'},
   {"y1"         , 1 , 0 , 'y'},
   {"width"      , 1 , 0 , 'w'},
   {"height"     , 1 , 0 , 'e'},
   {"help"       , 0 , 0 , 'h'},
   {0            , 0 , 0 , 0  }
  };

  c = getopt_long (argc, argv, "hd:x:y:w:e:",long_options, &option_index);
  if (c == -1)
   break;

  switch (c) {
    case 0:
     break;

    case 'h':
     usage();

    case 'd':
     displayname = (char*)malloc(80*sizeof(char));
     strcpy(displayname,optarg);
     break;

    case 'x':
     x1 = atoi(optarg);
     break;

    case 'y':
     y1 = atoi(optarg);
     break;

    case 'w':
     width = atoi(optarg);
     break;

    case 'e':
     height = atoi(optarg);
     break;

    default:
     /*fprintf (stderr,"?? getopt returned character code 0%o ??\n", c);*/
     usage();
     exit(1);
  }
 }

 // open display...
 // -----------------
 dpy = XOpenDisplay (displayname);
 if (!dpy) {
  fprintf (stderr, "unable to open display %s\n", XDisplayName(displayname));
  exit (1);
 }

 // get screen dimensions...
 // --------------------------
 if (width <= 0) {
  width = DisplayWidth(dpy,XDefaultScreen(dpy));
 }
 if (height <= 0) {
  height = DisplayHeight(dpy,XDefaultScreen(dpy));
 }

 // get root window and default context
 // ------------------------------------
 root  = RootWindow  (dpy,XDefaultScreen(dpy));
 mgc   = DefaultGC   (dpy,XDefaultScreen(dpy));
 depth = DefaultDepth(dpy,XDefaultScreen(dpy));

 xswa.event_mask        = EnterWindowMask | 
  LeaveWindowMask         | 
  ExposureMask            | 
  VisibilityChangeMask    | 
  StructureNotifyMask     | 
  SubstructureNotifyMask  | 
  SubstructureRedirectMask
 ; 
 xswa.background_pixmap = None;
 xswa.override_redirect = True;
 xswa.backing_store     = NotUseful;
 xswa.save_under        = False;
 vis.visualid           = CopyFromParent;
 mask  |= (
  CWBackPixmap   |
  CWOverrideRedirect |
  CWBackingStore |
  CWSaveUnder
 );

 // create windows (top,bottom,left,right)...
 // ---------------------------------------------
 win = XCreateWindow(
  dpy,root,x1,y1,width,height,0,depth,InputOutput,&vis,mask,&xswa
 );
 XMapWindow(dpy,win);
 bg = XCreatePixmap(dpy,win,width,height,depth);
 XCopyArea(dpy,win,bg,mgc,0,0,width,height,0,0);
 XFlush(dpy);

 XGetWindowAttributes(dpy, win, &wattr);
 if (wattr.all_event_masks & ButtonPressMask) {
  xswa.event_mask &= ~ButtonPressMask;
 }
 xswa.event_mask &= ~SubstructureRedirectMask;
 XSelectInput(dpy, win, xswa.event_mask);

 while(1) {
  XEvent event;
  XNextEvent (dpy, &event);
  switch (event.type) {
   case Expose:
    XCopyArea(dpy,bg,win,mgc,0,0,width,height,0,0);
    break;
  }
 }

 // flush and close...
 // -------------------
 XCloseDisplay(dpy);
}


void usage(void) {
 printf("usage: xmirror [ options ]\n");
 printf("options:\n");
 printf("  [ -d | --display ]\n");
 printf("    set display to use\n");
 printf("  [ -h | --help ]\n");
 printf("    you already got it\n");
 printf("  [ -x | --x1 ]\n");
 printf("  [ -y | --y1 ]\n");
 printf("    top left corner of the area\n");
 printf("  [ -w | --width  ]\n");
 printf("    width of the area\n");
 printf("  [ -e | --height ]\n");
 printf("    height of the area\n");
 exit(1);
}

