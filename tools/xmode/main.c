/***************
FILE          : main.c
***************
PROJECT       : X11 Modeline algorithm
              :
BELONGS TO    : XFree86 Inc.
              :  
              :
DESCRIPTION   : xmode provides the libXmode to 
              : calculate modelines with a minimum on
              : requirements
              : 
              :
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "xmode.h"

//=====================================
// Main Modeline stuff...
//-------------------------------------
int main(int argc,char *argv[]) {
	struct xmode *data;
	struct xtiming *timing;
	int state;
	int nocheck;
	float zf;
	float rr;

	data   = (struct xmode*) malloc(sizeof(struct xmode));
	timing = (struct xtiming*) malloc(sizeof(struct xtiming));

	state = XmodeInit(argc,argv,data,&nocheck);
	if (state < 0) {
		XmodeUsage(); 
		exit(0);
	}
	XmodeTiming(data,timing);
	if (nocheck == FALSE) { 
		XmodeCheck(data,timing); 
	}
	XmodeProp(data,timing,&zf,&rr);
	// ...
	// give us the result hsync/vsync/modeline...
	// ---
	printf("%.0f\n",zf/1000);
	if (timing->flag == FLAG_DOUBLE_SCAN) {
		rr /= 2;
	} 
	printf("%.0f\n",rr);

	if (data->fbdev == 1) {
		//=====================================
		// Framebuffer Modeline...
		//-------------------------------------
		XmodeCalculateFbMode(data,timing);
		printf(
			"Modeline \"%dx%d\" %.0f %d %d %d %d %d %d",
			data->x,data->y,data->dac,
			timing->hdisp,timing->hsyncstart,timing->hsyncend,
			timing->vdisp,timing->vsyncstart,timing->vsyncend
		);
	} else {
		//=====================================
		// XFree86 Modeline...
		//-------------------------------------
		printf(
			"Modeline \"%dx%d\" %.2f %d %d %d %d %d %d %d %d",
			data->x,data->y,data->dac/1e6,
			timing->hdisp,timing->hsyncstart,timing->hsyncend,timing->htotal,
			timing->vdisp,timing->vsyncstart,timing->vsyncend,timing->vtotal
		);
	}
	if (timing->flag == FLAG_DOUBLE_SCAN) {
		printf(" Doublescan\n");
	} else {
		printf("\n");
	}
	return(0);
}
