/**************
FILE          : memory.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
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
	hd_data_t* hd_data;
	hd_res_t* res;
	hd_t* hd;
	unsigned long mem = 0;

	hd_data = (hd_data_t*)calloc(1, sizeof *hd_data);
	hd = hd_list(hd_data, hw_framebuffer, 1, NULL);

	if (! hd) {
		return (mem);
	}
	for (res = hd->res; res; res = res->next) {
	switch(res->any.type) {
		case res_phys_mem:
			mem = res->phys_mem.range / 1024;
			return (mem);
		break;
		default:
			// nothing to do for default...
		break;
	}	
	}
	hd = hd_free_hd_list (hd);
	return (mem);
}
