/**************
FILE          : mouse.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : provide functions to obtain mouse
              : information using [libhd] 
              :
              :
STATUS        : development
**************/
#include "hwdata.h"

//===================================
// GetMouseData...
//-----------------------------------
MouseData* MouseGetData(void) {
	hd_data_t *hd_data = NULL;
	driver_info_t *di  = NULL;
	MouseData* data    = NULL;
	MouseData* first   = NULL;
	MouseData* last    = NULL;
	hd_t *hd           = NULL;
	hd_t *first_dev    = NULL;

	hd_data = (hd_data_t*)calloc(1, sizeof *hd_data);
	hd_data->flags.fast = 1;
	hd = hd_list(hd_data, hw_mouse, 1, NULL);
	first_dev = hd;

	for(; hd; hd = hd->next) {
		//===================================
		// create new element and defaults...
		// -----------------------------------
		data = (MouseData*)malloc(sizeof(MouseData));
		strcpy(data->name,hd->model);
		strcpy(data->device,"/dev/mouse");
		strcpy(data->protocol,"Auto");
		data->buttons = -1;
		data->wheel   = -1;
		data->emulate = 1;
		data->next    = NULL;

		if (last != NULL) {
			last->next = data;
		}
		if (hd == first_dev) {
			first = data;
		}
		//===================================
		// save hddata to struct...
		// ----------------------------------
		#if HD_VERSION >= 5
		di = hd->driver_info;
		#else
		di = hd_driver_info(hd_data, hd);
		#endif
		if(di && di->any.type == di_mouse && di->mouse.xf86) {
			strcpy(data->protocol,di->mouse.xf86);
			#ifdef WHEEL_SUPPORT
			if (di->mouse.buttons) {
				data->buttons = di->mouse.buttons;
			}
			if (di->mouse.wheels) {
				data->wheel   = di->mouse.wheels;
			}
			if (data->buttons) {
			if (
				(data->buttons >= 3) || 
				(strcmp(data->protocol,"imps/2") == 0) ||
				(strcmp(data->protocol,"explorerps/2") == 0)
			) {
				data->emulate = 0;
			}
			}
			if (data->wheel) {
			if (data->wheel >= 1) {
				data->buttons = (data->wheel * 2) + data->buttons;
			}
			}
			#endif
		}
		if (hd->unix_dev_name) {
			strcpy(data->device,hd->unix_dev_name);
		}
		last = data;
	}
	return(first);
}
