/**************
FILE          : config.h
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
#ifndef SAX_CONFIG_H
#define SAX_CONFIG_H 1

//====================================
// Includes...
//------------------------------------
#include <stdarg.h>
extern "C" {
#include <xf86Parser.h>
}

#include "export.h"

//====================================
// Defines...
//------------------------------------
#define SAX_NEW         20
#define SAX_MERGE       21
#define SAX_CREATE_API  "/var/lib/sax/createAPI.pl"
#define SAX_TEST_CONFIG "/var/lib/sax/createTST.pl"
#define SAX_API_FILE    "/var/lib/sax/apidata"
#define SAX_API_CONFIG  "/var/lib/sax/xorg.conf"
#define SAX_SYS_CONFIG  "/etc/X11/xorg.conf"
#define SAX_SYS_CSAVED  "/etc/X11/xorg.conf.saxsave"
#define SAX_X11_LOADER  "/usr/X11R6/bin/Xorg"
#define CONFPATH        "%A,%R,/etc/%R,%P/etc/X11/%R,%E,%F,/etc/X11/%F"
#define CONFERRORLINE   80

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXConfigIF : public SaXException {
	public:
	virtual void setParseErrorValue ( char* ) = 0;
	virtual void setParseError ( char* ) = 0;
	virtual void addImport ( SaXImport* ) = 0;
	virtual void setMode ( int ) = 0;

	public:
	virtual QString getParseErrorValue ( void ) = 0;
	virtual QString getParseError ( void ) = 0;

	public:
	virtual bool createConfiguration  ( void ) = 0;
	virtual void installConfiguration ( void ) = 0;
	virtual bool testConfiguration    ( void ) = 0;

	public:
	virtual ~SaXConfigIF ( void ) { }
};
//====================================
// Class SaXConfig...
//------------------------------------
class SaXConfig : public SaXConfigIF {
	private:
	QList <SaXImport> mImportList;
	SaXImport* mDesktop;
	int configMode;

	private:
	QString* mParseErrorString;
	QString* mParseErrorValue;

	private:
	bool gotCard;
	bool gotDesktop;
	bool gotPointers;
	bool gotKeyboard;
	bool gotLayout;
	bool gotPath;
	bool gotExtensions;

	private:
	void enableXFineCache (bool = true);

	public:
	void setParseErrorValue ( char* );
	void setParseError ( char* );
	void addImport ( SaXImport* );
	void setMode ( int );
	
	public:
	QString getParseErrorValue ( void );
	QString getParseError ( void );

	public:
	bool createConfiguration  ( void );
	void installConfiguration ( void );
	bool testConfiguration    ( void );

	public:
	SaXConfig ( int = SAX_MERGE );
};
#endif
