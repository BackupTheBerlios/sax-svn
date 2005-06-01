/**************
FILE          : capi.cpp
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : commandline configuration tool for the X
              : window system released under the GPL license
              :
DESCRIPTION   : SaX2 commandline system using libsax to provide
              : configurations for the most important X11 capabilities
              : //.../
              : - capi.cpp: main program start sequence
              : - capi.h: buildins and defines
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef CAPIMAIN_H
#define CAPIMAIN_H 1

//====================================
// Includes...
//------------------------------------
#include <qapplication.h>
#include <getopt.h>

//====================================
// Includes...
//------------------------------------
#include "../libsax/sax.h"
#include "../api/common/log.h"

//====================================
// Includes...
//------------------------------------
#include "color.h"

//===================================
// Prototypes...
//-----------------------------------
void usage ( int );

#endif
