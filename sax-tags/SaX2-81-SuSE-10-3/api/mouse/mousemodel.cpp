/**************
FILE          : mousemodel.cpp
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
              : - mousemodel.h: SaXGUI::SCCMouseModel implementation
              : - mousemodel.cpp: configure mouse model
              : ----
              :
STATUS        : Status: Development
**************/
#include "mousemodel.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCMouseModel::SCCMouseModel (
	QDict<QString>* text, QDict<SaXImport> section, const QString& title,
	int display, QWidget* parent
) : SCCDialog ( 0,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);
    
	//=====================================
	// create layout for this widget
	//-------------------------------------
	mModelTab  = new QVBox ( this );
   
	//=====================================
	// prepare tabed dialog
	//-------------------------------------
	mModelDialog = new QTabDialog  ( this,0,true );
	mModelDialog -> setCaption  ( mText["MouseCaption"] );
	mModelDialog -> setOkButton ( mText["Ok"] );
	mModelDialog -> setCancelButton ( mText["Cancel"] );
	mModelDialog -> addTab ( mModelTab, title );

	//=====================================
	// set import flag
	//-------------------------------------
	mSaxMouse    = 0;
	mNeedImport  = true;
	mOptionState = true;
	mDisplay = display;

	//=====================================
	// create macro widgets [Model]
	//-------------------------------------
	mModelTab -> setMargin ( 20 );
	mModelVendorGroup = new QButtonGroup (
		1,Vertical,mText["MouseSelection"],mModelTab
	);
	mVendorList = new QListBox ( mModelVendorGroup );
	mModelList  = new QListBox ( mModelVendorGroup );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mModelDialog , SIGNAL (applyButtonPressed ( void )),
		this         , SLOT   (slotOk ( void ))
	);
	QObject::connect (
		mVendorList  , SIGNAL (selectionChanged (QListBoxItem *)),
		this         , SLOT   (slotVendor       (QListBoxItem *))
	);
}
//=====================================
// show tabed dialog
//-------------------------------------
void SCCMouseModel::show (void) {
	mModelDialog->setGeometry  ( 0,0,640,480 );
	mModelDialog->move (
		getPosition (mModelDialog->width(),mModelDialog->height())
	);
	import();
	mModelDialog->show();
	mModelDialog->raise();
}
//====================================
// init
//------------------------------------
void SCCMouseModel::init ( void ) {
	//====================================
	// create manipulators... 
	//------------------------------------   
	mSaxMouse = new SaXManipulateMice (
		mSection["Pointers"]
	);
	//====================================
	// select pointer
	//------------------------------------
	mSaxMouse -> selectPointer ( mDisplay );

	//====================================
	// insert CDB mice
	//------------------------------------
	mCDBMouseVendors = mSaxMouse->getMouseVendorList();
	QListIterator<QString> it (mCDBMouseVendors);
	for (; it.current(); ++it) {
		mVendorList -> insertItem (*it.current());
	}
	mVendorList -> sort();
}
//====================================
// import
//------------------------------------
void SCCMouseModel::import ( void ) {
	if ( mNeedImport ) {
		//=====================================
		// check manipulator access
		//-------------------------------------
		if (! mSaxMouse ) {
			return;
		}
		//====================================
		// handle mouse vendor/model name
		//------------------------------------
		QString mouseModel  = mSection["Pointers"]->getItem("Name");
		QString mouseVendor = mSection["Pointers"]->getItem("Vendor");
		if (mouseVendor != "Sysp") {
			mouseModel=mouseVendor+";"+mouseModel;
		}
		if (! mouseModel.isEmpty()) {
			QStringList vnlist = QStringList::split ( ";", mouseModel );
			if (vnlist.count() == 2) {
				mSelectedMouseVendor = vnlist.first();
				mSelectedMouseName = vnlist.last();
			}
		}
		//====================================
		// import done set flag
		//------------------------------------
		mNeedImport = false;
	}
	//====================================
	// setup vendor name listboxes
	//------------------------------------
	QListBoxItem* vendor = mVendorList -> findItem ( mSelectedMouseVendor );
	if ( vendor ) {
		mVendorList -> setSelected ( vendor, true );
		slotVendor ( vendor );
		QListBoxItem* name = mModelList -> findItem ( mSelectedMouseName );
		if ( name ) {
			mModelList -> setSelected ( name, true );
		}
	}
}
//====================================
// getVendorName
//------------------------------------
QString SCCMouseModel::getVendorName ( void ) {
	return mSelectedMouseVendor;
}
//====================================
// getModelName
//------------------------------------
QString SCCMouseModel::getModelName ( void ) {
	return mSelectedMouseName;
}
//====================================
// getOptionState
//------------------------------------
bool SCCMouseModel::getOptionState ( void ) {
	return mOptionState;
}
//====================================
// slotVendor
//------------------------------------
void SCCMouseModel::slotVendor ( QListBoxItem* item ) {
	if (! mSaxMouse ) {
		return;
	}
	mModelList -> clear();
	mCDBMouseModels = mSaxMouse->getMouseModelList (
		item->text()
	);
	QListIterator<QString> it (mCDBMouseModels);
	for (; it.current(); ++it) {
		mModelList -> insertItem (*it.current());
	}
	mModelList -> sort();
}
//====================================
// slotOk
//------------------------------------
void SCCMouseModel::slotOk ( void ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// get parent SCCMouseDisplay ptr
	//-------------------------------------
	SCCMouseDisplay* pDisplay = (SCCMouseDisplay*)mParent;

	//=====================================
	// save data to dialog export variables
	//-------------------------------------
	if ((! mVendorList->selectedItem()) || (! mModelList->selectedItem())) {
		return;
	}
	mSelectedMouseVendor = mVendorList -> selectedItem()->text();
	mSelectedMouseName   = mModelList  -> selectedItem()->text();

	//=====================================
	// check parents options
	//-------------------------------------
	QDict<QString> selectedData = mSaxMouse->getMouseData (
		mSelectedMouseVendor,mSelectedMouseName
	);
	pDisplay -> setWheelEmulationEnabled  ( false );
	pDisplay -> setButtonEmulationEnabled ( false );
	pDisplay -> setWheelEnabled ( false );
	QDictIterator<QString> it (selectedData);
	for (; it.current(); ++it) {
		QString key = it.currentKey();
		QString val = *it.current();
		if (key == "EmulateWheel") {
			pDisplay -> setWheelEmulationEnabled ( true );
			pDisplay -> setWheelButton ( val.toInt() );
		}
		if (key == "ZAxisMapping") {
			pDisplay -> setWheelEnabled ( true );
		}
		if (key == "Emulate3Buttons") {
		if (val == "on") {
			pDisplay -> setButtonEmulationEnabled ( true );
		}
		}
	}
	//=====================================
	// check availability of options box
	//-------------------------------------
	mOptionState = true;
	pDisplay -> setMouseOptionState ( true );
	QDict<QString> mouseData = mSaxMouse -> getMouseData (
		mSelectedMouseVendor,mSelectedMouseName
	);
	if (mouseData["Driver"]) {
	if (*mouseData["Driver"] == "synaptics") {
		pDisplay -> setMouseOptionState ( false );
		mOptionState = false;
	}
	}
	//=====================================
	// update parent mouse name label
	//-------------------------------------
	QString mouseName;
	QTextOStream (&mouseName) <<
		mText["Mouse"]<< ": " <<
		mSelectedMouseVendor << " " << mSelectedMouseName;
	pDisplay -> setMouseName ( mouseName );
}
} // end namespace
