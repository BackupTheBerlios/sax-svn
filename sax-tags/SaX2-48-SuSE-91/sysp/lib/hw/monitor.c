/**************
FILE          : monitor.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : provide functions to get monitor data
              : and resolution information using [libhd] 
              :
              :
STATUS        : development
**************/
#include "hwdata.h"

//===================================
// Internal functions...
//-----------------------------------
void toUpper  (char*);
#if HD_VERSION >= 5
char *dumpHID (hd_id_t*, char*, int);
#endif

//===================================
// MonitorGetData...
//-----------------------------------
MsgDetect MonitorGetData (void) {
	MsgDetect  display;
	hd_data_t* hd_data;
	hd_res_t*  res;
	hd_t *hd;
	driver_info_t *di;
	driver_info_t *di0;
	unsigned bestX = 0;
	unsigned bestY = 0;
	int i;
	char buf[256];

	hd_data = (hd_data_t*)calloc(1, sizeof *hd_data);
	hd = hd_list(hd_data, hw_monitor, 1, NULL);

	if (! hd) {
		return (display);
	}
	sprintf(display.model,hd->model);
    //printf("+++ %s\n",hd->id);
	// ...
	// Check for LCD panel with data located in the VESA
	// BIOS. Note not all display can be detected here
	// ---
	#if HD_VERSION >= 5
	sprintf(display.displaytype,"CRT");
	if (strstr (dumpHID (&hd->device, buf, sizeof buf),"LCD")) {
		sprintf(display.displaytype,"LCD/TFT");
	}
	#endif
	#if HD_VERSION >= 5
	di0 = hd->driver_info;
	#else
	di0 = hd_driver_info(hd_data, hd);
	#endif
	for(di = di0, i = 0; di; di = di->next, i++) {
	if (di->any.type == di_display) {
		display.hsync_max = 0;
		display.vsync_max = 0;
		if (di->display.max_hsync) {
		display.hsync_max = (int)di->display.max_hsync;
		}
		if (di->display.max_vsync) {
		display.vsync_max = (int)di->display.max_vsync;
		}
		if (di->display.width) {
		bestX = di->display.width;
		bestY = di->display.height;
		}
		break;
	}
	}

	for(; hd; hd = hd->next) {
		sprintf(display.ddc,"%c",'\0');

		#ifdef DDC_SUPPORT
		#if HD_VERSION >= 5
		snprintf(display.ddc, strlen(display.ddc) - 1, "%s%04x",
		vend_id2str(hd->vendor.id),ID_VALUE(hd->device.id)
		);
		#else
		snprintf(display.ddc, strlen(display.ddc) - 1, "%s%04x",
		vend_id2str(hd->vend),ID_VALUE(hd->dev)
		);
		#endif
		toUpper (display.ddc);
		#endif

		int count = 0;
		for(res = hd->res; res; res = res->next) {
		switch (res->any.type) {
			case res_size:
			if (res->size.unit == size_unit_cm) {
			display.dpix = 0;
			display.dpiy = 0;
			if ((res->size.val1) && (res->size.val2)) {
			display.dpix = (int)res->size.val1;
			display.dpiy = (int)res->size.val2;
			}
			}
			break;

			case res_monitor:
			if ( 
				((res->monitor.width  > bestX) && (bestX > 0)) &&
				((res->monitor.height > bestY) && (bestY > 0))
			) { 
				// ...
				// resolution(s) higher than best resolution
				// won`t be handled here
				// ---
				break;
			}
			display.vmodes[count].x = res->monitor.width;
			display.vmodes[count].y = res->monitor.height;
			display.vmodes[count].vsync = res->monitor.vfreq;
			display.vmodes[count].hsync = (int)(
				(res->monitor.height*res->monitor.vfreq)/1000
			);
			if (display.vmodes[count].hsync > display.hsync_max) {
				display.vmodes[count].hsync = display.hsync_max;
			}
			count++;
			display.vesacount = count;
			break;
		
			default:
			break;
		}
		}
		if ((bestX > 0) && (bestY > 0)) {
			display.vmodes[count].x = bestX;
			display.vmodes[count].y = bestY;
			display.vmodes[count].hsync = display.hsync_max;
			display.vmodes[count].vsync = display.vsync_max;
			count++;
			display.vesacount = count;
		}
		// ...
		// save only the first data block
		// normally hwinfo will only detect data from
		// one monitor
		// ---
		break;
	}
	return (display);
}

//=====================================
// Transform string to upper case
//-------------------------------------
void toUpper(char* string) {
	int l=strlen(string);
	int x=0;
	for (x=0;x<l;x++) {
	if ((char)string[x] >= 97 && (char)string[x] <= 122) {
		string[x]=string[x]-32;
	}
	}
}

//=====================================
// Dump HID device name
//-------------------------------------
#if HD_VERSION >= 5
char* dumpHID (hd_id_t *hid, char *buf, int buf_size) {
	char *s;
	int i;

	*buf = 0;
	i = strlen(buf);
	s = buf + i;
	if (hid->name) {
		snprintf(s, buf_size - 1, "%s", hid->name);
	}
	return buf;
}
#endif
