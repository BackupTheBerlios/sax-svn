/***************
FILE          : xmode.h
***************
PROJECT       : X11 Modeline algorithm
              :
BELONGS TO    : XFree86 Inc.
              :  
              :
DESCRIPTION   : header file for xmode.c
              : install this header to /usr/include/
              : 
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XMODE_H
#define XMODE_H 1

//===========================================
// Defines...
//-------------------------------------------
#define TRUE  1
#define FALSE 0
#define FLAG_DOUBLE_SCAN 7

//===========================================
// Structs...
//-------------------------------------------
struct xmode {
 double dac;
 float  hsync;
 float  vsync;
 float  vsync_orig;
 int    x;
 int    y;
 int    fbdev;
 int    flag;
};

struct xtiming {
 int hdisp;
 int hsyncstart;
 int hsyncend;
 int htotal;
 int vdisp;
 int vsyncstart;
 int vsyncend;
 int vtotal;
 int flag;
};

//===========================================
// Prototypes...
//-------------------------------------------
int XmodeInit (int argc,char *argv[],struct xmode *data,int *check);
int XmodeTiming (struct xmode *data,struct xtiming *vt);
int XmodeCheck (struct xmode *data,struct xtiming *t);
int XmodeProp (struct xmode *data,struct xtiming *t,float *zf,float *rr);
void XmodeUsage(void);
int XmodeValidate (struct xtiming *t);
int XmodeCalculateFbMode (struct xmode *data,struct xtiming *t);

#endif
