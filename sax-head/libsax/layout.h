/**************
FILE          : layout.h
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
#ifndef SAX_LAYOUT_H
#define SAX_LAYOUT_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"
#include "card.h"

//====================================
// Defines...
//------------------------------------
#define SAX_SINGLE_HEAD    300
#define SAX_XINERAMA       301
#define SAX_TRADITIONAL    302
#define SAX_CLONE          303
#define SAX_TWINVIEW       304
#define SAX_TWINVIEW_CLONE 305
#define SAX_UNKNOWN_MODE   306

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Layout class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXManipulateLayoutIF : public SaXException {
	public:
	virtual void setXOrgMultiheadMode ( int ) = 0;
	virtual bool setXOrgLayout ( int, int, int, int, int ) = 0;

	public:
	virtual int getMultiheadMode ( void ) = 0;
	virtual QList<QString> getXOrgLayout ( int ) = 0;

	public:
	virtual ~SaXManipulateLayoutIF ( void ) { }
};
//====================================
// Class SaXManipulateLayout...
//------------------------------------
class SaXManipulateLayout : public SaXManipulateLayoutIF {
	private:
	SaXImport* mLayout;
	SaXImport* mCard;

	public:
	void setXOrgMultiheadMode ( int );
	bool setXOrgLayout ( int, int, int, int, int );

	public:
	int getMultiheadMode ( void );
	QList<QString> getXOrgLayout ( int );

	public:
	SaXManipulateLayout ( SaXImport*,SaXImport* );
};

#endif
