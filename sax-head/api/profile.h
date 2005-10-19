/**************
FILE          : profile.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : SaX2 GUI system using libsax to provide
              : configurations for a wide range of X11 capabilities
              : //.../
              : - profile.h: SaXGUI::SCCWidgetProfile header definitions
              : - profile.cpp: provide access to libsax profile data
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCPROFILE_H
#define SCCPROFILE_H 1

#include <qdict.h>

#include "../libsax/sax.h"

namespace SaXGUI {
//====================================
// Class SCCWidgetProfile
//------------------------------------
class SCCWidgetProfile {
	private:
	QDict <SaXImportProfile> mProfile;

	public:
	SaXImportProfile* getProfile ( const QString& );

	public:
	SCCWidgetProfile ( void );
};
}
#endif
