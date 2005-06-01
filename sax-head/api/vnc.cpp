/**************
FILE          : vnc.cpp
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
              : - vnc.h: SaXGUI::SCCVNC header definitions
              : - vnc.cpp: configure VNC system
              : ----
              :
STATUS        : Status: Development
**************/
#include "vnc.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCVNC::SCCVNC (
	QWidgetStack* stack,QDict<QString>* text,
	QDict<SaXImport> section, QWidget* parent
) : SCCDialog ( stack,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create VNC dialog
	//-------------------------------------
	mVNCTab = new QTabWidget  ( mDialogFrame );

	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mDialogLayout -> addWidget  ( mVNCTab );

	//=====================================
	// create basic VNC tab
	//-------------------------------------
	mVNCDisplay = new SCCVNCDisplay ( text,section,0,this );
	mVNCTab -> addTab (
		mVNCDisplay, mText["VNC"]
	);
	//=====================================
	// initialize basic VNC tab
	//-------------------------------------
	mVNCDisplay -> init();
	mVNCDisplay -> import();
}
//=====================================
// init
//-------------------------------------
void SCCVNC::init ( void ) {
	//====================================
	// set init state
	//------------------------------------
	needInitData = false;
}
//=====================================
// import
//-------------------------------------
void SCCVNC::import ( void ) {
	//====================================
	// set import state
	//------------------------------------
	needImportData = false;
}
//=====================================
// exportData
//-------------------------------------
void SCCVNC::exportData ( void ) {
	// TODO
}
} // end namespace
