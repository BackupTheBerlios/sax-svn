/**************
FILE          : pointers.h
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
#ifndef SAX_POINTERS_H
#define SAX_POINTERS_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"
#include "device.h"

//====================================
// Defines...
//------------------------------------
#define SAX_CORE_POINTER     1
#define SAX_NEXT_POINTER(x)  ((x)+2)

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulatePointersIF : public SaXException {
	public:
	virtual void setDriver    ( const QString& ) = 0;
	virtual void setDevice    ( const QString& ) = 0;
	virtual void setProtocol  ( const QString& ) = 0;
	virtual void setNamePair  ( const QString& , const QString& ) = 0;
	virtual void setOption    ( const QString& , const QString& = 0 ) = 0;
	virtual void addOption    ( const QString& , const QString& = 0 ) = 0;
	virtual void deleteOption ( const QString& ) = 0;

	public:
	virtual QString getDevice ( void ) = 0;
	virtual QString getDriver ( void ) = 0;
	virtual QString getProtocol ( void ) = 0;
	virtual QDict<QString> getOptions ( void ) = 0;
	
	public:
	virtual bool selectPointer ( int ) = 0;

	public:
	virtual ~SaXManipulatePointersIF ( void ) { }
};
//====================================
// Class SaXManipulatePointers...
//------------------------------------
class SaXManipulatePointers : public SaXManipulatePointersIF {
	protected:
	SaXImport* mImport;
	int mPointer;

	public:
	void setDriver    ( const QString& );
	void setDevice    ( const QString& );
	void setProtocol  ( const QString& );
	void setNamePair  ( const QString& , const QString& );
	void setOption    ( const QString& , const QString& = 0 );
	void addOption    ( const QString& , const QString& = 0 );
	void deleteOption ( const QString& );

	public:
	QString getDevice ( void );
	QString getDriver ( void );
	QString getProtocol ( void );
	QDict<QString> getOptions ( void );
	
	public:
	bool selectPointer ( int );

	protected:
	SaXManipulatePointers ( SaXImport*, int = 1 );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateMiceIF : public SaXManipulatePointers {
	public:
	virtual void enableWheelEmulation    ( int  ) = 0;
	virtual void disableWheelEmulation   ( void ) = 0;
	virtual void enable3ButtonEmulation  ( void ) = 0;
	virtual void disable3ButtonEmulation ( void ) = 0;
	virtual void enableWheel  ( void ) = 0;
	virtual void disableWheel ( void ) = 0;

	public:
	virtual bool wheelEnabled   ( void ) = 0;
	virtual bool wheelEmulated  ( void ) = 0;
	virtual bool buttonEmulated ( void ) = 0;
	virtual bool isMouse ( void ) = 0;

	public:
	virtual ~SaXManipulateMiceIF ( void ) { }

	protected:
	SaXManipulateMiceIF (SaXImport*in,int id): SaXManipulatePointers (in,id) { }
};
//====================================
// Class SaXManipulateMice...
//------------------------------------
class SaXManipulateMice : public SaXManipulateMiceIF {
	public:
	void enableWheelEmulation    ( int  );
	void disableWheelEmulation   ( void );
	void enable3ButtonEmulation  ( void );
	void disable3ButtonEmulation ( void );
	void enableWheel  ( void );
	void disableWheel ( void );

	public:
	bool wheelEnabled   ( void );
	bool wheelEmulated  ( void );
	bool buttonEmulated ( void );
	bool isMouse ( void );

	public:
	SaXManipulateMice ( SaXImport*, int = 1 );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateTabletsIF : public SaXManipulatePointers {
	public:
	virtual QList<QString> getTabletList ( void ) = 0;
	virtual QList<QString> getPenList    ( void ) = 0;
	virtual QList<QString> getTabletDrivers ( void ) = 0;
	virtual QDict<QString> getTabletOptions ( const QString& ) = 0;
	virtual void setTablet ( const QString& ) = 0;
	virtual void setType   ( const QString& ) = 0;
	virtual void setMode   ( const QString& ) = 0;
	virtual int  addPen    ( const QString& ) = 0;
	virtual int  deletePen ( int ) = 0;

	public:
	virtual QString getName   ( void ) = 0;
	virtual QString getVendor ( void ) = 0;
	virtual QString getType   ( void ) = 0;
	virtual QString getMode   ( void ) = 0;
	virtual bool isTablet     ( void ) = 0;
	virtual bool isPen        ( void ) = 0;
	virtual bool isEraser     ( void ) = 0;

	public:
	virtual ~SaXManipulateTabletsIF ( void ) { }

	protected:
	SaXManipulateTabletsIF (
		SaXImport*in,int id
	): SaXManipulatePointers (in,id) { }
};
//====================================
// Class SaXManipulateTablets...
//------------------------------------
class SaXManipulateTablets : public SaXManipulateTabletsIF {
	private:
	SaXProcess*    mCDBTablets;
	SaXProcess*    mCDBTabletModules;
	QList<QString> mCDBTabletList;
	SaXProcess*    mCDBPens;
	QList<QString> mCDBPenList;
	QList<QString> mCDBTabletDrivers;
	QDict<QString> mCDBTabletOptions;

	private:
	SaXManipulateDevices* mManipInputDevices;

	public:
	QList<QString> getTabletList ( void );
	QList<QString> getPenList    ( void );
	QList<QString> getTabletDrivers ( void );
	QDict<QString> getTabletOptions ( const QString& );
	void setTablet ( const QString& );
	void setType   ( const QString& );
	void setMode   ( const QString& );
	int  addPen    ( const QString& );
	int  deletePen ( int );

	public:
	QString getName   ( void );
	QString getVendor ( void );
	QString getType   ( void );
	QString getMode   ( void );
	bool isTablet     ( void );
	bool isPen        ( void );
	bool isEraser     ( void );

	public:
	SaXManipulateTablets ( SaXImport*, SaXImport* , int = 1 );
};

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateTouchscreensIF : public SaXManipulatePointers {
	public:
	virtual QList<QString> getPanelList ( void ) = 0;
	virtual void setTouchPanel ( const QString& ) = 0;

	public:
	virtual bool isTouchpanel  ( void ) = 0;

	public:
	virtual ~SaXManipulateTouchscreensIF ( void ) { }

	protected:
	SaXManipulateTouchscreensIF (
		SaXImport*in,int id
	): SaXManipulatePointers (in,id) { }
};
//====================================
// Class SaXManipulateTouchscreens...
//------------------------------------
class SaXManipulateTouchscreens : public SaXManipulateTouchscreensIF {
	private:
	SaXProcess*    mCDBPanels;
	QList<QString> mCDBPanelList;

	public:
	QList<QString> getPanelList ( void );
	void setTouchPanel ( const QString& );

	public:
	bool isTouchpanel  ( void );

	public:
	SaXManipulateTouchscreens ( SaXImport*, int = 1 );
};

#endif
