/**************
FILE          : keyboard.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : provide functions to obtain keyboard
              : information using [libhd] 
              :
              :
STATUS        : development
**************/
#include "hwdata.h"

//===================================
// GetKeyboardData...
//-----------------------------------
KbdData* KeyboardGetData(void) {
	hd_data_t *hd_data;
	driver_info_t *di;
	KbdData* data  = NULL;
	KbdData* first = NULL;
	KbdData* last  = NULL;
	hd_t *hd;
	hd_t *first_dev;

	hd_data = (hd_data_t*)calloc(1, sizeof *hd_data);
	hd = hd_list(hd_data, hw_keyboard, 1, NULL);
	first_dev = hd;

	for(; hd; hd = hd->next) {
		// create new element and defaults...
		// -----------------------------------
		data = (KbdData*)malloc(sizeof(KbdData));
		strcpy(data->rules,"undef");
		strcpy(data->model,"undef");
		strcpy(data->layout,"undef");
		data->next = NULL;

		if (last != NULL) {
			last->next = data;
		}
		if (hd == first_dev) {
			first = data;
		}

		// save hddata to new struct...
		// -----------------------------
		#if HD_VERSION >= 5
		di = hd->driver_info;
		#else
		di = hd_driver_info(hd_data, hd);
		#endif
		if(di && di->any.type == di_kbd) {
		if (di->kbd.XkbRules) {
			strcpy(data->rules,di->kbd.XkbRules);
		}
		if (di->kbd.XkbModel) {
			strcpy(data->model,di->kbd.XkbModel);
		}
		if (di->kbd.XkbLayout) {
			strcpy(data->layout,di->kbd.XkbLayout);
		}
		}
		last = data;
	}
	return(first);
}
