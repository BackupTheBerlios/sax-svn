/***************
FILE          : main.c
***************
PROJECT       : X11 Modeline algorithm
              :
BELONGS TO    : XFree86 Inc.
              :  
DESCRIPTION   : xmode provides the libXmode to 
              : calculate modelines with a minimum on
              : requirements. The GTF formular is used
              : to generate the timings
              : 
              :
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

#include "xmode.h"

//=====================================
// Main Modeline stuff...
//-------------------------------------
int main(int argc,char *argv[]) {
	struct xmode   *data;
	struct xtiming *timing;

	data   = (struct xmode*)   malloc(sizeof(struct xmode));
	timing = (struct xtiming*) malloc(sizeof(struct xtiming));

	//========================================
	// Get options and init the xmode struct
	//----------------------------------------
	if ((XmodeInit (argc,argv,data)) < 0) {
		XmodeUsage(); exit(0);
	}
	//========================================
	// Calculate GTF timings
	//----------------------------------------
	if (data->dac > 0) {
	while (timing->dac < data->dac) {
		XmodeTiming ( data,timing );
		data->vsync++;
	}
	} else if (data->hsync > 0) {
	while (timing->hsync < data->hsync) {
		XmodeTiming ( data,timing );
		data->vsync++;
	}
	} else {
		XmodeTiming ( data,timing );
	}

	//========================================
	// Print Modeline
	//----------------------------------------
	printf("%.0f\n",timing->hsync);
	printf("%.0f\n",timing->vsync);

	if (data->fbdev == 1) {
	//=====================================
	// Framebuffer Modeline...
	//-------------------------------------
	printf (
		"Modeline \"%dx%d\" %d %d %d %d %d %d %d",
		data->x, data->y, (int) rint(1000000.0/timing->dac),
		timing->htotal - timing->hsyncend,
		timing->hsyncstart - timing->hdisp,
		timing->vtotal - timing->vsyncend,
		timing->vsyncstart - timing->vdisp,
		timing->hsyncend - timing->hsyncstart,
		timing->vsyncend - timing->vsyncstart
	);
	} else {
	//=====================================
	// XFree86 Modeline...
	//-------------------------------------
	printf (
		"Modeline \"%dx%d\" %.2f %d %d %d %d %d %d %d %d",
		data->x,data->y,timing->dac,
		timing->hdisp,timing->hsyncstart,timing->hsyncend,timing->htotal,
		timing->vdisp,timing->vsyncstart,timing->vsyncend,timing->vtotal
	);
	}
	printf("\n");
	return(0);
}
