/**************
FILE          : touchscreen.cpp
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
              : - touchscreen.h: SaXGUI::SCCTouchScreen header definitions
              : - touchscreen.cpp: configure touchscreen system
              : ----
              :
STATUS        : Status: Development
**************/
#include "touchscreen.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCTouchScreen::SCCTouchScreen (
	QWidgetStack* stack,QDict<QString>* text,
	QDict<SaXImport> section, QWidget* parent
) : SCCDialog ( stack,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// check for graphics cards
	//-------------------------------------
	SaXManipulateCard saxCard ( mSection["Card"] );
	mCard = saxCard.getDevices();

	//=====================================
	// create touchscreen dialog
	//-------------------------------------
	mTouchscreenTab = new QTabWidget  ( mDialogFrame );

	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mDialogLayout -> addWidget  ( mTouchscreenTab );

	//=====================================
	// create basic touchscreen tab
	//-------------------------------------
	for ( int n=0;n<mCard;n++) {
		QString toucherName;
		QTextOStream (&toucherName) << mText["Touchscreen"] << " " << (n + 1);
		SCCTouchSelection* toucher = new SCCTouchSelection  (
			text,section,n,this
		);
		mTouchDisplay.append ( toucher );
		mTouchscreenTab -> addTab (
			toucher, toucherName
		);
		toucher -> init();
		toucher -> import();
	}
}
//=====================================
// init
//-------------------------------------
void SCCTouchScreen::init ( void ) {
	//====================================
	// set init state
	//------------------------------------
	needInitData = false;
}
//=====================================
// import
//-------------------------------------
void SCCTouchScreen::import ( void ) {
	//====================================
	// set import state
	//------------------------------------
	needImportData = false;
}
//=====================================
// exportData
//-------------------------------------
void SCCTouchScreen::exportData ( void ) {
	// TODO
}
} // end namespace
