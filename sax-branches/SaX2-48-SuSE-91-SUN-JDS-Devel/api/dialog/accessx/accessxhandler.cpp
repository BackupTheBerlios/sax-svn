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
              : configure accessx for XFree86. accessx provide
              : the possibility to handle the mouse via the
              : number pad
              :
              : --> signal action slots/callbacks
              :
STATUS        : Status: Up-to-date
**************/
#include "accessx.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XAccessX reset and switch to Intro
//-------------------------------------
void XAccessX::resetPage (int reload) {
	// ...
	// this function is called if the xaccess dialog is finished or canceled
	// AccessX is an X11 extension which need a seperate configuration file
	// which is outside of the normal configuration create via ISaX. Therefore
	// we don`t need to serialize any data here
	// ---
	if (reload == PAGE_RELOAD) {
	if (mEnable -> isChecked()) {
		QFile* mHandle = new QFile (
			"/usr/X11R6/lib/X11/xkb/X0-config.keyboard"
		);
		if (! mHandle -> open(IO_WriteOnly)) {
		log (L_ERROR,
			"XAccessX::open failed on: %s -> %s\n",
			mHandle->name().ascii(),strerror(errno)
		);
		}
		QString* speed = new QString();
		speed -> sprintf ( 
			"%d",mLCD -> intValue()
		);
		QDict<char> data;
		data.insert ("MouseKeysMaxSpeed=" , 
			speed->ascii()
		);
		data.insert ("MouseKeysDelay="    , "40");
		data.insert ("MouseKeysInterval=" , "10" );
		data.insert ("MouseKeysTimeToMax=", "1000" );
		data.insert ("MouseKeysCurve="    , "0"  );
		data.insert ("Controls+=",
			"MouseKeysAccel + AccessxKeys"
		);
		data.insert ("Feedback+=",
			"SlowKeysPress + SlowKeysAccept + StickyKeys + LatchToLock"
		);
		QDictIterator<char> it (data);
		for (; it.current(); ++it) {
			QString line;
			line.sprintf ("%s %s\n",
				it.currentKey().ascii(),it.current()
			);
			mHandle -> writeBlock (
				line.ascii(),line.length()
			);
		}
		mHandle -> close();
	}
	}
	if (reload == PAGE_RELOAD) {
		slotApply();
	}
	mStatus -> clear();
	slotIntro (mIndex);
	XTemplate::resetPage ();
	mStack -> raiseWidget (Intro);
}

//=====================================
// XAccessX internal page called...
//-------------------------------------
void XAccessX::pageCalled (int index) {
	// ...
	// this function is called if the next page of the dialog
	// is called. Note if the dialog has only one page this function
	// will not be called because there is no next page in this case
	// ---
	switch (index) {
	case XAccessPage1: {
		break;
	}
	};
}

//=====================================
// XAccessX slotRun...
//-------------------------------------
bool XAccessX::slotRun (int index) {
	if (XTemplate::slotRun (index)) {
	// log(L_INFO,"XAccessX::slotRun() called: %d\n",index);
	// ...
	// this function is called if the xaccess page is activated.
	// use this function to init the dialog with the current
	// setup of the accessx status information
	// ---
	XWrapPointer< QDict<char> > mText (mTextPtr);
	mStatus -> message (mText["RunXAccess"]);
	mFrame  -> nextButton() -> setText (mText["finish"]);
	// ...
	// if AccessX is active we will set the current speed
	// value here otherwhise the slider is disabled
	// ---
	XkbGetControls(x11Display(), XkbAllControlsMask, xkb);
	if (xkb->ctrls == NULL) {
		xkb->ctrls = (XkbControlsPtr)malloc(sizeof(XkbControlsRec));
		isActive = FALSE;
	} else {
	if (XACCESS_CODE(xkb)) {
		isActive = TRUE;
	} else {
		isActive = FALSE;
	}
	}
	if (isActive) {
		slotState (true);
		mEnable -> setChecked  ( true );
		mSpeed  -> setValue ( xkb->ctrls->mk_max_speed );
	} else {
		mEnable  -> setChecked  ( false );
		mDisable -> setChecked  ( true );
		slotState (false);
	}
	}
	return (TRUE);
}

//=====================================
// XAccessX init page with data infos 
//-------------------------------------
void XAccessX::initPage (void) {
	// log (L_INFO,"XAccessX::initPage() called\n");
	// ...
	// this function is called after the page was created.
	// It must only be used to init the widgets contents
	// with the data available for selections and other stuff
	// like that
    // ---	
	// Query XKB Extension...
	// -----------------------
	int timeout = 5;
	major = XkbMajorVersion;
	minor = XkbMinorVersion;
	if (XkbQueryExtension(
		x11Display(), &op, &event, &error, &major, &minor) == 0
	) {
	log(L_WARN,
		"XKB: Unable to initialize XKEYBOARD extension\n"
	);
	}
	while (timeout > 0) {
		xkb = XkbGetKeyboard(
			x11Display(), XkbGBN_AllComponentsMask, XkbUseCoreKbd
		);
		if (xkb == NULL || xkb->geom == NULL) {
		timeout -= 1; sleep(1);
		} else {
		break;
		}
	}
	if (timeout <= 0) {
	log (L_WARN,
		"XKB: Couldn't get keyboard\n"
	);
	} 
}

//=====================================
// XAccessX sync LCD value with slider  
//-------------------------------------
void XAccessX::slotLCD (int value) {
	mLCD -> display (value);
}

//=====================================
// XAccessX apply access X
//-------------------------------------
void XAccessX::slotApply (void) {
	// log (L_INFO,"XAccessX::slotApply() called\n");
	// ...
	// this function is called if the apply button
	// is pressed. We will check the check button
	// if accessX should be enabled/disabled
	// ---
	bool active = true;
	xkb->ctrls->enabled_ctrls |= 
		XkbMouseKeysMask | XkbMouseKeysAccelMask;
	// ...
	// standard values
	// ---
	xkb->ctrls->mk_delay       = 40;
	xkb->ctrls->mk_interval    = 10;
	xkb->ctrls->mk_time_to_max = 1000;
	xkb->ctrls->mk_curve       = 0;
	if (mEnable -> isChecked()) {
		xkb->ctrls->mk_max_speed  = mLCD -> intValue();
	} else {
		xkb->ctrls->enabled_ctrls &= 
			~(XkbMouseKeysMask | XkbMouseKeysAccelMask);
		active = false;
	}
	// ...
	// set controls now
	// ---
	XkbSetControls (
		x11Display(), XkbAllControlsMask, xkb
	);
	XSync (x11Display(), False);
	XWrapPointer< QDict<char> > mText (mTextPtr);
	QString message;
	if (active) {
		message = mText["ACCXEnabled"];
	} else {
		message = mText["ACCXDisabled"];
	}
	XBox mb (
		mText["ACCXInfo"],message,
		XBox::Information,
		XBox::Yes,
		0,0,mFrame,
		globalFrameWidth
	);
	mb.setButtonText (
		XBox::Yes,mText["Ok"]
	);
	mb.exec();
	mFrame -> enterEvent ( 0 );
}

//=====================================
// XAccessX adapt slider change
//-------------------------------------
void XAccessX::slotState (bool on) {
	// ...
	// this function is called if the check box value has changed. 
	// We will enable/disable the slider than
	// ---
	if (on) {
		mControl -> setDisabled ( FALSE );
		mSpeed   -> setDisabled ( FALSE );
		mLCD     -> setDisabled ( FALSE );
	} else {
		mControl -> setDisabled ( TRUE );
		mSpeed   -> setDisabled ( TRUE );
		mLCD     -> setDisabled ( TRUE );
	}
}
