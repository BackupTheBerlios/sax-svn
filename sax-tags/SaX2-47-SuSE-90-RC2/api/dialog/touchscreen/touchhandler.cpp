/**************
FILE          : action.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure touchscreens as input devices mostly
              : used from elographics
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "touchscreen.h"

//=====================================
// XTouchScreen reset/switch to Intro
//-------------------------------------
void XTouchScreen::resetPage (int reload) {
	// ...
	// this function is called if the touchscreen dialog is finished or 
	// canceled The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_INPUT";

	if (reload == PAGE_RELOAD) {
		XLayout* layoutDialog;
		layoutDialog = (XLayout*) mIntro -> retrieve (Layout);
		layoutDialog -> updateServerLayout();
    }
	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
        if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_LAYOUT";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_PATH";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
	}
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XTouchScreen internal page called...
//-------------------------------------
void XTouchScreen::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case TouchScreenPage1: {
		break;
	}
	};
}

//=====================================
// XTouchScreen virtual slots...
//-------------------------------------
bool XTouchScreen::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	log(L_INFO,"XTouchScreen::slotRun() called: %d\n",index);
	// ...
	// this function is called if the touchscreen page is activated.
	// use this function to init the dialog with the current
	// setup of the touchscreen
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXTouchScreen"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// fill the touchList listbox with the currently
	// configured touchscreen records
	// ---
	QDict<char> toucherInfo;
	bool gotToucher = false;
	touchList -> clear();
	mIndexList.clear();
	QString topic; 
	XData* sysData;
	int toucher = mFiles["sys_INPUT"]->getDeviceCount();
	int count = -1;
	for (int n=0; n<toucher; n++) {
		count += 2;
		sysData = mFiles["sys_INPUT"] -> getDevice(count);
		if (! sysData) {
		continue;
		}
		toucherInfo = sysData -> getData();
		if (QString(toucherInfo["InputFashion"]) != "Touchpanel") {
		continue;
		}
		gotToucher = true;
		QString topic (toucherInfo["Name"]);
		XStringList name (toucherInfo["Name"]);
		name.setSeperator (";");
		QList<char> toucherName = name.getList();
		topic.sprintf("%s %s",
		  toucherName.at(0),toucherName.at(1)
		);
		int* index = (int*) malloc (sizeof(int));
		*index = count;
		mIndexList.append (index);
		touchList -> insertItem (
			QPixmap(PIXINTROTOUCH),topic
		);
	}
	// ...
	// enable/disable configure/remove buttons
	// and select first element of the listbox
	// ---
	if (gotToucher) {
		mDel   -> setDisabled ( false );
		mSetup -> setDisabled ( false );
		touchList -> setCurrentItem ( 0 );
		touchList -> setSelected    ( 0,true );
		mSelected = 0;
	} else {
		mDel   -> setDisabled ( true );
		mSetup -> setDisabled ( true );
	}
	}
	return (TRUE);
}

//=====================================
// XTouchScreen init page with data infos 
//-------------------------------------
void XTouchScreen::initPage (void) {
	// log (L_INFO,"XTouchScreen::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert touchscreen vendors...
	// -----------------------------
	QList<char> vendors = mFiles["cdb_TOUCHER"]->cdbGetVendorList();
	QListIterator<char> it (vendors);
	for (; it.current(); ++it) {
		mVendor -> insertItem (it.current());
	}
}

//=====================================
// XTouchScreen slotVendor...
//-------------------------------------
void XTouchScreen::slotVendor (QListBoxItem* item) {
	// log (L_INFO,"XTouchScreen::slotVendor() called\n");
	// ...
	// this function is called if you select a touchscreen vendor
	// from the touchscreen vendor listbox in the setup toplevel window
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	if (! mVendor->isSelected (item)) {
		return;
	}
	mName -> clear();
	QString selectedVendor = mVendor -> currentText();
	QList<char> names = mFiles["cdb_TOUCHER"]->cdbGetNameList (selectedVendor);
	QListIterator<char> it (names);
	for (; it.current(); ++it) {
		mName -> insertItem (it.current());
	}
	mName -> sort();
}

//=====================================
// XTouchScreen slotName...
//-------------------------------------
void XTouchScreen::slotName (QListBoxItem*) {
	// log (L_INFO,"XTouchScreen::slotName() called\n");
	// ...
	// this function is called if you select a touchscreen name
	// from the touchscreen name listbox in the setup toplevel window
	// ---
}

//=====================================
// XTouchScreen slotTopOk...
//-------------------------------------
void XTouchScreen::slotTopOk (void) {
	// log (L_INFO,"XTouchScreen::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
	bool newToucher = false;
	switch (mState) {
	case TOUCHER_NEW: 
		if ( ! mIndexList.isEmpty()) {
			mWorkingIndex = *mIndexList.at ( touchList->count()-1 );
			mWorkingIndex+= 2;
		} else {
			mWorkingIndex = 1;
		}
		newToucher = true;
	break;
	case TOUCHER_SETUP:
		mWorkingIndex = *mIndexList.at ( mSelected );
		newToucher = false;
	break; 
	}

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the workingToucher pointer wrapper. If a new toucher
	// should be added use addDevice to create a new object
	// otherwhise get the device according to the selection 
	// made in touchList and clear the contents
	// ---
	XWrapPointer<XData> workingToucher;
	if (newToucher) {
		while (mFiles["sys_INPUT"]->getDevice (mWorkingIndex)) {
			mWorkingIndex+= 2;
		}
		workingToucher.init (
		mFiles["sys_INPUT"]->addDevice (mWorkingIndex)
		);
	} else {
		workingToucher.init (
		mFiles["sys_INPUT"]->getDevice (mWorkingIndex)
		);
	}

	// ... /
	// set the selection data from the CDB if the user requested 
	// a new toucher from the list. The data set here may be overwritten
	// from the manual setup made after the vendor/name selection
	// -----
	// NOTE: 
	// -----
	// At the time there are not other controls which can influence
	// the CDB selection. This means all the configuration data is
	// obtained from the CDB files
	// ---
	// check first if the selection differs from
	// the current settings
	// ---
	bool changeRequest = false;
	QString* toucherName = new QString();
	toucherName -> sprintf ("%s;%s",
		mVendor->currentText().ascii(),mName->currentText().ascii()
	);
	if (! newToucher) {
	if (*toucherName != QString(workingToucher["Name"])) {
		changeRequest = true;
	}
	} else {
		changeRequest = true;
	}
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_TOUCHER"]->cdbGetSpec (
		mVendor->currentText(),mName->currentText()
	);
	if ((pCDB) && (changeRequest)) {
		spec = pCDB -> getHash();
		// ...
		// clear the working toucher contents...
		// ---
		workingToucher.clear();
		// ...
		// set base items for the touchscreen...
		// ---
		QString* ident  = new QString;
		ident -> sprintf ("Mouse[%d]",mWorkingIndex);
		workingToucher.setPair ("InputFashion","Touchpanel");
		workingToucher.setPair ("Identifier",ident->ascii());
		workingToucher.setPair ("Name", toucherName->ascii());
		// ...
		// set CDB data information...
		// ---
		QDictIterator<char> it (*spec);
		for (; it.current(); ++it) {
		QString* key = new QString (it.currentKey());
		QString* val = new QString;
		val->sprintf("%s",it.current());
		if (val->isEmpty()) {
			continue;
		}
		if (*key == "Load") {
			// ...
			// The load key/value pair must go to the module
			// section. Therefore the data record is loaded now
			// ---
			QString* moduleList = new QString();
			XData* workingPath = mFiles["sys_PATH"]->getDevice (0);
			moduleList -> sprintf ("%s,%s",
				workingPath->operator[]("ModuleLoad"),
				val->ascii()
			);
			workingPath -> setPair ("ModuleLoad",
				moduleList->ascii()
			);
		} else {
			workingToucher.setPair ( 
				key->ascii(),val->ascii() 
			);
		}
		}
		// ...
		// check if the card is rotated...
		// ---
		int rotate = isRotated();
		if (rotate > 0) {
			QString* degree = new QString();
			degree->sprintf("%d",rotate);
			workingToucher.setPair (
				"Rotation",degree->ascii()
			);
		}
	}

	// ...
	// add an entry to touchList if a new touchscreen 
	// should be added to the system. Otherwhise update
	// the current entry of the touchList if changed
	// ---
	if (newToucher) {
	if (pCDB) {
		QString topic;
		XData* sysData = mFiles["sys_INPUT"] -> getDevice(mWorkingIndex);
		if (sysData) {
			QDict<char> touchInfo = sysData -> getData();
			topic.sprintf ("%s %s",
				mVendor->currentText().ascii(),mName->currentText().ascii()
			);
			int* newIndex = (int*) malloc(sizeof(int));
			*newIndex = mWorkingIndex;
			mIndexList.append (newIndex);
			touchList -> insertItem (
				QPixmap(PIXINTROTOUCH),topic
			);
			mSelected = mIndexList.find ( newIndex );
			touchList -> setSelected ( mSelected,true );
			touchList -> setCurrentItem ( mSelected );
			mDel      -> setDisabled ( false );
			mSetup    -> setDisabled ( false );
		}
	}
	} else {
	if ((changeRequest) && (pCDB)) {
		QString topic;
		topic.sprintf ("%s %s",
			mVendor->currentText().ascii(),mName->currentText().ascii()
		);
		touchList -> changeItem (
			QPixmap(PIXINTROTOUCH),topic,mSelected
		);
	}
	}
}

//=====================================
// XTouchScreen slotTopCancel...
//-------------------------------------
void XTouchScreen::slotTopCancel (void) {
	// log (L_INFO,"XTouchScreen::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
}

//=====================================
// XTouchScreen slotAdd...
//------------------------------------
void XTouchScreen::slotAdd (void) { 
	// log (L_INFO,"XTouchScreen::slotAdd() called\n");
	// ...
	// this function is called if a new touchscreen 
	// should be added to the graphics system
	// ---
	setupTop (TOUCHER_NEW);
	mTop -> show();
	mTop -> enterEvent ( 0 );
}   
    
//=====================================
// XTouchScreen slotRemove...
//-------------------------------------
void XTouchScreen::slotRemove (void) {
	// log (L_INFO,"XTouchScreen::slotRemove() called\n");
	// ...
	// this function is called if the currently selected 
	// touchscreen should be removed from the graphics system
	// ---
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// restructure the ID tree...
	// ---
	int touchID = *mIndexList.at (mSelected);
	int inputDV = mFiles["sys_INPUT"]->getDeviceCount();
	int count = -1;
	for (int n=0; n < inputDV; n++) {
	count += 2;
	if (count > touchID) {
		mFiles["sys_INPUT"] -> sysMove ( count,count-2 );
	}
	}
	for (int n=0;n < (int)mIndexList.count(); n++) {
	if (n > mSelected) {
		int* index = (int*) malloc ( sizeof(int) );
		*index = *mIndexList.at(n) - 2;
		mIndexList.remove (n);
		mIndexList.insert (n,index);
	}
	}
	// ...
	// remove the device(s) from the data structure
	// the mIndexList and the touchList
	// ---
	mFiles["sys_INPUT"] -> deleteDevice (*mIndexList.at(mSelected));
	touchList -> removeItem (mSelected);
	mIndexList.remove (mSelected);
	// ...
	// select the icon above the removed one, check
	// if the touchList is empty now
	// ---
	mSelected--;
	if (mSelected < 0) {
		mSelected = 0;
	}
	if ((touchList -> count()) == 0) {
		mDel   -> setDisabled ( true );
		mSetup -> setDisabled ( true );
	} else {
		touchList -> setCurrentItem ( mSelected );
		touchList -> setSelected	( mSelected,true );
	}
}

//=====================================
// XTouchScreen slotSetup...
//-------------------------------------
void XTouchScreen::slotSetup (void) {
	// log (L_INFO,"XTouchScreen::slotSetup() called\n");
	// ...
	// this function is called if the currently selected
	// touchscreen should change its configuration setup
	// --- 
	setupTop (TOUCHER_SETUP);
	mTop -> show();
}

//=====================================
// XTouchScreen init function for toplevel
//-------------------------------------
void XTouchScreen::setupTop (int state) {
	// log (L_INFO,"XTouchScreen::setupTop() called: %d\n",state);
	// ...
	// this function is called if the setup toplevel window
	// is created either for add or configure a touchscreen
	// ---
	mState = state;

	// ...
	// add or configure a mouse, save this as a bool
	// value for later handling
	// ---
	bool newToucher;
	switch (mState) {
	case TOUCHER_NEW:
		newToucher = true;
	break;
	case TOUCHER_SETUP:
		newToucher = false;
	break;
	default:
		newToucher = false;
	}

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// setup the current data information if we want to
	// configure a touchscreen otherwhise prepare the dialog for
	// adding a new touchscreen
	// ---
	if (! newToucher) {
	// ...
	// first get the selected device and try to select
	// the touchscreen in the CDB base list
	// ---
	XWrapPointer<XData> workingToucher (
		mFiles["sys_INPUT"]->getDevice (*mIndexList.at(mSelected))
	);
	// select vendor...
	mVendor -> clearSelection();
	mName   -> clear();
	XStringList name (workingToucher["Name"]);
	name.setSeperator (";");
	QList<char> nameList = name.getList();
	for (int i=0;i < (int)mVendor->count();i++) {
	if (mVendor -> text(i) == QString(nameList.at(0))) {
		mVendor -> setSelected (i,true);
		slotVendor (mVendor -> item(i));
		break;
	}
	}
	// select name...
	for (int i=0;i < (int)mName->count();i++) {
	if (mName -> text(i) == QString(nameList.at(1))) {
		mName -> setSelected (i,true);
		break;
	}
	}

	} else {
	// ...
	// prepare dialog to add a new touchscreen...
	// ---
	// clear CDB vendor selection and clear the
	// name listbox contents
	// ---
	mVendor  -> clearSelection();
	mName    -> clear();
	}
}

//=====================================
// XTouchScreen slotSelect...
//-------------------------------------
void XTouchScreen::slotSelect (QListBoxItem* item) {
	// log (L_INFO,"XTouchScreen::slotSelect() called\n");
	// ...
	// this function is called if you select a touchscreen from the
	// main touchscreen listbox which hold all the touchers
	// ---
	if (touchList->isSelected (item)) {
		mSelected = touchList -> currentItem();
	}
}

//=====================================
// XTouchScreen check rotate state
//-------------------------------------
int XTouchScreen::isRotated ( void ) {
	// log (L_INFO,"XTouchScreen::isRotated() called\n");
	// ...
	// this function is called to check the card section
	// if it is rotated. If yes return the appropriate
	// degree count or 0 if not rotated
	// ---
	XData* data;
	QDict<char> card;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		data = mFiles["sys_CARD"] -> getDevice(n);
		if (! data) {
			continue;
		}
		card = data -> getData();
		if (QString(card["Rotate"]) == "CW") {
		//========================================
		// CW -> 90 degrees for Touchpanel
		//----------------------------------------
		return (90);
		} else
		if (QString(card["Rotate"]) == "CCW") {
		//========================================
		// CCW -> 270 degrees for Touchpanel
		//----------------------------------------
		return (270);
		} else {
		//========================================
		// No rotation
		//----------------------------------------
		return (0);
		}
	}
	return (0);
}

