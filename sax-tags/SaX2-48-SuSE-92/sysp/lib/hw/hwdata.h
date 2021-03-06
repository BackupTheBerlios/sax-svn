/**************
FILE          : hwdata.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Sch�fer <ms@suse.de>
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

#include "../syslib.h"

//===================================
// Defines...
//-----------------------------------
#define ID_VALUE(id) ((id) & 0xffff)
#ifndef HD_VERSION
#define HD_VERSION 1
#endif

//===================================
// Prototypes...
//-----------------------------------
extern "C" char *vend_id2str(unsigned vend);

KbdData* KeyboardGetData (void);
MouseData* MouseGetData (void);
int TvSupport (void);
MsgDetect MonitorGetData (void);
unsigned long MemorySize (void);

#endif
