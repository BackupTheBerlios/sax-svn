/**************
FILE          : card.h
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
#ifndef SAX_CARD_H
#define SAX_CARD_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateCardIF : public SaXException {
	public:
	virtual void setCardDriver ( const QString& ) = 0;
	virtual void setCardOption ( const QString& , const QString& = 0 ) = 0;
	virtual void addCardOption ( const QString& , const QString& = 0 ) = 0;
	virtual void deleteCardOption ( const QString& ) = 0;
	virtual void setBusID ( const QString& ) = 0;
	virtual void setScreenID ( int ) = 0;

	public:
	virtual QList<QString> getCardDrivers ( void ) = 0;
	virtual QDict<QString> getCardOptions ( const QString& ) = 0;
	virtual QDict<QString> getOptions ( void ) = 0;
	virtual QString getCardName ( void ) = 0;
	virtual QString getBusID    ( void ) = 0;
	virtual QString getScreenID ( void ) = 0;

	public:
	virtual bool selectCard ( int ) = 0;

	public:
	virtual ~SaXManipulateCardIF ( void ) { }
};
//====================================
// Class SaXManipulateCard...
//------------------------------------
class SaXManipulateCard : public SaXManipulateCardIF {
	protected:
	SaXImport*     mImport;
	SaXProcess*    mCDBCardModules;
	QList<QString> mCDBCardDrivers;
	QDict<QString> mCDBCardOptions;
	int            mCard;

	public:
	void setCardDriver ( const QString& );
	void setCardOption ( const QString& , const QString& = 0 );
	void addCardOption ( const QString& , const QString& = 0 );
	void deleteCardOption ( const QString& );
	void setBusID ( const QString& );
	void setScreenID ( int );

	public:
	QList<QString> getCardDrivers ( void );
	QDict<QString> getCardOptions ( const QString& );
	QDict<QString> getOptions ( void );
	QString getCardName ( void );
	QString getBusID    ( void );
	QString getScreenID ( void );

	public:
	bool selectCard ( int );

	public:
	SaXManipulateCard ( SaXImport*, int = 0 );
};

#endif
