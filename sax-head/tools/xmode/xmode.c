/***************
FILE          : xmode.c
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
#include <string.h>
#include "xmode.h"

int HGTn = 7;

//================================================
// calculate the fbmode from the XFree86 timings
// -----------------------------------------------
int XmodeCalculateFbMode (struct xmode *data,struct xtiming *t) {
	int pixclock;
	int left_margin;
	int right_margin;
	int upper_margin;
	int lower_margin;
	int hsync_len;
	int vsync_len;

	// clock...
	// ----------
	pixclock     = 1000000 / (data->dac/1e6);

	// horizontal fb timing...
	// ------------------------
	left_margin  = t->htotal - t->hsyncend;
	right_margin = t->hsyncstart - t->hdisp; 
	hsync_len    = t->hsyncend - t->hsyncstart;

	// vertical fb timing...
	// -----------------------
	upper_margin = t->vtotal - t->vsyncend;
	lower_margin = t->vsyncstart - t->vdisp;
	vsync_len    = t->vsyncend - t->vsyncstart;

	// save framebuffer timings to the XFree86 timings
	// ------------------------------------------------
	data->dac     = pixclock;
	t->hdisp      = left_margin;
	t->hsyncstart = right_margin;
	t->hsyncend   = lower_margin;   // hsync_len
	t->htotal     = 0;

	t->vdisp      = upper_margin;
	t->vsyncstart = hsync_len;      // lower_margin
	t->vsyncend   = vsync_len;
	t->vtotal     = 0;

	return(0);
}

//================================================
// check horizontal and vertical timings
//------------------------------------------------
int XmodeValidate (struct xtiming *t) {
	int ht = 0;
	int vt = 0;

	// check horizontal timing...
	if ((t->hdisp < t->hsyncstart)     &&
		(t->hsyncstart < t->hsyncend)  && 
		(t->hsyncend < t->htotal)      
	) {
		ht = 1;
	}

	// check vertical timing...
	if ((t->vdisp <= t->vsyncstart)    &&
		(t->vsyncstart <= t->vsyncend) && 
		(t->vsyncend <= t->vtotal)     
	) {
		vt = 1;
	}
	if ((ht == 0) || (vt == 0)) {
		return(0);
	}
	return(1);
}

//================================================
// get properties hsync and vsync from the timing
//------------------------------------------------
int XmodeProp (struct xmode *data,struct xtiming *t,float *zf,float *rr) {
	float hsync;
	float vsync;

	*zf = 0;
	*rr = 0;
	hsync = data->dac / t->htotal;
	vsync = hsync / t->vtotal;
	if ((hsync <= 0) || (vsync <= 0)) {
		return(-1);
	}
	*zf = hsync;
	*rr = vsync;
	return(0);
}

//================================================
// check size and position and correct it
//------------------------------------------------
int XmodeCheck (struct xmode *data,struct xtiming *t) {
	int step = (data->x + data->vsync - data->y) / 100;
	int corr = 8*step*step;
	int nstart;
	int nend;

	// check size...
	t->htotal = t->htotal + corr;

	// check position...
	nstart = t->hsyncstart + corr;
	nend   = t->hsyncend   + corr;
	if ((t->hdisp >= nstart) && (nend >= t->htotal)) {
		t->hsyncstart = nstart;
		t->hsyncend   = nend;
	} 
	return(0);
}

//================================================
// calculate horizontal/vertical timing values
//------------------------------------------------
int XmodeTiming (struct xmode *data,struct xtiming *ht) {
	struct xmode *saveData = NULL;
	double HConst  = 1.3;    // 1.3
	double VConst  = 1.045;  // 1.045
	double HSP_min = 1;
	double DCF;
	double HSP_us;
	double RR;

	int HFL;                // Horizontal frame length
	int VFL;                // Vertical frame length
	int HFT_us;             // Horizontal frame timing in u sec 
	int HSP;                // Number of horizontal sync impuls
	int VSP;                // Number of vertical sync impulse

	int HGT1    = 16;
	int HGT2    = HGTn*16;
	int iterate = 255;

	saveData = (struct xmode*) malloc (sizeof(struct xmode));
	memcpy (saveData,data,sizeof(struct xmode));
	while(iterate != 0) {
		iterate = 0;
 
		// calculate...
		// -------------- 
		HFL = (int)(data->x * HConst);
		HFL = (HFL / 8) * 8;        
		DCF = (HFL * (data->hsync/1000)) / 1000;

		HSP = HFL - (data->x + HGT1 + HGT2);
		HSP_us = HSP / DCF;

		VFL = (int)(data->y * VConst);
		HFT_us = (int)(HFL / DCF);
		VSP = 150 / HFT_us;

		RR = ( ((DCF / HFL) * 1000) / VFL)*1000;

		// check...
		// ---------
		if (DCF > (data->dac/1e6)) {
			data->hsync-=100; iterate = 255;
		}
		if (RR > data->vsync) {
			data->hsync-=100; iterate = 255;
		}
		if (HSP_us < HSP_min) {
			data->hsync-=100; iterate = 255;
		}
	}

	if ( (data->hsync / VFL) > data->vsync_orig) {
		data->vsync = data->vsync_orig;
		data->hsync = data->y * data->vsync;
		data->dac = ((int)((data->x * 1.3) / 8)) *8 *data->hsync;
		XmodeTiming (data,ht);
	}

	ht->flag       = data->flag; 
	ht->hdisp      = data->x;
	ht->hsyncstart = data->x + HGT1;
	ht->hsyncend   = data->x + HGT1 + HSP;
	ht->htotal     = HFL;
	ht->vdisp      = data->y;
	ht->vsyncstart = data->y;
	ht->vsyncend   = data->y + VSP;
	ht->vtotal     = VFL;

	if (! XmodeValidate (ht)) {
	if ( HGTn >= 0) {
		HGTn--;
		memcpy (data,saveData,sizeof(struct xmode));
		free (saveData);
		XmodeTiming (data,ht);
	}
	}
	// Nvidia Modeline Checks
	// 1) vsyncend - vsyncstart <= 16
	// ---
	if (ht->vsyncend - ht->vsyncstart > 16) {
		ht->vsyncend = ht->vsyncstart + 16;
	}
	// 2) [ Add missing checks here ]
	// ---
	return(0); 
}

//================================================
// init data and look at the options
//------------------------------------------------
int XmodeInit (int argc,char *argv[],struct xmode *data,int *check) {
	int  dacdefined     = FALSE;
	int  hsyncdef       = FALSE;
	int  vsyncdef       = FALSE; 
	int  nocheckdefined = FALSE;
	int  c;
	int  sum;

	data->x     = 640;
	data->y     = 480;
	data->vsync = 60;
	data->hsync = data->y * data->vsync;
	data->dac   = ((int)((data->x * 1.3) / 8)) *8 *data->hsync;
	data->fbdev = 0;
	data->flag  = 0;
	data->vsync_orig = data->vsync;

	while (1) {
	int option_index = 0;
	static struct option long_options[] =
	{
		{"dacspeed"   , 1 , 0 , 'd'},
		{"xdim"       , 1 , 0 , 'x'},
		{"ydim"       , 1 , 0 , 'y'},
		{"refresh"    , 1 , 0 , 'r'},
		{"fbdev"      , 1 , 0 , 'f'},
		{"sync"       , 1 , 0 , 's'},
		{"nocheck"    , 0 , 0 , 'n'},
		{"help"       , 0 , 0 , 'h'},
		{0            , 0 , 0 , 0  }
	};
	c = getopt_long (
		argc, argv, "hd:x:y:r:s:nf",long_options, &option_index
	);
	if (c == -1) {
		break;
	}

	switch (c) {
	case 0:
		fprintf (stderr,"option %s", long_options[option_index].name);
		if (optarg) {
			fprintf (stderr," with arg %s", optarg);
		}
		fprintf (stderr,"\n");
	break;

	case 'h':
		return(-1);

	case 'r':
		vsyncdef = TRUE;
		data->vsync = atof(optarg);
		data->vsync_orig = data->vsync;
	break;

	case 's':
		hsyncdef = TRUE; 
		data->hsync = atof(optarg) * 1e3;
	break;

	case 'x':
		data->x = atoi(optarg);
	break;

	case 'y':
		data->y = atoi(optarg);
	break;
   
	case 'd':
		dacdefined = TRUE;
		data->dac = atof(optarg) * 1e6;
	break;

	case 'f':
		data->fbdev = 1;
	break;

	case 'n':
		nocheckdefined = TRUE;
	break;

	default:
		return(-1);
	}
	}

	// if no check is defined add 3 Hz for exactly matching
	// -----------------------------------------------------
	if (nocheckdefined == TRUE) {
		data->vsync += 3;
	}

	// use doublescan lines for resolutions
	// lower than 320x200
	// ----
	sum = data->x + data->y;
	if (sum < 1120) {
		data->vsync *= 2;
		data->vsync_orig = data->vsync;
		data->flag = FLAG_DOUBLE_SCAN;
	}

	// look which sync frequencies we will use
	// -----------------------------------------
	if ((hsyncdef == TRUE) && (vsyncdef == TRUE)) {
	if ((data->hsync/1e3) > data->vsync) { 
		data->hsync = data->y * data->vsync;
	} else {
		data->vsync = data->hsync / data->y;
	}
	} else if (hsyncdef == TRUE) {
		data->vsync = data->hsync / data->y;
	} else if (vsyncdef == TRUE) {
		data->hsync = data->y * data->vsync;
	}
	if (dacdefined == FALSE) {
		data->dac = ((int)((data->x * 1.3) / 8)) *8 *data->hsync;
	}
	*check = nocheckdefined;
	return(0);
}

//================================================
// how to use this program
//------------------------------------------------
void XmodeUsage(void) {
	printf("usage: xmode [ options ]\n");
	printf("options:\n");
	printf("  [ -d | --dacspeed ]\n");
	printf("    ramdac speed in Mhz\n");
	printf("  [ -r | --refresh ]\n"); 
	printf("    refresh rate in Hz\n");
	printf("  [ -s | --sync ]\n");
	printf("    horizontal sync rate in Khz\n");
	printf("  [ -x | --xdim ]\n");
	printf("    X - dimension in pixels\n");
	printf("  [ -y | --ydim ]\n");
	printf("    Y - dimension in pixels\n");
	printf("  [ -n | --nocheck ]\n");
	printf("    do not check the geometry\n");
	printf("  [ -f | --fbdev ]\n");
	printf("    calculate framebuffer mode\n");
	exit(0);
}

