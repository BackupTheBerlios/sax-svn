/**************
FILE          : fake.c
***************
PROJECT       : SaX ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool X11 version 4.0
              :  
DESCRIPTION   : fake bind mouse buttons to a specific key
              : to be invoked as it would be the mouse itself
              : 
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

//=================================
// Functions...
//---------------------------------
void usage(void);

int main (int argc, char **argv) {
	Display *display;
	int BtnNr = 1;
	char mydisp[256] = "";

	if ((argc != 3) && (argc != 4)) {
		usage();
		exit(1);
	}
	if (argc == 3) {
		strcpy(mydisp,getenv("DISPLAY"));
	} else {
		strcpy(mydisp,argv[3]);
	}
	BtnNr = atoi(argv[1]);
	if (BtnNr <= 0) {
		printf ("wrong button number...\n");
		exit(1);
	} 
	if (!(display = XOpenDisplay(mydisp))) {
		return fprintf(stderr, "unable to open display\n"), 11;
	}
	if (strcmp(argv[2],"release") == 0) { 
		XTestFakeButtonEvent(display, BtnNr, False, 0);
	} else {
		XTestFakeButtonEvent(display, BtnNr, True, 0);
	}
	XFlush(display);
	XCloseDisplay(display);
	return(0);
}

//=================================
// usage...
//---------------------------------
void usage(void) {
	printf("usage: fake <ButtonNumber> <press|release> [ display ]\n");
}

