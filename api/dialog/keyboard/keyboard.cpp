/**************
FILE          : keyboard.c
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
              :
STATUS        : Status: Up-to-date
**************/
#include "keyboard.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XKeyboard Constructor...
//-------------------------------------
XKeyboard::XKeyboard (void) {
}

//=====================================
// XKeyboard Constructor...
//-------------------------------------
XKeyboard::XKeyboard (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
    addTo (xf,xi);
}

//=====================================
// XKeyboard add widget to mainframe...
//-------------------------------------
void XKeyboard::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Keyboard;
    mPage     = mIndex;
    mLastPage = KeyboardPage1;
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XKeyboard create the dialog...
//-------------------------------------
void XKeyboard::dialogCreate (void) {
	mDialog = new QFrame ( mStack );
	mTop    = new XTabDialog ( mFrame,"",TRUE,0,globalFrameWidth );
	XWrapPointer< QDict<char> > mText (mTextPtr);

	// first page dialog
	// -----------------
	QBoxLayout* layer0 = new QVBoxLayout ( mDialog );
	QHBox* type = new QHBox ( mDialog );
	QLabel* pixlayout = new QLabel ( type );
	pixlayout -> setPixmap (
		QPixmap (mInit["pix_kbd_layout"])
	);
	QLabel* typeText = new QLabel ( type );
	typeText -> setText (
		mText["kbdtype"]
	);
	type -> setSpacing ( 10 );
	type -> setStretchFactor ( typeText,10 );
	QHBox* selectType = new QHBox ( mDialog );
	mType = new QComboBox ( FALSE,selectType );
	selectType -> setSpacing ( 10 );
	mOption = new QPushButton ( mText["kbdoption"],selectType );
	selectType -> setStretchFactor ( mType,10 );
	QLabel* layoutText = new QLabel ( mDialog );
	layoutText -> setText (mText["kbdlayout"]);
	mLayout   = new QListBox  ( mDialog );
	mDeadKeys = new QCheckBox ( mDialog );
	mDeadKeys -> setText (
		mText["deadkey"]
	);
	QLabel* testText = new QLabel( mDialog );
	testText -> setText (
		mText["kbdtest"]
	);
	mTest = new QLineEdit ( mDialog );

	// Options XTab Dialog
	// -------------------
	QVBox* layer1 = new QVBox ( mTop );
	layer1 -> setMargin  (20);
	QHBox* shiftLockBox  = new QHBox ( layer1 );
	layer1 -> setSpacing (10);
	QHBox* ctrlMapBox    = new QHBox ( layer1 );
	QHBox* dummyBox1 = new QHBox ( layer1 );
	QFrame* line = new QFrame ( dummyBox1 );
	line -> setFrameStyle (
		QFrame::HLine|QFrame::Raised
	);
	layer1 -> setStretchFactor  ( dummyBox1,2 );
	layer1 -> setSpacing (40);
	QHBox* leftAltBox    = new QHBox ( layer1 );
	layer1 -> setSpacing (10);
	QHBox* rightAltBox   = new QHBox ( layer1 );
	layer1 -> setSpacing (10);
	QHBox* scrollLockBox = new QHBox ( layer1 );
	layer1 -> setSpacing (10);
	QHBox* rightCtrlBox  = new QHBox ( layer1 );
	QHBox* dummyBox2 = new QHBox ( layer1 );
	layer1 -> setStretchFactor  ( dummyBox2,4 );

	QLabel* shiftLockLabel  = new QLabel ( shiftLockBox );
	shiftLockLabel -> setText (
		mText["ShiftLockGroup"]
	);
	shiftLockLabel -> setFixedWidth ( 150 );
	mXkbOption[0] = new QComboBox ( FALSE,shiftLockBox );
	shiftLockBox -> setStretchFactor ( mXkbOption[0],2 );
	QLabel* ctrlMapLabel    = new QLabel ( ctrlMapBox );
	ctrlMapLabel -> setText (
		mText["CtrlPos"]
	);
	ctrlMapLabel -> setFixedWidth ( 150 );
	mXkbOption[1] = new QComboBox ( FALSE,ctrlMapBox );
	ctrlMapBox -> setStretchFactor ( mXkbOption[1],2 );
	QLabel* leftAltLabel    = new QLabel ( leftAltBox );
	leftAltLabel -> setText (
		mText["LeftAlt"]
	);
	leftAltLabel -> setFixedWidth ( 150 );
	mXkbOption[2] = new QComboBox ( FALSE,leftAltBox );
	leftAltBox -> setStretchFactor ( mXkbOption[2],2 );
	QLabel* rightAltLabel   = new QLabel ( rightAltBox );
	rightAltLabel -> setText (
		mText["RightAlt"]
	);
	rightAltLabel -> setFixedWidth ( 150 );
	mXkbOption[3] = new QComboBox ( FALSE,rightAltBox );
	rightAltBox -> setStretchFactor ( mXkbOption[3],2 );
	QLabel* scrollLockLabel = new QLabel ( scrollLockBox );
	scrollLockLabel -> setText (
		mText["ScrollLock"]
	);
	scrollLockLabel -> setFixedWidth ( 150 );
	mXkbOption[4] = new QComboBox ( FALSE,scrollLockBox );
	scrollLockBox -> setStretchFactor ( mXkbOption[4],2 );
	QLabel* rightCtrlLabel  = new QLabel ( rightCtrlBox );
	rightCtrlLabel -> setText (
		mText["RightCtl"]
	);
	rightCtrlLabel -> setFixedWidth ( 150 );
	mXkbOption[5] = new QComboBox ( FALSE,rightCtrlBox );
	rightCtrlBox -> setStretchFactor ( mXkbOption[5],2 );

	QString* xkbDefault = new QString (
		mText["Default"]
	);
	QString* grpSwitch  = new QString (
		mText["R-Alt-switch-group-while-pressed"]
	);
	QString* grpToggle = new QString (
		mText["R-Alt-switch-group"]
	);
	QString* grpShiftToggle = new QString (
		mText["Both-Shift-Keys-change-group"]
	);
	QString* grpCtrlShiftToggle = new QString (
		mText["Ctrl-and-Shift-change-group"]
	);
	QString* grpCtrlAltToggle = new QString (
		mText["Alt-and-Ctrl-change-group"]
	);
	mTranslateOption.insert ( "default",    *xkbDefault );
	mTranslateOption.insert ( "grp:switch", *grpSwitch );
	mTranslateOption.insert ( "grp:toggle", *grpToggle );
	mTranslateOption.insert ( "grp:shift_toggle",     *grpShiftToggle );
	mTranslateOption.insert ( "grp:ctrl_shift_toggle",*grpCtrlShiftToggle );
	mTranslateOption.insert ( "grp:ctrl_alt_toggle",  *grpCtrlAltToggle );

	mXkbOption[0]->insertItem (mTranslateOption["default"]);
	mXkbOption[0]->insertItem (mTranslateOption["grp:switch"]);
	mXkbOption[0]->insertItem (mTranslateOption["grp:toggle"]);
	mXkbOption[0]->insertItem (mTranslateOption["grp:shift_toggle"]);
	mXkbOption[0]->insertItem (mTranslateOption["grp:ctrl_shift_toggle"]);
	mXkbOption[0]->insertItem (mTranslateOption["grp:ctrl_alt_toggle"]);

	QString* ctrlNocaps = new QString (
		mText["CapsLock-as-additional-control"]
	);
	QString* ctrlSwapCaps = new QString (
		mText["Swap-Ctrl-and-CapsLock"]
	);
	QString* ctrlCtrlAC = new QString (
		mText["Ctrl-Key-at-left-of-<A>"]
	);
	QString* ctrlCtrlAA = new QString (
		mText["Ctrl-Key-at-bottom-left"]
	);
	mTranslateOption.insert ( "ctrl:nocaps",  *ctrlNocaps );
	mTranslateOption.insert ( "ctrl:swapcaps",*ctrlSwapCaps);
	mTranslateOption.insert ( "ctrl:ctrl_ac", *ctrlCtrlAC );
	mTranslateOption.insert ( "ctrl:ctrl_aa", *ctrlCtrlAA );

	mXkbOption[1]->insertItem (mTranslateOption["default"]);
	mXkbOption[1]->insertItem (mTranslateOption["ctrl:nocaps"]);
	mXkbOption[1]->insertItem (mTranslateOption["ctrl:swapcaps"]);
	mXkbOption[1]->insertItem (mTranslateOption["ctrl:ctrl_ac"]);
	mXkbOption[1]->insertItem (mTranslateOption["ctrl:ctrl_aa"]);
	
	for (int i=2;i<6;i++) {
		mXkbOption[i]->insertItem (*xkbDefault);
		mXkbOption[i]->insertItem ("Meta");
		mXkbOption[i]->insertItem ("Compose");
		mXkbOption[i]->insertItem ("ModeShift");
		mXkbOption[i]->insertItem ("ModeLock");
		mXkbOption[i]->insertItem ("ScrollLock");
		mXkbOption[i]->insertItem ("Control");
	}

	// ...
	// AutoRepeat setting
	// ---
	QVBox* layer2 = new QVBox ( mTop );
	layer2 -> setMargin  (20);
	QButtonGroup* kbdRate = new QButtonGroup (
		1,Horizontal,mText["repeatrate"],layer2
	);
	QButtonGroup* kbdDelay = new QButtonGroup (
		1,Horizontal,mText["delaytime"],layer2
	);
	QHBox* repeat = new QHBox ( kbdRate );
	mRepeat = new QSlider (
		5,80,1,XKBDRATE_DEFAULT,QSlider::Horizontal,repeat
	);
	mRepeat -> setTickmarks    ( QSlider::Below );
	mRepeat -> setTickInterval ( 5 );
	mRepeatLCD = new QLCDNumber( 
		3,repeat 
	);
	mRepeatLCD -> setSegmentStyle ( QLCDNumber::Filled );
	mRepeatLCD -> setPalette      ( QPalette( Qt::darkGray, Qt::darkGray ) );
	mRepeatLCD -> setFixedWidth   ( 60 );
	mRepeatLCD -> setFixedHeight  ( 30 );
	mRepeatLCD -> display         ( XKBDRATE_DEFAULT );
	// ...
	layer2 -> setSpacing (10);
	QHBox* delay  = new QHBox ( kbdDelay );
	mDelay = new QSlider (
		50,1000,5,XKBDDELAY_DEFAULT,QSlider::Horizontal,delay
	);
	mDelay -> setTickmarks    ( QSlider::Below );
	mDelay -> setTickInterval ( 20 );
	mDelayLCD = new QLCDNumber(    
		3,delay 
	);
	mDelayLCD -> setSegmentStyle ( QLCDNumber::Filled );
	mDelayLCD -> setPalette      ( QPalette( Qt::darkGray, Qt::darkGray ) );
	mDelayLCD -> setFixedWidth   ( 60 );
	mDelayLCD -> setFixedHeight  ( 30 );
	mDelayLCD -> display         ( XKBDDELAY_DEFAULT );
	// ...
	QObject::connect ( 
		mRepeat    , SIGNAL ( valueChanged   ( int ) ),
		mRepeatLCD , SLOT   ( display        ( int ) )
	);
	QObject::connect (
		mDelay     , SIGNAL ( valueChanged   ( int ) ),
		mDelayLCD  , SLOT   ( display        ( int ) )
	);
	QObject::connect (
		mRepeat    , SIGNAL ( sliderReleased ( void ) ),
		this       , SLOT   ( autoRepeat     ( void ) )
	);
	QObject::connect (
		mDelay     , SIGNAL ( sliderReleased ( void ) ),
		this       , SLOT   ( autoRepeat     ( void ) )
	);
	// ...
	layer2 -> setSpacing (10);
	QFontMetrics metrics ( font() );
	QLabel* autoRepeatTest = new QLabel( layer2 );
	autoRepeatTest -> setFixedHeight (metrics.height() + 5);
	autoRepeatTest -> setText (
		mText["kbdtest"]
	);
	new QLineEdit ( layer2 );

	// Add widgets to tab dialog
	// -------------------------
	mTop -> addTab ( layer1,mText["kbdmapping"] );
	mTop -> addTab ( layer2,mText["kbdrepeat"] );

	// set OK/Cancel buttons...
	// ------------------------
	mTop -> setOkButton      ( mText["Ok"] );
	mTop -> setCancelButton  ( mText["Cancel"] );

	// connect me to the world
	// -----------------------
	QObject::connect (
		mLayout   , SIGNAL ( clicked      (QListBoxItem *) ),
		this      , SLOT   ( slotSelect   (QListBoxItem *) )
	);
	QObject::connect (
		mType     , SIGNAL ( activated    (int) ),
		this      , SLOT   ( slotType     (int) )
	);
	QObject::connect (
		mOption   , SIGNAL ( clicked    () ),
		this      , SLOT   ( slotOption () )
	);
	QObject::connect (
		mDeadKeys , SIGNAL ( toggled      (bool) ),
		this      , SLOT   ( slotVariant  (bool) )
	);
	QObject::connect (
		mTop, SIGNAL ( applyButtonPressed() ),
		this, SLOT   ( slotTopOk() )
	);
	QObject::connect (
		mTop, SIGNAL ( cancelButtonPressed() ),
		this, SLOT   ( slotTopCancel() )
	);

	// add widgets to layout
	// ---------------------
	layer0 -> addWidget  ( type );
	layer0 -> addSpacing ( 5 );
	layer0 -> addWidget  ( selectType );
	layer0 -> addSpacing ( 10 );
	layer0 -> addWidget  ( layoutText );
	layer0 -> addWidget  ( mLayout );
	layer0 -> addSpacing ( 5 );
	layer0 -> addWidget  ( mDeadKeys );
	layer0 -> addSpacing ( 10 );
	layer0 -> addWidget  ( testText );
	layer0 -> addWidget  ( mTest );
	layer0 -> setMargin  ( 5 );

	initPage();

	mStack -> addWidget  ( mDialog , mIndex );
}

//=====================================
// XKeyboard slotRun...
//-------------------------------------
void XKeyboard::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> keyboard;
	XData*      data;

	info    = mIntro -> getTextView();
	message = mText["keyboard_header"];
	info    -> setText (message);
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";
	message += idents;

	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	data = mFiles["sys_KEYBOARD"] -> getDevice(0);
	if (! data) {
		return;
	}
	keyboard = data -> getData();

	// first row
	message += "<tr>";
	QTextOStream(&idents) << "<td width=30%>" << mText["model"] << "</td>";
	message += idents;
	idents.sprintf("<td width=60%c>%s</td>",'%',keyboard["MapName"]);
	message += idents;
	message += "</tr>";	

	// second row
	message += "<tr>";
	QTextOStream(&idents) << "<td width=30%>" << mText["klayout"] << "</td>";
	message += idents;
	idents.sprintf("<td width=60%c>%s</td>",'%',keyboard["XkbLayout"]);
	message += idents;
	message += "</tr>";

	// end table...
	message += "</table>";
	info -> setText (message);
	}
}
