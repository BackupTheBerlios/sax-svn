/**************
FILE          : path.h
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
#ifndef SAX_PATH_H
#define SAX_PATH_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Path class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXManipulatePathIF : public SaXException {
	public:
	virtual void setFontPath ( const QString& ) = 0;
	virtual void addFontPath ( const QString& ) = 0;
	virtual void removeFontPath ( const QString& ) = 0;

	public:
	virtual void setLoadableModule ( const QString& ) = 0;
	virtual void addLoadableModule ( const QString& ) = 0;
	virtual void removeLoadableModule ( const QString& ) = 0;

	public:
	virtual void setServerFlag ( const QString& ) = 0;
	virtual void addServerFlag ( const QString& ) = 0;
	virtual void removeServerFlag ( const QString& ) = 0;

	public:
	virtual QList<QString> getFontPaths   ( void ) = 0;
	virtual QList<QString> getModules     ( void ) = 0;
	virtual QList<QString> getServerFlags ( void ) = 0;

	public:
	virtual ~SaXManipulatePathIF ( void ) { }
};
//====================================
// Class SaXManipulatePath...
//------------------------------------
class SaXManipulatePath : public SaXManipulatePathIF {
	private:
	SaXImport* mImport;
	int mPath;

	private:
	QList<QString> createList ( const QString& );

	public:
	void setFontPath ( const QString& );
	void addFontPath ( const QString& );
	void removeFontPath ( const QString& );

	public:
	void setLoadableModule ( const QString& );
	void addLoadableModule ( const QString& );
	void removeLoadableModule ( const QString& );

	public:
	void setServerFlag ( const QString& );
	void addServerFlag ( const QString& );
	void removeServerFlag ( const QString& );

	public:
	QList<QString> getFontPaths   ( void );
	QList<QString> getModules     ( void );
	QList<QString> getServerFlags ( void );

	public:
	SaXManipulatePath ( SaXImport*, int = 0 );
};

#endif
