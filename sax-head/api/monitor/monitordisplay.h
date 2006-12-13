/**************
FILE          : monitordisplay.h
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
              : - monitordisplay.h: SaXGUI::SCCMonitorDisplay header defs
              : - monitordisplay.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCMONITOR_DISPLAY_H
#define SCCMONITOR_DISPLAY_H 1

//=====================================
// Includes
//-------------------------------------
#include <qpushbutton.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qtabdialog.h>
#include <qlabel.h>
#include <qspinbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"
#include "monitorcard.h"
#include "monitorfig.h"

//=====================================
// Defines...
//-------------------------------------
#define DTA_DIR   "/usr/share/sax/api/data"
#define RES_FILE  DTA_DIR "/MonitorResolution"
#define COL_FILE  DTA_DIR "/MonitorColors"

namespace SaXGUI {
//=====================================
// Pre-class definitions
//-------------------------------------
class SCCMonitorModel;
class SCCMonitorDual;

//====================================
// Class SCCMonitorDisplay
//------------------------------------
class SCCMonitorDisplay : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	int              mDisplay;
	bool             mEnabled;
	bool             mDualHeadEnabled;
	bool             mShowDisplaySizeMessage;
	QList<QString>   mSelectedResolution;
	int              mSelectedColor;
	QString          mCardModel;
	QString          mCardVendor;
	QString          mMonitorModel;
	QString          mMonitorVendor;
	QString          mDualInfo;

	private:
	//====================================
	// private widget data contents
	//------------------------------------
	QDict<QString>   mResolutionDict;
	QDict<QString>   mColorDict;

	private:
	QBoxLayout*      mMainLayout;

	private:
	QCheckBox*       mCheckEnable;
	QButtonGroup*    mCardMonitorGroup;
	QLabel*          mLabelCardName;
	QLabel*          mLabelMonitorName;
	QLabel*          mLabelCardText;
	QLabel*          mLabelMonitorText;
	QPushButton*     mCardOptions;
	QPushButton*     mChangeMonitor;
	QButtonGroup*    mPropertyGroup;
	QLabel*          mLabelResolution;
	QLabel*          mLabelColors;
	QComboBox*       mResolution;
	QComboBox*       mColors;
	QButtonGroup*    mDualHeadGroup;
	QCheckBox*       mCheckDualHead;
	QPushButton*     mConfigureDualHead;
	QLabel*          mDualHeadInfoLabel;
	SCCMonitorCard*  mCardOptionDialog;
	SCCMonitorModel* mChangeMonitorDialog;
	SCCMonitorDual*  mConfigureDualHeadDialog;

	public:
	bool isEnabled  ( void );
	void setEnabled ( bool = true );
	bool isDualModeEnabled ( void );
	bool isDualHeadDisplay ( void );
	QList<QString> getResolution ( void );
	int getColorDepth ( void );
	SCCMonitorModel* getMonitorData ( void );
	SCCMonitorCard*  getCardData ( void );
	SCCMonitorDual*  getDualData ( void );
	QPixmap getDualPixmap  ( void );
	QString getMonitorName ( void );
	void setMonitorName  ( const QString& );
	void setMonitorResolution ( const QString& );
	void setDualModeInfo ( const QString& );
	void setCommonButtonWidth ( void );
	void setCombinedDisplaySize ( bool );
	QString getCardName    ( void );
	int getDisplay ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotConfigureDualHead ( void );
	void slotChangeMonitor     ( void );
	void slotChangeCardOptions ( void );
	void slotActivateDisplay   ( void );
	void slotActivateDualHead  ( void );
	void slotResolution        ( int  );
	void slotColors            ( int  );

	signals:
	void sigActivate ( void );
	void sigDualModeChanged ( SCCMonitorDisplay* );

	public:
	SCCMonitorDisplay (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace

//=====================================
// Includes
//-------------------------------------
#include "monitormodel.h"
#include "monitordual.h"

#endif
