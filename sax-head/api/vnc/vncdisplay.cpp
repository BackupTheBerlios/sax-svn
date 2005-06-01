/**************
FILE          : vncdisplay.cpp
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
              : - vncdisplay.h: SaXGUI::SCCVNCDisplay implementation
              : - vncdisplay.cpp: configure VNC system
              : ---- 
              :
STATUS        : Status: Development
**************/
#include "vncdisplay.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCVNCDisplay::SCCVNCDisplay (
	QDict<QString>* text, QDict<SaXImport> section,
	int, QWidget* parent
) : SCCDialog ( 0,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create layout for this widget 
	//-------------------------------------
	mMainLayout = new QVBoxLayout ( this );

	//=====================================
	// create macro widgets
	//-------------------------------------
	mCheckVNC = new QCheckBox ( mText["EnableVNC"], this );
	mOptions = new QButtonGroup (
		1,Horizontal,mText["VNCOptions"],this
	);
	mOptions -> setDisabled (true);
	mCheckPWD = new QCheckBox ( mText["PWDProtect"], mOptions );

	QHBox* space = new QHBox ( mOptions );
	QLabel* hidden1 = new QLabel ( space );
	hidden1 -> setFixedWidth ( 22 );
	mEntries = new QVBox ( space );
	mEntries -> setSpacing (5);
	mEntries -> setDisabled ( true );

	QVBox* pwd1 = new QVBox ( mEntries );
	QLabel* pwd1text = new QLabel ( pwd1 );
	pwd1text -> setText ( mText["Password"] );
	mPWD1 = new QLineEdit ( pwd1 );
	mPWD1 -> setText ( mText["NewPassword"] );
	mPWD1 -> setMaximumWidth ( 400 );

	QVBox* pwd2 = new QVBox ( mEntries );
	QLabel* pwd2text = new QLabel ( pwd2 );
	pwd2text -> setText ( mText["PasswordVerify"] );
	mPWD2 = new QLineEdit ( pwd2 );
	mPWD2 -> setEchoMode ( QLineEdit::Password );
	mPWD2 -> setMaximumWidth ( 400 );

	QLabel* hidden2 = new QLabel ( mEntries );
	hidden2 -> setFixedHeight ( 5 );
	
	mCheckShared   = new QCheckBox ( mText["VNCShared"], mOptions );
	mCheckHTTP = new QCheckBox ( mText["VNCHTTP"], mOptions );
	QHBox* space2 = new QHBox ( mOptions );
	QLabel* hidden3 = new QLabel ( space2 );
	hidden3 -> setFixedWidth ( 22 );
	mHTTPEntries = new QVBox ( space2 );
	mHTTPEntries -> setSpacing (5);
	mHTTPEntries -> setDisabled ( true );

	QVBox* http = new QVBox ( mHTTPEntries );
	QLabel* httptext = new QLabel ( http );
	httptext -> setText ( mText["HTTPPort"] );
	mHTTP = new QSpinBox (5000,10000,1,http);
	mHTTP -> setValue ( 5800 );
	mHTTP -> setMaximumWidth ( 400 );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mCheckVNC , SIGNAL ( toggled         ( bool ) ),
		this      , SLOT   ( slotActivateVNC ( bool ) )
	);
	QObject::connect (
		mCheckPWD , SIGNAL ( toggled         ( bool ) ),
		this      , SLOT   ( slotPassword    ( bool ) )
	);
	QObject::connect (
		mCheckHTTP , SIGNAL ( toggled        ( bool ) ),
		this       , SLOT   ( slotHTTP       ( bool ) )
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mMainLayout -> setMargin  ( 15 );
	mMainLayout -> addWidget  ( mCheckVNC );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mOptions );
}
//====================================
// init
//------------------------------------
void SCCVNCDisplay::init ( void ) {
	// nothing to be done for init...
}
//====================================
// import
//------------------------------------
void SCCVNCDisplay::import ( void ) {
	//====================================
	// create VNC manipulator
	//------------------------------------
	SaXManipulateVNC mVNC (
		mSection["Card"],mSection["Pointers"],mSection["Keyboard"],
		mSection["Layout"],mSection["Path"]
	);
	//====================================
	// handle basic VNC switch
	//------------------------------------
	if (mVNC.isVNCEnabled()) {
		mCheckVNC -> setChecked ( true );
	}
	//====================================
	// handle VNC password feature
	//------------------------------------
	if (mVNC.isPwdProtectionEnabled()) {
		mCheckPWD -> setChecked ( true );
	}
	//====================================
	// handle VNC multi connection feature
	//------------------------------------
	if (mVNC.isMultiConnectEnabled()) {
		mCheckShared -> setChecked ( true );
	}
	//====================================
	// handle VNC HTTP connection feature
	//------------------------------------
	if (mVNC.isHTTPAccessEnabled()) {
		mCheckHTTP -> setChecked ( true );
		mHTTP -> setValue ( mVNC.getHTTPPort() );
	}
}
//====================================
// isEnabled
//------------------------------------
bool SCCVNCDisplay::isEnabled ( void ) {
	return mCheckVNC->isChecked();
}
//====================================
// isHTTPEnabled
//------------------------------------
bool SCCVNCDisplay::isHTTPEnabled ( void ) {
	return mCheckHTTP->isChecked();
}
//====================================
// isPWDProtected
//------------------------------------
bool SCCVNCDisplay::isPWDProtected ( void ) {
	return mCheckPWD->isChecked();
}
//====================================
// isShared
//------------------------------------
bool SCCVNCDisplay::isShared ( void ) {
	return mCheckShared->isChecked();
}
//====================================
// getHTTPPort
//------------------------------------
int SCCVNCDisplay::getHTTPPort ( void ) {
	return mHTTP->value();
}
//====================================
// setupPassword
//------------------------------------
void SCCVNCDisplay::setupPassword ( void ) {
	if (mCheckPWD -> isOn()) {
		if (mPWD2 -> isEnabled()) {
		if (mPWD1->text() != mPWD2->text()) {
			SCCMessage* mMessageBox = new SCCMessage (
				qApp->mainWidget(), mTextPtr, SaXMessage::OK, "PWDMismatch",
				"MessageCaption", SaXMessage::Critical
			);
			mMessageBox -> showMessage();
			mPWD1->clear();
			mPWD2->clear();
			return;
		} else if (mPWD1->text().length() < 6) {
			SCCMessage* mMessageBox = new SCCMessage (
				qApp->mainWidget(), mTextPtr, SaXMessage::OK, "PWDTooShort",
				"MessageCaption", SaXMessage::Critical
			);
			mMessageBox -> showMessage();
			mPWD1->clear();
			mPWD2->clear();
			return;
		} else {
			QProcess* proc = new QProcess ();
			proc -> addArgument ( GETVNCPWD );
			proc -> addArgument ( mPWD1->text() );
			if ( ! proc -> start() ) {
				return;
			}
			while (proc->isRunning()) {
				usleep (1000);
			}
		}
		}
	}
}
//====================================
// slotActivateVNC
//------------------------------------
void SCCVNCDisplay::slotActivateVNC ( bool on ) {
	if (on) {
		mOptions -> setDisabled (false);
	} else {
		mOptions -> setDisabled (true);
	}
}
//====================================
// slotPassword
//------------------------------------
void SCCVNCDisplay::slotPassword ( bool on ) {
	if (on) {
		mEntries -> setDisabled (false);
	} else {
		mEntries -> setDisabled (true);
	}
}
//====================================
// slotHTTP
//------------------------------------
void SCCVNCDisplay::slotHTTP ( bool on ) {
	if (on) {
		mHTTPEntries -> setDisabled (false);
	} else {
		mHTTPEntries -> setDisabled (true);
	}
}
} // end namespace
