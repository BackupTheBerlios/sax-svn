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
              : configure the primary keyboard properties and
              : handle the font settings here too
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "keyboard.h"
#include <X11/XKBlib.h>

//=====================================
// XKeyboard reset and switch to Intro
//-------------------------------------
void XKeyboard::resetPage (int reload) {
	// ...
	// this function is called if the keyboard dialog is finished or canceled
	// The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	QString update = "sys_KEYBOARD";

	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
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
// XKeyboard internal page called...
//-------------------------------------
void XKeyboard::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case KeyboardPage1: {
		break;
	}
	};
}

//=====================================
// XKeyboard init page with infos...
//-------------------------------------
void XKeyboard::initPage (void) {
	// log (L_INFO,"XKeyboard::initPage() called\n");
    // ...
    // this function is called after the page was created.
    // It must only be used to init the widgets contents
    // with the data available for selections and other stuff
    // like that
    // ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	// insert keyboard layouts...
	// --------------------------
	XApiData kbd = mFiles["ext_KLANGUAGE"]->getAPI();
	mKBDPtr = kbd.getHash();
	
	QDictIterator<char> it (*mKBDPtr);
	for (; it.current(); ++it) {
		mLayout -> insertItem (it.currentKey());
	}
	mLayout -> sort();

	// insert keyboard types...
	// ------------------------
	QList<char> types = mFiles["cdb_KEYBOARDS"]->cdbGetVendorList();
	QListIterator<char> io (types);
	for (; io.current(); ++io) {
		mType -> insertItem (io.current());
	}
	mCurrentDelay = XKBDDELAY_DEFAULT;
	mCurrentKrate = XKBDRATE_DEFAULT;
}

//=====================================
// XKeyboard virtual slots...
//-------------------------------------
bool XKeyboard::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XKeyboard::slotRun() called: %d\n",index);
	// ...
	// this function is called if the keyboard page is activated.
	// use this function to init the dialog with the current
	// setup of the keyboard
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXKeyboard"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	// ...
	// get the mFiles pointer wrapper from the Intro
	// object which has read all the data files. Than
	// lookup the keyboard XData object
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XData* sysData = mFiles["sys_KEYBOARD"] -> getDevice(0);
	if (! sysData) {
		return (FALSE);
	}
	QDict<char> keyboardInfo = sysData -> getData();

	// ...
	// lookup keyboard type [MapName] and select
	// the model in the combobox
	// ---
	for (int n=0;n<mType->count();n++) {
	if (mType->text(n) == keyboardInfo["MapName"]) {
		mType->setCurrentItem (n);
		break;
	}
	}
	// ...
	// select current used language layout in the
	// mLayout listbox
	// ---
	XApiData kbd = mFiles["ext_KLANGUAGE"]->getAPI();
	mKBDPtr = kbd.getHash();
	for (unsigned int n=0;n<mLayout->count();n++) {
		QString XkbLayout (mKBDPtr->operator[](mLayout->text(n)));
		XStringList completeLanguage (XkbLayout);
		completeLanguage.setSeperator ("+");
		QList<char> xf86Layout = completeLanguage.getList();
		if (QString(xf86Layout.at(0)) == keyboardInfo["XkbLayout"]) {
			QListBoxItem* current = mLayout->item(n);
			mLayout -> setSelected (current,true);
			mLayout -> setCurrentItem (current);
			mLayout -> ensureCurrentVisible();
			break;
		}
	}
	// ...
	// lookup XkbVariant and enable/disable the
	// nodeadkeys checkbox
	// ---
	mDeadKeys -> setChecked ( false );
	if (keyboardInfo["XkbVariant"]) {
		mDeadKeys -> setChecked ( true );
	}
	}
	return (TRUE);
}

//=====================================
// XKeyboard select basic layout...
//-------------------------------------
void XKeyboard::slotSelect ( QListBoxItem* ) {
	// log(L_INFO,"XKeyboard::slotSelect() called\n");
	// ...
	// every time the language layout has changed
	// we will update the XkbLayout setting for the
	// currently working keyboard
	// ---
	// get the mFiles pointer wrapper first
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	// ...
	// get the currently used keyboard and set the
	// XkbLayout according to the selection
	// ---
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	XApiData kbd = mFiles["ext_KLANGUAGE"]->getAPI();
	mKBDPtr = kbd.getHash();
	QString XkbLayout (mKBDPtr->operator[](mLayout->currentText()));
	XStringList completeLanguage (XkbLayout);
	completeLanguage.setSeperator ("+");
	QList<char> xf86Layout = completeLanguage.getList();
	if (xf86Layout.at(0)) {
		workingKeyboard.setPair ("XkbLayout",xf86Layout.at(0));
	}
	apply();
}

//=====================================
// XKeyboard select hardware type...
//-------------------------------------
void XKeyboard::slotType ( int ) {
	// log(L_INFO,"XKeyboard::slotType() called\n");
	// ...
	// every time the keyboard type is changed we will
	// lookup the according CDB record for the new type
	// and reset the complete keyboard with the new data
	// ---
	// get the mFiles pointer wrapper first
    // ---
    QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	// ...
	// get the working keyboard and clear the current
	// contents and re-init the record
	// ---
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	workingKeyboard.clear();
	QString* mapname = new QString (mType->currentText());
	QString* ident   = new QString ("Keyboard[0]");
	QString* driver  = new QString ("Keyboard");
	workingKeyboard.setPair (
		"Identifier", ident->ascii()
	);
	workingKeyboard.setPair (
		"Driver", driver->ascii()
	);
	workingKeyboard.setPair (
		"MapName",mapname->ascii()
	);
	// ...
	// set the type data now
	// ---
	XDb* pCDB = mFiles["cdb_KEYBOARDS"]->cdbGetSpec (
		mType->currentText(),"<...>"
	);
	if (pCDB) {
		QDict<char>* CDBData = NULL;
		CDBData = pCDB -> getHash();
		QDictIterator <char> it (*CDBData);
		for (; it.current(); ++it) {
			QString key (it.currentKey());
			QString* val = new QString;
			val->sprintf("%s",it.current());
			if (val->isEmpty()) {
				continue;
			}
			if (key == "Protocol") {
				workingKeyboard.setPair ("Protocol",val->ascii());
			}
			if (key == "XkbRules") {
				workingKeyboard.setPair ("XkbRules",val->ascii());
			}
			if (key == "XkbModel") {
				workingKeyboard.setPair ("XkbModel",val->ascii());
			}
			if (key == "XkbKeyCodes") {
				workingKeyboard.setPair ("XkbKeyCodes",val->ascii());
			}
			if (key == "AutoRepeat") {
				workingKeyboard.setPair ("AutoRepeat",val->ascii());
			}
			if (key == "ScrollLock") {
				workingKeyboard.setPair ("ScrollLock",val->ascii());
			}
			if (key == "RightCtl") {
				workingKeyboard.setPair ("RightCtl",val->ascii());
			}
			if (key == "LeftAlt") {
				workingKeyboard.setPair ("LeftAlt",val->ascii());
			}
			if (key == "RightAlt") {
				workingKeyboard.setPair ("RightAlt",val->ascii());
			}
			if (key == "XkbOptions") {
				workingKeyboard.setPair ("XkbOptions",val->ascii());
			}
			if (key == "XkbKeymap") {
				workingKeyboard.setPair ("XkbKeymap",val->ascii());
			}
			if (key == "XkbCompat") {
				workingKeyboard.setPair ("XkbCompat",val->ascii());
			}
		}
		// ...
		// set nodeadkey if checked
		// ---
		if (mDeadKeys -> isChecked()) {
			workingKeyboard.setPair ("XkbVariant","nodeadkeys");
		}
		// ...
		// set current selected language layout
		// ---
		XApiData kbd = mFiles["ext_KLANGUAGE"]->getAPI();
		mKBDPtr = kbd.getHash();
		QString XkbLayout (mKBDPtr->operator[](mLayout->currentText()));
		XStringList completeLanguage (XkbLayout);
		completeLanguage.setSeperator ("+");
		QList<char> xf86Layout = completeLanguage.getList();
		if (xf86Layout.at(0)) {
			workingKeyboard.setPair ("XkbLayout",xf86Layout.at(0));
		}
	}
	apply();
}

//=====================================
// XKeyboard open option dialog
//-------------------------------------
void XKeyboard::slotOption ( void ) {
	// log(L_INFO,"XKeyboard::slotOption() called\n");
	// ...
	// This slot is called if you press the options
	// button to enter the keyboard options dialog
	// ---
	setupTop ();
	mTop -> show();
	mTop -> enterEvent ( 0 );
}

//=====================================
// XKeyboard get index of item text
//-------------------------------------
int XKeyboard::getItem ( QComboBox* widget, const QString& item ) {
	QListBox* list = widget -> listBox();
	for (int i=0;i<widget->count();i++) {
	if (list->item(i)->text() == item ) {
		return (i);
	}
	}
	return (0);
}

//=====================================
// XKeyboard init function for toplevel
//-------------------------------------
void XKeyboard::setupTop ( void ) {
	// log (L_INFO,"XKeyboard::setupTop() called\n");
	// ...
	// this function is called if the setup toplevel window
	// is created to configure options for the selected
	// keyboard type
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	// ...
	// lookup AutoRepeat value to init slider
	// values within the options dialog
	// ---
	if (workingKeyboard["AutoRepeat"]) {
		XStringList valList;
		valList.setText (workingKeyboard["AutoRepeat"]);
		valList.setSeperator (" ");
		QList<char> kbd = valList.getList();
		mCurrentDelay = atoi(kbd.at(0));
		mCurrentKrate = atoi(kbd.at(1));
		mDelay  -> setValue (
			mCurrentDelay
		);
		mRepeat -> setValue (
			mCurrentKrate
		);
	}
	// ...
	// init all XKB values in the key-mapping dialog
	// overwrite the default setup with it
	// ---
	for (int i=0;i<6;i++) {
		mXkbOption[i]->setCurrentItem (0);
	}
	if (workingKeyboard["XkbOptions"]) {
		XStringList optList;
		optList.setText (workingKeyboard["XkbOptions"]);
		optList.setSeperator (",");
		QList<char> opt = optList.getList();
		QListIterator<char> it (opt);
		for (; it.current(); ++it) {
			QString optionText = mTranslateOption[it.current()];
			int index0 = getItem ( mXkbOption[0],optionText );
			int index1 = getItem ( mXkbOption[1],optionText );
			if (index0 != 0) {
				mXkbOption[0] -> setCurrentItem ( index0 );
			}
			if (index1 != 0) {
				mXkbOption[1] -> setCurrentItem ( index1 );
			}
		}
	}
	if (workingKeyboard["LeftAlt"]) {
		mXkbOption[2]->setCurrentItem (
		getItem (mXkbOption[2],workingKeyboard["LeftAlt"])
		);
	}
	if (workingKeyboard["RightAlt"]) {
		mXkbOption[3]->setCurrentItem (
		getItem (mXkbOption[3],workingKeyboard["RightAlt"])
		);
	}
	if (workingKeyboard["ScrollLock"]) {
		mXkbOption[4]->setCurrentItem (
		getItem (mXkbOption[3],workingKeyboard["ScrollLock"])
		);
	}
	if (workingKeyboard["RightCtl"]) {
		mXkbOption[5]->setCurrentItem (
		getItem (mXkbOption[3],workingKeyboard["RightCtl"])
		);
	}
}

//=====================================
// XKeyboard cancel pressed for mTop
//-------------------------------------
void XKeyboard::slotTopCancel ( void ) {
	// log (L_INFO,"XKeyboard::slotTopCancel() called\n");
	// ...
	// this function is called if you click onto the Cancel 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
	// ...
	// reset the symbol rate/delay 
	// using 'xset r' command
	// ---
	QString optr ("r");
	QString rate ("rate");
	QString delay; delay.sprintf ("%d",mCurrentDelay);
	QString krate; krate.sprintf ("%d",mCurrentKrate);
	qx ( XSET,STDNONE,4,"%s %s %s %s",
		optr.ascii(),rate.ascii(),
		delay.ascii(),krate.ascii()
	);
}

//=====================================
// XKeyboard Ok pressed for mTop
//-------------------------------------
void XKeyboard::slotTopOk ( void ) {
	// log (L_INFO,"XKeyboard::slotTopOk() called\n");
	// ...
	// this function is called if you click onto the OK 
	// button in the setup toplevel window
	// ---
	mFrame -> enterEvent ( 0 );
	QString* XkbOptions = NULL;
	QString* ScrollLock = NULL;
	QString* LeftAlt    = NULL;
	QString* RightAlt   = NULL;
	QString* RightCtl   = NULL;
	for (int i=0;i<6;i++) {
		QString selection = mXkbOption[i]->currentText();
		QDictIterator<char> it (mTranslateOption);
		for (; it.current(); ++it) {
		if (QString(it.current()) == selection) {
			selection = it.currentKey();
			break;
		}
		}
		if (selection == "default") {
			continue;
		}
		switch (i) {
		case 0:
			if (! XkbOptions) {
				XkbOptions = new QString();
			}
			XkbOptions -> sprintf ("%s,%s",
				XkbOptions->ascii(),selection.ascii()
			);
		break;
		case 1:
			if (! XkbOptions) {
				XkbOptions = new QString();
			}
			XkbOptions -> sprintf ("%s,%s",
				XkbOptions->ascii(),selection.ascii()
			);
		break;
		case 2:
			LeftAlt = new QString (selection);
		break;
		case 3:
			RightAlt = new QString (selection);
		break;
		case 4:
			ScrollLock = new QString (selection);
		break;
		case 5:
			RightCtl = new QString (selection);
		break;
		}
		if (XkbOptions) {
		if (XkbOptions -> at(0) == ',') {
			XkbOptions -> remove (0,1);
		}
		}
	}
	// ...
	// save the values to the working keyboard if
	// they are available and valid
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	if (XkbOptions) {
		workingKeyboard.setPair ("XkbOptions",XkbOptions->ascii());
	}
	if (LeftAlt) {
		workingKeyboard.setPair ("LeftAlt",LeftAlt->ascii());
	}
	if (RightAlt) {
		workingKeyboard.setPair ("RightAlt",RightAlt->ascii());
	}
	if (ScrollLock) {
		workingKeyboard.setPair ("ScrollLock",ScrollLock->ascii());
	}
	if (RightCtl) {
		workingKeyboard.setPair ("RightCtl",RightCtl->ascii());
	}
	// ...
	// save the values of the auto repeat sliders, if one
	// of them has another value than the default value
	// ---
	if (
		(mDelay->value() != XKBDDELAY_DEFAULT) || 
		(mRepeat->value() != XKBDRATE_DEFAULT)
	) {
		QString* autoRepeat = new QString();
		autoRepeat->sprintf("%d %d",
			mDelay->value(),mRepeat->value()
		);
		workingKeyboard.setPair (
			"AutoRepeat",autoRepeat->ascii()
		);
	}
}

//=====================================
// XKeyboard enable disable nodeadkeys
//-------------------------------------
void XKeyboard::slotVariant ( bool ) {
	// log(L_INFO,"XKeyboard::slotVariant() called\n");
	// ...
	// every time the nodeadkey checkbox toggles we
	// will set or reset the XkbVariant setting for the
	// currently used keyboard record
	// ---
	QDict<XFile>* mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	// ...
	// get currently used keyboard record...
	// ---
	XWrapPointer<XData> workingKeyboard (
		mFiles["sys_KEYBOARD"]->getDevice (0)
	);
	if (mDeadKeys -> isChecked()) {
	workingKeyboard.setPair (
		"XkbVariant","nodeadkeys"
	);
	} else {
	workingKeyboard.setPair (
		"XkbVariant",""
	);
	}
	apply();
}

//=====================================
// XKeyboard auto repeat set
//-------------------------------------
void XKeyboard::autoRepeat ( void ) {
	// log(L_INFO,"XKeyboard::autoRepeat() called\n");
	// ...
	// This function is called if either the repeat or
	// delay rate has changed. We will use xset to apply
	// the new rate/delay values to the X-Server. The
	// function is called after the slider has been 
	// released
	// ---
	XkbDescPtr xkb;
	int major = XkbMajorVersion;
	int minor = XkbMinorVersion;
	int op, event, error;
	int timeout = 500;
	Display* dpy = QApplication::desktop()->x11Display();
	if (XkbQueryExtension(dpy, &op, &event, &error, &major, &minor) == 0) {
		log (L_INFO,"XKeyboard::Unable to initialize XKB extension");
		return;
	}
	while (timeout > 0) {
		xkb = XkbGetKeyboard (
			dpy, XkbGBN_AllComponentsMask, XkbUseCoreKbd
		);
		if (xkb == NULL || xkb->geom == NULL) {
			timeout -= 1; usleep(100);
		} else {
			break;
		}
	}
	if (timeout <= 0) {
		log (L_INFO,"XKeyboard::Couldn't get keyboard\n");
		return;
	}
	XkbGetControls (dpy, XkbRepeatKeysMask, xkb);
	xkb->ctrls->repeat_delay = mDelay->value();
	xkb->ctrls->repeat_interval = 1000/mRepeat->value();
	XkbSetControls (
		dpy, XkbRepeatKeysMask, xkb
	);
	XSync (dpy, False);
}

//=====================================
// XKeyboard apply keyboard
//-------------------------------------
bool XKeyboard::apply ( void ) {
	// log(L_INFO,"XKeyboard::apply() called\n");
	// ...
	// apply the keyboard to the X-Server for immediate
	// usage. get the mFiles pointer wrapper first
    // ---
    QDict<XFile>* mFilePtr = mIntro->getFiles();
    XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	// ...
	// get the CDB pointer to the current used
	// keyboard type
	// ---
	XDb* pCDB = mFiles["cdb_KEYBOARDS"]->cdbGetSpec (
		mType->currentText(),"<...>"
	);
	if (pCDB) {
		QDict<char>* CDBData = NULL;
		CDBData = pCDB -> getHash();
		XWrapPointer< QDict<char> > selectedKeyboard ( CDBData );

		XApiData kbd = mFiles["ext_KLANGUAGE"]->getAPI();
		mKBDPtr = kbd.getHash();
		QString XkbLayout (mKBDPtr->operator[](mLayout->currentText()));
		XStringList completeLanguage (XkbLayout);
		completeLanguage.setSeperator ("+");
		QList<char> xf86Layout = completeLanguage.getList();

		QString symbols ( selectedKeyboard["ApplySymbols"]);
		if (QString (xf86Layout.at(0)) != "us") {
		if (mDeadKeys -> isChecked()) {
			symbols.sprintf("%s+%s(nodeadkeys)", 
			symbols.ascii(),xf86Layout.at(0)
			);
		} else {
			symbols.sprintf("%s+%s",
			symbols.ascii(),xf86Layout.at(0)
			);
		}
		}
		XkbComponentNamesRec  getcomps;
		QString keymap   (selectedKeyboard["ApplyKeymap"]);
		QString keycodes (selectedKeyboard["ApplyKeycodes"]);
		QString compat   (selectedKeyboard["ApplyCompat"]);
		QString types    (selectedKeyboard["ApplyTypes"]);
		QString geometry (selectedKeyboard["ApplyGeometry"]);
		getcomps.keymap   = (char*) keymap.ascii();
		getcomps.keycodes = (char*) keycodes.ascii();
		getcomps.compat   = (char*) compat.ascii();
		getcomps.types    = (char*) types.ascii();
		getcomps.geometry = (char*) geometry.ascii();
		getcomps.symbols  = (char*) symbols.ascii();
		#if 0
		printf("%s:%s:%s:%s:%s:%s\n",
			getcomps.keymap,getcomps.keycodes,getcomps.compat,
			getcomps.types,getcomps.symbols,getcomps.geometry
		);
		#endif
		XkbDescPtr xkb = XkbGetKeyboardByName (
			x11Display(), XkbUseCoreKbd, &getcomps,
            XkbGBN_AllComponentsMask, 0, True
		);
		if (! xkb) {
			return (false);
		}
		return ( true );
	}
	return ( false );
}
