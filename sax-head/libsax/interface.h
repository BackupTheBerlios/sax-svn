/**************
FILE          : interface.h
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
#ifndef SAX_INTERFACE_H
#define SAX_INTERFACE_H 1

//====================================
// Includes STL...
//------------------------------------
#include <deque>
#include <string>
#include <map>

//====================================
// Includes...
//------------------------------------
#include <qdict.h>
#include <qlist.h>
#include <errno.h>

#include "sax.h"

//====================================
// Entrypoint...
//------------------------------------
typedef void* (*Factory) ();
struct EntryPoint {
	const char* name;
	const char* version;
	Factory factory;
};

namespace LML {
//====================================
// Class saxPluglib abstract...
//------------------------------------
class saxPluglib {
	public:
	virtual SaXInit*    saxInit   ( void ) = 0;
	virtual SaXConfig*  saxConfig ( int = SAX_MERGE ) = 0;
	virtual SaXImport*  saxImport ( int ) = 0;
	virtual SaXImportSysp* saxImportSysp ( int ) = 0;
	virtual SaXImportCDB*  saxImportCDB  ( int ) = 0;
	virtual SaXImportProfile* saxImportProfile ( const char* ) = 0;

	public:
	virtual SaXManipulateDesktop* saxManipulateDesktop (
		SaXImport*,SaXImport*,SaXImport*,int = 0
	) = 0;
	virtual SaXManipulateCard* saxManipulateCard (
		SaXImport*,int = 0
	) = 0;
	virtual SaXManipulateKeyboard* saxManipulateKeyboard (
		SaXImport*,int = 0
	) = 0;
	virtual SaXManipulateDevices* saxManipulateDevices (
		SaXImport*,SaXImport*,SaXImport*
	) = 0;
	virtual SaXManipulateDevices* saxManipulateDevices (
		SaXImport*,SaXImport*
	) = 0;
	virtual SaXManipulateLayout* saxManipulateLayout (
		SaXImport*,SaXImport*
	) = 0;
	virtual SaXManipulatePath* saxManipulatePath (
		SaXImport*,int = 0
	) = 0;
	virtual SaXManipulateMice* saxManipulateMice (
		SaXImport*,int = 1
	) = 0;
	virtual SaXManipulateTablets* saxManipulateTablets (
		SaXImport*,SaXImport*,int = 1
	) = 0;
	virtual SaXManipulateVNC* saxManipulateVNC (
		SaXImport*,SaXImport*,SaXImport*,SaXImport*,SaXImport*,int = 0
	) = 0;
	virtual SaXManipulateTouchscreens* saxManipulateTouchscreens (
		SaXImport*,int = 1
	) = 0;
	virtual SaXManipulateExtensions* saxManipulateExtensions (
		SaXImport*
	) = 0;

	public:
	virtual std::string STLstring ( const QString& ) = 0;
	virtual std::deque<std::string> STLdeque ( const QList<QString>& ) = 0;
	virtual std::map<std::string,std::string> STLmap (
		const QDict<QString>&
	) = 0;

	public:
	virtual void setDebug  ( bool=true ) = 0;

	public:
	virtual ~saxPluglib ( void ) { }
};
//====================================
// Class SaXPluglib...
//------------------------------------
class SaXPluglib : public saxPluglib {
	public:
	SaXPluglib ( void );
	static SaXPluglib* factory ( void );

	public:
	SaXInit*    saxInit   ( void );
	SaXConfig*  saxConfig ( int = SAX_MERGE );
	SaXImport*  saxImport ( int );
	SaXImportSysp* saxImportSysp ( int );
	SaXImportCDB*  saxImportCDB  ( int );
	SaXImportProfile* saxImportProfile ( const char* );

	public:
	SaXManipulateDesktop* saxManipulateDesktop (
		SaXImport*,SaXImport*,SaXImport*,int = 0
	);
	SaXManipulateCard* saxManipulateCard (
		SaXImport*,int = 0
	);
	SaXManipulateKeyboard* saxManipulateKeyboard (
		SaXImport*,int = 0
	);
	SaXManipulateDevices* saxManipulateDevices (
		SaXImport*,SaXImport*,SaXImport*
	);
	SaXManipulateDevices* saxManipulateDevices (
		SaXImport*,SaXImport*
	);
	SaXManipulateLayout* saxManipulateLayout (
		SaXImport*,SaXImport*
	);
	SaXManipulatePath* saxManipulatePath (
		SaXImport*,int = 0
	);
	SaXManipulateMice* saxManipulateMice (
		SaXImport*,int = 1
	);
	SaXManipulateTablets* saxManipulateTablets (
		SaXImport*,SaXImport*,int = 1
	);
	SaXManipulateVNC* saxManipulateVNC (
		SaXImport*,SaXImport*,SaXImport*,SaXImport*,SaXImport*,int = 0
	);
	SaXManipulateTouchscreens* saxManipulateTouchscreens (
		SaXImport*,int = 1
	);
	SaXManipulateExtensions* saxManipulateExtensions (
		SaXImport*
	);

	public:
	std::string STLstring ( const QString& );
	std::deque<std::string> STLdeque ( const QList<QString>& );
	std::map<std::string,std::string> STLmap ( const QDict<QString>& );

	public:
	void setDebug  ( bool=true );
};
} // end namespace
#endif
