/**************
FILE          : sax.h
***************
PROJECT       : SaX2 - library interface [header]
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : SaX2 - SuSE advanced X11 configuration 
              : 
              :
DESCRIPTION   : native C++ class library to access SaX2
              : functionality. Easy to use interface for
              : //.../
              : - importing/exporting X11 configurations
              : - modifying/creating X11 configurations 
              : ---
              :
              :
STATUS        : Status: Development
**************/
#ifndef SAX_SAX_H
#define SAX_SAX_H 1

/*! \mainpage libsax documentation
* libsax is a X11 configuration library for XOrg version v6.x
*/

//====================================
// Defines...
//------------------------------------
#define SAX_VERSION "4.8"

//====================================
// Includes...
//------------------------------------
#include "storage.h"
#include "export.h"
#include "import.h"
#include "init.h"
#include "config.h"

//====================================
// Manipulators...
//------------------------------------
#include "keyboard.h"
#include "pointers.h"
#include "desktop.h"
#include "extensions.h"
#include "layout.h"
#include "path.h"
#include "card.h"

//====================================
// Interface...
//------------------------------------
#include "interface.h"

#endif
