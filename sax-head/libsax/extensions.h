/**************
FILE          : extensions.h
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
#ifndef SAX_EXTENSIONS_H
#define SAX_EXTENSIONS_H 1

//====================================
// Includes...
//------------------------------------
#include <qprocess.h>

#include "import.h"
#include "device.h"
#include "card.h"
#include "path.h"

//====================================
// Defines...
//------------------------------------
#define SAX_CREATE_VNC_PWD  "/var/lib/sax/createVNC.sh"

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Extensions class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXManipulateExtensionsIF : public SaXException {
	public:
	virtual ~SaXManipulateExtensionsIF ( void ) { }
};
//====================================
// Class SaXManipulateExtensions...
//------------------------------------
class SaXManipulateExtensions : public SaXManipulateExtensionsIF {
	private:
	SaXImport* mImport;
	int mExtensions;

	public:
	SaXManipulateExtensions ( SaXImport* );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  VNC class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXManipulateVNCIF : public SaXManipulateCard {
	public:
	virtual void addVNCKeyboard    ( void ) = 0;
	virtual void addVNCMouse       ( void ) = 0;
	virtual void removeVNCMouse    ( void ) = 0;
	virtual void removeVNCKeyboard ( void ) = 0;

	public:
	virtual void enableVNC   ( void ) = 0;
	virtual void disableVNC  ( void ) = 0;
	virtual void setPassword ( const QString& ) = 0;
	virtual void enablePasswordProtection  ( const QString& ) = 0;
	virtual void disablePasswordProtection ( void ) = 0;
	virtual void enableHTTPAccess  ( int = 5800 ) = 0;
	virtual void disableHTTPAccess ( void ) = 0;
	virtual void allowMultipleConnections ( bool = true ) = 0;

	public:
	virtual bool VNCEnabled           ( void ) = 0;
	virtual bool HTTPAccessEnabled    ( void ) = 0;
	virtual bool multiConnectEnabled  ( void ) = 0;
	virtual bool pwdProtectionEnabled ( void ) = 0;

	public:
	virtual ~SaXManipulateVNCIF ( void ) { }

	protected:
	SaXManipulateVNCIF (SaXImport* in,int id= 0) : SaXManipulateCard (in,id) { }
};
//====================================
// Class SaXManipulateVNC...
//------------------------------------
class SaXManipulateVNC : public SaXManipulateVNCIF {
	private:
	int mVNCMouse;
	int mVNCKeyboard;

	private:
	SaXManipulateDevices* mManipKeyboard;
	SaXManipulateDevices* mManipPointer;
	SaXManipulatePath*    mManipModules;
	SaXImport*            mLayout;

	public:
	void addVNCKeyboard    ( void );
	void addVNCMouse       ( void );
	void removeVNCMouse    ( void );
	void removeVNCKeyboard ( void );

	public:
	void enableVNC   ( void );
	void disableVNC  ( void );
	void setPassword ( const QString& );
	void enablePasswordProtection  ( const QString& );
	void disablePasswordProtection ( void );
	void enableHTTPAccess  ( int = 5800 );
	void disableHTTPAccess ( void );
	void allowMultipleConnections ( bool = true );

	public:
	bool VNCEnabled           ( void );
	bool HTTPAccessEnabled    ( void );
	bool multiConnectEnabled  ( void );
	bool pwdProtectionEnabled ( void );

	public:
	SaXManipulateVNC (
		SaXImport*, SaXImport*, SaXImport*,
		SaXImport*, SaXImport*, int = 0
	);
};

#endif
