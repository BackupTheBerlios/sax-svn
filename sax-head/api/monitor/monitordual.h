/**************
FILE          : monitordual.h
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
              : - monitordual.h: SaXGUI::SCCMonitor header definitions
              : - monitordual.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCMONITOR_DUAL_H
#define SCCMONITOR_DUAL_H 1

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

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"
#include "monitordisplay.h"

//=====================================
// Defines...
//-------------------------------------
#define PIXDIR           "/usr/share/sax/api/figures"
#define LEFT_OF_PIXMAP   PIXDIR "/dualleftof.png"
#define RIGHT_OF_PIXMAP  PIXDIR "/dualrightof.png"
#define ABOVE_OF_PIXMAP  PIXDIR "/dualaboveof.png"
#define BELOW_OF_PIXMAP  PIXDIR "/dualbelowof.png"

//=====================================
// Defines...
//-------------------------------------
#define DUAL_TRADITIONAL_KEY "Traditional"
#define DUAL_CLONE_KEY       "Clone"
#define DUAL_XINERAMA_KEY    "Xinerama"
#define DUAL_TRADITIONAL     0
#define DUAL_CLONE           1
#define DUAL_XINERAMA        2

//=====================================
// Defines...
//-------------------------------------
#define DUAL_LEFTOF_KEY      "LeftOf"
#define DUAL_ABOVEOF_KEY     "Above"
#define DUAL_RIGHTOF_KEY     "RightOf"
#define DUAL_BELOWOF_KEY     "Below"
#define DUAL_LEFTOF          0
#define DUAL_ABOVEOF         1
#define DUAL_RIGHTOF         2
#define DUAL_BELOWOF         3

namespace SaXGUI {
//=====================================
// Pre-class definitions...
//-------------------------------------
class SCCMonitorDualModel;

//====================================
// Class SCCMonitorDual
//------------------------------------
class SCCMonitorDual : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	bool           mNeedImport;
	int            mDisplay;
	QString        mMonitorModel;
	QString        mMonitorVendor;
	QString        mSelectedResolution;
	QList<QString> mSelectedResolutionList;
	int            mDualHeadMode;
	int            mDualHeadArrangement;

	//====================================
	// private widget data contents
	//------------------------------------
	QDict<QString> mResolutionDict;
	QDict<QString> mProfileDriverOptions;

	private:
	QVBox*         mCardTab;
	
	private:
	QTabDialog*    mDualHeadDialog;
	QButtonGroup*  mChangeGroup;
	QHBox*         mMoniBox;
	QLabel*        mLabelSecondMonitor;
	QPushButton*   mChangeMonitor;
	QHBox*         mResBox;
	QLabel*        mLabelSecondResolution;
	QComboBox*     mResolution;
	QButtonGroup*  mModeGroup;
	QRadioButton*  mTraditional;
	QRadioButton*  mClone;
	QRadioButton*  mXinerama;
	QButtonGroup*  mOrientationGroup;
	QPushButton*   mLeftOfPrimary;
	QPushButton*   mRightOfPrimary;
	QPushButton*   mAboveOfPrimary;
	QPushButton*   mBelowOfPrimary;
	SCCMonitorDualModel* mChangeMonitorDualModelDialog;

	public:
    void show ( void );

	public:
	int getMode   ( void );
	int getLayout ( void );
	QString getResolution   ( void );
	QList<QString> getResolutionList ( void );
	QPixmap getLayoutPixmap ( void );
	SCCMonitorDualModel* getDualModelData ( void );
	void setTitle ( const QString& );
	void setMonitorName ( const QString&,const QString& );

	private:
	QList<QString> setupList ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotDualHead ( QPushButton* );
	void slotMonitor  ( void );
	void slotOk       ( void );
	void slotClone    ( bool );

	signals:
	void sigDualModeChanged (SCCMonitorDisplay*);

	public:
	SCCMonitorDual (
		QDict<QString>*,QDict<SaXImport>,
		const QString&,int,QWidget*
	);
};
} // end namespace

//=====================================
// Includes
//-------------------------------------
#include "monitordualmodel.h"

#endif
