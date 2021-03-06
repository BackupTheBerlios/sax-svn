/**************
FILE          : monitordualmodel.cpp
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
              : - monitordualmodel.h: SaXGUI::SCCMonitor header definitions
              : - monitordualmodel.cpp: configure monitor system
              : ----
              :
STATUS        : Status: Development
**************/
#include "monitordualmodel.h"

namespace SaXGUI {
//====================================
// Constructor
//------------------------------------
SCCMonitorDualModel::SCCMonitorDualModel (
	QDict<QString>* text, QDict<SaXImport> section,
	const QString& cardname, const QString& sync, int display,QWidget* parent
) : SCCDialog ( 0,text,section,parent ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (mTextPtr);

	//=====================================
	// create layout for this widget
	//-------------------------------------
	mModelTab = new QVBox ( this );
	mSyncTab  = new QVBox ( this );

	//=====================================
	// prepare tabed dialog
	//-------------------------------------
	mDualModelDialog = new QTabDialog ( this,0,true );
	mDualModelDialog -> setCaption  ( mText["DualModelCaption"] );
	mDualModelDialog -> setOkButton ( mText["Ok"] );
	mDualModelDialog -> setCancelButton ( mText["Cancel"] );
	mDualModelDialog -> addTab ( mModelTab, cardname );
	mDualModelDialog -> addTab ( mSyncTab, sync );

	//=====================================
	// set import flag
	//-------------------------------------
	mSaxDesktop = 0;
	mNeedImport = true;
	mDisplay = display;

	//=====================================
	// create macro widgets [Model]
	//-------------------------------------
	mModelTab -> setMargin ( 20 );
	mModelVendorGroup = new QButtonGroup (
		1,Vertical,mText["MonitorSelection"],mModelTab
	);
	mVendorList = new QListBox ( mModelVendorGroup );
	mModelList  = new QListBox ( mModelVendorGroup );

	//=====================================
	// create macro widgets [Sync]
	//-------------------------------------
	mSyncTab -> setMargin ( 20 );
	mSyncGroup = new QButtonGroup (
		1,Vertical,mText["SyncSelection"],mSyncTab
	);
	QVBox* textBox = new QVBox ( mSyncGroup );
	mLabelWarning  = new QLabel ( mText["FrqNote"],textBox );
	mLabelWarning -> setFixedWidth ( 200 );
	QLabel* latent1 = new QLabel ( textBox );
	textBox -> setStretchFactor ( latent1,10 );

	QVBox* seperatorBox = new QVBox ( mSyncGroup );
	QFrame* seperator   = new QFrame ( seperatorBox );
	seperator -> setFixedWidth (2);
	seperator -> setFrameStyle (
		QFrame::VLine | QFrame::Raised
	);
	seperatorBox -> setStretchFactor ( seperator,10 );

	mSyncBox = new QVBox ( mSyncGroup );
	QHBox* mHSyncBox = new QHBox ( mSyncBox );
	mLabelHorizontal  = new QLabel ( mText["Horizontal"],mHSyncBox );
	mHSyncBox -> setSpacing  ( 10 );
	mHSpinMin = new QSpinBox ( 30,300,1,mHSyncBox );
	mHSyncBox -> setSpacing  ( 5 );
	QLabel* hbar = new QLabel ( mHSyncBox );
	hbar -> setText ("-");
	mHSyncBox -> setSpacing  ( 5 );
	mHSpinMax = new QSpinBox ( 50,300,1,mHSyncBox );
	mHSyncBox -> setSpacing  ( 10 );
	mLabelHUnit = new QLabel ( mText["Khz"],mHSyncBox );
	mLabelHUnit -> setFixedWidth ( 50 );
	mHSyncBox -> setStretchFactor ( mHSpinMin,10 );
	mHSyncBox -> setStretchFactor ( mHSpinMax,10 );

	mSyncBox -> setSpacing ( 15 );
	QHBox* mVSyncBox = new QHBox ( mSyncBox );
	mLabelVertical  = new QLabel ( mText["Vertical"],mVSyncBox );
	mVSyncBox -> setSpacing  ( 10 );
	mVSpinMin = new QSpinBox ( 50,300,1,mVSyncBox );
	mVSyncBox -> setSpacing  ( 5 );
	QLabel* vbar = new QLabel ( mVSyncBox );
	vbar -> setText ("-");
	mVSyncBox -> setSpacing  ( 5 );
	mVSpinMax = new QSpinBox ( 60,300,1,mVSyncBox );
	mVSyncBox -> setSpacing  ( 10 );
	mLabelVUnit = new QLabel ( mText["Hz"],mVSyncBox );
	mLabelVUnit -> setFixedWidth ( 50 );
	mVSyncBox -> setStretchFactor ( mVSpinMin,10 );
	mVSyncBox -> setStretchFactor ( mVSpinMax,10 );

	QLabel* latent2 = new QLabel ( mSyncBox );
	mSyncBox -> setStretchFactor ( latent2,10 );
	QFontMetrics metrics ( font() );
	int charVS = metrics.width (mText["Vertical"]);
	int charHS = metrics.width (mText["Horizontal"]);
	if ( charVS < charHS ) {
		mLabelHorizontal->setFixedWidth ( charHS );
		mLabelVertical->setFixedWidth ( charHS );
	} else {
		mLabelHorizontal->setFixedWidth ( charVS );
		mLabelVertical->setFixedWidth ( charVS );
	}

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect (
		mDualModelDialog, SIGNAL (applyButtonPressed ( void )),
		this            , SLOT   (slotOk ( void ))
	);
	QObject::connect (
		mVendorList  , SIGNAL (clicked    (QListBoxItem *)),
		this         , SLOT   (slotVendor (QListBoxItem *))
	);
	QObject::connect (
		mVendorList  , SIGNAL (returnPressed (QListBoxItem *)),
		this         , SLOT   (slotVendor    (QListBoxItem *))
	);
	QObject::connect (
		mModelList   , SIGNAL (clicked  (QListBoxItem *)),
		this         , SLOT   (slotName (QListBoxItem *))
	);
}
//=====================================
// show tabed dialog
//-------------------------------------
void SCCMonitorDualModel::show (void) {
	mDualModelDialog->setGeometry  ( 0,0,600,400 );
	mDualModelDialog->move (
		getPosition (mDualModelDialog->width(),mDualModelDialog->height())
	);
	import();
	mDualModelDialog->show();
	mDualModelDialog->raise();
}
//====================================
// init
//------------------------------------
void SCCMonitorDualModel::init ( void ) {
	//====================================
	// create manipulators...
	//------------------------------------
	mSaxDesktop = new SaXManipulateDesktop (
		mSection["Desktop"],mSection["Card"],mSection["Path"]
	);
	//====================================
	// select desktop
	//------------------------------------
	mSaxDesktop -> selectDesktop ( mDisplay );

	//====================================
	// import dualhead profile if set
	//------------------------------------
	QString profile = mSaxDesktop -> getDualHeadProfile();
	if (! profile.isEmpty()) {
		SaXImportProfile* pProfile = new SaXImportProfile ( profile );
		pProfile -> doImport();
		SaXImport* mImport = pProfile -> getImport ( SAX_CARD );
		if ( mImport ) {
			SaXManipulateCard saxProfileCard ( mImport );
			mProfileDriverOptions = saxProfileCard.getOptions();
		}
	}
	//====================================
	// insert CDB monitors
	//------------------------------------
	mCDBMonitorVendors = mSaxDesktop->getCDBMonitorVendorList();
	QListIterator<QString> it (mCDBMonitorVendors);
	for (; it.current(); ++it) {
		mVendorList -> insertItem (*it.current());
	}
	mVendorList -> sort();
}
//====================================
// import
//------------------------------------
void SCCMonitorDualModel::import ( void ) {
	if ( mNeedImport ) {
		//====================================
		// create needed manipulators
		//------------------------------------
		SaXManipulateCard saxCard (
			mSection["Card"]
		);
		//====================================
		// select card
		//------------------------------------
		saxCard.selectCard ( mDisplay );

		//====================================
		// get options defined for the card
		//------------------------------------
		QDict<QString> mOptions = saxCard.getOptions();

		//====================================
		// handle monitor vendor/model name
		//------------------------------------
		if ((mOptions["SaXDualMonitorVendor"]) &&
			(mOptions["SaXDualMonitorModel"])
		) {
			mSelectedMonitorVendor = *mOptions["SaXDualMonitorVendor"];
			mSelectedMonitorName   = *mOptions["SaXDualMonitorModel"];
		} else
		if ((mProfileDriverOptions["SaXDualMonitorVendor"]) &&
			(mProfileDriverOptions["SaXDualMonitorModel"])
		) {
			QString vendor = *mProfileDriverOptions["SaXDualMonitorVendor"];
			QString model  = *mProfileDriverOptions["SaXDualMonitorModel"];
			mSelectedMonitorVendor = vendor;
			mSelectedMonitorName   = model;
		}
		//====================================
		// handle hsync/vsync values
		//------------------------------------
		if (mOptions["SaXDualHSync"]) {
			QStringList hsyncRange = QStringList::split ("-",
				*mOptions["SaXDualHSync"]
			);
			mHsyncMin = hsyncRange.first().toInt();
			mHsyncMax = hsyncRange.last().toInt();
		} else
		if (mProfileDriverOptions["SaXDualHSync"]) {
			QStringList hsyncRange = QStringList::split ("-",
				*mProfileDriverOptions["SaXDualHSync"]
			);
			mHsyncMin = hsyncRange.first().toInt();
			mHsyncMax = hsyncRange.last().toInt();
		}
		if (mOptions["SaXDualVSync"]) {
			QStringList vsyncRange = QStringList::split ("-",
				*mOptions["SaXDualVSync"]
            );
			mVsyncMin = vsyncRange.first().toInt();
			mVsyncMax = vsyncRange.last().toInt();
		} else
		if (mProfileDriverOptions["SaXDualVSync"]) {
			QStringList vsyncRange = QStringList::split ("-",
				*mProfileDriverOptions["SaXDualVSync"]
			);
			mVsyncMin = vsyncRange.first().toInt();
			mVsyncMax = vsyncRange.last().toInt();
		}
		//====================================
		// import done set flag
		//------------------------------------
		mNeedImport = false;
	}
	//====================================
	// setup vendor name listboxes
	//------------------------------------
	QListBoxItem* vendor = mVendorList -> findItem ( mSelectedMonitorVendor );
	if ( vendor ) {
		mVendorList -> setSelected ( vendor, true );
		slotVendor ( vendor );
		QListBoxItem* name = mModelList -> findItem ( mSelectedMonitorName );
		if ( name ) {
			mModelList -> setSelected ( name, true );
			slotName ( name );
		}
	}
	//====================================
	// setup hsync/vsync widgets
	//------------------------------------
	mHSpinMin -> setValue ( mHsyncMin );
	mHSpinMax -> setValue ( mHsyncMax );
	mVSpinMin -> setValue ( mVsyncMin );
	mVSpinMax -> setValue ( mVsyncMax );
}
//=====================================
// getVendorName
//-------------------------------------
QString SCCMonitorDualModel::getVendorName ( void ) {
	return mSelectedMonitorVendor;
}
//=====================================
// getModelName
//-------------------------------------
QString SCCMonitorDualModel::getModelName  ( void ) {
	return mSelectedMonitorName;
}
//=====================================
// getHSmin
//-------------------------------------
int SCCMonitorDualModel::getHSmin ( void ) {
	return mHsyncMin;
}
//=====================================
// getHSmax
//-------------------------------------
int SCCMonitorDualModel::getHSmax ( void ) {
	return mHsyncMax;
}
//=====================================
// getVSmin
//-------------------------------------
int SCCMonitorDualModel::getVSmin ( void ) {
	return mVsyncMin;
}
//=====================================
// getVSmax
//-------------------------------------
int SCCMonitorDualModel::getVSmax ( void ) {
	return mVsyncMax;
}
//=====================================
// slotOk
//-------------------------------------
void SCCMonitorDualModel::slotOk ( void ) {
	//=====================================
	// get parent SCCMonitorDisplay ptr
	//-------------------------------------
	SCCMonitorDual* pDual = (SCCMonitorDual*)mParent;

	//=====================================
	// save data to dialog export variables
	//-------------------------------------
	mHsyncMin = mHSpinMin -> value();
	mHsyncMax = mHSpinMax -> value();
	mVsyncMin = mVSpinMin -> value();
	mVsyncMax = mVSpinMax -> value();

	//=====================================
	// save data to dialog export variables
	//-------------------------------------
	if ((! mVendorList->selectedItem()) || (! mModelList->selectedItem())) {
		return;
	}
	mSelectedMonitorVendor = mVendorList -> selectedItem()->text();
	mSelectedMonitorName   = mModelList  -> selectedItem()->text();

	//=====================================
	// update monitor label
	//-------------------------------------
	pDual -> setMonitorName (
		mSelectedMonitorVendor,mSelectedMonitorName
	);
}
//=====================================
// slotName
//-------------------------------------
void SCCMonitorDualModel::slotName ( QListBoxItem* item ) {
	//=====================================
	// check manipulator access
	//-------------------------------------
	if (! mSaxDesktop ) {
		return;
	}
	mCDBMonitorData = mSaxDesktop->getCDBMonitorData (
		mVendorList->selectedItem()->text(),item->text()
	);
	mHSpinMin  -> setValue   ( 30 );
	mHSpinMax  -> setValue   ( 50 );
	mVSpinMin  -> setValue   ( 50 );
	mVSpinMax  -> setValue   ( 60 );
	QDictIterator<QString> it (mCDBMonitorData);
	for (; it.current(); ++it) {
		QString key = it.currentKey();
		QString val = *it.current();
		if (key == "VertRefresh") {
			QStringList tokens = QStringList::split ( "-", val );
			mVSpinMin -> setValue ( tokens.first().toInt() );
			mVSpinMax -> setValue ( tokens.last().toInt() );
		}
		if (key == "HorizSync") {
			QStringList tokens = QStringList::split ( "-", val );
			mHSpinMin -> setValue ( tokens.first().toInt() );
			mHSpinMax -> setValue ( tokens.last().toInt() );
		}
	}
}
//=====================================
// slotVendor
//-------------------------------------
void SCCMonitorDualModel::slotVendor ( QListBoxItem* item ) {
	if (! mSaxDesktop ) {
		return;
	}
	mModelList -> clear();
	mCDBMonitorModels = mSaxDesktop->getCDBMonitorModelList (
		item->text()
	);
	QListIterator<QString> it (mCDBMonitorModels);
	for (; it.current(); ++it) {
		mModelList -> insertItem (*it.current());
	}
	mModelList -> sort();
}
} // end namespace
