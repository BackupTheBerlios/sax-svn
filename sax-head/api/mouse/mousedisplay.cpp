/**************
FILE          : mousedisplay.cpp
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
              : - mousedisplay.h: SaXGUI::SCCMouseDisplay header definitions
              : - mousedisplay.cpp: configure mouse system
              : ----
              :
STATUS        : Status: Development
**************/
#include "mousedisplay.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCMouseDisplay::SCCMouseDisplay (
	QDict<QString>* text, QDict<SaXImport> section,
	int display, QWidget* parent
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
	// toplevel activate check box
	mCheckEnable = new QCheckBox ( mText["ActivateMouse"],this );
	// first group with mouse name
	mMouseNameGroup = new QButtonGroup (
		1,Horizontal,"",this
	);
	QHBox* mouseBox = new QHBox ( mMouseNameGroup );
	mLabelMouseName = new QLabel ( mText["Mouse"],mouseBox );
	mChangeMouse    = new QPushButton ( mText["Change"],mouseBox);
	mouseBox -> setStretchFactor ( mLabelMouseName, 20 );
	// second group with mouse options
	QHBox* splitBox = new QHBox ( this );
	QVBox* leftBox  = new QVBox ( splitBox );
	mMouseOptionGroup = new QButtonGroup (
		1,Horizontal,mText["MouseOptions"],leftBox
	);
	mCheck3BtnEmulation = new QCheckBox (
		mText["ButtonEmulation"],mMouseOptionGroup
	);
	mCheckMouseWheel    = new QCheckBox (
		mText["ActivateWheel"],mMouseOptionGroup
	);
	QHBox* wheelBox = new QHBox ( mMouseOptionGroup );
	mCheckEmulateWheel = new QCheckBox (
		mText["EmulateWheel"],wheelBox
	);
	wheelBox -> setSpacing ( 10 );
	mEmulateWheelButton = new QSpinBox ( 4,10,1,wheelBox );
	// third group with mouse test field
	splitBox -> setSpacing ( 15 );
	QVBox* rightBox = new QVBox ( splitBox );
	mMouseTestGroup = new QButtonGroup (
		1,Horizontal,mText["MouseTest"],rightBox
	);
	mTestField = new SCCMouseTest (
		mTextPtr,mMouseTestGroup
	);
	splitBox -> setStretchFactor ( leftBox, 20 );

	//=====================================
	// create toplevel dialogs
	//-------------------------------------
	mChangeMouseModelDialog = new SCCMouseModel (
		text,section,mText["SetupMouse"],display,this
	);

	//=====================================
	// initialize toplevel dialogs
	//-------------------------------------
	mChangeMouseModelDialog -> init();
	mChangeMouseModelDialog -> import();

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mChangeMouse , SIGNAL (clicked         ( void )),
		this         , SLOT   (slotChangeMouse ( void ))
	);
	QObject::connect (
		mCheckEnable , SIGNAL (clicked             ( void )),
		this         , SLOT   (slotActivateDisplay ( void ))
	);
	QObject::connect (
		mCheckEmulateWheel , SIGNAL (clicked            ( void )),
		this               , SLOT   (slotWheelEmulation ( void ))
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mMainLayout -> setMargin  ( 15 );
	mMainLayout -> addWidget  ( mCheckEnable );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mMouseNameGroup );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( splitBox );

	//=====================================
	// save display number
	//-------------------------------------
	mDisplay  = display;
	mSaxMouse = 0;
}
//====================================
// init
//------------------------------------
void SCCMouseDisplay::init ( void ) {
	//====================================
	// create manipulators...
	//------------------------------------
	mSaxMouse = new SaXManipulateMice (
		mSection["Pointers"]
	);
	//====================================
	// obtain device count
	//------------------------------------
	int usedDevices = 0;
	mSaxMouse -> selectPointer (SAX_CORE_POINTER);
	for (int i=SAX_CORE_POINTER;i<mSection["Pointers"]->getCount();i+=2) {
		if (mSaxMouse -> selectPointer (i)) {
		if (mSaxMouse -> isMouse()) {
			usedDevices++;
		}
		}
	}
	//====================================
	// select pointer device
	//------------------------------------
	mSaxMouse -> selectPointer ( mDisplay );

	//====================================
	// check activate box
	//------------------------------------
	mCheckEnable -> setChecked ( true );
	if (usedDevices == 1) {
		mCheckEnable -> hide();
		mEnabled = true;
	} else {
		mCheckEnable -> setChecked ( true );
		mEnabled = true;
	}
	//====================================
	// check availability of options box
	//------------------------------------
	if (mSaxMouse -> getDriver() == "synaptics") {
		mMouseOptionGroup->setDisabled ( true );
	}
	//====================================
	// disable wheel button spin box
	//------------------------------------
	mEmulateWheelButton -> setDisabled ( true );
}
//====================================
// import
//------------------------------------
void SCCMouseDisplay::import ( void ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//====================================
	// check manipulators...
	//------------------------------------
	if (! mSaxMouse ) {
		return;
	}
	//====================================
	// select pointer device
	//------------------------------------
	mSaxMouse -> selectPointer ( mDisplay );

	//====================================
	// handle mouse name
	//------------------------------------
	QString mouseModel  = mSection["Pointers"]->getItem("Name");
	if (! mouseModel.isEmpty()) {
		mouseModel.replace(QRegExp(";")," ");
		mLabelMouseName -> setText (
			mText["Mouse"] + ": "+ mouseModel
		);
	} else {
		mLabelMouseName -> setText (
			mText["Mouse"] + ": " + mText["UnknownMousePleaseChange"]
		);
	}
	//====================================
	// handle 3 Button emulation
	//------------------------------------
	if (mSaxMouse -> isButtonEmulated()) {
		mCheck3BtnEmulation -> setChecked ( true );
	}
	//====================================
	// handle mouse wheel
	//------------------------------------
	if (mSaxMouse -> isWheelEnabled()) {
		mCheckMouseWheel -> setChecked ( true );
	}
	//====================================
	// handle mouse wheel emulation
	//------------------------------------
	if (mSaxMouse -> isWheelEmulated()) {
		mCheckEmulateWheel -> setChecked ( true );
	}
}
//====================================
// slotChangeMouse
//------------------------------------
void SCCMouseDisplay::slotChangeMouse ( void ) {
	mChangeMouseModelDialog -> show();
}
//====================================
// slotActivateDisplay
//------------------------------------
void SCCMouseDisplay::slotActivateDisplay ( void ) {
	if (mCheckEnable -> isChecked()) {
		mMouseNameGroup   -> setDisabled (false);
		mMouseOptionGroup -> setDisabled (false);
		mMouseTestGroup   -> setDisabled (false);
		mEnabled = true;
	} else {
		mMouseNameGroup   -> setDisabled (true);
		mMouseOptionGroup -> setDisabled (true);
		mMouseTestGroup   -> setDisabled (true);
		mEnabled = false;
	}
	if (! mSaxMouse) {
		return;
	}
	//====================================
	// check mouse driver from config
	//------------------------------------
	mSaxMouse -> selectPointer ( mDisplay );
	if (mSaxMouse -> getDriver() == "synaptics") {
		mMouseOptionGroup->setDisabled ( true );
	} else {
		//====================================
		// check mouse driver from V/N dialog
		//------------------------------------
		QDict<QString> mouseData = mSaxMouse -> getMouseData (
			mChangeMouseModelDialog->getVendorName(),
			mChangeMouseModelDialog->getModelName()
		);
		if (mouseData["Driver"]) {
		if (*mouseData["Driver"] == "synaptics") {
			mMouseOptionGroup->setDisabled ( true );
		}
		}
	}
	emit sigActivate();
}
//====================================
// slotWheelEmulation
//------------------------------------
void SCCMouseDisplay::slotWheelEmulation ( void ) {
	if (mCheckEmulateWheel -> isChecked()) {
		mEmulateWheelButton -> setDisabled ( false );
	} else {
		mEmulateWheelButton -> setDisabled ( true );
	}
}
//====================================
// isEnabled
//------------------------------------
bool SCCMouseDisplay::isEnabled  ( void ) {
	return mEnabled;
}
//====================================
// setEnabled
//------------------------------------
void SCCMouseDisplay::setEnabled ( bool status ) {
	mCheckEnable -> setChecked ( status );
	slotActivateDisplay();
}
//====================================
// setMouseName
//------------------------------------
void SCCMouseDisplay::setMouseName ( const QString& name ) {
	mLabelMouseName -> clear();
	mLabelMouseName -> setText ( name );
}
//====================================
// setMouseOptionState
//------------------------------------
void SCCMouseDisplay::setMouseOptionState ( bool status ) {
	mMouseOptionGroup->setDisabled ( ! status );
}
//====================================
// isButtonEmulationEnabled
//------------------------------------
bool SCCMouseDisplay::isButtonEmulationEnabled ( void ) {
	return mCheck3BtnEmulation->isChecked();
}
//====================================
// isWheelEmulationEnabled
//------------------------------------
bool SCCMouseDisplay::isWheelEmulationEnabled  ( void ) {
	return mCheckEmulateWheel->isChecked();
}
//====================================
// isWheelEnabled
//------------------------------------
bool SCCMouseDisplay::isWheelEnabled ( void ) {
	return mCheckMouseWheel->isChecked();
}
//====================================
// getWheelButton
//------------------------------------
int SCCMouseDisplay::getWheelButton ( void ) {
	return mEmulateWheelButton->value();
}
//====================================
// getModelData
//------------------------------------
SCCMouseModel* SCCMouseDisplay::getModelData ( void ) {
	return mChangeMouseModelDialog;
}
//====================================
// getDisplay
//------------------------------------
int SCCMouseDisplay::getDisplay ( void ) {
	return mDisplay;
}
//====================================
// setButtonEmulationEnabled
//------------------------------------
void SCCMouseDisplay::setButtonEmulationEnabled ( bool state ) {
	mCheck3BtnEmulation -> setChecked ( state );
}
//====================================
// setWheelEmulationEnabled
//------------------------------------
void SCCMouseDisplay::setWheelEmulationEnabled  ( bool state ) {
	mCheckEmulateWheel -> setChecked ( state );
	slotWheelEmulation();
}
//====================================
// setWheelEnabled
//------------------------------------
void SCCMouseDisplay::setWheelEnabled ( bool state ) {
	mCheckMouseWheel -> setChecked ( state );
}
//====================================
// setWheelButton
//------------------------------------
void SCCMouseDisplay::setWheelButton ( int btn ) {
	mEmulateWheelButton -> setValue ( btn );
}
} // end namespace
