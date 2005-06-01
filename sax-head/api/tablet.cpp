/**************
FILE          : tablet.cpp
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
              : - tablet.h: SaXGUI::SCCTablet header definitions
              : - tablet.cpp: configure tablet system
              : ----
              :
STATUS        : Status: Development
**************/
#include "tablet.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCTablet::SCCTablet (
	QWidgetStack* stack,QDict<QString>* text,
	QDict<SaXImport> section, QWidget* parent
) : SCCDialog ( stack,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create tablet dialog
	//-------------------------------------
	mTabletTab = new QTabWidget  ( mDialogFrame );

	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mDialogLayout -> addWidget  ( mTabletTab );

	//=====================================
	// create basic tablet tabs
	//-------------------------------------
	mTabletSelection  = new SCCTabletSelection  ( text,section,0,this );
	mTabletConnection = new SCCTabletConnection ( text,section,0,this );
	mTabletPens       = new SCCTabletPens       ( text,section,0,this );
	mTabletTab -> addTab (
		mTabletSelection, mText["Tablet"]
	);
	mTabletTab -> addTab (
		mTabletConnection, mText["TabletPortAndMode"]
	);
	mTabletTab -> addTab (
		mTabletPens, mText["TabletElectronicPens"]
	);
	QObject::connect (
		mTabletSelection, SIGNAL (sigActivate  ( bool )),
		this            , SLOT   (slotActivate ( bool ))
	);
	QObject::connect (
		mTabletSelection , SIGNAL (sigTablet  (const QString&,const QString&)),
		mTabletConnection, SLOT   (slotTablet (const QString&,const QString&))
	);
	QObject::connect (
		mTabletSelection , SIGNAL (sigTablet  (const QString&,const QString&)),
		mTabletPens      , SLOT   (slotTablet (const QString&,const QString&))
	);
	//=====================================
	// initialize basic tablet tabs
	//-------------------------------------
	mTabletSelection   -> init();
	mTabletSelection   -> import();
	mTabletConnection  -> init();
	mTabletConnection  -> import();
	mTabletPens        -> init();
	mTabletPens        -> import();
}
//====================================
// init
//------------------------------------
void SCCTablet::init ( void ) {
	//====================================
	// set init state
	//------------------------------------
	needInitData = false;
}
//====================================
// import
//------------------------------------
void SCCTablet::import ( void ) {
	//====================================
	// create manipulators... 
	//------------------------------------
	SaXManipulateTablets saxTablet (
		mSection["Pointers"],mSection["Layout"]
	);
	SaXManipulateLayout saxLayout (
		mSection["Layout"],mSection["Card"]
	);
	//=====================================
	// search tablet ID
	//-------------------------------------
	int tabletID = 0;
	for (int i=SAX_CORE_POINTER;i<mSection["Pointers"]->getCount();i+=2) {
		if (saxTablet.selectPointer (i)) {
		if (saxTablet.isTablet()) { 
			tabletID = i;
		}
		}
	}
	if ( tabletID == 0 ) {
		mTabletSelection -> setEnabled ( false );
		return;
	}
	//====================================
	// handle tablet's state
	//------------------------------------  
	QList<QString> inputLayout = saxLayout.getInputLayout();
	QListIterator<QString> it (inputLayout);
	bool foundID = false;
	for (; it.current();++it) {
		if (tabletID == it.current()->toInt()) {
			foundID = true;
			break;
		}
	}
	if ( foundID ) {
		mTabletSelection -> setEnabled ( true );
	} else {
		mTabletSelection -> setEnabled ( false );
	}
	//====================================
	// set import state
	//------------------------------------
	needImportData = false;
}
//====================================
// getSelectionData
//------------------------------------
SCCTabletSelection* SCCTablet::getSelectionData ( void ) {
	return mTabletSelection;
}
//====================================
// getConnectionData
//------------------------------------
SCCTabletConnection* SCCTablet::getConnectionData ( void ) {
	return mTabletConnection;
}
//====================================
// disablePens
//------------------------------------
void SCCTablet::enablePens ( bool state ) {
	mTabletTab -> setTabEnabled ( mTabletPens, state );
}
//====================================
// slotActivate
//------------------------------------
void SCCTablet::slotActivate ( bool state ) {
	mTabletTab -> setTabEnabled ( mTabletConnection, state );
	mTabletTab -> setTabEnabled ( mTabletPens, state );
	mTabletTab -> setFocus();
}
//====================================
// exportData
//------------------------------------
void SCCTablet::exportData ( void ) {
	// TODO
}
} // end namespace
