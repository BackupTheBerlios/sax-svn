/**************
FILE          : vnchandler.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure VNC for X11. The VNC protocol provides
              : the possibility to export the X-Server desktop
              : for remote access
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "vnc.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// Xvnc reset and switch to Intro
//-------------------------------------
void Xvnc::resetPage (int reload) {
	// ...
	// this function is called if the VNC dialog is finished or 
	// canceled The changes are serialized and re-imported if the user want
	// to conclude the dialog. Otherwhise the original file is imported
	// and the changes will be lost.
	// ---
	if (reload == PAGE_RELOAD) {
	if (! saveConfiguration() ) {
		return;
	}
	}

	QString update = "sys_PATH";

	mStatus -> clear();
	if (reload == PAGE_RELOAD) {
		mFilePtr = mIntro->getFiles();
		XWrapFile < QDict<XFile> > mFiles (mFilePtr);
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_CARD";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_LAYOUT";
		if (mFiles [update] -> sysSerialize()) {
			mFiles [update] -> isModified ( mFrame );
		}
		update = "sys_INPUT";
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
// Xvnc internal page called...
//-------------------------------------
void Xvnc::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case XVncPage1: {
		break;
	}
	};
}

//=====================================
// Xvnc slotRun...
//-------------------------------------
bool Xvnc::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"Xvnc::slotRun() called: %d\n",index);
	// ...
	// this function is called if the VNC page is activated.
	// use this function to init the dialog with the current
	// setup of the VNC status information
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXVNC"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);

	bool hasVNCconfig  = false;
	bool hasRFBAuth    = false;
	bool isViewOnly    = false;
	bool isShared      = false;
	bool hasHTTPaccess = false;

	if ( getDeviceOption ("usevnc") == "yes" ) {
		hasVNCconfig = true;
		VNCPrepared  = true;
	}
	if ( ! getDeviceOption ("rfbauth").isEmpty() ) {
		hasRFBAuth = true;
	}
	if ( getDeviceOption ("viewonly") == "viewonly" ) {
		isViewOnly = true;
	}
	if ( getDeviceOption ("alwaysshared") == "alwaysshared" ) {
		isShared = true;
	}
	if ( ! getDeviceOption ("httpport").isEmpty() ) {
		hasHTTPaccess = true;
	}

	mPWD = mText ["newpwd"];
	mCheckViewOnly -> setChecked (false);
	mCheckShared   -> setChecked (false);
	mCheckHTTP -> setChecked (false);
	mCheckVNC  -> setChecked (false);
	mCheckPWD  -> setChecked (false);

	if (hasVNCconfig) {
		//=====================================
		// enable VNC checkbox
		//-------------------------------------
		mCheckVNC -> setChecked (true);
		//=====================================
		// check for authentification password
		//-------------------------------------
		if (hasRFBAuth) {
			mCheckPWD -> setChecked (true);
			mPWD1 -> setText ( mText["pwdchange"] );
			mPWD1 -> setEchoMode ( QLineEdit::Normal );
			mPWD2 -> setDisabled ( true );
			mPWD = mText ["pwdchange"];
		}
		//=====================================
		// check for view only mode
		//-------------------------------------
		if (isViewOnly) {
			mCheckViewOnly -> setChecked (true);
		}
		//=====================================
		// check for multiple VNC connections
		//-------------------------------------
		if (isShared) {
			mCheckShared -> setChecked (true);
		}
		//=====================================
		// check for HTTP access status
		//-------------------------------------
		if (hasHTTPaccess) {
			QString port = getDeviceOption ("httpport");
			mCheckHTTP -> setChecked (true);
			mHTTP -> setValue (port.toInt());
		}
	}
	if ( noInputConnection ) {
		QObject::connect (
			mPWD1 , SIGNAL ( textChanged       ( const QString &) ),
			this  , SLOT   ( slotInput1Changed ( const QString &) )
		);
		noInputConnection = false;
	}
	}
	return (TRUE);
}

//=====================================
// Xvnc init page with data infos 
//-------------------------------------
void Xvnc::initPage (void) {
	// log (L_INFO,"Xvnc::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
    // ---	
}

//=====================================
// Xvnc enable/disable VNC
//-------------------------------------
void Xvnc::slotState (bool on) {
	// ...
	// this function is called if the check box value has changed. 
	// We will enable/disable VNC according to the state 
	// ---
	if (on) {
		mOptions -> setDisabled (false);
	} else {
		mOptions -> setDisabled (true);
	}
}

//=====================================
// Xvnc enable/disable PWD protection
//-------------------------------------
void Xvnc::slotPassword (bool on) {
	// ...
	// this function is called if the check box value has changed. 
	// We will enable/disable VNC according to the state 
	// ---
	if (on) {
		mEntries -> setDisabled (false);
	} else {
		mEntries -> setDisabled (true);
	}
}

//=====================================
// Xvnc enable/disable HTTP setup
//-------------------------------------
void Xvnc::slotHTTP (bool on) {
	// ...
	// this function is called if the check box value has changed. 
	// We will enable/disable HTTP according to the state 
	// ---
	if (on) {
		mHTTPEntries -> setDisabled (false);
	} else {
		mHTTPEntries -> setDisabled (true);
	}
}

//=====================================
// Xvnc save configuration sections
//-------------------------------------
bool Xvnc::saveConfiguration (void) {
	// ...
	// this function will create all the needed InputDevice
	// sections and options to already existing sections
	// to be able to export the display as VNC server
	// ---
	mFilePtr = mIntro->getFiles();
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	if (mCheckPWD -> isOn()) {
		if (mPWD2 -> isEnabled()) {
		if (mPWD1->text() != mPWD2->text()) {
			setMessage ("pwdmismatch",XBox::Critical);
			mPWD1->clear();
			mPWD2->clear();
			return false;
		} else if (mPWD1->text().length() < 6) {
			setMessage ("pwdtooshort",XBox::Critical);
			return false;
		} else {
			qx ( GETVNCPASSWORD,STDNONE,1,"%s",mPWD1->text().ascii() );
			resetDeviceRawOption ("rfbauth","/root/.vnc/passwd");
		}
		}
	} else {
		unsetDeviceRawOption ("rfbauth");
	}

	if (! VNCPrepared) {
		//=====================================
		// get number of input devices
		//-------------------------------------
		int inputDevices = mFiles["sys_INPUT"]->getDeviceCount();
		int mouseID = inputDevices * 2 + 1;
		int kbdID   = inputDevices * 2;

		//=====================================
		// create new VNC mouse and keyboard
		//-------------------------------------
		XWrapPointer<XData> newKeyboard;
		newKeyboard.init (
			mFiles["sys_INPUT"]->addDevice (inputDevices + 1)
		);
		XWrapPointer<XData> newMouse;
		newMouse.init (
			mFiles["sys_INPUT"]->addDevice (inputDevices + 2)
		);

		//=====================================
		// setup data for new VNC mouse / kbd
		//-------------------------------------
		QString* identmouse = new QString;
		identmouse -> sprintf ("Mouse[%d]",mouseID);
		newMouse.setPair ("InputFashion","VNC");
		newMouse.setPair ("Identifier",identmouse->ascii());
		newMouse.setPair ("Driver","rfbmouse");
	
		QString* identkbd = new QString;
		identkbd -> sprintf ("Keyboard[%d]",kbdID);
		newKeyboard.setPair ("InputFashion","VNC");
		newKeyboard.setPair ("Identifier",identkbd->ascii());
		newKeyboard.setPair ("Driver","rfbkeyb");

		//=====================================
		// include vnc module to be loaded
		//-------------------------------------
		QString* mModuleList = new QString;
		XWrapPointer<XData> path ( mFiles["sys_PATH"]->getDevice (0) );
		if (! path["ModuleLoad"].contains ("vnc")) {
			mModuleList -> sprintf ("%s,vnc",path["ModuleLoad"].ascii());
			path.setPair ("ModuleLoad",mModuleList->ascii());
		}

		//=====================================
		// add flag to server layout
		//-------------------------------------
		QString* vncid = new QString;
		vncid -> sprintf ("%d %d",mouseID,kbdID);
		XWrapPointer<XData> serverLayout (
			mFiles["sys_LAYOUT"] -> getDevice(0)
		);
		serverLayout.setPair ("VNC",vncid->ascii());
		VNCPrepared = true;
	}

	//=====================================
	// set "usevnc" option to yes/no
	//-------------------------------------
	if (mCheckVNC -> isOn()) {
		resetDeviceRawOption ("usevnc","yes");
	} else {
		resetDeviceRawOption ("usevnc","no");
	}

	//=====================================
	// handle VNC options
	//-------------------------------------
	if (mCheckViewOnly -> isOn()) {
		resetDeviceOption ("viewonly");
	} else {
		unsetDeviceOption ("viewonly");
	}
	if (mCheckShared -> isOn()) {
		resetDeviceOption ("alwaysshared");
		unsetDeviceOption ("nevershared");
	} else {
		unsetDeviceOption ("alwaysshared");
		unsetDeviceOption ("nevershared");
	}
	if (mCheckHTTP -> isOn()) {
		resetDeviceRawOption ("httpdir","/usr/share/vnc/classes");
		resetDeviceRawOption ("httpport",mHTTP->text());
	} else {
		unsetDeviceRawOption ("httpport");
		unsetDeviceRawOption ("httpdir");
	}
	return true;
}

//=====================================
// Xvnc set option to all devices
//-------------------------------------
void Xvnc::setDeviceOption (QString value) {
	// ...
	// this function will set the given option to all
	// known device sections
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		if (! mFiles["sys_CARD"]->getDevice (n)) {
			continue;
		}
		XWrapPointer<XData> card ( mFiles["sys_CARD"]->getDevice (n) );
		QString* options = new QString;
		options -> sprintf ("%s,%s",card["Option"].ascii(),value.ascii());
		card.setPair ("Option",options->ascii());
	}
}

//=====================================
// Xvnc set raw option to all devices
//-------------------------------------
void Xvnc::setDeviceRawOption (QString key,QString value) {
	// ...
	// this function will set the given option to all
	// known device sections
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		if (! mFiles["sys_CARD"]->getDevice (n)) {
			continue;
		}
		XWrapPointer<XData> card ( mFiles["sys_CARD"]->getDevice (n) );
		QString* options = new QString;
		options -> sprintf ("%s,Option \"%s\" \"%s\"",
			card["RawData"].ascii(),key.ascii(),value.ascii()
		);
		card.setPair ("RawData",options->ascii());
	}
}

//=====================================
// Xvnc unset option in all devices
//-------------------------------------
void Xvnc::unsetDeviceOption (QString key) {
	// ...
	// this function will unset the given option in all
	// known device sections
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		if (! mFiles["sys_CARD"]->getDevice (n)) {
			continue;
		}
		XWrapPointer<XData> card ( mFiles["sys_CARD"]->getDevice (n) );
		QStringList optlist = QStringList::split ( ",", card["Option"] );
		QStringList result;
		for ( QStringList::Iterator 
			in = optlist.begin(); in != optlist.end(); ++in
		) {
			QString item (*in);
			if (item == key) {
				continue;
			}
			result.append (item);
		}
		QString* options = new QString;
		*options = result.join (",");
		card.setPair ("Option",options->ascii());
	}
}

//=====================================
// Xvnc unset raw option in all devices
//-------------------------------------
void Xvnc::unsetDeviceRawOption (QString key) {
	// ...
	// this function will unset the given raw option in
	// all known device sections
	// ---
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		if (! mFiles["sys_CARD"]->getDevice (n)) {
			continue;
		}
		XWrapPointer<XData> card ( mFiles["sys_CARD"]->getDevice (n) );
		QStringList optlist = QStringList::split ( ",", card["RawData"] );
		QStringList result;
		for ( QStringList::Iterator
			in = optlist.begin(); in != optlist.end(); ++in
		) {
			bool handleOption = true;
			QString item (*in);
			QStringList optitems = QStringList::split ( "\"", item );
			for ( QStringList::Iterator
				in = optitems.begin(); in != optitems.end(); ++in
			) {
				QString part (*in);
				part = part.stripWhiteSpace();
				if (part == key) {
					handleOption = false;
					break;
				}
			}
			if (handleOption) {
				result.append (item);
			}
		}
		QString* options = new QString;
		*options = result.join (",");
		card.setPair ("RawData",options->ascii());
	}
}

//=====================================
// Xvnc return option value
//-------------------------------------
QString Xvnc::getDeviceOption (QString key) {
	// ...
	// this function returns the value for an option
	// if the option has no value an empty string is
	// returned within the QString object
	// ---
	QString result;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		if (! mFiles["sys_CARD"]->getDevice (n)) {
			continue;
		}
		XWrapPointer<XData> card ( mFiles["sys_CARD"]->getDevice (n) );
		QStringList optlist = QStringList::split ( ",", card["Option"] );
		for ( QStringList::Iterator
			in = optlist.begin(); in != optlist.end(); ++in
		) {
			QString item (*in);
			if (item == key) {
				return key;
			}
		}
	}
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		if (! mFiles["sys_CARD"]->getDevice (n)) {
			continue;
		}
		XWrapPointer<XData> card ( mFiles["sys_CARD"]->getDevice (n) );
		QStringList optlist = QStringList::split ( ",", card["RawData"] );
		for ( QStringList::Iterator
			in = optlist.begin(); in != optlist.end(); ++in
		) {
			QString item (*in);
			QStringList optitems = QStringList::split ( "\"", item );
			bool handlePart = false;
			for ( QStringList::Iterator
				in = optitems.begin(); in != optitems.end(); ++in
			) {
				QString part (*in);
				part = part.stripWhiteSpace();
				if ((part.isEmpty()) || (part == "Option")) {
					continue;
				}
				if (part == key) {
					handlePart = true;
					continue;
				}
				if (handlePart) {
					return part;
				}
			}
		}
	}
	return result;
}

//=====================================
// Xvnc unset/set raw option 
//-------------------------------------
void Xvnc::resetDeviceRawOption (QString key,QString value) {
	// ...
	// this function combines unset and set for raw
	// option values
	// ---
	unsetDeviceRawOption (key);
	setDeviceRawOption   (key,value);
}

//=====================================
// Xvnc unset/set option 
//-------------------------------------
void Xvnc::resetDeviceOption (QString value) {
	// ...
	// this function combines unset and set for simple
	// option values
	// ---
	unsetDeviceOption (value);
	setDeviceOption   (value);
}

//=====================================
// Xvnc create password 
//-------------------------------------
QString Xvnc::cryptPassword (QString phrase) {
	// ...
	// this function will create standard encrypted
	// DES Unix password
	// ---
	char salt[9];
	srand ((int) time((time_t *) NULL));
	ap_to64 (&salt[0], rand(), 8);
	salt[8] = '\0';
	QString encrypt ( crypt (phrase,salt) );
	return encrypt;
}

//=====================================
// Xvnc create salt for password 
//-------------------------------------
void Xvnc::ap_to64 (char *s, unsigned long v, int n) {
	// ...
	// this function will create the salt for the later
	// crypt() call from a random identifier (v). The
	// parameter (n) limits the lenght of the salt which
	// is 8 characters for standard Unix passwords
	// ---
	static unsigned char itoa64[] =         /* 0 ... 63 => ASCII - 64 */
	"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	while (--n >= 0) {
		*s++ = itoa64[v&0x3f];
		v >>= 6;
	}
}

//=====================================
// Xvnc set OK message box...
//-------------------------------------
void Xvnc::setMessage (
	const QString& textKey,XBox::Icon icon,const QString& addon,int width
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
	if (width) {
		mb.setFixedWidth (width);
	}
	mb.setButtonText (
		XBox::Ok, mText["Ok"]
	);
	mb.exec();
}

//=====================================
// Xvnc PWD changed...
//-------------------------------------
void Xvnc::slotInput1Changed (const QString& data) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	if (mPWD1 -> echoMode() != QLineEdit::Password) {
		mPWD1 -> setEchoMode ( QLineEdit::Password );
		mPWD1 -> clear();
		if (! mPWD2 -> isEnabled()) {
			mPWD2 -> setDisabled (false);
		}
		QChar start;
		for (unsigned int i=0;i< data.length();i++) {
		if (data.at(i) != mPWD.at(i)) {
			start = data.at(i);
			break;
		}
		}
		mPWD1 -> setText (start);
	}
}
