/**************
FILE          : xbound.c
***************
PROJECT       : SaX ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool X11 version 4.x
              :  
DESCRIPTION   : xbound create a bounding rectangle around
              : the display to show the screen borders
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

#define PATH_DEV_NULL "/dev/null"

void usage(void);
int  daemon(int nochdir, int noclose);

int main(int argc,char *argv[]) {
 XSetWindowAttributes xswa;
 char *displayname = NULL;
 unsigned long mask = 0;
 Display *dpy;
 Window root;
 Window win1,win2,win3,win4;
 GC mgc;
 Visual vis;
 int c;
 int x2,y2;
 int depth;

 // get options...
 // ---------------
 while (1) {
  int option_index = 0;
  static struct option long_options[] =
  {
   {"display"    , 1 , 0 , 'd'},
   {"help"       , 0 , 0 , 'h'},
   {0            , 0 , 0 , 0  }
  };

  c = getopt_long (argc, argv, "hd:",long_options, &option_index);
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

 // go for daemon...
 // -----------------
 // printf("%d\n",getpid());
 // daemon(1,1);

 // get screen dimensions...
 // --------------------------
 x2 = DisplayWidth(dpy,XDefaultScreen(dpy));
 y2 = DisplayHeight(dpy,XDefaultScreen(dpy));

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
 xswa.border_pixel      = WhitePixel(dpy,XDefaultScreen(dpy));
 xswa.background_pixel  = WhitePixel(dpy,XDefaultScreen(dpy));
 xswa.override_redirect = True;
 xswa.backing_store     = NotUseful;
 xswa.save_under        = False;
 vis.visualid           = CopyFromParent;
 mask  |= (
  CWBackPixmap   |
  CWOverrideRedirect |
  CWBackingStore |
  CWSaveUnder |
  CWBackPixel | 
  CWBorderPixel
 );

 // create windows (top,bottom,left,right)...
 // ---------------------------------------------
 win1 = XCreateWindow(
  dpy,root,0,0,x2,1,0,depth,InputOutput,&vis,mask,&xswa
 );
 win2 = XCreateWindow(
  dpy,root,0,y2-1,x2,y2,0,depth,InputOutput,&vis,mask,&xswa
 );
 win3 = XCreateWindow(
  dpy,root,0,0,1,y2,0,depth,InputOutput,&vis,mask,&xswa
 );
 win4 = XCreateWindow(
  dpy,root,x2-1,0,x2,y2,0,depth,InputOutput,&vis,mask,&xswa
 );
 XMapWindow(dpy,win1);
 XMapWindow(dpy,win2);
 XMapWindow(dpy,win3);
 XMapWindow(dpy,win4);
 XFlush(dpy);
 

 while(1) {
  XEvent event;
  XNextEvent (dpy, &event);
  switch (event.type) {
   case Expose:
   break;
  }
 }

 // flush and close...
 // -------------------
 XCloseDisplay(dpy);
}

int daemon(int nochdir, int noclose) {
 int fd;

 switch (fork()) {
  case -1:
   return (-1);
  case 0:
   /**********************************
    we are the child... leave case
   **********************************/
   break;

  default:
   /**********************************
    we are the parent process.. exit
   **********************************/
   sleep(1);
   _exit(0);
 }

 if (setsid() == -1)
  return (-1);

 if (!nochdir)
  (void)chdir("/");

 if (!noclose && (fd = open(PATH_DEV_NULL, O_RDWR, 0)) != -1) {
  (void)dup2(fd, STDIN_FILENO);
  (void)dup2(fd, STDOUT_FILENO);
  (void)dup2(fd, STDERR_FILENO);
  if (fd > 2)
   (void)close (fd);
 }
 return (0);
}

void usage(void) {
 printf("usage: xbound [ options ]\n");
 printf("options:\n");
 printf("  [ -d | --display ]\n");
 printf("    set display to use\n"); 
 printf("  [ -h | --help ]\n");
 printf("    you already got it\n");
 exit(1);
}

