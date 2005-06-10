/**************
FILE          : keyboard.cpp
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
              : - keyboard.h: SaXGUI::SCCKeyboard implementation
              : - keyboard.cpp: configure keyboard system
              : ----
              :
STATUS        : Status: Development
**************/
#include "keyboard.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCKeyboard::SCCKeyboard (
	QWidgetStack* stack,QDict<QString>* text,
	QDict<SaXImport> section, QWidget* parent
) : SCCDialog ( stack,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create keyboard dialog
	//-------------------------------------
	mKeyboardTab   = new QTabWidget  ( mDialogFrame );
	QHBox* testBox = new QHBox       ( mDialogFrame );
	mTestLabel     = new QLabel      ( mText["Test"],testBox );
	testBox -> setSpacing ( 10 );
	mTestField     = new QLineEdit   ( testBox );

	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mDialogLayout -> addWidget  ( mKeyboardTab );
	mDialogLayout -> addSpacing ( 15 );
	mDialogLayout -> addWidget  ( testBox );

	//=====================================
	// create basic keyboard tabs
	//-------------------------------------
	mKeyboardLayout  = new SCCKeyboardLayout  ( text,section,0,this );
	mKeyboardOptions = new SCCKeyboardOptions ( text,section,0,this );
	mKeyboardTab -> addTab (
		mKeyboardLayout , mText["XKBLayout"] 
	);
	mKeyboardTab -> addTab (
		mKeyboardOptions, mText["XKBOptions"]
	);
	QObject::connect (
		mKeyboardLayout , SIGNAL (sigApply ( void )),
		this            , SLOT   (apply    ( void ))
	);
	QObject::connect (
		mKeyboardOptions, SIGNAL (sigApply ( void )),
		this            , SLOT   (apply    ( void ))
	);
	//=====================================
	// initialize basic keyboard tabs
	//-------------------------------------
	mKeyboardLayout  -> init();
	mKeyboardLayout  -> import();
	mKeyboardOptions -> init();
	mKeyboardOptions -> import();
}
//====================================
// init
//------------------------------------
void SCCKeyboard::init ( void ) {
	//====================================
	// set init state
	//------------------------------------
	needInitData = false;
}
//====================================
// import
//------------------------------------
void SCCKeyboard::import ( void ) {
	//====================================
	// set import state
	//------------------------------------
	needImportData = false;
}
//====================================
// apply keyboard
//------------------------------------
void SCCKeyboard::apply ( void ) {
	#if 0
	if (mKeyboardTab->isVisible()) {
		qApp->setOverrideCursor ( Qt::forbiddenCursor );
		QString layoutApply = mKeyboardLayout  -> getApplyString();
		QString optionApply = mKeyboardOptions -> getApplyString();
		QString complete = "setxkbmap " + layoutApply + " " + optionApply;
		QProcess* proc = new QProcess ();
		proc -> setArguments ( QStringList::split( " ",complete) );
		if ( ! proc -> start() ) {
			qApp->restoreOverrideCursor();
			return;
		}
		while (proc->isRunning()) {
			usleep (1000);
		}
		qApp->restoreOverrideCursor();
	}
	#endif
}
//====================================
// exportData
//------------------------------------
bool SCCKeyboard::exportData ( void ) {
	//====================================
	// create Manipulator
	//------------------------------------
	SaXManipulateKeyboard saxKeyboard (
		mSection["Keyboard"]
	);
	//====================================
	// retrieve dialog data
	//------------------------------------
	QString type    = mKeyboardLayout  -> getType();
	QString layout  = mKeyboardLayout  -> getLayout();
	QString variant = mKeyboardLayout  -> getVariant();
	QString option  = mKeyboardOptions -> getOptions();

	//====================================
	// save keyboard model/type
	//------------------------------------
	saxKeyboard.setXKBModel  ( type );

	int count = 0;
	//====================================
	// save keyboard layout(s)
	//------------------------------------
	QStringList variants = QStringList::split ( ",",variant,true );
	QStringList layouts  = QStringList::split ( ",",layout );
	for (QStringList::Iterator it=layouts.begin();it!=layouts.end();++it) {
		QString layout  (*it);
		QString variant (variants[count]);
		if (count == 0) {
			saxKeyboard.setXKBLayout ( layout );
			saxKeyboard.setXKBVariant (layout,variant);
		} else {
			saxKeyboard.addXKBLayout ( layout );
			if (variant.isEmpty()) {
				variant = ",";
			}
			saxKeyboard.setXKBVariant (layout,variant);
		}
		count++;
	}
	count = 0;
	//====================================
	// save keyboard options
	//------------------------------------
	QStringList options = QStringList::split ( ",",option );
	for (QStringList::Iterator it=options.begin();it!=options.end();++it) {
		QString option (*it);
		if (count == 0) {
			saxKeyboard.setXKBOption ( option );
		} else {
			saxKeyboard.addXKBOption ( option );
		}
		count++;
	}
	return true;
}
} // end namespace
