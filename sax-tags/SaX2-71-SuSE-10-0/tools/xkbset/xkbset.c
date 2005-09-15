/**************
FILE          : xkbset.c
***************
PROJECT       : SaX ( SuSE advanced X configuration )
              :
BELONGS TO    : Configuration tool X11 version 4.0
              :  
              :
BESCHREIBUNG  : xkbset change the keyboard properties on the
              : fly of a running X-Server
              : 
              :
STATUS        : Status: Up-to-date
**************/
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBgeom.h>
#include <X11/extensions/XKM.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBui.h>
#include <X11/extensions/XKBrules.h>
#include <X11/Xproto.h>
#include <X11/Xfuncs.h>

#include "../whois/remote.h"

//===============================
// Types...
//-------------------------------
typedef char str[256];

//===============================
// Global variables...
//-------------------------------
int MajorVersion, MinorVersion;
int EventBase, ErrorBase;

//===============================
// Defines...
//-------------------------------
#define MINMAJOR 0
#define MINMINOR 5
#define TRUE     1
#define FALSE    0

//===============================
// Prototypes...
//-------------------------------
Display* XOpen(char name[256]);
int  strsplit(str string,char deli,str left,str right);
int  LoadKBD(Display *disp, int argc, str argv[]);
void ShowKBD (Display *disp);
void usage(void);

//===============================
// main...
//-------------------------------
int main (int argc, char*argv[]) {
	Display* dpy;               // Display pointer
	int  error = 0;             // error codes
	int  c;                     // commandline parameters
	int  load = FALSE;          // Keyboard load flag
	char mydisp    [256] = "";  // display string
	char xkbstring [256] = "";  // apply string
	str  list[8];               // str list for single apply parameters

	//=================================
	// get commandline options...
	//---------------------------------
	while (1) {
	int option_index = 0;
	static struct option long_options[] =
	{
		{"load"     , 1 , 0 , 'l'},
		{"display"  , 1 , 0 , 'd'},
		{"show"     , 0 , 0 , 's'},
		{"help"     , 0 , 0 , 'h'},
		{0          , 0 , 0 , 0  }
	};
	c = getopt_long (argc, argv, "hd:l:s",long_options, &option_index);
	if (c == -1)
	break;

	switch (c) {
	case 0:
		break;
	case 'h':
		usage();
	case 'd':
		strcpy(mydisp,optarg);
		break;
	case 'l':
		load = TRUE;
		strcpy(xkbstring,optarg);
		break; 
	case 's':
		load = FALSE;
		break;
	default:
		exit(1);
	}
	}

	//=================================
	// open display...
	//---------------------------------
	if (strcmp(mydisp,"") == 0) {
		strcpy(mydisp,"null");
	}
	dpy = XOpen(mydisp);
	if (! dpy) {
		fprintf(stderr,"xkbset: couldn't open display: %s\n",mydisp);
		exit(1);
	}

	//=================================
	// apply new keyboard definition
	// -------------------------------- 
	if (load == TRUE) {
		strsplit(xkbstring,':',list[1],xkbstring);
		strsplit(xkbstring,':',list[2],xkbstring);
		strsplit(xkbstring,':',list[3],xkbstring);
		strsplit(xkbstring,':',list[4],xkbstring);
		strsplit(xkbstring,':',list[5],xkbstring);
		strsplit(xkbstring,':',list[6],xkbstring);
  
		strcpy(list[7],"load");
		error = LoadKBD(dpy,7,list);
	}

	//=================================
	// show cur. keyboard information
	// --------------------------------
	if (load == FALSE) {
		ShowKBD(dpy);
	}
	exit(error); 
}


//===============================
// LoadKBD...
//-------------------------------
int LoadKBD (Display *disp, int argc, str argv[]) {
	XkbComponentNamesRec    getcomps;
	XkbDescPtr              xkb;
	Bool                    loadit = True;

	if (argc == 7) {
	if (!strcmp(argv[7], "load")) {
		loadit = True;
	} else
	if (!strcmp(argv[7], "noload")) {
		loadit = False;
	} else {
		return (-1);
	}
	} else {
		return (-1);
	}
	getcomps.keycodes = argv[1];
	getcomps.compat   = argv[2];
	getcomps.types    = argv[3];
	getcomps.symbols  = argv[4];
	getcomps.geometry = argv[5];
	xkb = XkbGetKeyboardByName (
		disp, XkbUseCoreKbd, &getcomps, XkbGBN_AllComponentsMask, 
		XkbGBN_AllComponentsMask&(~XkbGBN_GeometryMask),
		loadit
	);
	if (!xkb) {
		return(-1);
	}
	if (xkb->names->geometry == 0) {
		xkb->names->geometry = xkb->geom->name;
	}
	return(0);
}

//===============================
// ShowKBD...
//-------------------------------
void ShowKBD (Display *disp) {
	XkbDescPtr  xkb;
	Bool        loadit = False;

	xkb = XkbGetKeyboardByName(
		disp, XkbUseCoreKbd, 0, XkbGBN_AllComponentsMask,
		XkbGBN_AllComponentsMask&(~XkbGBN_GeometryMask),
		loadit
	);
	if (! xkb) {
		fprintf(stderr,"xkbset: could not obtain keyboard setup\n");
	}
	if (xkb->names->geometry == 0) {
		xkb->names->geometry = xkb->geom->name;
	}

	printf("symbols:   %s\n",XGetAtomName(disp,xkb->names->symbols));
	printf("keycodes:  %s\n",XGetAtomName(disp,xkb->names->keycodes));
	printf("geometry:  %s\n",XGetAtomName(disp,xkb->names->geometry));
	printf("types:     %s\n",XGetAtomName(disp,xkb->names->types));
	printf("compat:    %s\n",XGetAtomName(disp,xkb->names->compat));
}

//===============================
// XOpen...
//-------------------------------
Display* XOpen(char name[256]) {
	Display *dpy;
	char *display = NULL;

	display = (char*)malloc(sizeof(char)*256);

	if (strcmp(name,"null") == 0) {
	if (getenv("DISPLAY") != NULL) {
		display = getenv("DISPLAY");
	}
	if (display == NULL) {
		return(NULL);
	}
	} else {
		strcpy(display,name);
	}
	if (remoteDisplay (display)) {
		exit (0);
	}
	if(!(dpy = XOpenDisplay(display))) {
		return(NULL);
	}
	return(dpy);
}

//===============================
// usage...
//-------------------------------
void usage(void) {
	printf("usage: xkbset -l <xkbstring> [ -d <display> ]\n");
	printf("  xkbstring is a : seperated list including\n");
	printf("  the following items\n");
	printf("items:\n");
	printf("  [ keycodes ] default: xfree86\n");
	printf("  [ compat   ] default: complete\n");
	printf("  [ types    ] default: complete\n");
	printf("  [ symbols  ] default: en_US(pc101) us keyboard\n");
	printf("  [ geometry ] default: pc(pc101)\n");
	exit(0);
}
