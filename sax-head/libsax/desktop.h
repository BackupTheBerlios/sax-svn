/**************
FILE          : desktop.h
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
#ifndef SAX_DESKTOP_H
#define SAX_DESKTOP_H 1

//====================================
// Includes...
//------------------------------------
#include "import.h"
#include "card.h"
#include "path.h"

//====================================
// Defines...
//------------------------------------
#define XSLOAD  "/usr/X11R6/bin/xsload"

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXManipulateDesktopIF : public SaXException {
	public:
	virtual void calculateModelines ( bool ) = 0;
	virtual void setResolution ( int,int,int ) = 0;
	virtual void addResolution ( int,int,int ) = 0;
	virtual void removeResolution ( int,int,int ) = 0;
	virtual void setVirtualResolution ( int,int,int ) = 0;
	virtual void removeVirtualResolution ( int ) = 0;
	virtual void setColorDepth ( int ) = 0;
	virtual bool enable3D  ( void ) = 0;
	virtual bool disable3D ( void ) = 0;
	virtual void setDisplaySize ( int,int ) = 0;
	virtual void setHsyncRange ( double,double ) = 0;
	virtual void setVsyncRange ( double,double ) = 0;
	virtual void enableDPMS  ( void ) = 0;
	virtual void disableDPMS ( void ) = 0;
	virtual void setMonitorVendor ( const QString& ) = 0;
	virtual void setMonitorName   ( const QString& ) = 0;
	virtual void setCDBMonitor ( const QString& ) = 0;
	virtual QList<QString> getCDBMonitorList ( void ) = 0;

	public:
	virtual QList<QString> getResolutions ( int  ) = 0;
	virtual QList<QString> getDisplaySize ( void ) = 0;
	virtual QList<QString> getHsyncRange  ( void ) = 0;
	virtual QList<QString> getVsyncRange  ( void ) = 0;
	virtual bool is3DEnabled ( void ) = 0;
	virtual bool is3DCard    ( void ) = 0;
	virtual bool DPMSEnabled ( void ) = 0;
	virtual QString getMonitorVendor ( void ) = 0;
	virtual QString getMonitorName   ( void ) = 0;
	virtual QString getColorDepth    ( void ) = 0;
	virtual QString getVirtualResolution ( int  ) = 0;
	virtual bool willCalculateModelines  ( void ) = 0;

	public:
	virtual bool selectDesktop (int) = 0;

	public:
	virtual ~SaXManipulateDesktopIF ( void ) { }
};
//====================================
// Class SaXManipulateDesktop...
//------------------------------------
class SaXManipulateDesktop : public SaXManipulateDesktopIF {
	private:
	SaXImport*     mDesktop;
	SaXImport*     mCard;
	SaXImport*     mPath;
	SaXProcess*    mCDBMonitors;
	QList<QString> mCDBMonitorList;
	int            mDesktopID;

	public:
	void calculateModelines ( bool );
	void setResolution ( int,int,int );
	void addResolution ( int,int,int );
	void removeResolution ( int,int,int );
	void setVirtualResolution ( int,int,int );
	void removeVirtualResolution ( int );
	void setColorDepth ( int );
	bool enable3D  ( void );
	bool disable3D ( void );
	void setDisplaySize ( int,int );
	void setHsyncRange ( double,double );
	void setVsyncRange ( double,double );
	void enableDPMS  ( void );
	void disableDPMS ( void );
	void setMonitorVendor ( const QString& );
	void setMonitorName   ( const QString& );
	void setCDBMonitor ( const QString& );
	QList<QString> getCDBMonitorList ( void );

	public:
	QList<QString> getResolutions ( int  );
	QList<QString> getDisplaySize ( void );
	QList<QString> getHsyncRange  ( void );
	QList<QString> getVsyncRange  ( void );
	bool is3DEnabled ( void );
	bool is3DCard    ( void );
	bool DPMSEnabled ( void );
	QString getMonitorVendor ( void );
	QString getMonitorName   ( void );
	QString getColorDepth    ( void );
	QString getVirtualResolution ( int  );
	bool willCalculateModelines  ( void );

	public:
	bool selectDesktop (int);

	public:
	SaXManipulateDesktop (
		SaXImport*, SaXImport*, SaXImport*, int = 0
	);
};

#endif
