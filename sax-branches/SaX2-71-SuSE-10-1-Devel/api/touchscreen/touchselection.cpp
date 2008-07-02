/**************
FILE          : touchselection.cpp
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
              : - touchselection.h: SaXGUI::SCCTouchSelection implementation
              : - touchselection.cpp: configure touchscreen system
              : ----
              :
STATUS        : Status: Development
**************/
#include "touchselection.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCTouchSelection::SCCTouchSelection (
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
	QString displayNr;
	QTextOStream (&displayNr) << (display + 1);
	QString translation = mText["AssignToucher"];
	translation.replace ( QRegExp("\%1"),displayNr );
	mCheckEnable = new QCheckBox ( translation,this );
	mModelVendorGroup = new QButtonGroup (
		1,Vertical,mText["TouchSelection"],this
	);
	mVendorList = new QListBox ( mModelVendorGroup );
	mModelList  = new QListBox ( mModelVendorGroup );
	mPortGroup = new QButtonGroup (
		1,Vertical,mText["ConnectionPort"],this
	);
	mPortBox = new QComboBox ( mPortGroup );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mCheckEnable , SIGNAL (clicked             ( void )),
		this         , SLOT   (slotActivateToucher ( void ))
	);
	QObject::connect (
		mVendorList  , SIGNAL (selectionChanged ( QListBoxItem* )),
		this         , SLOT   (slotVendor       ( QListBoxItem* ))
	);
	QObject::connect (
		mModelList   , SIGNAL (selectionChanged ( QListBoxItem* )),
		this         , SLOT   (slotName         ( QListBoxItem* ))
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mMainLayout -> setMargin  ( 15 );
	mMainLayout -> addWidget  ( mCheckEnable );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mModelVendorGroup );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mPortGroup ); 

	//=====================================
	// save display
	//-------------------------------------
	mDisplay = display;
}
//====================================
// init
//------------------------------------
void SCCTouchSelection::init ( void ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//====================================
	// create manipulators... 
	//------------------------------------   
	mSaxToucher = new SaXManipulateTouchscreens (
		mSection["Pointers"]
	);
	//====================================
	// insert CDB touchscreen vendors
	//------------------------------------
	mCDBTouchScreenVendors = mSaxToucher->getPanelVendorList();
	QListIterator<QString> it (mCDBTouchScreenVendors);
	for (; it.current(); ++it) {
		mVendorList -> insertItem (*it.current());
	}
	mVendorList -> sort();

	//=====================================
	// insert available port device names
	//-------------------------------------
	mPortBox -> insertItem ( mText["USB"] );
	mPortBox -> insertItem ( mText["Serial1"] );
	mPortBox -> insertItem ( mText["Serial2"] );
	mPortBox -> insertItem ( mText["Serial3"] );
	mPortBox -> insertItem ( mText["Serial4"] );

	//====================================
	// update widget status...
	//------------------------------------
	slotActivateToucher();
}
//====================================
// import
//------------------------------------
void SCCTouchSelection::import ( void ) {
	//=====================================
	// search touchscreen ID's
	//-------------------------------------
	int n = 0;
	int x = 0;
	int touchID[mSection["Pointers"]->getCount()];
	for (int i=SAX_CORE_POINTER;i<mSection["Pointers"]->getCount();i+=2) {
		touchID[x] = 0;
		if (mSaxToucher->selectPointer (i)) {
		if (mSaxToucher->isTouchpanel()) {
			touchID[n] = i;
			n++;
		}
		}
		x++;
	}
	//=====================================
	// no touchers found... return
	//-------------------------------------
	if (touchID[mDisplay] == 0) {
		return;
	}
	//=====================================
	// select touchscreen
	//-------------------------------------
	mSaxToucher -> selectPointer ( touchID[mDisplay] );

	//=====================================
	// select vendor and name
	//-------------------------------------
	QString vendorName = mSaxToucher -> getVendor();
	QString modelName  = mSaxToucher -> getName(); 
	if ((! vendorName.isEmpty()) && (! modelName.isEmpty())) {
		QListBoxItem* vendor = mVendorList -> findItem ( vendorName );
		if ( vendor ) {
			mVendorList -> setSelected ( vendor, true );
			slotVendor ( vendor );
			QListBoxItem* name = mModelList -> findItem ( modelName );
			if ( name ) {
				mModelList -> setSelected ( name, true );
				slotName ( name );
			}
		}
	}
	//=====================================
	// setup port
	//-------------------------------------
	QString device = mSaxToucher -> getDevice();
	QRegExp identifier ("/dev/input/mice");
	if (identifier.search (device) >= 0) {
		mPortBox -> setCurrentItem ( 0 );
		mPortBox -> setCurrentText (device);
	}
	if (device == "/dev/ttyS0") {
		mPortBox -> setCurrentItem ( 1 );
	}
	if (device == "/dev/ttyS1") {
		mPortBox -> setCurrentItem ( 2 );
	}

	//=====================================
	// setup touchscreen display state
	//-------------------------------------
	SaXManipulateLayout saxLayout ( mSection["Layout"],mSection["Card"] );
	QList<QString> inputLayout = saxLayout.getInputLayout();
	QListIterator<QString> it (inputLayout);
	bool foundID = false;
	for (; it.current();++it) {
		if (touchID[mDisplay] == it.current()->toInt()) {
			foundID = true;
			break;
		}
	}
	if ( foundID ) {
		mCheckEnable -> setChecked ( true );
		slotActivateToucher();
	}
}
//====================================
// slotActivateToucher
//------------------------------------
void SCCTouchSelection::slotActivateToucher ( void ) {
	if ( mCheckEnable -> isChecked()) {
		mModelVendorGroup -> setDisabled ( false );
		mPortGroup -> setDisabled ( false );
		mEnabled = true;
	} else {
		mModelVendorGroup -> setDisabled ( true );
		mPortGroup -> setDisabled ( true );
		mEnabled = false;
	}
}
//====================================
// slotVendor
//------------------------------------
void SCCTouchSelection::slotVendor ( QListBoxItem* item ) {
	if (! mSaxToucher ) {
		return;
	}
	mModelList -> clear();
	mCDBTouchScreenModels = mSaxToucher->getPanelModelList (
		item->text()
	);
	QListIterator<QString> it (mCDBTouchScreenModels);
	for (; it.current(); ++it) {
		mModelList -> insertItem (*it.current());
	}
	mModelList -> sort();
}
//====================================
// slotName
//------------------------------------
void SCCTouchSelection::slotName ( QListBoxItem* item ) {
	if (! mSaxToucher ) {
		return;
	}
	QDict<QString> dataDict = mSaxToucher->getPanelData (
		mVendorList->currentText(),item->text()
	);
	if (dataDict["Device"]) {
		QString device = *dataDict["Device"];
		QRegExp identifier ("/dev/input/event");
		if (identifier.search (device) >= 0) {
			mPortBox -> setCurrentItem ( 0 );
		}
		if (device == "/dev/ttyS0") {
			mPortBox -> setCurrentItem ( 1 );
		}
		if (device == "/dev/ttyS1") {
			mPortBox -> setCurrentItem ( 2 );
		}
	}
}
//====================================
// getVendor
//------------------------------------
QString SCCTouchSelection::getVendor ( void ) {
	if (mVendorList->selectedItem()) {
		return mVendorList->selectedItem()->text();
	}
	return QString();
}
//====================================
// getModel
//------------------------------------
QString SCCTouchSelection::getModel ( void ) {
	if (mModelList->selectedItem()) {
		return mModelList->selectedItem()->text();
	}
	return QString();
}
//====================================
// getPortName
//------------------------------------
QString SCCTouchSelection::getPortName ( void ) {
	return mPortBox->currentText();
}
//====================================
// isEnabled
//------------------------------------
bool SCCTouchSelection::isEnabled ( void ) {
	return mEnabled;
}
} // end namespace
