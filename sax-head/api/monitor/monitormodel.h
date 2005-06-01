/**************
FILE          : monitormodel.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : SaX2 GUI system using libsax to provide
              : configurations for a wide range of X11 capabilities
              : //.../
              : - monitormodel.h: SaXGUI::SCCMonitor header definitions
              : - monitormodel.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCMONITOR_MODEL_H
#define SCCMONITOR_MODEL_H 1

//=====================================
// Includes
//-------------------------------------
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qtabdialog.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlistbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"
#include "monitordisplay.h"

//=====================================
// Defines
//-------------------------------------
#define DTA_DIR        "/usr/share/sax/api/data"
#define MACROS_DIR     "/usr/share/sax/api/macros"
#define ASPECT_FILE    DTA_DIR    "/MonitorRatio"
#define DIAGONAL_FILE  DTA_DIR    "/MonitorTraversal"
#define GETINF         MACROS_DIR "/getINF"

namespace SaXGUI {
//====================================
// Class SCCMonitorModel
//------------------------------------
class SCCMonitorModel : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	bool           mNeedImport;
	int            mDisplay;
	QString        mSelectedMonitorName;
	QString        mSelectedMonitorVendor;
	bool           mDPMSStatus;
	int            mDisplaySizeX;
	int            mDisplaySizeY;
	int            mHsyncMin;
	int            mHsyncMax;
	int            mVsyncMin;
	int            mVsyncMax;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QList<QString> mCDBMonitorVendors;
	QList<QString> mCDBMonitorModels;
	QDict<QString> mCDBMonitorData;
	QList<QString> mTraversalList;
	QList<QString> mAspectList;

	private:
	SaXManipulateDesktop* mSaxDesktop;

	private:
	QVBox*         mModelTab;
	QVBox*         mAspectTab;
	QVBox*         mSyncTab;

	private:
	QTabDialog*    mModelDialog;
	QButtonGroup*  mModelVendorGroup;
	QListBox*      mVendorList;
	QListBox*      mModelList;
	QHBox*         mToolBox;
	QCheckBox*     mCheckDPMS;
	QPushButton*   mDisk;

	private:
	QButtonGroup*  mSyncGroup;
	QLabel*        mLabelWarning;
	QVBox*         mSyncBox;
	QLabel*        mLabelHorizontal;
	QSpinBox*      mHSpinMin;
	QSpinBox*      mHSpinMax;
	QLabel*        mLabelHUnit;
	QLabel*        mLabelVertical;
	QSpinBox*      mVSpinMin;
	QSpinBox*      mVSpinMax;
	QLabel*        mLabelVUnit;

	private:
	QButtonGroup*  mAspectGroup;
	QLabel*        mLabelAspect;
	QLabel*        mLabelRatio;
	QComboBox*     mAspect;
	QComboBox*     mTraversal;
	QButtonGroup*  mDisplaySizeGroup;
	QLabel*        mLabelX;
	QLabel*        mLabelY;
	QSpinBox*      mSizeX;
	QSpinBox*      mSizeY;

	public:
	void show ( void );

	public:
	bool isDPMSEnabled ( void );
	QString getVendorName ( void );
	QString getModelName  ( void );
	void setTitle ( const QString& );
	int getSizeX ( void );
	int getSizeY ( void );
	int getHSmin ( void );
	int getHSmax ( void );
	int getVSmin ( void );
	int getVSmax ( void ); 

	public:
	void init   ( void );
	void import ( void );

	private:
	double getTraversal ( int,int );
	QPoint getRatio     ( int,int );
	QPoint translateTraversal ( double,int,int );

	public slots:
	void slotOk   ( void );
	void slotDisk ( void );
	void slotName   ( QListBoxItem* );
	void slotVendor ( QListBoxItem* );
	void slotRatio     ( int );
	void slotTraversal ( int );

	signals:
	void sigMonitorChanged ( SCCMonitorDisplay* );

	public:
	SCCMonitorModel (
		QDict<QString>*, QDict<SaXImport>,
		const QString&,const QString&,const QString&,
		int,QWidget*
	);
};
} // end namespace
#endif
