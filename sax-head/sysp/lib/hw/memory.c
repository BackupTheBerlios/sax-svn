/**************
FILE          : memory.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : provide functions to get memory info from
              : the VESA BIOS using [libhd] 
              :
              :
STATUS        : development
**************/
#include "hwdata.h"

//===================================
// MonitorSize...
//-----------------------------------
unsigned long MemorySize (void) {
	char* video = qx ( GETMEMORY,STDOUT );
	if (video) {
		return (atol (video));
	} else {
		return (0L);
	}
}
