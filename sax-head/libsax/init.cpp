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
		excInvalidArgument ( errno );
		qError (errorString(),EXC_INVALIDARGUMENT);
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
} // end namespace
