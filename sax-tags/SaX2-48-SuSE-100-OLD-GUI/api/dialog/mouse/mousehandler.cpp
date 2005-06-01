/**************
FILE          : action.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure real mouse devices including scroll
              : wheels and multiple button mice
              :
              : --> signal action slots/callbacks
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "mouse.h"

//=====================================
// XMouse reset and switch to Intro
//-------------------------------------
void XMouse::resetPage (int reload) {
	// log (L_INFO,"XMouse::resetPage() called\n");
	// ...
	// this function is called if the mouse dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
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
	} 
	mIntro -> importFile (update);
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XMouse internal page called...
//-------------------------------------
void XMouse::pageCalled (int index) {
	// log (L_INFO,"XMouse::pageCalled() called\n");
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case MousePage1: {
		break;
	}
	};
}

//=====================================
// XMouse slotAdd...
//------------------------------------
void XMouse::slotAdd (void) {
	// log (L_INFO,"XMouse::slotAdd() called\n");
	// ...
	// this function is called if a new mouse should be added to the
	// graphics system
	// ---
	setupTop (MOUSE_NEW);
	mTop->show();
	mTop->enterEvent ( 0 );
}

//=====================================
// XMouse slotRemove...
//-------------------------------------
void XMouse::slotRemove (void) {
	// log (L_INFO,"XMouse::slotRemove() called\n");
	// ...
	// this function is called if the currently selected mouse
	// should be removed from the graphics system
	// ---
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// restructure the ID tree...
	// ---
	int mice;
	mice = mFiles["sys_INPUT"]->getDeviceCount();
	int count = -1;
	for (int n=0; n<mice; n++) {
	count += 2;
	if (count > *mIndexList.at (mSelected)) {
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
	// the mIndexList and the mouseList
    // ---
	mFiles["sys_INPUT"] -> deleteDevice (*mIndexList.at(mSelected));
	mouseList -> removeItem (mSelected);
	mIndexList.remove (mSelected); 
	// ...
	// select the icon above the removed one, check
	// if the mouseList is empty now
	// ---
	mSelected--;
	if (mSelected < 0) {
		mSelected = 0;
	}
	if ((mouseList -> count()) == 0) {
		mDel   -> setDisabled ( true );
		mSetup -> setDisabled ( true );
	} else {
		mouseList -> setCurrentItem ( mSelected );
		mouseList -> setSelected    ( mSelected,true );
	}
}

//=====================================
// XMouse slotSetup...
//-------------------------------------
void XMouse::slotSetup (void) {
	// log (L_INFO,"XMouse::slotSetup() called\n");
	// ...
	// this function is called if the currently selected mouse
	// should change its configuration setup
	// --- 
	setupTop (MOUSE_SETUP);
	mTop->show();
}

//=====================================
// XMouse slotSelect...
//-------------------------------------
void XMouse::slotSelect (QListBoxItem* item) {
	// log (L_INFO,"XMouse::slotSelect() called\n");
	// ...
	// this function is called if you select a mouse from the
	// main mouse listbox which hold all the mouse devices
	// ---
	if (mouseList->isSelected (item)) {
		mSelected = mouseList -> currentItem();
	} 
}

//=====================================
// XMouse slotVendor...
//-------------------------------------
void XMouse::slotVendor (QListBoxItem* item) {
	// log (L_INFO,"XMouse::slotVendor() called\n");
	// ...
	// fill the name listbox with the devices
	// apropriate to the selected mouse vendor
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	if (! mVendor->isSelected (item)) {
		return;
	}
	mName -> clear();
	QString selectedVendor = mVendor -> currentText();
	QList<char> names = mFiles["cdb_POINTER"]->cdbGetNameList (selectedVendor);
	QListIterator<char> it (names);
	for (; it.current(); ++it) {
		mName -> insertItem (it.current());
	}
	mName -> sort();
}

//=====================================
// XMouse slotName...
//-------------------------------------
void XMouse::slotName (QListBoxItem*) {
	// log (L_INFO,"XMouse::slotName() called\n");
	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the selection data from the CDB
	// ---
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_POINTER"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
		spec = pCDB -> getHash();
	}

	// ...
	// create pointer wrapper around the specifiaction
	// pointer for the CDB mouse information hash
	// ---
	XWrapPointer< QDict<char> > selectedMouse ( spec );

	// ...
	// set the default port used for the selected
	// mouse device
	// ---
	bool hasPort = false;
	int  defaultIndex = 0;
	for (int i=0;i<mPort -> count();i++) {
	if (mPort -> text(i) == "/dev/mouse") {
		defaultIndex = i;
	}
	if (mPort -> text(i) == selectedMouse["Device"]) {
		mPort -> setCurrentItem (i);
		hasPort = true;
		break;
	}
	} 
	if (! hasPort) {
		mPort -> setCurrentItem (defaultIndex);
	}
	// ...
	// set the Emulate3Button Wheel functions and emulate
	// wheel functions feature
	// ---
	mCheckWheelEmu -> setChecked (false);
	mCheckWheel    -> setChecked (false);
	mCheckEmulate  -> setChecked (false);
	if (QString(selectedMouse["Emulate3Buttons"]) == "on") {
		mCheckEmulate -> setChecked (true);
	}
	if (selectedMouse["ZAxisMapping"]) {
		mCheckWheel -> setChecked (true);
	}
	if (selectedMouse["Buttons"]) {
	QString mouseButtons (selectedMouse["Buttons"]);
	int buttons = mouseButtons.toInt();
	if ((! selectedMouse["ZAxisMapping"]) && (buttons >= 4)) {
		mCheckWheelEmu -> setChecked (true);
	}
	}
	if (selectedMouse["EmulateWheel"]) {
		mCheckWheelEmu -> setChecked (true);
		mWheelKey -> setValue (selectedMouse["EmulateWheel"].toInt());
	}
}

//=====================================
// XMouse slotPort...
//-------------------------------------
void XMouse::slotPort (int) {
	// log (L_INFO,"XMouse::slotPort() called\n");
	// ...
	// this function is called if the mouse port has
	// changed its setup
	// ---
}

//=====================================
// XMouse slotWheelKey...
//-------------------------------------
void XMouse::slotWheelKey (int state) {
	// ...
	// this function is called if the check box value for 
	// the mouse wheel emulation has changed its value.
	// We will enable/disable the mWheelKey SpinBox than
	// ---
	switch (state) {
	case 0: 
		mWheelKey -> setDisabled ( TRUE );
	break;
	case 2: 
		mWheelKey -> setDisabled ( FALSE );
	break;
	}
}

//=====================================
// XMouse slotTopOk...
//-------------------------------------
void XMouse::slotTopOk (void) {
	// log (L_INFO,"XMouse::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
	bool newMouse = false;
	switch (mState) {
	case MOUSE_NEW:
		if (mouseList->count() > 0) {
			mWorkingIndex = *mIndexList.at(mouseList->count()-1);
			mWorkingIndex+= 2;
		} else {
			mWorkingIndex = 1;
		}
		newMouse = true;
	break;
	case MOUSE_SETUP:
		mWorkingIndex = *mIndexList.at(mSelected);
		newMouse = false;
	break; 
	}

	// ...
	// create the text pointer wrapper object and
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// get the workingMouse pointer wrapper. If a new mouse
	// should be added use addDevice to create a new object
	// otherwhise get the device according to the selection 
	// made in mouseList and clear the contents
	// ---
	XWrapPointer<XData> workingMouse;
	if (newMouse) {
		while (mFiles["sys_INPUT"]->getDevice (mWorkingIndex)) {
			mWorkingIndex+= 2;
		}
		workingMouse.init (
		mFiles["sys_INPUT"]->addDevice (mWorkingIndex)
		);
	} else {
		workingMouse.init (
		mFiles["sys_INPUT"]->getDevice (mWorkingIndex)
		);
	}

	// ...
	// set the selection data from the CDB if the user requested 
	// a new mouse from the list. The data set here may be overwritten
	// from the manual setup made after the vendor/name selection
	// ---
	// check first if the selection differs from
	// the current settings
	// ---
	bool changeRequest = false;
	QString* mouseName = new QString();
	mouseName -> sprintf ("%s;%s",
		mVendor->currentText().ascii(),mName->currentText().ascii()
	);
	if (! newMouse) {
	if (*mouseName != QString(workingMouse["Name"])) {
		changeRequest = true;
	}
	} else {
		changeRequest = true;
	}
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_POINTER"]->cdbGetSpec (
		mVendor->currentText(),mName->currentText()
	);
	if ((pCDB) && (changeRequest)) {
		spec = pCDB -> getHash();
		// ...
		// clear the working mouse contents...
		// ---
		workingMouse.clear();
		// ...
		// set base items for the mouse...
		// ---
		QString* ident  = new QString;
		ident -> sprintf ("Mouse[%d]",mWorkingIndex);
		workingMouse.setPair ("InputFashion","Mouse");
		workingMouse.setPair ("Identifier",ident->ascii());
		workingMouse.setPair ("Name", mouseName->ascii());
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
		workingMouse.setPair (
			key->ascii(),val->ascii()
		);
		}
	}

	// ...
	// handle manual setup parameters outside the CDB vendor/name
	// selection: Port setup and special features like
	// Wheel, WheelEmulation and ButtonEmulation
	// ---
	// Port
	QString* currentPort = new QString (mPort -> currentText());
	workingMouse.setPair ("Device",
		currentPort->ascii()
	);
	// Emulate3Buttons
	workingMouse.setPair ("Emulate3Buttons","no");
	if (mCheckEmulate -> isChecked()) {
		workingMouse.setPair ("Emulate3Buttons","on");
	}
	// Wheel
	workingMouse.setPair ("ZAxisMapping","off");
	if (mCheckWheel -> isChecked()) {
		workingMouse.setPair (
			"ZAxisMapping","4 5"
		);
		if ( spec ) {
		XWrapPointer< QDict<char> > selectedMouse ( spec );
		if (selectedMouse["ZAxisMapping"]) {
			QString* zmap = new QString(
				selectedMouse["ZAxisMapping"]
			);
			workingMouse.setPair (
				"ZAxisMapping",zmap->ascii()
			);
		} 
		}
	}
	// Emulate Wheel
	workingMouse.setPair ("EmulateWheel","no");
	if (mCheckWheelEmu -> isChecked()) {
		QString* button = new QString(mWheelKey->text());
		workingMouse.setPair ("EmulateWheel",button->ascii());
	}
	
	// ...
	// add an entry to mouseList if a new mouse 
	// should be added to the system. Otherwhise update
	// the current entry of the mouseList if changed
	// ---
	if (newMouse) {
	if (pCDB) {
		QString topic;
		XData* sysData = mFiles["sys_INPUT"] -> getDevice(mWorkingIndex);
		if (sysData) {
			QDict<char> mouseInfo = sysData -> getData();
			QTextOStream (&topic)
				<< mVendor->currentText() << " " << mName->currentText()
				<< " ( " << mouseInfo["Protocol"] << " " << mText["at"] << " "
				<< mouseInfo["Device"] << " )";
			int* newIndex = (int*) malloc(sizeof(int));
			*newIndex = mWorkingIndex;
			mIndexList.append (newIndex);
			mouseList -> insertItem (
				QPixmap(PIXINTROMOUSE),topic
			);
			mSelected = mIndexList.find ( newIndex );
			mouseList -> setSelected ( mSelected,true );
			mouseList -> setCurrentItem ( mSelected );
			mDel      -> setDisabled ( false );
			mSetup    -> setDisabled ( false );
		}
	}
	} else {
		QString topic;
		if (pCDB) {
			QTextOStream (&topic)
				<< mVendor->currentText() << " " << mName->currentText()
				<< " ( " << workingMouse["Protocol"] << " " << mText["at"] 
				<< " " << workingMouse["Device"] << " )";
		} else {
			QTextOStream (&topic)
				<< workingMouse["Name"]
				<< " ( " << workingMouse["Protocol"] << " " << mText["at"]
				<< " " << workingMouse["Device"] << " )";
		}
		mouseList -> changeItem (
			QPixmap(PIXINTROMOUSE),topic,mSelected
		);
	}
}

//=====================================
// XMouse slotTopCancel...
//-------------------------------------
void XMouse::slotTopCancel (void) {
	// log (L_INFO,"XMouse::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
}

//=====================================
// XMouse init function for toplevel
//-------------------------------------
void XMouse::setupTop (int state) {
	// log (L_INFO,"XMouse::setupTop() called: %d\n",state);
	// ...
	// this function is called if the setup toplevel window
	// is created either for add or configure a mouse
	// ---
	mState = state;

	// ...
	// add or configure a mouse, save this as a bool
	// value for later handling
	// ---
	bool newMouse;
	switch (mState) {
	case MOUSE_NEW:
		newMouse = true;
	break;
	case MOUSE_SETUP:
		newMouse = false;
	break;
	default:
		newMouse = false;
	}

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// setup the current data information if we want to
	// configure a mouse otherwhise prepare the dialog for
	// adding a new mouse
	// ---
	if (! newMouse) {
	// ...
	// first get the selected device and try to select
	// the mouse in the CDB base list
	// ---
	XWrapPointer<XData> workingMouse (
		mFiles["sys_INPUT"]->getDevice (*mIndexList.at(mSelected))
	);
	// select vendor...
	mVendor -> clearSelection();
	mName   -> clear();
	XStringList name (workingMouse["Name"]);
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
	// ...
	// setup connection port for this mouse
	// ---
	bool hasPort = false;
	int  defaultIndex = 0;
	for (int i=0;i<mPort -> count();i++) {
	if (mPort -> text(i) == "/dev/mouse") {
		defaultIndex = i;
	}
	if (mPort -> text(i) == workingMouse["Device"]) {
		mPort -> setCurrentItem (i);
		hasPort = true;
		break;
	}
	}
	if (! hasPort) {
		mPort -> setCurrentItem (defaultIndex);
	}
	// ...
	// setup enhanced features for this mouse
	// Wheel, WheelEmulation and ButtonEmulation
	// ---
	mCheckWheelEmu -> setChecked (false);
	mCheckWheel    -> setChecked (false);
	mCheckEmulate  -> setChecked (false);
	if ((workingMouse["Emulate3Buttons"]) &&
		(QString(workingMouse["Emulate3Buttons"]) == "on")
	) {
		mCheckEmulate -> setChecked (true);
	}
	if ((workingMouse["ZAxisMapping"]) &&
		(QString(workingMouse["ZAxisMapping"]) != "off")
	) {
		mCheckWheel -> setChecked (true);
	}
	if ((workingMouse["EmulateWheel"]) &&
		(QString(workingMouse["EmulateWheel"]) != "no")
	) {
		QString button ( workingMouse["EmulateWheel"] );
		mCheckWheelEmu -> setChecked ( true );
		mWheelKey -> setValue ( button.toInt() );
	}
	
	} else {

	// ...
	// prepare dialog to add a new mouse...
	// ---
	// clear CDB vendor selection and clear the
	// name listbox contents
	// ---
	mVendor  -> clearSelection();
	mName    -> clear();
	// ...
	// set the port to /dev/mouse
	// ---
	for (int i=0;i<mPort -> count();i++) {
	if (mPort -> text(i) == "/dev/mouse") {
		mPort -> setCurrentItem (i);
		break;
	}
	}
	// ...
	// disable all expert features: Wheel
	// WheelEmulation and ButtonEmulation
	// ---
	mCheckWheelEmu -> setChecked (false);
	mCheckWheel    -> setChecked (false);
	mCheckEmulate  -> setChecked (false);

	// ...
	// show the vendor/name selection page
	// ---
	mTop -> showPage (layer6);
	}
}

//=====================================
// XMouse init page with data infos 
//-------------------------------------
void XMouse::initPage (void) {
	// log (L_INFO,"XMouse::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert mouse vendors...
	// ------------------------
	QList<char> types = mFiles["cdb_POINTER"]->cdbGetVendorList();
	QListIterator<char> it (types);
	for (; it.current(); ++it) {
		mVendor -> insertItem (it.current());
	}

	// insert port device names...
	// ---------------------------
	XApiData port = mFiles["std_PDEVICE"]->getAPI();
	QList<char>* plist = port.getList();
	QListIterator<char> io (*plist);
	for (; io.current(); ++io) {
		mPort->insertItem (io.current());	
	}
}

//=====================================
// XMouse virtual slots...
//-------------------------------------
bool XMouse::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XMouse::slotRun() called: %d\n",index);
	// ...
	// this function is called if the mouse page is activated.
	// use this function to init the dialog with the current
	// setup of the mice
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);	
	mStatus -> message (mText["RunXMouse"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// fill the mouseList listbox with the 
	// configured simple pointer devices
	// --- 
	mouseList -> clear();
	mIndexList.clear();
	QString topic;
	XData* sysData;
	QDict<char> mouseInfo;
	int mice;
	mice = mFiles["sys_INPUT"]->getDeviceCount();
	int count = -1;
	for (int n=0; n<mice; n++) {
		count += 2;
		sysData = mFiles["sys_INPUT"] -> getDevice(count);
		if (! sysData) {
		continue;
		}
		mouseInfo = sysData -> getData();
		if (QString(mouseInfo["InputFashion"]).isNull()) {
			mouseInfo.insert ("InputFashion","Mouse");
		}
		if (QString(mouseInfo["InputFashion"]) != "Mouse") {
		continue;
		}
		QString topic (mouseInfo["Name"]);
		XStringList name (mouseInfo["Name"]);
		name.setSeperator (";");
		QList<char> mouseName = name.getList();
		if (mouseName.count() == 2) {
			topic.sprintf("%s %s",mouseName.at(0),mouseName.at(1));
		}
		QString topicStream;
		QTextOStream (&topicStream)
			<< topic << " ( " << mouseInfo["Protocol"] << " "
			<< mText["at"] << " " << mouseInfo["Device"] << " )";
		topic = topicStream;
		int* index = (int*) malloc (sizeof(int));
		*index = count;
		mIndexList.append (index);
		mouseList -> insertItem (
			QPixmap(PIXINTROMOUSE),topic
		);
	}
	// ...
	// enable/disable configure/remove buttons
	// and select first element of the listbox
	// ---
	if (mice > 0) {
		mDel   -> setDisabled ( false );
		mSetup -> setDisabled ( false );
		mouseList -> setCurrentItem ( 0 );
		mouseList -> setSelected    ( 0,true );
		mSelected = 0;
	} else {
		mDel   -> setDisabled ( true );
		mSetup -> setDisabled ( true );
	}
	}
	return (TRUE);
}
