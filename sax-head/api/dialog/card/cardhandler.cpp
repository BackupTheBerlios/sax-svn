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
              : configure the graphics card properties
              : you may configure more than one card to be
              : able for managing multiple desktops
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "card.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;
extern int global3DActive;

//=====================================
// XCard reset and switch to Intro
//-------------------------------------
void XCard::resetPage (int reload) {
	// ...
	// this function is called if the card dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_CARD";
	if (reload == PAGE_RELOAD) {
		if (! validatePage()) {
			return;
		}
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
		update = "sys_DESKTOP";
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
		update = "sys_INPUT";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
	}
	mIntro -> importFile (update);

	// ...
	// check if we need to call resetPage() of the
	// desktop dialog. This is only needed if somebody
	// activated the framebuffer configuration
	// ---
	if (reload == PAGE_RELOAD) {
		XDesktop* desktopDialog;
		desktopDialog = (XDesktop*) mIntro -> retrieve (Colors);
		desktopDialog -> resetPage ( reload );
	}

	// ...
	// Update this page now
	// ---
	slotIntro (mIndex);
    XTemplate::resetPage ();
    mStack -> raiseWidget (Intro);
}

//=====================================
// XCard internal page called...
//-------------------------------------
void XCard::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case CardPage1: {
		break;
	}
	};
}

//=====================================
// XCard init page with data infos 
//-------------------------------------
void XCard::initPage (void) {
	// log (L_INFO,"XCard::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert card vendors...
	// -------------------------
	QList<char> vendors = mFiles["cdb_CARDS"]->cdbGetVendorList();
	QListIterator<char> it (vendors);
	for (; it.current(); ++it) {
		mVendor->insertItem (it.current());
	}
	mVendor -> sort();
	
	// insert busID`s of all cards...
	// ------------------------------
	char* data = NULL;
	char* line = NULL;
	QString* result;
	mBusID->insertItem ("Single");
	QString ids = qx(GETBUS,STDOUT);
	data = (char*) malloc (ids.length() + 1);
	sprintf (data,"%s",ids.ascii());

	line = strtok (data,"\n");
	while (1) {
		if (! line) {
		break;
		}
		result = new QString (line);
		mBusID->insertItem (*result);
		line = strtok (NULL,"\n");
	}

	// set rotate to noRotate...
	// -------------------------
	mRotateNone -> setChecked (true);
}

//=====================================
// XCard slotRun...
//-------------------------------------
bool XCard::slotRun (int index) {
	bool isValid = true;
	if (mShowSetupWindow) {
		isValid = XTemplate::slotRun (index);
	}
	if (isValid) {
	// log(L_INFO,"XCard::slotRun() called: %d\n",index);
	// ...
	// this function is called if the card page is activated.
	// use this function to init the dialog with the current
	// setup of the card(s)
	// ---
	m3DStateChanged = false;

	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (mShowSetupWindow) {
		mStatus -> message (mText["RunXCard"]);
		mFrame  -> nextButton() -> setText (mText["finish"]);
	}

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// fill the mCardList listbox with the 
	// configured cards
	// ---
	mCardList -> clear();
	QString topic;
	XData* sysData;
	QDict<char> cardInfo;
	int cards;
	cards = mFiles["sys_CARD"]->getDeviceCount();
	for (int n=0; n<=cards; n++) {
		sysData = mFiles["sys_CARD"] -> getDevice(n);
		if (! sysData) {
		continue;
		}
		cardInfo = sysData -> getData();
		topic.sprintf ("%s %s",cardInfo["Vendor"],cardInfo["Name"]);
		mCardList -> insertItem (QPixmap(PIXCARD),topic);
	}
	// ...
	// enable/disable configure/remove buttons
	// and select first element of the listbox
	// ---
	if (cards > 0) {
		mDel   -> setDisabled ( false );
		mSetup -> setDisabled ( false );
		mCardList -> setCurrentItem ( 0 );
		mCardList -> setSelected    ( 0,true );
		mSelected = 0;
	} else {
		mDel   -> setDisabled ( true );
		mSetup -> setDisabled ( true );
	}

	// ...
	// lookup current Module list in  sys_PATH
	// and save the list in mModuleList
	// ---
	XWrapPointer<XData> path ( mFiles["sys_PATH"]->getDevice (0) );
	mModuleList -> sprintf ("%s",path["ModuleLoad"].ascii());

	// ...
	// create the current layout structure
	// ---
	XLayout* layoutDialog;
	layoutDialog = (XLayout*) mIntro -> retrieve (Layout);
	layoutDialog -> setupLayout();
	}
	return (TRUE);
}

//=====================================
// XCard init function for toplevel
//-------------------------------------
void XCard::setupTop (int state) {
	// log (L_INFO,"XCard::setupTop() called: %d\n",state);
	// ...
	// this function is called if the setup toplevel window
	// is created either for add or configure a card
	// ---
	mState = state;

	// ...
	// save current value of m3D to be restored if
	// the user canceled the dialog
	// ---
	m3Dorig = new QString (*m3D);

	// ...
	// add or configure a card, save this as a bool
	// value for later switches
	// ---
	bool newCard;
	switch (mState) {
	case CARD_NEW:
		newCard = true;
	break;
	case CARD_SETUP:
		newCard = false;
	break;
	default:
		newCard = false;
	}

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// ...
	// setup the current data information if we want to
	// configure a card otherwhise prepare the dialog for
	// adding a new card
	// ---
	if (! newCard) {
	// ...
	// first get the selected device and try to select
	// the card in the CDB base list
	// ---
	XWrapPointer<XData> workingCard (
		mFiles["sys_CARD"]->getDevice (mSelected)
	);
	// select vendor...
	mVendor -> clearSelection();
	mName   -> clear();
	for (int i=0;i < (int)mVendor->count();i++) {
	if (mVendor -> text(i) == QString(workingCard["Vendor"])) {
		mVendor -> setSelected (i,true);
		slotVendor (mVendor -> item(i));
		break;
	}
	}
	// select name...
	for (int i=0;i < (int)mName->count();i++) {
	if (mName -> text(i) == QString(workingCard["Name"])) {
		mName -> setSelected (i,true);
		break;
	}
	}
	// ...
	// setup BusID for this card
	// ---
	bool found    = false;
	int  singleID = 0;
	for (int i=0;i < (int)mBusID->count();i++) {
	if (mBusID -> text(i) == "Single") {
		singleID = i;
	}
	if (mBusID -> text(i) == QString(workingCard["BusID"])) {
		mBusID -> setCurrentItem (i);
		found = true;
		break;
	}
	}
	if (! found) {
		mBusID -> setCurrentItem (singleID);
	}
	// ...
	// setup Screen ID for this card
	// ---
	mScrID -> setCurrentItem (0);
	if (workingCard["Screen"]) {
		int sid = QString(workingCard["Screen"]).toInt();
		mScrID -> setCurrentItem (sid);
	}

	// ===================================================
	// 3D setup algorithm...
	// ---------------------------------------------------
	// The initial state of the 3D checkbox is enabled and
	// not checked. Next is to find out if the currently selected
	// card is capable of using 3D. If the global3DActive variable 
	// is true we will set the 3D box as checked. global3DActive conforms
	// to the exit code of 3Ddiag-result and therefore indicates
	// the currently status of 3D used with this system.
	// ---
	// Now device between a single card or a multihead system.
	// If more than one card is used we will disable the checkbox
	// because 3D will not work in multihead systems. If the checkbox
	// is checked at this point we will show a message about the
	// fact and uncheck the checkbox then. If only a single card
	// is used we had to check if this card is capable of 3D.
	// ---
	mCheck3D -> setChecked  ( false );
	mCheck3D -> setDisabled ( false );
	// ...
	// Is the currently selected card
	// capable to use the 3D feature(s)
	// ---
	bool D3Capable = false;
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_CARDS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
	spec = pCDB -> getHash();
	XWrapPointer< QDict<char> > selectedCard ( spec );
	QString flag (selectedCard["Flag"]);
	if (flag == "3D") {
		D3Capable = true;
	}
	}
	// ...
	// set currently active 3D state
	// ---
	if (global3DActive) {
		mCheck3D -> setChecked  ( true );
	}
	// ...
	// handle 3D in multihead environments or check
	// for the 3D capability of a single card
	// ---
	if (mCardList->count() > 1) {
		mCheck3D -> setDisabled ( true );
		if (global3DActive) {
			mCheck3D -> setChecked  ( false );
			// setMessage ("no3DinMulti");
		}
	} else {
		if (! D3Capable) {
			mCheck3D -> setChecked  ( false );
			mCheck3D -> setDisabled ( true );
		}
	}
	// ...
	// setup options for this card
	// ---
	mRotate -> setDisabled (true);
	XApiData modOptions = mFiles["ext_CMODULES"]->getAPI();
	QString options ( modOptions[workingCard["Driver"]] );
	mOptions -> clear ();
	XStringList cur;
	if (! options.isEmpty()) {
		mOptFlags.clear();
		cur.setText (options);
		cur.setSeperator (" ");
		QList<char> strlist = cur.getList();
		QListIterator<char> it (strlist);
		for (; it.current(); ++it) {
			XStringList opt;
			opt.setText (it.current());
			opt.setSeperator (":");
			QList<char> optlist = opt.getList();
			if (QString(optlist.at(0)) == "Rotate") {
				mRotate -> setDisabled (false);
			} else {
				mOptFlags.insert (optlist.at(0),optlist.at(1));
				mOptions -> insertItem (optlist.at(0));
			}
		}
	}
	mOptions -> sort();
	if (workingCard["Option"]) {
		cur.setText (workingCard["Option"]);
		cur.setSeperator (",");
		QList<char> strlist = cur.getList();
		QListIterator<char> it (strlist);
		for (; it.current(); ++it) {
			QListBoxItem* item = mOptions -> findItem (it.current());
			if (! item) {
				mOptions -> insertItem     (it.current());
				mOptions -> setSelected    (mOptions->count() - 1,true);
				mOptions -> setCurrentItem (mOptions->count() - 1);
			} else {
				mOptions -> setSelected    (item,true);
				mOptions -> setCurrentItem (item);
			}
		}
		mOptions -> sort();
		mOptions -> ensureCurrentVisible();
	}
	mHiddenRawOptions.clear();
	if (workingCard["RawData"]) {
		QString rawopt = qx (
			GETRAWLIST,STDOUT,1,"%s",workingCard["RawData"].ascii()
		);
		cur.setText (rawopt);
		cur.setSeperator ("|");
		QList<char> strlist = cur.getList();
		QListIterator<char> it (strlist);
		for (; it.current(); ++it) {
			int count = 0;
			QList<char> cleanOptionList;
			QStringList optlist = QStringList::split ( "\"", it.current() );
			for ( QStringList::Iterator 
				in = optlist.begin(); in != optlist.end(); ++in 
			) {
				QString result (*in);
				result = result.stripWhiteSpace();
				if (result.isEmpty()) {
					continue;
				}
				if (result == "Option") {
					count++; continue;
				}
				if ((count == 0) && (result != "Option")) {
					mHiddenRawOptions.append (it.current());
				}
				cleanOptionList.append (result.ascii());
				count++;
			}
			if (cleanOptionList.count() == 2) {
			QString option;
			option.sprintf("%s --> %s",
				cleanOptionList.at(0),cleanOptionList.at(1)
			);
			QListBoxItem* item = mOptions -> findItem (
				cleanOptionList.at(0)
			);
			if (! item) {
				mOptions -> insertItem (cleanOptionList.at(0));
				mOptFlags.insert (cleanOptionList.at(0),"any");
				item = mOptions -> findItem (cleanOptionList.at(0));
			}
			mOptions -> setCurrentItem (item);
			mOptions -> changeItem (
				option,mOptions->currentItem()
			);
			mOptions -> setSelected (
				mOptions->currentItem(),true
			);
			}
		}
	}

	// ...
	// setup rotate state for this card
	// ---
	mRotateNone -> setChecked (true);
	if (workingCard["Rotate"]) {
		if (QString(workingCard["Rotate"]) == "CW") {
		mRotateRight -> setChecked (true);
		}
		if (QString(workingCard["Rotate"]) == "CCW") {
		mRotateLeft -> setChecked (true);
		}
	}
	} else {

	// ...
	// prepare dialog to add a new card...
	// ---
	// clear CDB vendor selection and clear the
	// name listbox contents
	// ---
	mVendor  -> clearSelection();
	mName    -> clear(); 
	// ...
	// clear options combo box and set the BusID
	// to Single and the Screen ID to primary screen
	// ---
	mOptions -> clear(); 
	for (int i=0;i < (int)mBusID->count();i++) {
	if (mBusID -> text(i) == "Single") {
		mBusID -> setCurrentItem (i);
		break;
	}
	}
	// ...
	// Set Screen ID to primary screen
	// ---
	mScrID -> setCurrentItem (0);
	// ...
	// disable the 3D feature by default
	// ---
	mCheck3D -> setChecked  (false);
	mCheck3D -> setDisabled (true);

	// ...
	// set rotate state to noRotate
	// ---
	mRotateNone -> setChecked (true);
	mTop -> showPage (layer6);
	}

	// ...
	// save 3D checkbox status
	// ---
	mCheckBoxStatus = mCheck3D -> isChecked();
}

//=====================================
// XCard slotAdd...
//------------------------------------
void XCard::slotAdd (void) {
	// log (L_INFO,"XCard::slotAdd() called\n");
	// ...
	// this function is called if a new card should be added to the
	// graphics system
	// ---
	setupTop (CARD_NEW);
	if (mShowSetupWindow) {
		mTop->show();
		mTop->enterEvent ( 0 );
	}
}

//=====================================
// XCard slotRemove...
//-------------------------------------
void XCard::slotRemove (void) {
	// log (L_INFO,"XCard::slotRemove() called\n");
	// ...
	// this function is called if the currently selected card
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
	int cards;
	cards = mFiles["sys_CARD"]->getDeviceCount();
	for (int n=0; n<=cards; n++) {
	if (n > mSelected) {
		mFiles["sys_CARD"]    -> sysMove ( n,n-1 );
		mFiles["sys_DESKTOP"] -> sysMove ( n,n-1 );
	}
	}

	// ...
	// remove the device(s) from the data structure
	// ---
	mFiles["sys_CARD"]    -> deleteDevice (mSelected);
	mFiles["sys_DESKTOP"] -> deleteDevice (mSelected);

	// remove the device from the mCardList
	// and the mDesktopList
	// ---
	XDesktop* desktopDialog;
	desktopDialog = (XDesktop*) mIntro -> retrieve (Colors);
	QListBox* mDesktopList = desktopDialog -> getList ();
	mCardList    -> removeItem (mSelected);
	mDesktopList -> removeItem (mSelected);

	// ...
	// select the icon above the removed one, check
	// if the CardList is empty now
	// ---
	mSelected--;
	if (mSelected < 0) {
		mSelected = 0;
	}
	if ((mCardList -> count()) == 0) {
		mDel   -> setDisabled ( true );
		mSetup -> setDisabled ( true );
	} else { 
		mCardList -> setCurrentItem ( mSelected );
		mCardList -> setSelected    ( mSelected,true );
	}

	// ...
	// Adapt the server layout according to the
	// new card situation. Get the layout dialog and the
	// according matrix pointer first
	// ---
	XLayout* layoutDialog;
	layoutDialog = (XLayout*) mIntro -> retrieve (Layout);
	XPlot* mMatrix = layoutDialog -> getMatrix();
	// ...
	// now remove the widget, this will automatically
	// trigger the layout validation
	// ---
	mMatrix -> removeWidget (mSelected);
}

//=====================================
// XCard slotSetup...
//-------------------------------------
void XCard::slotSetup (void) {
	// log (L_INFO,"XCard::slotSetup() called\n");
	// ...
	// this function is called if the currently selected card
	// should change its configuration setup
	// --- 
	setupTop (CARD_SETUP);
	if (mShowSetupWindow) {
		mTop->show();
		mTop->enterEvent ( 0 );
	}
}

//=====================================
// XCard slotSelect...
//-------------------------------------
void XCard::slotSelect (QListBoxItem* item) {
	// log (L_INFO,"XCard::slotSelect() called\n");
	// ...
	// this function is called if you select a card from the
	// main card listbox which hold all the mouse devices
	// ---
	if (mCardList->isSelected (item)) {
		mSelected = mCardList -> currentItem();
	}
}

//=====================================
// XCard slotTopOk...
//-------------------------------------
void XCard::slotTopOk (void) {
	// log (L_INFO,"XCard::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
	// first get the working index according to the
	// fact of adding or configuring a card
	// ---
	mFrame -> enterEvent ( 0 );
	XWrapPointer< QDict<char> > mText (mTextPtr);
	bool newCard;
	newCard = false;
	switch (mState) {
	case CARD_NEW:
		mWorkingIndex = mCardList->count();
		newCard = true;
	break;
	case CARD_SETUP:
		mWorkingIndex = mSelected;
		newCard = false;
	break;
	}

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
    
	// ...
	// get the workingCard pointer wrapper. If a new card
	// should be added use addDevice to create a new object
	// otherwhise get the device according to the selection 
	// made in mCardList and clear the contents
	// ---
	QString* saveBusID = new QString ("Single");
	XWrapPointer<XData> workingCard;
	XWrapPointer<XData> workingDesktop;
	if (newCard) {
		workingCard.init (
		mFiles["sys_CARD"]->addDevice (mWorkingIndex)
		);
		workingDesktop.init (
		mFiles["sys_DESKTOP"]->addDevice (mWorkingIndex)
		);
	} else {
		workingCard.init (
		mFiles["sys_CARD"]->getDevice (mWorkingIndex)
		);
	}

	// ...
	// lookup current 3D checkbox status
	// ---
	if (mCheckBoxStatus != (mCheck3D->isChecked())) {
		m3DStateChanged = true;
	} else {
		m3DStateChanged = false;
	}

	// ...
	// set the selection data from the CDB if the
	// user requested a new card from the list
	// ---
	// check first if the selection differs from
	// the current settings or the 3D state has changed
	// ---
	bool changeRequest = false;
	if (! newCard) {
	QString vendor = workingCard["Vendor"];
	QString name   = workingCard["Name"];
	if (
		((mVendor->currentText()!=vendor) && (mName->currentText()!=name)) ||
		m3DStateChanged
	) {
		changeRequest = true;
	}
	} else {
		changeRequest = true;
	}
	QDict<char>* spec = NULL;
	XDb* pCDB = mFiles["cdb_CARDS"]->cdbGetSpec (
		mVendor->currentText(),mName->currentText()
	);
	if ((pCDB) && (changeRequest)) {
		spec = pCDB -> getHash();
		// ...
		// save BusID from current card if we dont`t
		// want to add a new card the old BusID is the
		// correct one for this card
		// ---
		if (! newCard) {
			saveBusID -> sprintf("%s",workingCard["BusID"].ascii());
		}
		// ...
		// clear the working card contents...
		// ---
		workingCard.clear();

		// ...
		// set base items for the card...
		// ---
		QString* ident  = new QString;
		QString* screen = new QString;
		QString* monitor= new QString;
		QString* device = new QString;
		QString* name   = new QString (mName->currentText());
		QString* vendor = new QString (mVendor->currentText());
		ident -> sprintf ("Device[%d]",mWorkingIndex);
		workingCard.setPair ("Identifier",ident->ascii());
		workingCard.setPair ("Name"   , name->ascii());
		workingCard.setPair ("Vendor" , vendor->ascii());
		workingCard.setPair ("BusID"  , saveBusID->ascii());
		workingCard.setPair ("Screen" , "0");
		// ...
		// [ Card ] done
		// set base items for the desktop only if a new
		// card is added...
		// ---
		if (newCard) {
			screen  -> sprintf ("Screen[%d]",mWorkingIndex);
			monitor -> sprintf ("Monitor[%d]",mWorkingIndex);
			device  -> sprintf ("Device[%d]",mWorkingIndex);
			workingDesktop.setPair ("Identifier",screen->ascii());
			workingDesktop.setPair ("Monitor",monitor->ascii());
			workingDesktop.setPair ("Device",device->ascii());
			workingDesktop.setPair ("ColorDepth","16");
			workingDesktop.setPair ("ModelName" ,"Unknown");
			workingDesktop.setPair ("VendorName","Unknown");
			workingDesktop.setPair ("VertRefresh","43-72");
			workingDesktop.setPair ("HorizSync"  ,"30-33");
			workingDesktop.setPair ("Modes:4" ,"640x480");
			workingDesktop.setPair ("Modes:8" ,"640x480");
			workingDesktop.setPair ("Modes:15","640x480");
			workingDesktop.setPair ("Modes:16","640x480");
			workingDesktop.setPair ("Modes:24","640x480");
			workingDesktop.setPair ("CalcModelines","on");
			workingDesktop.setPair ("CalcAlgorithm","CheckDesktopGeometry");
		}
		// [ Desktop ] done

		QDictIterator<char> it (*spec);
		for (; it.current(); ++it) {
		QString key (it.currentKey());
		QString* val = new QString;
		val->sprintf("%s",it.current());
		if (val->isEmpty()) {
			continue;
		}
		if (mCheck3D -> isChecked()) {
		// ====================================
		// Include 3D Selection data
		// ------------------------------------
		global3DActive = 1;
		if (key == "3DDriver")    {
			workingCard.setPair ("Driver",val->ascii());
		}
		if (key == "3DOption")    {
			workingCard.setPair ("Option",val->ascii());
		}
		if (key == "3DRawData")   {
			workingCard.setPair ("RawData",val->ascii());
		}
		if (key == "Script3D")    {
			qx ( GETACTIVATED,STDNONE,1,"%s",val->ascii() );
		}
		} else {
		// ====================================
		// Include DEFAULT Selection data
		// ------------------------------------
		QList<char> data3D = get3Dstatus();
		QString script = data3D.at(2);
		qx ( GETACTIVATED,STDNONE,1,"%s",script.ascii() );
		global3DActive = 0;
		if (key == "Driver")    {
			workingCard.setPair ("Driver",val->ascii());
		}
		if (key == "Option")    {
			workingCard.setPair ("Option",val->ascii());
		}
		if (key == "RawData")   {
			workingCard.setPair ("RawData",val->ascii());
		}
		}
		}
	}

	// ...
	// check if a profile exist for the selected Card
	// if yes call ISaX (getprofile) and include the profile data.
	// Note:
	// =====
	// A profile may add new devices here. In this case
	// we had to add the following sections too:
	// 1) Screen/Monitor -> desktop
	// 2) ServerLayout   -> layout
	// ---
	if ((pCDB) && (changeRequest)) {
	QString profile;
	XWrapPointer< QDict<char> > selectedCard ( spec );
	if (selectedCard["Profile"]) {
		profile.sprintf("%s%s",
			PRODIR,selectedCard["Profile"].ascii()
		);
		QString optp("-p");
		QString optd("-d");
		QString opta("-a");
		QString index;
		index.sprintf("%d",mWorkingIndex);
		QString action;
		if (newCard) {
		action.sprintf("add");
		} else {
		action.sprintf("config");
		}
		qx( GETPROFILE,STDNONE,6,"%s %s %s %s %s %s",
			optp.ascii(),profile.ascii(),
			optd.ascii(),index.ascii(),
			opta.ascii(),action.ascii()
		);
	
		sleep (1);
		QFileInfo checkProfile;
		pCard    = NULL;
		pLayout  = NULL;
		pDesktop = NULL;
		// check and read the layout profile...
		// ---
		checkProfile.setFile (LAYOUTPROFILE);
		if ((checkProfile.exists()) && (checkProfile.size() > 0)) {
			pLayout = new XFile (LAYOUTPROFILE);
			pLayout -> sysRead();
		}
		// check and read the desktop profile...
		// ---
		checkProfile.setFile (DESKTOPPROFILE);
		if ((checkProfile.exists()) && (checkProfile.size() > 0)) {
			pDesktop = new XFile (DESKTOPPROFILE);
			pDesktop -> sysRead();
		}
		// check and read the card profile...
		// ---
		checkProfile.setFile (CARDPROFILE);
		if ((checkProfile.exists()) && (checkProfile.size() > 0)) {
			pCard = new XFile (CARDPROFILE);
			pCard -> sysRead();
		}
		// include card profile data
		// ---
		if (pCard) {
			int index;
			XData* data;
			XData* current;
			QDict<char> card;
			int cards = pCard -> getDeviceCount();
			for (int n=mWorkingIndex; n < cards + mWorkingIndex; n++) {
			data = pCard -> getDevice (n);
			if (! data) {
				continue;
			}
			index = data -> getID();
			card  = data -> getData();
			// ...
			// if the current index exceeds the workingIndex
			// we had to add a new device
			// ---
			if (index > mWorkingIndex) {
				mFiles["sys_CARD"] -> addDevice (index);
			}
			// ...
			// get the current card XData object and
			// include the additional profile information now
			// ---
			current = mFiles["sys_CARD"] -> getDevice (index);
			QDictIterator<char> it (card);
			for (; it.current(); ++it) {
				current -> setPair ( it.currentKey().ascii(),it.current() );
			}
			QString* name   = new QString (mName->currentText());
			QString* vendor = new QString (mVendor->currentText());
			current -> setPair ("Name"   , name->ascii());
			current -> setPair ("Vendor" , vendor->ascii());
			}
		}
		// include desktop profile data
		// ---
		if (pDesktop) {
			int index;
			XData* data;
			XData* current;
			QDict<char> desktop;
			int desktops = pDesktop -> getDeviceCount();
			for (int n=mWorkingIndex; n <= desktops + mWorkingIndex; n++) {
			data = pDesktop -> getDevice (n);
			if (! data) {
				continue;
			}
			index   = data -> getID();
			desktop = data -> getData();
			if (index > mWorkingIndex) {
				mFiles["sys_DESKTOP"] -> addDevice (index);
			}
			current = mFiles["sys_DESKTOP"] -> getDevice (index);
			QDictIterator<char> it (desktop);
			for (; it.current(); ++it) {
				current -> setPair ( it.currentKey().ascii(),it.current() );
			}
			}
		}
		// include layout profile data
		// ---
		if (pLayout) {
			XData* data;
			XData* current;
			QDict<char> layout;
			data = pLayout -> getDevice (0);
			if (data) {
			layout  = data -> getData();
			current = mFiles["sys_LAYOUT"] -> getDevice (0);
			QDictIterator<char> it (layout);
			for (; it.current(); ++it) {
				current -> setPair ( it.currentKey().ascii(),it.current() );
			}
			}
		}
	}
	}

	// ...
	// Include the configuration data of the expert
	// dialog to the mFiles structure:
	// ---
	// 3D check...
	// ---
	XData* workingPath = mFiles["sys_PATH"]->getDevice (0);
	if (! m3D -> isNull()) {
	workingPath -> setPair ("ModuleLoad",
		m3D->ascii()
	);
	}
	// ...
	// Card Options [bool]...
	// ---
	mCheckType = false;
	slotOption (NULL);
	if (mCardOptions->isEmpty()) {
		workingCard.setPair ("Option","");
	} else {
		QString* addOptions = new QString (mCardOptions->ascii());
		workingCard.setPair ("Option",
			addOptions->ascii()
		);
	}
	// ...
	// Card Options [with value]...
	// ---
	if (! mHiddenRawOptions.isEmpty()) {
		QString baseOptions;
		QListIterator<char> it (mHiddenRawOptions);
		for (; it.current(); ++it) {
			baseOptions.sprintf ("%s,%s",
				baseOptions.ascii(),it.current()
			);
		}
		if (baseOptions.at(0) == ',') {
			baseOptions.remove (0,1);
		}
		if (mCardRawOptions->isEmpty()) {
			mCardRawOptions->sprintf ("%s",
				baseOptions.ascii()
			);
		} else {
			mCardRawOptions->sprintf ("%s,%s",
				baseOptions.ascii(),mCardRawOptions->ascii()
			);
		}
	}
	if (mCardRawOptions->isEmpty()) {
		workingCard.setPair ("RawData","");
	} else {
		QString* addOptions = new QString (mCardRawOptions->ascii());
		workingCard.setPair ("RawData",
			addOptions->ascii()
		);
	}
	// ...
	// BusID...
	// ---
	workingCard.setPair ("BusID",
		mBusID->currentText()
	);
	// ...
	// ScreenID...
	// ---
	QString* scrID = new QString;
	scrID -> sprintf ("%d",mScrID->currentItem());
	workingCard.setPair ("Screen",
		scrID->ascii()
	);
	// ...
	// Rotate state...
	// ---
	QString* rotate = new QString ("off");
    setInputRotateState ("off");
	setRandRExtension ("on");
	if (mRotateLeft -> isChecked()) {
		rotate->sprintf("CCW");
	}
	if (mRotateRight -> isChecked()) {
		rotate->sprintf("CW");
	}
	setInputRotateState (rotate->ascii());
	setRandRExtension (rotate->ascii());
	workingCard.setPair ("Rotate",
		rotate->ascii()
	);

	// ...
	// add an entry to mCardList if a new card 
	// should be added to the system. Otherwhise update
	// the current entry of the mCardList if changed
	// ---
	if (newCard) {
	if (pCDB) {
		QString topic;
		
		int cards = mFiles["sys_CARD"] -> getDeviceCount();
		int start = mCardList -> count();
		for (int n=start; n<cards; n++) {
			XData* sysData = mFiles["sys_CARD"] -> getDevice(n);
			if (! sysData) {
			continue;
			}
			QDict<char> cardInfo = sysData -> getData();
			topic.sprintf ("%s %s",
				cardInfo["Vendor"],cardInfo["Name"]
			);
			mCardList -> insertItem ( 
				QPixmap(PIXCARD),topic 
			);
			// ...
			// update desktop list in mDesktopList too
			// ---
			// 1) get the listbox pointer from the dialog
			// 2) create topic line in the same way as desktop/action.c does
			// 3) insert item to mDesktopList
			// ----
			XDesktop* desktopDialog;
			desktopDialog = (XDesktop*) mIntro -> retrieve (Colors);
			QListBox* mDesktopList = desktopDialog -> getList ();
			// ...
			// create the topic line for the listbox out
			// of the current data structure
			// ---
			sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
			if (! sysData) {
				continue;
			}
			QDict<char> desktopInfo = sysData -> getData();
			QString defaultColor (desktopInfo["ColorDepth"]);
			QString reskey; reskey.sprintf("Modes:%s",defaultColor.ascii());
			XStringList res (desktopInfo[reskey]);
			res.setSeperator (",");
			QString primary = res.getList().at(0);
			QTextOStream (&topic) 
				<< mText["desktop"] << " " << primary.ascii() 
				<< " " << mText["with"] << " " << defaultColor.ascii() 
				<< " bit";
			mDesktopList -> insertItem (
				QPixmap(PIXDESKTOP),topic
			);
			// ...
			// recreate the server layout because a 
			// card was added. We will overwrite the current
			// layout and create serial layout structure
			// ---
			sysData = mFiles["sys_LAYOUT"] -> getDevice(0);
			if (! sysData) {
				continue;
			}
			QDict<char> layoutInfo = sysData -> getData();
			int cardCount = mCardList -> count();
			int lastCard  = cardCount - 1;
			if (cardCount == 1) {
			sysData -> setPair(
				"Screen:Screen[0]","<none> <none> <none> <none>"
			);
			} else {
			for (int i=0;i < cardCount; i++) {
				QString* key   = new QString();
				QString* value = new QString();
				key -> sprintf("Screen:Screen[%d]",i);
				if (i == 0) {
				value -> sprintf (
					"<none> Screen[%d] <none> <none>",i+1
				);
				} else if (i == lastCard) {
				value -> sprintf (
					"Screen[%d] <none> <none> <none>",i-1
				);
				} else {	
				value -> sprintf (
					"Screen[%d] Screen[%d] <none> <none>",i-1,i+1
				);
				}
				sysData -> setPair(
					key->ascii(),value->ascii()
				);
			}
			}
			XLayout* layoutDialog;
			layoutDialog = (XLayout*) mIntro -> retrieve (Layout);
			layoutDialog -> setupLayout();
		}
		mCardList -> setSelected (mWorkingIndex,true);
		mCardList -> setCurrentItem ( mWorkingIndex );
		mDel      -> setDisabled ( false );
		mSetup    -> setDisabled ( false );
		mSelected = mWorkingIndex;
	}
	} else {
	if (changeRequest) {
		QString topic;
		topic.sprintf ("%s %s",
			workingCard["Vendor"].ascii(),workingCard["Name"].ascii()
		);
		mCardList -> changeItem ( 
			QPixmap(PIXCARD),topic,mWorkingIndex 
		);
	}
	}
}

//=====================================
// XCard slotTopCancel...
//-------------------------------------
void XCard::slotTopCancel (void) {
	// log (L_INFO,"XCard::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
	if (m3Dorig) {
		m3D = m3Dorig;
	} else {
		m3D = NULL;
	}
}

//=====================================
// XCard vendor selection...
//-------------------------------------
void XCard::slotVendor (QListBoxItem* item) {
	// log (L_INFO,"XCard::slotVendor() called\n");
	// ...
	// fill the name listbox with the devices
	// apropriate to the selected vendor
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	if (! mVendor->isSelected (item)) {
		return;
	} 
	mName -> clear();
	QString selectedVendor = mVendor -> currentText();
	QList<char> names = mFiles["cdb_CARDS"]->cdbGetNameList (selectedVendor);
	QListIterator<char> it (names);
	for (; it.current(); ++it) {
		mName -> insertItem (it.current());
	}
	mName -> sort();
}

//=====================================
// XCard name selection...
//-------------------------------------
void XCard::slotName (QListBoxItem*) {
	// log (L_INFO,"XCard::slotName() called\n");
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
	XDb* pCDB = mFiles["cdb_CARDS"]->cdbGetSpec(
		mVendor->currentText(),mName->currentText()
	);
	if (pCDB) {
		spec = pCDB -> getHash();
	}

	// ...
	// set BusID to Single
	// ---
	mBusID -> setCurrentItem ( 0 );

	// ...
	// set ScreenID to primary screen
	// ---
	mScrID -> setCurrentItem ( 0 );

	// ...
	// check if the 3D flag is set for this card
	// and enable/disable this control
	// ---
	XWrapPointer< QDict<char> > selectedCard ( spec );
	QString flag (selectedCard["Flag"]);
	if ((flag == "3D") && (mCardList->count() <= 1)) {
		mCheck3D -> setDisabled ( false );
		mCheck3D -> setChecked  ( true );
	} else {
		mCheck3D -> setChecked  ( false );
		mCheck3D -> setDisabled ( true );
	}

	// ...
	// check which simple options are defined and
	// setup the option list
	// ---
	XStringList strings;
	mRotate -> setDisabled (true);
	XApiData modOptions = mFiles["ext_CMODULES"]->getAPI();
	QString options ( modOptions[selectedCard["Driver"]] );
	mOptions -> clear ();
	mOptFlags.clear();
	if (! options.isEmpty()) {
	strings.setText (options);
	QList<char> strlist = strings.getList();
	QListIterator<char> it (strlist);
	for (; it.current(); ++it) {
		XStringList opt;
		opt.setText (it.current());
		opt.setSeperator (":");
		QList<char> optlist = opt.getList();
		if (QString(optlist.at(0)) == "Rotate") {
			mRotate -> setDisabled (false);
		} else {
			mOptFlags.insert (optlist.at(0),optlist.at(1));
			mOptions -> insertItem (optlist.at(0));
		}
	}
	}
	mOptions -> sort();
	if (selectedCard["Option"]) {
		strings.setText (selectedCard["Option"]);
		strings.setSeperator (",");
		QList<char> strlist = strings.getList();
		QListIterator<char> it (strlist);
		for (; it.current(); ++it) {
			QListBoxItem* item = mOptions -> findItem (it.current());
			if (! item) {
				mOptions -> insertItem     (it.current());
				mOptions -> setSelected    (mOptions->count() - 1,true);
				mOptions -> setCurrentItem (mOptions->count() - 1);
			} else {
				mOptions -> setSelected    (item,true);
				mOptions -> setCurrentItem (item);
			}
		}
		mOptions -> sort();  
		mOptions -> ensureCurrentVisible();
	}
	mHiddenRawOptions.clear(); 
	if (selectedCard["RawData"]) {
		strings.setText (selectedCard["RawData"]);
		strings.setSeperator (",");
		QList<char> strlist = strings.getList();
		QListIterator<char> it (strlist);
		for (; it.current(); ++it) {
			int count = 0;
			QList<char> cleanOptionList;
			QStringList optlist = QStringList::split ( "\"", it.current() );
			for ( QStringList::Iterator 
				in = optlist.begin(); in != optlist.end(); ++in 
			) {
				QString result (*in);
				result = result.stripWhiteSpace();
				if (result.isEmpty()) {
					continue;
				}
				if (result == "Option") {
					count++; continue;
				}
				if ((count == 0) && (result != "Option")) {
					mHiddenRawOptions.append (it.current());
				} 
				cleanOptionList.append (result.ascii());
				count++;
			}
			if (cleanOptionList.count() == 2) {
			QString option;
			option.sprintf("%s --> %s",
				cleanOptionList.at(0),cleanOptionList.at(1)
			);
			QListBoxItem* item = mOptions -> findItem (
				cleanOptionList.at(0)
			);
			if (! item) {
				mOptions -> insertItem (cleanOptionList.at(0));
				mOptFlags.insert (cleanOptionList.at(0),"any");
				item = mOptions -> findItem (cleanOptionList.at(0));
			}
			mOptions -> setCurrentItem (item);
			mOptions -> changeItem (
				option,mOptions->currentItem()
			);
			mOptions -> setSelected (
				mOptions->currentItem(),true
			);
			}
		}
	}
	mCheckType = false;
	slotOption (NULL);
}

//=====================================
// XCard graphics option selection...
//-------------------------------------
void XCard::slotOption (QListBoxItem*) {
	// log (L_INFO,"XCard::slotOption() called\n");
	// ...
	// create the options string as comma seperated
	// list of string items
	// ---
	QString flag ("bool");
	QString current = mOptions->currentText();
	int index = mOptions->currentItem();
	if (mOptions->currentText()) {
		XStringList opt;
		opt.setText (current);
		opt.setSeperator (" --> ");
		current = opt.getList().at(0);
		flag = mOptFlags[current];
	}
	//=========================================
	// if option was selected check for type
	// and update the item if necessary
	//-----------------------------------------
	if (mCheckType) {
	if (mOptions->isSelected (index)) {
	if (flag == "integer") {
		if (! setTypedMessage (
			"optionfor",XBox::Information,current,XBOX_OPT_INT
		)) {
			mOptions -> setSelected ( index,false );
		} else {
			QString newText;
			newText.sprintf("%s --> %d",current.ascii(),mTypeValue);
			mOptions -> changeItem (newText,index);
			mOptions -> setSelected (index,true);
		}
	} else
	if ((flag == "string") || (flag == "any")) {
		if (! setTypedMessage (
			"optionfor",XBox::Information,current,XBOX_OPT_ANY
		)) {
			mOptions -> setSelected ( index,false );
		} else {
			QString newText;
			newText.sprintf("%s --> %s",current.ascii(),mTypeString.ascii());
			mOptions -> changeItem (newText,index);
			mOptions -> setSelected (index,true);
		}
	} else
	if (flag == "frq") {
		// ...
	}
	} else {
		mOptions -> changeItem (current,index);
	}
	}
	//=========================================
	// setup bool options
	//-----------------------------------------
	delete (mCardOptions);
	mCardOptions = new QString();
	for (int i=0;i < (int)mOptions->count();i++) {
	QListBoxItem* item = mOptions->item (i);
	if (mOptions -> isSelected(item)) {
		XStringList opt;
		opt.setText (mOptions->text(i));
		opt.setSeperator (" --> ");
		QList<char> optList = opt.getList();
		if (optList.count() == 1) {
			mCardOptions->sprintf (
				"%s,%s",mCardOptions->ascii(),mOptions->text(i).ascii()
			);
		}
	}
	}
	if (mCardOptions -> at(0) == ',') {
		mCardOptions -> remove (0,1);
	}
	//=========================================
	// setup non bool options
	//-----------------------------------------
	delete (mCardRawOptions);
	mCardRawOptions = new QString();
	for (int i=0;i < (int)mOptions->count();i++) {
	QListBoxItem* item = mOptions->item (i);
	if (mOptions -> isSelected(item)) {
		XStringList opt;
		opt.setText (mOptions->text(i));
		opt.setSeperator (" --> ");
		QList<char> optList = opt.getList();
		if (optList.count() == 2) {
			mCardRawOptions->sprintf (
				"%s,Option \"%s\" \"%s\"", mCardRawOptions->ascii(),
				optList.at(0),optList.at(1)
			);
		}
	}
	}
	if (mCardRawOptions -> at(0) == ',') {
		mCardRawOptions -> remove (0,1);
	}
	mCheckType = true;
}

//=====================================
// XCard Bus ID selection...
//-------------------------------------
void XCard::slotBusID (int) {
	// log (L_INFO,"XCard::slotBusID() called\n");
}

//=====================================
// XCard Screen ID selection...
//-------------------------------------
void XCard::slotScreenID (int) {
	// log (L_INFO,"XCard::slotScreenID() called\n");
}

//=====================================
// XCard enable or disable 3D feature
//-------------------------------------
void XCard::slot3D (void) {
	// log (L_INFO,"XCard::slot3D() called\n");
	// ...
	// slot called to enable or disable 3D
	// ---
	bool enable = false;
	if ( mCheck3D -> isChecked() ) {
		enable = true;
	}
	// ...
	// get the pointer to the CDB with the current
	// vendor/name selection.
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XDb* pCDB = mFiles["cdb_CARDS"]->cdbGetSpec (
		mVendor->currentText(),mName->currentText()
	);
	if (! pCDB) {
		mCheck3D -> setChecked  (false);
		mCheck3D -> setDisabled (true);
		global3DActive = 0;
		return;
	}
	QDict<char>* spec = pCDB -> getHash();
	XWrapPointer< QDict<char> > mText (mTextPtr);

	#if 0
	// ...
	// show a 3D warning message because 3D for
	// Linux is fine but not treated as stable 
	// ---
	if (enable) {
		QString D3Text;
		QString cardName;
		QString textKey ("question3d");
		D3Text = mText[textKey];
		QTextOStream (&cardName)
			<< mVendor->currentText() << " " << mName->currentText();
		D3Text.replace (
			QRegExp("\%1"),cardName
		);
		XBox mb (
			mText["topic3d"],D3Text,
			XBox::Information,
			XBox::Yes,
			XBox::No,0,
			mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,mText["Yes"]
		);
		mb.setButtonText (
			XBox::No,mText["No"]
		);
		switch (mb.exec()) {
		case XBox::No:
			mCheck3D -> setChecked  (false);
			return;
		break;	
		}
		mTop -> enterEvent ( 0 );
	}
	#endif
	// ...
	//=============================================
	// 3D selection algorithm
	//---------------------------------------------
	// 1) disable 3D
	// Call the software 3D script after Ok (slotTopOk()) 
	// and reset the module list to its original value
	// ---
	// 2) enable 3D
	// Call the appropriate 3D script after Ok (slotTopOk()) and 
	// add the extension modules to the module list. Check the 
	// colordepth and warn if not in 16bit, additionally check the 
	// packages/dummy needed to use 3D
	// ---
	m3D -> sprintf ("%s",spec->find ("Extension"));
	// ...
	// get the extension modules used for 3D for the current
	// CDB card selection and add them to the current module
	// list if we want to enable 3D.
	// ---
	XStringList cur ( *mModuleList ); cur.setSeperator (",");
	XStringList ext ( *m3D ); ext.setSeperator (",");
	QList<char> curModules = cur.getList();
	QList<char> extModules = ext.getList();
	QListIterator<char> it (extModules);
	for (; it.current(); ++it) {
		bool found = false;
		QListIterator<char> in (curModules);
		for (; in.current(); ++in) {
		if (QString(in.current()) == QString(it.current())) {
			found=true; break;
		}
		}
		if (enable) {
		if (! found) {
			curModules.append (it.current());
		}
		} 
	} 
	m3D -> sprintf ("%c",'\0');
	QListIterator<char> io (curModules);
	for (; io.current(); ++io) {
		m3D -> sprintf ("%s,%s",m3D->ascii(),io.current());
	}
	if (m3D->at(0) == ',') {
		m3D->remove (0,1);
	}
	// ...
	// check the currently used colordepth
	// and show a warning if not in 16 bit color depth
	// ---
	if (enable) {
		XData* sysData = mFiles["sys_DESKTOP"] -> getDevice(0);
		if ( sysData ) {
		QDict<char> desktopInfo = sysData -> getData();
		QString defaultColor (desktopInfo["ColorDepth"]);
		if (defaultColor.toInt() != 16) {
			setMessage ("require16bit");
		}
		}
	}
	// ...
	// check for the packages and flags needed to enable/disable
	// 3D if not all packages installed or the flag value point
	// to a commercial driver then show a warning message
	// ---
	QList<char> data3D = get3Dstatus();
	QString flag3D = data3D.at(6);
	if (enable) {
		QString pacs;
		pacs.sprintf("%s",spec->find ("Package3D"));
		QString installed = qx (
			GETPACKAGE,STDOUT,1,"%s",pacs.ascii()
		);
		if (! installed.toInt() ) {
			setMessage ("pacsmissing",XBox::Critical,pacs);
			mCheck3D -> setChecked ( false );
			return;
		}
		if (flag3D == "NVDummy") {
			setMessage ("nvidia3Denable",XBox::Critical);
			mCheck3D -> setChecked ( false );
			return;
		}
	} else {
		#if 1
		if (flag3D == "NVReal") {
			setMessage ("nvidia3Ddisable",XBox::Critical);
			mCheck3D -> setChecked ( true );
			return;
		}
		#endif
	}
	// ...
	// show a 3D warning message because 3D for
	// Linux is fine but not treated as stable 
	// ---
	if (enable) {
		QString D3Text;
		QString cardName;
		QString textKey ("question3d");
		D3Text = mText[textKey];
		QTextOStream (&cardName)
			<< mVendor->currentText() << " " << mName->currentText();
		D3Text.replace (
			QRegExp("\%1"),cardName
		);
		XBox mb (
			mText["topic3d"],D3Text,
			XBox::Information,
			XBox::Yes,
			XBox::No,0,
			mFrame,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,mText["Yes"]
		);
		mb.setButtonText (
			XBox::No,mText["No"]
		);
		switch (mb.exec()) {
		case XBox::No:
			mCheck3D -> setChecked  (false);
			return;
		break;
		}
		mTop -> enterEvent ( 0 );
	}
}

//=====================================
// XCard get sysp 3D data...
//-------------------------------------
QList<char> XCard::get3Dstatus (void) {
	QString noscan ("noscan");
	XStringList packageInfo ( 
		qx (GET3D,STDOUT,1,"%s",noscan.ascii())
	);
    packageInfo.setSeperator (":");
	QList<char> result = packageInfo.getList();
	return (result);
}

//=====================================
// XCard validatePage...
//-------------------------------------
bool XCard::validatePage (void) {
	// log (L_INFO,"XCard::validatePage() called\n");
	// ...
	// this function is called if the finish button is
	// pressed to validate the current page setup
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	bool valid = false;
	
	// ...
	// check the card number in mCardList
	// ---
	if (mCardList->count() >= 1) {
		valid = true;
	}

	// ...
	// give notify message if not valid
	// ---
	if (! valid) {
		XBox mb ( 
		mText["dialoginfo"], mText["cardinvalid"],
		XBox::Warning,
		XBox::Ok,0,0,mFrame,
		globalFrameWidth
		);
		mb.setButtonText (
		XBox::Ok,mText["Ok"]
		);
		mb.exec();
		mFrame -> enterEvent ( 0 );
	}
	return (valid);
}

//=====================================
// XCard set OK message box...
//-------------------------------------
void XCard::setMessage (
	const QString& textKey,XBox::Icon icon,const QString& addon
) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString text;
	QTextOStream (&text)
		<< mText[textKey] << " " << addon;
	XBox mb (
		mText["hint"],text,
		icon,XBox::Ok, 0,0,mFrame,
		globalFrameWidth
	);
	mb.setButtonText (
		XBox::Ok, mText["Ok"]
	);
	mb.exec();
	if (mTop->isVisible()) {
		mTop -> enterEvent ( 0 );
	} else {
		mFrame -> enterEvent ( 0 );
	}
}

//=====================================
// XCard set OK/Cancel typed box...
//-------------------------------------
int XCard::setTypedMessage (
	const QString& textKey,XBox::Icon icon,
	const QString& option,int type
) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString text (mText[textKey]);
	text.replace (
		QRegExp("\%1"),option
	);
	XBox* mb = new XBox (
		mText["hint"],text,
		icon,XBox::Ok, XBox::Cancel,0,mFrame,
		globalFrameWidth,type
	);
	mb->setFixedWidth (400);
	mb->setButtonText (
		XBox::Ok,mText["Ok"]
	);
	mb->setButtonText (
		XBox::Cancel, mText["Cancel"]
	);
	int choose = mb->exec();
	switch (choose) {
	case XBox::Cancel: {
		delete (mb); return (0);
	}
	}
	switch (type) {
	case XBOX_OPT_ANY:
		mTypeString = mb->getLineEditText();
	break;
	case XBOX_OPT_INT:
		mTypeValue = mb->getSpinBoxCount();
	break;
	}
	if (mTop->isVisible()) {
		mTop -> enterEvent ( 0 );
	} else {
		mFrame -> enterEvent ( 0 );
	}
	delete (mb);
	return (1);
}

//=====================================
// XCard show setup window yes/no...
//-------------------------------------
void XCard::showSetupWindow (bool value) {
	mShowSetupWindow = value;
}

//=====================================
// XCard check/uncheck 3D box...
//-------------------------------------
void XCard::check3D (bool value) {
	mCheck3D -> setChecked (value);
}

//=====================================
// XCard 3D checkbox status...
//-------------------------------------
bool XCard::isChecked ( void ) {
	return (mCheck3D->isChecked());
}

//=====================================
// XCard check inpute device rotation
//-------------------------------------
void XCard::setInputRotateState ( const QString& direction ) {
	// log (L_INFO,"XCard::setInputRotateState() called\n");
	// ...
	// this function is called if the rotate state of the
	// device section gets changed. Special devices like
	// Touchscreens needs to be rotated too.
	// ---
	int count = -1;
	XData* data;
	QDict<char> input;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<mFiles["sys_INPUT"]->getDeviceCount(); n++) {
		count += 2;
		data = mFiles["sys_INPUT"] -> getDevice (count);
		if (! data) {
			continue;
		}
		input = data -> getData();
		if (QString(input["InputFashion"]) != "Touchpanel") {
			continue;
		}
		if (direction == "CW") {
		//========================================
		// CW -> 90 degrees for Touchpanel
		//----------------------------------------
		data -> setPair ("Rotation","90");
		} else
		if (direction == "CCW") {
		//========================================
		// CCW -> 270 degrees for Touchpanel
		//----------------------------------------
		data -> setPair ("Rotation","270");
		} else {
		//========================================
		// No rotation
		//----------------------------------------
		data -> setPair ("Rotation","0");
		}
	}
}

//=========================================
// XCard disable randr extension on rotate
//-----------------------------------------
void XCard::setRandRExtension ( const QString& direction ) {
	// log (L_INFO,"XCard::setRandRExtension() called\n");
	// ...
	// this function will include a SpecialFlags line into
	// the path file which set the RandR Option in the
	// ServerFlags section later
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XData* systemServerFlags = mFiles["sys_PATH"]->getDevice (0);
	if ((direction == "CW") || (direction == "CCW")) {
	//========================================
	// Display is rotated; disable RandR
	//----------------------------------------
	log(L_INFO,"disable RandR Extension in rotate mode\n");
	systemServerFlags -> setPair (
		"SpecialFlags","RandR,off"
	);
	} else {
	//========================================
	// Display is not rotated; enable RandR
	//----------------------------------------
	systemServerFlags -> setPair (
		"SpecialFlags","RandR,on"
	);
	}
}
