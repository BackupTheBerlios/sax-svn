/**************
FILE          : device.h
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
#ifndef SAX_DEVICE_H
#define SAX_DEVICE_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"

//====================================
// Defines...
//------------------------------------
#define SAX_INPUT_TOUCHPANEL  "Touchpanel"
#define SAX_INPUT_TABLET      "Tablet"
#define SAX_INPUT_PEN         "Pen"
#define SAX_INPUT_ERASER      "Eraser"
#define SAX_INPUT_MOUSE       "Mouse"
#define SAX_INPUT_VNC         "VNC"
#define SAX_INPUT_KEYBOARD    "Keyboard"

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateDevicesIF : public SaXException {
	public:
	virtual int addDesktopDevice ( void ) = 0;
	virtual int addInputDevice   ( const char* ) = 0;

	public:
	virtual int deleteDesktopDevice ( int ) = 0;
	virtual int deleteInputDevice   ( int ) = 0;

	public:
	virtual ~SaXManipulateDevicesIF ( void ) { }
};
//====================================
// Class SaXManipulateDevices...
//------------------------------------
class SaXManipulateDevices : public SaXManipulateDevicesIF {
	private:
	SaXImport* mDesktop;
	SaXImport* mCard;
	SaXImport* mInput;
	SaXImport* mLayout;

	private:
	bool desktopHandlingAllowed;
	bool inputHandlingAllowed;

	private:
	void updateLayout (int);

	public:
	int addDesktopDevice ( void );
	int addInputDevice   ( const char* );

	public:
	int deleteDesktopDevice ( int );
	int deleteInputDevice   ( int );

	public:
	SaXManipulateDevices ( SaXImport*,SaXImport*,SaXImport* );
	SaXManipulateDevices ( SaXImport*,SaXImport* );
};

#endif