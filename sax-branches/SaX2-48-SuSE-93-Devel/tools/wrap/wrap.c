/**************
FILE          : warp.c
***************
PROJECT       : SaX ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool X11 version 4.0
              :  
DESCRIPTION   : wrap set the mouse to a new position 
              : given through commandline x y data
              : 
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86misc.h>
#include <math.h>

//================================
// Functions...
//--------------------------------
void usage(void);
long cvtd(char str[100]);

int main (int argc, char **argv) {
	Display *display;
	int xpos = 0;
	int ypos = 0;
	char mydisp[256] = "";

	if (argc == 3) {
		xpos = atoi(argv[1]);
		ypos = atoi(argv[2]);
		strcpy(mydisp,getenv("DISPLAY"));
	} else {
	if (argc == 4) {
		xpos = atoi(argv[1]);
		ypos = atoi(argv[2]);
		strcpy(mydisp,argv[3]);
	} else {
		usage();
		exit(0);
	}
	}
	if (!(display = XOpenDisplay(mydisp))) {
		return fprintf(stderr, "unable to open display\n"), 11;
	}
	XWarpPointer (
		display,None,DefaultRootWindow(display),0,0,0,0,xpos,ypos
	);
	XFlush(display);
	return(0);
}

//================================
// usage...
//--------------------------------
void usage(void) {
	printf("usage: wrap X Y [ display ]\n");
}

//================================
// cvtd...
//--------------------------------
long cvtd(char str[100]) {
	int  base  = 16;
	long erg   = 0;
	int  l,i,x;
	char w[2];

	l = strlen(str);

	for (i=l-1;i>=0;i--) {
		w[0] = str[i];
		w[1] = '\0';
		switch(w[0]) {
			case 'a'|'A' : sprintf(w,"10"); break; 
			case 'b'|'B' : sprintf(w,"11"); break;
			case 'c'|'C' : sprintf(w,"12"); break;
			case 'd'|'D' : sprintf(w,"13"); break;
			case 'e'|'E' : sprintf(w,"14"); break;
			case 'f'|'F' : sprintf(w,"15"); break;
		}
		x = atoi(w);
		erg = erg + x * pow(base,l-i-1);
	}
	return(erg);
}
