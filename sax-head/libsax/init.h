/**************
FILE          : init.h
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
#ifndef SAX_INIT_H
#define SAX_INIT_H 1

//====================================
// Includes...
//------------------------------------
#include <qprocess.h>

#include "exception.h"

//====================================
// Defines...
//------------------------------------
#define SAX_INIT        "/usr/X11R6/lib/sax/init.pl"
#define CACHE_CONFIG    "/var/cache/sax/files/config"

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Init class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXInitIF : public SaXException {
	public:
	virtual bool needInit (void) = 0;
	virtual void ignoreProfile (void) = 0;

	public:
	virtual void doInit (void) = 0;

	public:
	virtual ~SaXInitIF ( void ) { }
};
//====================================
// Class SaXInit...
//------------------------------------
class SaXInit : public SaXInitIF {
	private:
	QList<char> mOptions;

	private:
	bool needUpdate   ( void );
	bool scanHDstatus ( bool = false );
	bool saveHDstatus ( int,char*,char* );

	public:
	bool needInit (void);
	void ignoreProfile (void);

	public:
	void doInit (void);

	public:
	SaXInit ( QList<char> );
	SaXInit ( void );
};

#endif
