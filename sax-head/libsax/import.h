/**************
FILE          : import.h
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
#ifndef SAX_IMPORT_H
#define SAX_IMPORT_H 1

//====================================
// Includes...
//------------------------------------
#include <qprocess.h>
#include <qregexp.h>

#include "storage.h"
#include "exception.h"
#include "process.h"

//====================================
// Defines...
//------------------------------------
#define SAX_AUTO_PROBE     1
#define SAX_SYSTEM_CONFIG  3

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Import class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXImportIF : public SaXProcess {
	public:
	virtual void doImport  ( void )   = 0;
	virtual void setSource ( int  )   = 0;
	virtual int getSectionID ( void ) = 0;
	virtual QString getSectionName ( void ) = 0;

	public:
	virtual ~SaXImportIF ( void ) { }
};
//====================================
// Class SaXImport...
//------------------------------------
class SaXImport : public SaXImportIF {
	private:
	QList<char> mOptions;
	QString mSectionName;
	int mSection;

	public:
	void doImport  ( void );
	void setSource ( int  );
	int getSectionID ( void );
	QString getSectionName ( void );

	public:
	SaXImport ( int );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Import SYSP class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXImportSyspIF : public SaXProcess {
	public:
	virtual void doImport    ( void ) = 0;
	virtual int getSectionID ( void ) = 0;
	virtual QString getSectionName ( void ) = 0;

	public:
	virtual ~SaXImportSyspIF ( void ) { }
};
//====================================
// Class SaXImportSysp...
//------------------------------------
class SaXImportSysp : public SaXImportSyspIF {
	private:
	QList<char> mOptions;
	QString mSectionName;
	int mSection;

	public:
	void doImport    ( void );
	int getSectionID ( void );
	QString getSectionName ( void );

	public:
	SaXImportSysp ( int );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Import CDB class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXImportCDBIF : public SaXProcess {
	public:
	virtual void doImport ( void ) = 0;

	public:
	virtual ~SaXImportCDBIF ( void ) { };
};
//====================================
// Class SaXImportCDB...
//------------------------------------
class SaXImportCDB : public SaXImportCDBIF {
	private:
	int mID;

	public:
	void doImport ( void );

	public:
	SaXImportCDB ( int );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
/*! \brief SaX2 -  Import Profile class interface.
*
* The interface class is provided to be able to dlopen the
* library and have all methods available in the compilers
* virtual table. For a detailed description of the class itself
* please refer to the derived class definition
*/
class SaXImportProfileIF : public SaXProcess {
	public:
	virtual void setDevice ( int  ) = 0;
	virtual void doImport  ( void ) = 0;
	virtual SaXImport* getImport ( int = SAX_CARD ) = 0;

	public:
	virtual ~SaXImportProfileIF ( void ) { }
};
//====================================
// Class SaXImportProfile...
//------------------------------------
class SaXImportProfile : public SaXImportProfileIF {
	private:
	QList<char> mOptions;
	SaXImport*  mLayout;
	SaXImport*  mDesktop;
	SaXImport*  mCard;
	SaXImport*  mPointers;

	private:
	void splitImport ( void );

	public:
	void setDevice ( int  );
	void doImport  ( void );
	SaXImport* getImport ( int = SAX_CARD );
	
	public:
	SaXImportProfile ( const QString& );
};

#endif
