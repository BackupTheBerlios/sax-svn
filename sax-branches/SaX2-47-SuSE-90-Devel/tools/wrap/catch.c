/**************
FILE          : catch.c
***************
PROJECT       : SaX ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool XFree86 version 4.0
              :  
DESCRIPTION   : catch catches the mouse into a rectangele of
              : specific coordinates
              : 
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86misc.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/Xmu/WinUtil.h>

//==============================
// Functions...
//------------------------------
void GetPointerGeo(Display *dpy, Window w,int *x,int *y,int *rx,int *ry);
void usage(void);

int main (int argc, char **argv) {
	Display *display;
	Window w;
	int x  = 0;
	int y  = 0;
	int rx = 0;
	int ry = 0;
	int x1,y1,x2,y2;
	XSetWindowAttributes attr;
	XWindowAttributes wattr;
	char mydisp[256] = "";

	if ((argc != 5) && (argc != 6)) {
		usage(); 
		exit(1);
	} 

	if (argc == 5) {
		strcpy(mydisp,getenv("DISPLAY"));
	} else {
		strcpy(mydisp,argv[5]);
	}

	x1 = atoi(argv[1]);
	y1 = atoi(argv[2]);
	x2 = atoi(argv[3]);
	y2 = atoi(argv[4]); 

	if (!(display = XOpenDisplay(mydisp))) {
		return fprintf(stderr, "unable to open display\n"), 11;
	}
	w = DefaultRootWindow(display);


	attr.event_mask = PointerMotionMask;
	XGetWindowAttributes(display, w, &wattr);
	if (wattr.all_event_masks & ButtonPressMask) {
		attr.event_mask &= ~ButtonPressMask;
	}
	attr.event_mask &= ~SubstructureRedirectMask;
	XSelectInput(display, w, attr.event_mask);

	while(1) {
		XEvent event;
		XNextEvent (display, &event);
		GetPointerGeo (display,w,&x,&y,&rx,&ry);
		if (x < x1) {
		XWarpPointer (
			display,None,DefaultRootWindow(display),0,0,0,0,x1,y
		);
		}
		if (x > x2) {
		XWarpPointer (
			display,None,DefaultRootWindow(display),0,0,0,0,x2,y
		);
		}
		if (y < y1) {
		XWarpPointer (
			display,None,DefaultRootWindow(display),0,0,0,0,x,y1
		);
		}
		if (y > y2) {
		XWarpPointer (
			display,None,DefaultRootWindow(display),0,0,0,0,x,y2
		);
		}
	}
}

//==============================
// GetPointerGeo...
//------------------------------
void GetPointerGeo(Display *dpy, Window w,int *x,int *y,int *rx,int *ry) {
	Window dummy;
	int mask;
	int X,Y,RX,RY;

	XQueryPointer((Display *)dpy,w,&dummy,&dummy,&RX,&RY,&X,&Y,&mask);
	*rx = RX;
	*ry = RY;
	*x  = X;
	*y  = Y;
	return;
}

//==============================
// usage...
//------------------------------
void usage(void) {
	printf("usage: catch <x1 y1 x2 y2> [ display ]\n");
}
