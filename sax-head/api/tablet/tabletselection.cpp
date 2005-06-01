/**************
FILE          : tabletselection.cpp
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
              : - tabletselection.h: SaXGUI::SCCTabletSelection implementation
              : - tabletselection.cpp: configure tablet system
              : ----
              :
STATUS        : Status: Development
**************/
#include "tabletselection.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCTabletSelection::SCCTabletSelection (
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
	mCheckEnable = new QCheckBox ( mText["ActivateTablet"],this );
	mModelVendorGroup = new QButtonGroup (
		1,Vertical,mText["TabletSelection"],this
	);
	mVendorList = new QListBox ( mModelVendorGroup );
	mModelList  = new QListBox ( mModelVendorGroup );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mCheckEnable , SIGNAL (clicked            ( void )),
		this         , SLOT   (slotActivateTablet ( void ))
	);
	QObject::connect (
		mVendorList  , SIGNAL (clicked       ( QListBoxItem* )),
		this         , SLOT   (slotVendor    ( QListBoxItem* ))
	);
	QObject::connect (
		mVendorList  , SIGNAL (returnPressed ( QListBoxItem* )),
		this         , SLOT   (slotVendor    ( QListBoxItem* ))
	);
	QObject::connect (
		mModelList   , SIGNAL (clicked       ( QListBoxItem* )),
		this         , SLOT   (slotName      ( QListBoxItem* ))
	);
	//=====================================
	// add widgets to the layout
	//-------------------------------------
	mMainLayout -> setMargin  ( 15 );
	mMainLayout -> addWidget  ( mCheckEnable );
	mMainLayout -> addSpacing ( 15 );
	mMainLayout -> addWidget  ( mModelVendorGroup );
}
//====================================
// init
//------------------------------------
void SCCTabletSelection::init ( void ) {
	//====================================
	// create manipulators... 
	//------------------------------------   
	mSaxTablet = new SaXManipulateTablets (
		mSection["Pointers"],mSection["Layout"]
	);
	//====================================
	// insert CDB mice
	//------------------------------------
	mCDBTabletVendors = mSaxTablet->getTabletVendorList();
	QListIterator<QString> it (mCDBTabletVendors);
	for (; it.current(); ++it) {
		mVendorList -> insertItem (*it.current());
	}
	mVendorList -> sort();

	//====================================
	// update widget status...
	//------------------------------------
	slotActivateTablet();
	emit sigActivate ( false );
}
//====================================
// import
//------------------------------------
void SCCTabletSelection::import ( void ) {
	//=====================================
	// search tablet ID
	//-------------------------------------
	int tabletID = 0;
	for (int i=SAX_CORE_POINTER;i<mSection["Pointers"]->getCount();i+=2) {
		if (mSaxTablet->selectPointer (i)) {
		if (mSaxTablet->isTablet()) {
			tabletID = i;
		}
		}
	}
	if ( tabletID == 0 ) {
		return;
	}
	//=====================================
	// select tablet
	//-------------------------------------
	mSaxTablet -> selectPointer ( tabletID );
}
//====================================
// setEnabled
//------------------------------------
void SCCTabletSelection::setEnabled ( bool status ) {
	mCheckEnable -> setChecked ( status );
	slotActivateTablet();
}
//====================================
// isEnabled
//------------------------------------
bool SCCTabletSelection::isEnabled ( void ) {
	return mEnabled;
}
//====================================
// getVendor
//------------------------------------
QString SCCTabletSelection::getVendor ( void ) {
	if (mVendorList->selectedItem()) {
		return mVendorList->selectedItem()->text();
	}
	return QString();
}
//====================================
// getModel
//------------------------------------
QString SCCTabletSelection::getModel  ( void ) {
	if (mModelList->selectedItem()) {
		return mModelList->selectedItem()->text();
	}
	return QString();
}
//====================================
// slotVendor
//------------------------------------
void SCCTabletSelection::slotVendor ( QListBoxItem* item ) {
	if (! mSaxTablet ) {
		return;
	}
	mModelList -> clear();
	mCDBTabletModels = mSaxTablet->getTabletModelList (
		item->text()
	);
	QListIterator<QString> it (mCDBTabletModels);
	for (; it.current(); ++it) {
		mModelList -> insertItem (*it.current());
	}
	mModelList -> sort();
	emit sigActivate ( false );
}
//====================================
// slotName
//------------------------------------
void SCCTabletSelection::slotName ( QListBoxItem* ) {
	emit sigActivate ( true );
	emit sigTablet (
		mVendorList->currentText(),
		mModelList->currentText()
	);
}
//====================================
// slotActivateTablet
//------------------------------------
void SCCTabletSelection::slotActivateTablet ( void ) {
	if ( mCheckEnable -> isChecked()) {
		mModelVendorGroup -> setDisabled ( false );
		if ( mModelList->selectedItem() ) {
			emit sigActivate ( true );
			QDict<QString> tabletDict = mSaxTablet->getTabletData (
				mVendorList->currentText(),
				mModelList->currentText()
			);
			bool hasPen    = false;
			bool hasEraser = false;
			if (tabletDict["EraserLink"]) {
				hasEraser = true;
			}
			if (tabletDict["StylusLink"]) {
				hasPen = true;
			}
			SCCTablet* parent = (SCCTablet*)mParent;
			parent -> enablePens ( true );
			if ((! hasPen) && (! hasEraser)) {
				parent->enablePens ( false );
			}
		}
		mEnabled = true;
	} else {
		mModelVendorGroup -> setDisabled ( true );
		emit sigActivate ( false );
		mEnabled = false;
	}
}
//====================================
// selectTablet
//------------------------------------
void SCCTabletSelection::selectTablet (
	const QString& vendorName, const QString& modelName
) {
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
} // end namespace
