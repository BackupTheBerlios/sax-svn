/**************
FILE          : export.h
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
#ifndef SAX_EXPORT_H
#define SAX_EXPORT_H 1

//====================================
// Includes...
//------------------------------------
#include <qfile.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>

#include "import.h"

//====================================
// Defines...
//------------------------------------
#define CONFDIR         "/var/lib/sax/"
#define ISAXCARD        CONFDIR "card"
#define ISAXDESKTOP     CONFDIR "desktop"
#define ISAXINPUT       CONFDIR "input"
#define ISAXKEYBOARD    CONFDIR "keyboard"
#define ISAXLAYOUT      CONFDIR "layout"
#define ISAXPATH        CONFDIR "path"
#define ISAXEXTENSIONS  CONFDIR "extensions"

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Export class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXExportIF : public SaXException {
	public:
	virtual bool doExport (void) = 0;

	public:
	virtual ~SaXExportIF ( void ) { }
};
//====================================
// Class SaXExport...
//------------------------------------
class SaXExport : public SaXExportIF {
	private:
	SaXException* mTo;

	protected:
	SaXImport* mImport;
	QFile*     mHandle;
	QString    mFile;

	public:
	bool doExport (void);

	public:
	SaXExport ( SaXImport*,SaXException* );
};

#endif
