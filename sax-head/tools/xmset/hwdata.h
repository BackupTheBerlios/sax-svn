/**************
FILE          : hwdata.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : provide header to include the syslib.h 
              : and all other related headers
              :
              :
STATUS        : development
**************/
#ifndef HWDATA_H
#define HWDATA_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hd.h>

//===================================
// Defines...
//-----------------------------------
#define ID_VALUE(id) ((id) & 0xffff)
#ifndef HD_VERSION
#define HD_VERSION 1
#endif

#define MAXVESA    100

//===================================
// Types...
//-----------------------------------
typedef char str[512];

typedef struct {
	int x;
	int y;
	int hsync;
	int vsync;
} mode;

typedef struct MouseData_t {
	str name;
	str protocol;
	str device;
	int emulate;
	int wheel;
	int buttons;
	struct MouseData_t* next;
} MouseData;

typedef struct {
	str id;
	int cards;
	long memory;
	int dacspeed;
	int clkcount;
	float *clocks;
	int vesacount;
	mode vmodes[MAXVESA];
	int hsync_max;
	int vsync_max;
	str chipset;
	str primary;
	str ddc;
	str dds;
	str clkstr;
	str displaytype;
	int vmdepth;
	int dpix;
	int dpiy;
	str model;
	str vendor;
} MsgDetect;

//===================================
// Prototypes...
//-----------------------------------
MouseData* MouseGetData   (void);
MsgDetect  MonitorGetData (void);

#endif
