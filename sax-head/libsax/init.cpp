/**************
FILE          : init.cpp
***************
PROJECT       : SaX2 - library interface
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
#include <hd.h>
#include "init.h"

namespace SaX {
//====================================
// Constructor...
//------------------------------------
SaXInit::SaXInit ( QList<char> opt ) {
	// .../
	//! An object of this type is used to initialize the SaX2
	//! cache. The cache provide the current configuration suggestion
	//! which can be used to create an automatic X11 config file.
	//! If additional options are used this constructor will apply it
	// ----
	mOptions.append ("--quiet");
	mOptions.append ("no");
	QListIterator<char> it (opt);
	for (; it.current(); ++it) {
		mOptions.append ( it.current() );
	}
}

//====================================
// Constructor...
//------------------------------------
SaXInit::SaXInit ( void ) {
	// .../
	//! An object of this type is used to initialize the SaX2
	//! cache. The cache provide the current configuration suggestion
	//! which can be used to create an automatic X11 config file.
	// ----
	mOptions.append ("--quiet");
	mOptions.append ("no");
}

//====================================
// needInit...
//------------------------------------
bool SaXInit::needInit (void) {
	// .../
	//! check if the cache is up to date. This member function will
	//! check if the cache file exists and it will check if there is
	//! hardware which has been changed
	// ----
	if (! setLock()) {
		return false;
	}
	QFileInfo info (CACHE_CONFIG);
	if (! info.exists()) {
		unsetLock();
		return true;
	}
	#if 0
	// .../
	// disabled libhd hardware flags check, this will be
	// changed in the future and needs adaption
	// ---
	if (needUpdate()) {
		unsetLock();
		return true;
	}
	#endif
	unsetLock();
	return false;
}

//====================================
// ignoreProfile...
//------------------------------------
void SaXInit::ignoreProfile (void) {
	// .../
	//! ignore any automatically applied profiles during initializing
	//! this will prevent any multihead cards to become configured as
	//! multihead card
	// ----
	mOptions.append ("-i");
}

//====================================
// doInit...
//------------------------------------
void SaXInit::doInit (void) {
	// .../
	//! start the init.pl SaX2 startup sequence to create the
	//! cache which is the base for all further actions
	// ----
	if (! havePrivileges()) {
		excPermissionDenied();
		qError (errorString(),EXC_PERMISSIONDENIED);
		return;
	}
	if (setenv ("HW_UPDATE","1",1) != 0) {
		return;
	}
	SaXProcessCall* proc = new SaXProcessCall ();
	proc -> addArgument ( SAX_INIT );
	QListIterator<char> it (mOptions);
	for (; it.current(); ++it) {
		proc -> addArgument ( it.current() );
	}
	if ( ! proc -> start() ) {
		excProcessFailed();
		qError (errorString(),EXC_PROCESSFAILED);
		return;
	}
}

//====================================
// needUpdate...
//------------------------------------
bool SaXInit::needUpdate (void) {
	// .../
	//! check via libhd if there is hardware in a configuration
	//! state which is new or unconfigured. If there is such
	//! hardware the SaX2 cache and detection has to be recalled
	// ----
	if (! havePrivileges()) {
		excPermissionDenied();
		qError (errorString(),EXC_PERMISSIONDENIED);
		return false;
	}
	return scanHDstatus();
}

//====================================
// scanHDstatus...
//------------------------------------
bool SaXInit::scanHDstatus (bool save) {
	// .../
	//! handle the configuration flags for the monitor
	//! display and mouse hardware. See the code documentation
	//! for an explanation about the single states
	// ----
	hd_t* hd;
	hd_data_t* hd_data;
	hd_status_t status;
	status.invalid    = 0;
	status.reconfig   = 0;
	status.configured = 0;
	status.available  = 0;
	status.needed     = 0;
	hd_hw_item_t scanItem[] = {
		hw_display,
		hw_monitor,
		hw_mouse
	};
	bool needNewConfig = false;
	for (int item=0;item<3;item++) {
		//===============================================
		// scan for hardware identified by item and
		// write it into the hardware database
		//-----------------------------------------------
		hd_data = (hd_data_t*)calloc (1,sizeof *hd_data);
		hd_data -> flags.list_all = 1;
		hd_data -> flags.fast = 1;
		hd = hd_list (
			hd_data, scanItem[item], 1, 0
		);
		for (hd_t* hd1 = hd; hd1; hd1 = hd1->next) {
			int err = hd_write_config (hd_data, hd1);
			if (err) {
				return false;
			}
		}
		hd = hd_free_hd_list (hd);
		//===============================================
		// 1) search for new hardware and set it to
		//    configured and available
		//-----------------------------------------------
		status.configured = status_new;
		hd = hd_list_with_status (hd_data, scanItem[item], status);
		if (hd) {
			needNewConfig = true;
			if (save) {
			for (hd_t* hd1 = hd; hd1; hd1 = hd1->next) {
				saveHDstatus (1,"yes",hd1->unique_id);
				saveHDstatus (3,"yes",hd1->unique_id);
			}
			}
		}
		//===============================================
		// 2) search for configured hardware which is not
		//    available. This is a state which is not
		//    possible and we will set the flag to "no"
		//-----------------------------------------------
		status.configured = status_yes;
		status.available  = status_no;
		hd = hd_list_with_status (hd_data, scanItem[item], status);
		if (hd) {
			needNewConfig = true;
			if (save) {
			for (hd_t* hd1 = hd; hd1; hd1 = hd1->next) {
				saveHDstatus (1,"no",hd1->unique_id);
			}
			}
		}
		//===============================================
		// 3) search for unconfigured hardware which is
		//    available. This hardware is tagged as
		//    configured = "yes" using this program will
		//    never lead to this case, so we will setup a
		//    clean base if there is such a hardware found
		//-----------------------------------------------
		status.configured = status_no;
		status.available  = status_yes;
		hd = hd_list_with_status (hd_data, scanItem[item], status);
		if (hd) {
			needNewConfig = true;
			if (save) {
			for (hd_t* hd1 = hd; hd1; hd1 = hd1->next) {
				saveHDstatus (1,"yes",hd1->unique_id);
				saveHDstatus (3,"yes",hd1->unique_id);
			}
			}
		}
		//===============================================
		// 4) search for hardware which is not configured
		//    and not available and set the need flag to no.
		//    This is needed to prevent hwscan from
		//    starting YaST2 again
		//-----------------------------------------------
		status.configured = status_no;
		status.available  = status_no;
		hd = hd_list_with_status (hd_data, scanItem[item], status);
		if (hd) {
			if (save) {
			for (hd_t* hd1 = hd; hd1; hd1 = hd1->next) {
				saveHDstatus (3,"no",hd1->unique_id);
			}
			}
		}
		hd = hd_free_hd_list (hd);
		hd_free_hd_data (hd_data);
		free (hd_data);
	}
	return needNewConfig;
}

//====================================
// saveHDstatus...
//------------------------------------
bool SaXInit::saveHDstatus (int type, char* val, char* id) {
	// .../
	//! save the value <val> for the given type
	//! <configured,available or new> of the unique ID <id>
	// ----
	hd_t* hd;
	hd_data_t* hd_data;
	hd_status_value_t status = (hd_status_value_t)0;
	hd_data = (hd_data_t*)calloc(1, sizeof *hd_data);
	hd = hd_read_config(hd_data, id);
	if (! hd) {
		return false;
	}
	for (int i = 1; i < 8; i++) {
		char* s = hd_status_value_name ((hd_status_value_t)i);
		if (s && !strcmp(val, s)) {
			status = (hd_status_value_t)i;
			break;
		}
	}
	if ( ! status ) {
		return false;
	} else {
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
		hd_write_config (hd_data, hd);
	}
	hd = hd_free_hd_list (hd);
	hd_free_hd_data (hd_data);
	free (hd_data);
	return true;
}
} // end namespace
