/**************
FILE          : mouse.c
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
              :
STATUS        : Status: Up-to-date
**************/
#include "mouse.h"
#include "xmouse.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XMouse Constructor...
//-------------------------------------
XMouse::XMouse (void) {
	mSelected = -1;
}

//=====================================
// XMouse Constructor...
//-------------------------------------
XMouse::XMouse (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	mSelected = -1;
	addTo (xf,xi);
}

//=====================================
// XMouse add widget to mainframe...
//-------------------------------------
void XMouse::addTo (XFrame* xf,XIntro* xi) {
	mIntro    = xi;
	mFrame    = xf;
	mInit     = mFrame -> getInit();
	mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
	mIndex    = Mouse;
	mPage     = mIndex;
	mLastPage = MousePage1;
	getConnected (xf);
	dialogCreate ();
}

//=====================================
// XMouse create the dialog...
//-------------------------------------
void XMouse::dialogCreate (void) {
	mDialog = new QFrame     ( mStack );
	mTop    = new XTabDialog ( mFrame,"",TRUE,0,globalFrameWidth );
	XWrapPointer< QDict<char> > mText (mTextPtr);

	// create layout structure...
	// ---------------------------
	QBoxLayout* layer1 = new QVBoxLayout ( mDialog );
	QBoxLayout* layer2 = new QHBoxLayout ( layer1 );
	layer2 -> addSpacing ( 5 );
	QBoxLayout* layer3 = new QHBoxLayout ( layer1 );
	layer3 -> addSpacing ( 5 );
	QBoxLayout* layer4 = new QVBoxLayout ( layer3 );
	layer3 -> addSpacing ( 10 );
	QBoxLayout* layer5 = new QVBoxLayout ( layer3 );
	layer3 -> addSpacing ( 10 );
		 
	// create widgets...
	// ------------------
	QFontMetrics metrics ( font() );
	QTextView* topic = new QTextView ( mDialog );
	topic -> setVScrollBarMode (QScrollView::AlwaysOff);
	topic -> setFixedHeight (40);
	topic -> setFixedWidth  ( metrics.width (mText["curmouse"]) );
	topic -> setPaper ( QBrush(mDialog->backgroundColor()) );
	topic -> setFrameStyle ( QFrame::NoFrame );
	topic -> setText (mText["curmouse"]);

	mouseList = new QListBox    ( mDialog );
	mSetup    = new QPushButton ( mText["confmouse"], mDialog );
	mAdd      = new QPushButton ( mText["addmouse"], mDialog );
	mDel      = new QPushButton ( mText["removemouse"], mDialog );
	mEvents   = new XShowMouse  ( mTextPtr,mDialog );
	layer6    = new QVBox       ( mTop );
	layer7    = new QVBox       ( mTop );
	layer8    = new QVBox       ( mTop );

	// create tab dialog widgets...
	// -----------------------------
	//-----------------------------------
	// general...
	//===================================
	QButtonGroup* select = new QButtonGroup ( 
		1,Vertical,mText["vnselect"],layer6 
	);
	layer6 -> setMargin ( 10 );
	mVendor = new QListBox ( select );
	mName   = new QListBox ( select );

	//-----------------------------------
	// expert...
	//===================================
	QButtonGroup* expert = new QButtonGroup ( 
		1,Horizontal,mText["mouseexpert"],layer7 
	);
	layer7 -> setMargin (10);
	QLabel* pixexpert = new QLabel ( expert );
	pixexpert -> setPixmap (
        QPixmap (mInit["pix_intro_mouse"])
	);
	mCheckEmulate  = new QCheckBox ( mText["enableemu"]  , expert );
	mCheckWheel    = new QCheckBox ( mText["enablewheel"], expert );
	QHBox* wbox    = new QHBox     ( expert );
	mCheckWheelEmu = new QCheckBox ( mText["emuwheel"]   , wbox );
	wbox -> setSpacing ( 10 );
	mWheelKey      = new QSpinBox  ( 4,10,1,wbox );
	mWheelKey     -> setDisabled   ( TRUE );
	mWheelKey     -> setFixedWidth ( 50 );
	QLabel* nope   = new QLabel    ( wbox );
	wbox -> setStretchFactor ( mCheckWheelEmu,0 );
	wbox -> setStretchFactor ( mWheelKey,0 );
	wbox -> setStretchFactor ( nope,100 );

	//-----------------------------------
	// port [if device is serial only]...
	//===================================
	QButtonGroup* port = new QButtonGroup (
		1,Horizontal,mText["port"],layer8
	);
	layer8 -> setMargin (10);
	QLabel* pixport = new QLabel ( port );
	pixport -> setPixmap (
		QPixmap (mInit["pix_intro_port"])
	);
	mPort = new QComboBox (true,port);
	
	// add tab dialogs to toplevel...
	// ------------------------------
	mTop -> addTab ( layer6,mText["general"] );
	mTop -> addTab ( layer8,mText["port"] );
	mTop -> addTab ( layer7,mText["expert"] );

	mTop -> setOkButton      ( mText["Ok"] );
	mTop -> setCancelButton  ( mText["Cancel"] );

	// disable delete and setup button...
	// -----------------------------------
	mDel   -> setDisabled ( TRUE );
	mSetup -> setDisabled ( TRUE );

	// add widgets to the layout(s)
	// -----------------------------
	layer2 -> addWidget  ( topic );
	layer2 -> addStretch ( 10 );
	layer4 -> addWidget  ( mouseList );
	layer5 -> addWidget  ( mSetup );
	layer5 -> addSpacing ( 20 );
	layer5 -> addWidget  ( mAdd );
	layer5 -> addSpacing ( 5 );
	layer5 -> addWidget  ( mDel );
	layer5 -> addStretch ( 10 );
	layer5 -> addWidget  ( mEvents );

	// connect me to the world...
	// ---------------------------
	QObject::connect ( 
		mouseList     , SIGNAL ( clicked    (QListBoxItem *) ),
		this          , SLOT   ( slotSelect (QListBoxItem *) )
	);
	QObject::connect (
		mouseList     , SIGNAL ( selectionChanged (QListBoxItem *) ),
		this          , SLOT   ( slotSelect       (QListBoxItem *) )
	);
	QObject::connect ( 
		mAdd          , SIGNAL ( clicked() ),
		this          , SLOT   ( slotAdd() ) 
	);
	QObject::connect ( 
		mDel          , SIGNAL ( clicked() ), 
		this          , SLOT   ( slotRemove() ) 
	);
	QObject::connect ( 
		mSetup        , SIGNAL ( clicked() ), 
		this          , SLOT   ( slotSetup() ) 
	);
	QObject::connect (
        mTop          , SIGNAL ( applyButtonPressed() ),
		this          , SLOT   ( slotTopOk() )
	);
	QObject::connect (
	    mTop          , SIGNAL ( cancelButtonPressed() ),
		this          , SLOT   ( slotTopCancel() )
	);
	QObject::connect ( 
		mVendor       , SIGNAL ( clicked    (QListBoxItem *) ),
		this          , SLOT   ( slotVendor (QListBoxItem *) )
	);
	QObject::connect (
		mVendor       , SIGNAL ( returnPressed (QListBoxItem *)),
		this          , SLOT   ( slotVendor    (QListBoxItem *))
	);
	QObject::connect (
		mVendor       , SIGNAL ( highlighted (QListBoxItem *)),
		this          , SLOT   ( slotVendor  (QListBoxItem *))
	);
	QObject::connect ( 
		mName         , SIGNAL ( clicked  (QListBoxItem *) ),
		this          , SLOT   ( slotName (QListBoxItem *) )
	);
	QObject::connect (
		mName         , SIGNAL ( highlighted (QListBoxItem *) ),
		this          , SLOT   ( slotName    (QListBoxItem *) )
	);
	QObject::connect (
		mPort         , SIGNAL ( activated  (int) ),
		this          , SLOT   ( slotPort   (int) )
	);
	QObject::connect (
		mCheckWheelEmu, SIGNAL ( stateChanged  ( int ) ),
		this          , SLOT   ( slotWheelKey  ( int ) )
	);

	initPage ();
	  
	// add dialog to the widget stack
	// -------------------------------
	mStack -> addWidget ( mDialog , mIndex );
}

//=====================================
// XMouse slotIntro...
//-------------------------------------
void XMouse::slotIntro (int index) {
    if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QDict<char> input;
	QString     message;
	QString     idents;
	XData*      data;

	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText["mouse_header"];
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	int count = -1;
	for (int n=0; n<mFiles["sys_INPUT"]->getDeviceCount(); n++) {
		count += 2;
		data = mFiles["sys_INPUT"] -> getDevice(count);
		if (! data) {
			continue;
		}
		input = data -> getData();
		if (QString(input["InputFashion"]).isNull()) {
			input.insert ("InputFashion","Mouse");
		}
		if (QString(input["InputFashion"]) != "Mouse") {
		continue;
		}
		message += "<tr>";
		idents.sprintf(
			"<td width=10%c align=center>( %d )</td>",'%',n
		);
		message += idents;
		QString name (input["Name"]);
		name.replace(QRegExp(";")," ");
		idents.sprintf(
			"<td width=40%c>%s</td>",'%',name.ascii()
		);
		message += idents;
		idents.sprintf(
			"<td width=40%c>%s</td>",'%',input["Device"]
		);
		message += idents;
		message += "</tr>";
	}
	
	message += "</table><br>";
	info -> setText (message);
	}
}

