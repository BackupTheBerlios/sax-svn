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
	QVBox* selectType = new QVBox ( mDialog );
	QButtonGroup* primary = new QButtonGroup (
		1,Horizontal,mText["kbdprimary"],selectType
	);
	selectType -> setSpacing (10);
	QButtonGroup* additional = new QButtonGroup (
		1,Horizontal,mText["kbdadditional"],selectType
	);
	selectType -> setStretchFactor ( additional, 100 );
	QHBox* primaryBox = new QHBox ( primary );
	QVBox* lBox = new QVBox ( primaryBox );
	primaryBox -> setSpacing ( 10 );
	QVBox* rBox = new QVBox ( primaryBox );
	QHBox* typeBox    = new QHBox ( lBox );
	QHBox* layoutBox  = new QHBox ( lBox );
	QHBox* variantBox = new QHBox ( lBox );
	
	QLabel* typeLabel = new QLabel (
		mText["kbdtypelabel"],typeBox
	);
	typeLabel -> setFixedWidth ( 100 );
	mType    = new QComboBox ( FALSE,typeBox );
	mType -> setMaximumWidth ( 480 );
	QLabel* layoutLabel = new QLabel (
		mText["kbdlayoutlabel"],layoutBox
	);
	layoutLabel -> setFixedWidth ( 100 );
	mLayout  = new QComboBox ( FALSE,layoutBox );
	mLayout -> setMaximumWidth ( 480 );
	QLabel* variantLabel = new QLabel (
		mText["kbdvariantlabel"],variantBox
	);
	variantLabel -> setFixedWidth ( 100 );
	mVariant = new QComboBox ( FALSE,variantBox );
	mVariant -> setMaximumWidth ( 480 );
	mOption  = new QPushButton (
		mText["kbdoption"],rBox 
	);
	QLabel* hidden = new QLabel ( rBox );
	hidden -> setFixedHeight  ( 60 );
	mAddView = new QListView  ( additional );
	mAddView -> setItemMargin ( 2 );
	mAddView -> setAllColumnsShowFocus (true);
	mViewStatus = mAddView -> addColumn ( mText["kbdstatus"] );
	mViewLayout = mAddView -> addColumn ( mText["kbdaddlayout"] );
	mViewKey    = mAddView -> addColumn ( mText["kbdkey"] );
	mViewVariant= mAddView -> addColumn ( mText["kbdvariantlabel"] );
	mAddView -> setResizeMode (QListView::LastColumn);
	QHBox* testBox = new QHBox ( additional );
	QLabel* addvariantLabel = new QLabel (
		mText["kbdvariantlabel"],testBox
	);
	addvariantLabel -> setFixedWidth ( 100 );
	mAddVariant = new QComboBox ( FALSE,testBox );
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
	layer1 -> setSpacing (10);
	QHBox* miscBox       = new QHBox ( layer1 );
	layer1 -> setSpacing (10);
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
	// ...
	QLabel* shiftLockLabel  = new QLabel ( shiftLockBox );
	shiftLockLabel -> setText (
		mText["ShiftLockGroup"]
	);
	shiftLockLabel -> setFixedWidth ( 200 );
	mXkbOption[0] = new QComboBox ( FALSE,shiftLockBox );
	shiftLockBox -> setStretchFactor ( mXkbOption[0],2 );
	// ...
	QLabel* ctrlMapLabel    = new QLabel ( ctrlMapBox );
	ctrlMapLabel -> setText (
		mText["CtrlPos"]
	);
	ctrlMapLabel -> setFixedWidth ( 200 );
	mXkbOption[1] = new QComboBox ( FALSE,ctrlMapBox );
	ctrlMapBox -> setStretchFactor ( mXkbOption[1],2 );
	// ...
	QLabel* miscLabel       = new QLabel ( miscBox );
	miscLabel -> setText (
		mText["kbdmisc"]
	);
	miscLabel -> setFixedWidth ( 200 );
	mXkbOption[6] = new QComboBox ( FALSE,miscBox );
	miscBox -> setStretchFactor ( mXkbOption[6],2 );
	// ...
	QLabel* leftAltLabel    = new QLabel ( leftAltBox );
	leftAltLabel -> setText (
		mText["LeftAlt"]
	);
	leftAltLabel -> setFixedWidth ( 200 );
	mXkbOption[2] = new QComboBox ( FALSE,leftAltBox );
	leftAltBox -> setStretchFactor ( mXkbOption[2],2 );
	QLabel* rightAltLabel   = new QLabel ( rightAltBox );
	rightAltLabel -> setText (
		mText["RightAlt"]
	);
	rightAltLabel -> setFixedWidth ( 200 );
	mXkbOption[3] = new QComboBox ( FALSE,rightAltBox );
	rightAltBox -> setStretchFactor ( mXkbOption[3],2 );
	QLabel* scrollLockLabel = new QLabel ( scrollLockBox );
	scrollLockLabel -> setText (
		mText["ScrollLock"]
	);
	scrollLockLabel -> setFixedWidth ( 200 );
	mXkbOption[4] = new QComboBox ( FALSE,scrollLockBox );
	scrollLockBox -> setStretchFactor ( mXkbOption[4],2 );
	QLabel* rightCtrlLabel  = new QLabel ( rightCtrlBox );
	rightCtrlLabel -> setText (
		mText["RightCtl"]
	);
	rightCtrlLabel -> setFixedWidth ( 200 );
	mXkbOption[5] = new QComboBox ( FALSE,rightCtrlBox );
	rightCtrlBox -> setStretchFactor ( mXkbOption[5],2 );

	// ...
	// AutoRepeat setting
	// ---
	// X11 no longer support this to be set with the config file
	// ...
	#if 0
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
	#endif

	// Add widgets to tab dialog
	// -------------------------
	mTop -> addTab ( layer1,mText["kbdmapping"] );
	//mTop -> addTab ( layer2,mText["kbdrepeat"] );

	// set OK/Cancel buttons...
	// ------------------------
	mTop -> setOkButton      ( mText["Ok"] );
	mTop -> setCancelButton  ( mText["Cancel"] );

	// connect me to the world
	// -----------------------
	QObject::connect (
		mLayout   , SIGNAL ( activated        (int) ),
		this      , SLOT   ( slotLayout       (int) )
	);
	QObject::connect (
		mType     , SIGNAL ( activated        (int) ),
		this      , SLOT   ( slotType         (int) )
	);
	QObject::connect (
		mVariant  , SIGNAL ( activated        (int) ),
		this      , SLOT   ( slotVariant      (int) )
	);
	QObject::connect (
		mAddVariant , SIGNAL ( activated      (int) ),
		this        , SLOT   ( slotAddVariant (int) )
	);
	QObject::connect (
		mOption   , SIGNAL ( clicked    () ),
		this      , SLOT   ( slotOption () )
	);
	QObject::connect(
		mAddView  , SIGNAL ( selectionChanged (QListViewItem *)),
		this      , SLOT   ( slotAddLayout    (QListViewItem *))
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
	layer0 -> addSpacing ( 5 );
	layer0 -> addWidget  ( testText );
	layer0 -> addWidget  ( mTest );
	layer0 -> setMargin  ( 5 );

	initPage();

	mStack -> addWidget  ( mDialog , mIndex );
}

//===========================================
// XKeyboard set Warning (OK) message box...
//-------------------------------------------
void XKeyboard::setMessage (
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
// XKeyboard slotRun...
//-------------------------------------
void XKeyboard::slotIntro (int index) {
	if (index == mIndex) {
	QDict<char> mModelHash  = mRules.getModels();
	QDict<char> mLayoutHash = mRules.getLayouts();
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
	QString XKBModel = keyboard["XkbModel"];
	QDictIterator<char> itModel (mModelHash);
	for (; itModel.current(); ++itModel) {
	if (itModel.currentKey() == XKBModel) {
		XKBModel = QString::fromLocal8Bit (itModel.current());
		break;
	}
	}
	XStringList completeLayout (keyboard["XkbLayout"]);
	completeLayout.setSeperator (",");
	QString XKBLayout = completeLayout.getList().getFirst();
	QDictIterator<char> itLayout (mLayoutHash);
	for (; itLayout.current(); ++itLayout) {
	if (itLayout.currentKey() == XKBLayout) {
		XKBLayout = QString::fromLocal8Bit (itLayout.current());
	}
	}
	// first row
	message += "<tr>";
	QTextOStream(&idents)
		<< "<td width=30%>" << mText["model"]
		<< "</td> <td width=60%>" << XKBModel << "</td>";
	message += idents;
	message += "</tr>";	

	// second row
	message += "<tr>";
	QTextOStream(&idents)
		<< "<td width=30%>" << mText["klayout"]
		<< "</td> <td width=60%>" << XKBLayout << "</td>";
	message += idents;
	message += "</tr>";

	// end table...
	message += "</table>";
	info -> setText (message);
	}
}
