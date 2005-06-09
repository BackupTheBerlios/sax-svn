/**************
FILE          : file.h
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
              : - file.h: SaXGUI::SCCFile header definitions
              : - file.cpp: provide access to GUI files like xapi.gtx
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCFILE_H
#define SCCFILE_H 1

//====================================
// Includes
//------------------------------------
#include <qfile.h>
#include <qdict.h>
#include <qstringlist.h>

//====================================
// Includes
//------------------------------------
#include "common/log.h"

namespace SaXGUI {
//====================================
// Defines
//------------------------------------
#define MAX_LINE_LENGTH    8192

//====================================
// Class SCCFile
//------------------------------------
class SCCFile : public QFile {
	private:
	QFile* mHandle;
	QDict<QString> gtx;
	QList<QString> gtxlist;

	private:
	void fileOpen ( void );
	
	public:
	QList<QString> readList ( void );
	QDict<QString> readDict ( void );

	public:
	QDict<QString> getDataDict ( void );
	QList<QString> getDataList ( void );

	public:
	SCCFile ( const QString& );
};
} // end namespace
#endif
