/**************
FILE          : vncdisplay.h
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
              : - vncdisplay.h: SaXGUI::SCCVNCDisplay header defs
              : - vncdisplay.cpp: configure VNC system
              : ----
              :
STATUS        : Status: Development
**************/
#ifndef SCCVNC_DISPLAY_H
#define SCCVNC_DISPLAY_H 1

//=====================================
// Includes
//-------------------------------------
#include <qcheckbox.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qspinbox.h>
#include <qvbox.h>

//=====================================
// Includes
//-------------------------------------
#include "dialog.h"

//=====================================
// Defines
//-------------------------------------
#define MACROS_DIR  "/usr/share/sax/api/macros"
#define GETVNCPWD   MACROS_DIR "/getVNCPassword"

namespace SaXGUI {
//====================================
// Class SCCTabletSelection
//------------------------------------
class SCCVNCDisplay : public SCCDialog {
	Q_OBJECT

	private:
	//====================================
	// private dialog data for exporting
	//------------------------------------
	bool           mEnabled;

	private:
	//====================================
	// private widget data contents
	//------------------------------------

	private:
	QBoxLayout*    mMainLayout;

	private:
	QCheckBox*     mCheckVNC;
	QButtonGroup*  mOptions;
	QCheckBox*     mCheckPWD;
	QVBox*         mEntries;
	QLineEdit*     mPWD1;
	QLineEdit*     mPWD2;
	QCheckBox*     mCheckShared;
	QCheckBox*     mCheckHTTP;
	QVBox*         mHTTPEntries;
	QSpinBox*      mHTTP;

	public:
	bool isEnabled      ( void );
	bool isHTTPEnabled  ( void );
	bool isPWDProtected ( void );
	bool isShared       ( void );
	int  getHTTPPort    ( void );
	void setupPassword  ( void );

	public:
	void init   ( void );
	void import ( void );

	public slots:
	void slotActivateVNC ( bool );
	void slotPassword    ( bool );
	void slotHTTP        ( bool );

	public:
	SCCVNCDisplay (
		QDict<QString>*, QDict<SaXImport>,
		int, QWidget*
	);
};
} // end namespace

#endif
