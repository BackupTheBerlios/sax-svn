/**************
FILE          : mousedisplay.h
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
              : - mousedisplay.h: SaXGUI::SCCMouseDisplay header defs
              : - mousedisplay.cpp: configure mouse system
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCMOUSE_DISPLAY_H
#define SCCMOUSE_DISPLAY_H 1

//=====================================
// Includes
//-------------------------------------
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qspinbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"
#include "mousetest.h"

namespace SaXGUI {
//=====================================
// Pre-class definitions
//-------------------------------------
class SCCMouseModel;

//====================================
// Class SCCMouseDisplay
//------------------------------------
class SCCMouseDisplay : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	int              mDisplay;
	bool             mEnabled;

	private:
	SaXManipulateMice* mSaxMouse;

	private:
	QBoxLayout*      mMainLayout;
	int              mMouseOptID;
    int              mSynapOptID;

	private:
	QCheckBox*       mCheckEnable;
	QButtonGroup*    mMouseNameGroup;
	QLabel*          mLabelMouseName;
	QPushButton*     mChangeMouse;
	QButtonGroup*    mMouseOptionGroup;
	QCheckBox*       mCheck3BtnEmulation;
	QCheckBox*       mCheckMouseWheel;
	QCheckBox*       mCheckEmulateWheel;
	QCheckBox*       mCheckInvertXAxis;
	QCheckBox*       mCheckInvertYAxis;
	QSpinBox*        mEmulateWheelButton;
	QButtonGroup*    mMouseTestGroup;
	SCCMouseTest*    mTestField;
	SCCMouseModel*   mChangeMouseModelDialog;
	QWidgetStack*    mOptionStack;

	public:
	bool isEnabled  ( void );
	void setEnabled ( bool = true );
	int  getDisplay ( void );
	bool isButtonEmulationEnabled ( void );
	bool isWheelEmulationEnabled  ( void );
	bool isWheelEnabled ( void );
	bool isXInverted    ( void );
	bool isYInverted    ( void );
	int  getWheelButton ( void );
	void setButtonEmulationEnabled ( bool );
	void setWheelEmulationEnabled  ( bool );
	void setWheelEnabled ( bool );
	void setWheelButton ( int );
	void setMouseName   ( const QString& );
	void setMouseOptionState ( bool );
	SCCMouseModel* getModelData ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotChangeMouse ( void );
	void slotActivateDisplay ( void );
	void slotWheelEmulation  ( void );

	signals:
	void sigActivate ( void );

	public:
	SCCMouseDisplay (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace

//=====================================
// Includes
//-------------------------------------
#include "mousemodel.h"

#endif
