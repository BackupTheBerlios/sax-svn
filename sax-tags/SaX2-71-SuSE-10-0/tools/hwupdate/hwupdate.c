/**************
FILE          : hwupdate.c
***************
PROJECT       : SaX2 ( SuSE advanced X11 configuration )
              :
DESCRIPTION   : hwupdate is used to update the libhd database
              : the program will scan the four hardware classes
              : mouse, keyboard, gfxcard and monitor. The exit
              : code will give you the following information:
              : ---
              : 1 -> no hardware changes detected 
              : 0 -> hardware has changed
              : 2 -> error
              : ---
              : Except this information the program will update the
              : configure and need state of all components taged with
              : / 
              : status.configured = status_new
              : status.reconfig   = status_yes 
              :
              : 
STATUS        : Status: Up-to-date
**************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <hd.h>

#include "hwupdate.h"

//=====================================
// Globals...
//-------------------------------------
int HWall  = 0;
int HWtest = 0;

//=====================================
// Main...
//-------------------------------------
int main (int argc,char*argv[]) {
	int status = 1;
	int item;
	int itemCount = 1;
	hd_hw_item_t scan_item[4];
	scan_item[0] = hw_display;

	while (1) {
	int option_index = 0;
	static struct option long_options[] =
	{
        {"all"  , 0 , 0 , 'a'},
        {"test" , 0 , 0 , 't'},
		{"mouse", 0 , 0 , 'm'},
        {0      , 0 , 0 , 0  }
    };

	int c = getopt_long (
		argc,argv,"at",long_options, &option_index
	);
	if (c == -1)
	break;

	switch (c) {
	case 0:
		fprintf (stderr,"option %s", long_options[option_index].name);
		if (optarg)
			fprintf (stderr," with arg %s", optarg);
		fprintf (stderr,"\n");
	break;

	case 'a':
		HWall  = 1;
	break;

	case 'm':
		scan_item[0] = hw_mouse;
		HWall  = 0;
	break;

	case 't':
		HWtest = 1;
	break;

	default:
	break;
	}
	}

	if (HWall == 1) {
		scan_item[1] = hw_monitor;
		scan_item[2] = hw_keyboard;
		scan_item[3] = hw_mouse;
		itemCount = 4;
	}
	if (! accessAllowed()) {
		fprintf (stderr,"hwupdate: only root can do this\n");
		exit (2);
	}
	for (item=0;item<itemCount;item++) {
	if (! doScan (scan_item[item])) {
		status = 0;
		// doList(scan_item[item]);
	}
	}
	exit (status);
}


//==========================================
// doScan...
//------------------------------------------
int doScan (hd_hw_item_t item) {
	int run_config = 0;
	hd_status_t status = { };
	hd_data_t *hd_data;
	hd_t *hd, *hd1;
	int err = 0;

	// ... /
	// scan for hardware identified by item and
	// write it into the hardware database
	// ---
	hd_data = calloc (1, sizeof *hd_data);
	hd_data -> flags.list_all = 1;
	hd = hd_list (
		hd_data, item, 1, NULL
	);
	for (hd1 = hd; hd1; hd1 = hd1->next) {
		err = hd_write_config (hd_data, hd1);
		if (err) { break; }
	}
	if (err) {
	fprintf(stderr,
		"Error writing configuration for %s (%s)\n",
		hd1->unique_id, hd1->model
	);
	exit (1);
	}
	hd = hd_free_hd_list (hd);

	// 1)
	// search for new hardware and set it to configured
	// and available...
	// ---
	status.configured = status_new;
	hd = hd_list_with_status (hd_data, item, status);
	if (hd) {
	run_config = 1;
	if (HWtest == 0) {
	for (hd1 = hd; hd1; hd1 = hd1->next) {
		doConf (1,"yes",hd1->unique_id);
		doConf (3,"yes",hd1->unique_id);
	}
	}
	}

	// 2) 
	// search for configured hardware which is not 
	// available. This is a state which is not possible
	// and we will set the configure flag to "no"
	// ---
	status.configured = status_yes;
	status.available  = status_no;
	hd = hd_list_with_status (hd_data, item, status);
	if (hd) {
	run_config = 1;
	if (HWtest == 0) {
	for (hd1 = hd; hd1; hd1 = hd1->next) {
		doConf (1,"no",hd1->unique_id);
	}
	}
	}

	// 3) 
	// search for unconfigured hardware which is 
	// available. This hardware is tagged as configured = "yes"
	// using this program will never lead to this case, so we
	// will setup a clean base if there is such a hardware 
	// found here
	// ---
	status.configured = status_no;
	status.available  = status_yes;
	hd = hd_list_with_status (hd_data, item, status);
	if (hd) {
	run_config = 1;
	if (HWtest == 0) {
	for (hd1 = hd; hd1; hd1 = hd1->next) {
		doConf (1,"yes",hd1->unique_id);
		doConf (3,"yes",hd1->unique_id);
	}
	}
	}

	// 4)
	// search for hardware which is not configured and not
	// available and set the need flag to no. This is needed
	// to prevent hwscan from starting YaST2 again
	// ---
	status.configured = status_no;
	status.available  = status_no;
	hd = hd_list_with_status (hd_data, item, status);
	if (hd) {
	if (HWtest == 0) {
	for (hd1 = hd; hd1; hd1 = hd1->next) {
		doConf (3,"no",hd1->unique_id);
	}
	}
	}

	// ... /
	// free all data and return
	// ---
	hd = hd_free_hd_list(hd);
	hd_free_hd_data(hd_data);
	free(hd_data);
	return (
		run_config ^ 1
	);
}

//==========================================
// doConf...
//------------------------------------------
int doConf (int type, char *val, char *id) {
	hd_data_t *hd_data;
	hd_t *hd;
	hd_status_value_t status = 0;
	int i;
	char *s;

	hd_data = calloc(1, sizeof *hd_data);
	hd_list (hd_data, hw_display, 1, NULL);
	hd = hd_read_config(hd_data, id);

	if (hd) {
	for(i = 1; i < 8; i++) {
		s = hd_status_value_name(i);
		if (s && !strcmp(val, s)) {
			status = i;
			break;
		}
	}
	if (!status) {
		printf ("invalid status: %s\n", val);
	}
	else {
		switch(type) {
		case 1:
			hd->status.configured = status;
		break;

		case 2:
			hd->status.available = status;
		break;

		case 3:
			hd->status.needed = status;
		break;
		}
		hd_write_config(hd_data, hd);
	}
	hd = hd_free_hd_list(hd);
	}
	else {
		printf ("no such hardware item: %s\n", id);
	}
	hd_free_hd_data(hd_data);
	free(hd_data);
	return 0;
}

//==========================================
// doList...
//------------------------------------------
int doList (hd_hw_item_t item) {
	hd_data_t *hd_data;
	hd_t *hd, *hd_manual;
	char *s;
	char status[64];
	int i;

	hd_data = calloc (1, sizeof *hd_data);
	hd_manual = hd_list (hd_data, hw_manual, 1, NULL);
	for (hd = hd_manual; hd; hd = hd->next) {
		if (hd -> hw_class != item) {
			continue;
		}
		strcpy(status, "(");
		i = 0;
		// ...
		if (
			hd->status.configured && 
			(s = hd_status_value_name(hd->status.configured))
		) {
			sprintf(status + strlen(status), "%scfg=%s", i ? ", " : "", s);
			i++;
		}
		// ...
		if (
			hd->status.available && 
			(s = hd_status_value_name(hd->status.available))
		) {
			sprintf(status + strlen(status), "%savail=%s", i ? ", " : "", s);
			i++;
		}
		// ...
		if (
			hd->status.needed && 
			(s = hd_status_value_name(hd->status.needed))
		) {
			sprintf(status + strlen(status), "%sneed=%s", i ? ", " : "", s);
			i++;
		}
		// ...
		strcat(status, ")");
		printf ("%s: %-32s %s\n", 
			hd->unique_id, status, hd->model
		);
	}
	return (0);
}

//==========================================
// check root privileges...
//------------------------------------------
int accessAllowed (void) {
	if (getuid() == 0) { return (1); }
	return (0);
}
