/**************
FILE          : hwdata.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
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

//===================================
// Types...
//-----------------------------------
typedef char str[512];

typedef struct MouseData_t {
 str name;
 str protocol;
 str device;
 int emulate;
 int wheel;
 int buttons;
 struct MouseData_t* next;
} MouseData;

//===================================
// Prototypes...
//-----------------------------------
MouseData* MouseGetData (void);

#endif
