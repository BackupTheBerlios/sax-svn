/**************
FILE          : xbounce.c
***************
PROJECT       : Toshiba keyboard bounce bug
              :
BELONGS TO    : Workaround/Fix for newer Toshiba Notebooks
              :  
BESCHREIBUNG  : xbounce will enable the bouncing keys for
              : the X-Server and will therefore prevent a delay
              : timeout before the keyboard report additional
              : unwanted key events
              :
STATUS        : Status: Up-to-date
**************/
#include <unistd.h>
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

//===============================
// Structs...
//-------------------------------
typedef struct _xkbdisp {
	Display*   dpy;
	XkbDescPtr xkb;
} xkbdisp;

//===============================
// Prototypes...
//-------------------------------
xkbdisp* XOpen(void);

//===============================
// main...
//-------------------------------
int main (int argc, char*argv[]) {
	XkbDescPtr xkb;
	xkbdisp* desc = XOpen();
	if (! desc) {
		exit (1);
	}
	xkb = desc->xkb;
	//==================================================
	// Set bounce keys with a delay of 20 ms
	//--------------------------------------------------
	xkb->ctrls->enabled_ctrls    |= XkbAccessXKeysMask;
	xkb->ctrls->enabled_ctrls    |= XkbBounceKeysMask;
	xkb->ctrls->axt_ctrls_mask   |= XkbBounceKeysMask;
	xkb->ctrls->axt_ctrls_values |= XkbBounceKeysMask;
	xkb->ctrls->debounce_delay = 20;

	//==================================================
	// Set expire status of bounce keys to be
	// unchanged after accessX timed out. This makes
	// bouncekeys persist indefinitely
	//--------------------------------------------------
	xkb->ctrls->axt_ctrls_mask   &= ~XkbBounceKeysMask;
	xkb->ctrls->axt_ctrls_values &= ~XkbBounceKeysMask;
	
	//==================================================
	// Apply controls and exit
	//--------------------------------------------------
	XkbSetControls(desc->dpy, XkbAllControlsMask, xkb);
	XSync (desc->dpy, False);
	XCloseDisplay (desc->dpy);
	exit (0);
}

//===============================
// XOpen...
//-------------------------------
xkbdisp* XOpen (void) {
	xkbdisp* desc;
	Display *dpy;
	XkbDescPtr xkb;
	int major, minor,op, event, error;
	int timeout = 5;
	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf (stderr,
			"xbounce: could not open display\n"
		);
		return (NULL);
	}
	major = XkbMajorVersion;
	minor = XkbMinorVersion;
	if (XkbQueryExtension(dpy, &op, &event, &error, &major, &minor) == 0) {
		fprintf (stderr,
			"xbounce: unable to initialize XKEYBOARD extension"
		);
		return (NULL);
	}
	while (timeout > 0) {
		xkb = XkbGetKeyboard (
			dpy, XkbGBN_AllComponentsMask, XkbUseCoreKbd
		);
		if (xkb == NULL || xkb->geom == NULL) {
			timeout -= 1; sleep(1);
		} else {
			break;
		}
	}
	if (timeout <= 0) {
		fprintf (stderr,
			"xbounce: couldn't get keyboard\n"
		);
		return (NULL);
	}
	XkbGetControls(dpy, XkbAllControlsMask, xkb);
	if (xkb->ctrls == NULL) {
		xkb->ctrls = (XkbControlsPtr)malloc(sizeof(XkbControlsRec));
	}
	desc = (xkbdisp*) malloc (sizeof (xkbdisp));
    desc->dpy = dpy;
	desc->xkb = xkb;
	return (desc);
}
