/**************
FILE          : tablet.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure tablets including pens and erasers
              : and other tablet stuff
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "tablet.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XTablet Constructor...
//-------------------------------------
XTablet::XTablet (void) {
	deleteOnly = false;
}

//=====================================
// XTablet Constructor...
//-------------------------------------
XTablet::XTablet (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	deleteOnly = false;
    addTo (xf,xi);
}

//=====================================
// XTablet add widget to mainframe...
//-------------------------------------
void XTablet::addTo (XFrame* xf,XIntro* xi) {
	mIntro    = xi;
	mFrame    = xf;
	mInit     = mFrame -> getInit();
	mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
	mIndex    = Tablet;
	mPage     = mIndex;
	mLastPage = TabletPage1;
	getConnected (xf);
	dialogCreate ();
}

//=====================================
// XTablet create the dialog...
//-------------------------------------
void XTablet::dialogCreate (void) {
	mDialog = new QFrame ( mStack );
	mTop    = new XTabDialog ( mFrame,"",TRUE,0,globalFrameWidth );
	XWrapPointer< QDict<char> > mText (mTextPtr);

	// create layout structure...
	// --------------------------
	QBoxLayout* layer1 = new QVBoxLayout ( mDialog );
	QBoxLayout* layer2 = new QVBoxLayout ( layer1 );
	layer1 -> addSpacing ( 10 );
	QBoxLayout* layer3 = new QVBoxLayout ( layer1 );

	// create widgets...
	// -----------------
	QLabel* pixtablet = new QLabel ( mDialog );
	pixtablet -> setPixmap (
		QPixmap (mInit["pix_tablet"])
	);
	QHBox* SelectAndToolBox = new QHBox ( mDialog );
	SelectAndToolBox -> setSpacing ( 10 );
	QButtonGroup* select = new QButtonGroup (
		1,Vertical,mText["vnselect"],SelectAndToolBox
	);
	SelectAndToolBox -> setStretchFactor ( select,100 );
	QVBox* SelectAndToolBoxFrame = new QVBox ( SelectAndToolBox );
	QButtonGroup* toolbar = new QButtonGroup (
		1,Horizontal,mText["toolbar"],SelectAndToolBoxFrame
	);
	QButtonGroup* options = new QButtonGroup (
		1,Horizontal,mText["tabletopts"],SelectAndToolBoxFrame
	);
	SelectAndToolBoxFrame -> setStretchFactor ( options,100 );
	QButtonGroup* port = new QButtonGroup (
		1,Horizontal,mText["port"],SelectAndToolBoxFrame
	);
	QButtonGroup* mode = new QButtonGroup (
		1,Horizontal,mText["mode"],SelectAndToolBoxFrame
	);
	
	mTabletOptions = new QListBox ( options );
	mTabletOptions -> setSelectionMode ( QListBox::Multi );
	mTabletOptions -> setDisabled ( true );

	mPort = new QComboBox ( false, port);
	mPort -> insertItem ( mText["usb"] );
	mPort -> insertItem ( mText["serial1"] );
	mPort -> insertItem ( mText["serial2"] );
	mPort -> setDisabled ( true );

	mModeRelative = new QRadioButton ( mText["relative"], mode );
	mModeAbsolute = new QRadioButton ( mText["absolute"], mode );
	mModeAbsolute -> setChecked  ( true );
	mModeRelative -> setDisabled ( true );
	mModeAbsolute -> setDisabled ( true );

	QHBox* penbox = new QHBox ( toolbar );
	penbox -> setSpacing ( 10 );
	mPen = new QCheckBox ( penbox );
	mPenConfigure = new QPushButton (mText["conftablet"],penbox);
	mPenConfigure -> setDisabled ( true );
	mPen -> setText ( mText["addpen"] );
	mPen -> setDisabled ( true );

	QHBox* eraserbox = new QHBox ( toolbar );
	eraserbox -> setSpacing ( 10 );
	mEraser = new QCheckBox ( eraserbox );
	mEraserConfigure = new QPushButton (mText["conftablet"],eraserbox);
	mEraserConfigure -> setDisabled ( true );
	mEraser -> setText ( mText["adderaser"] );
	mEraser -> setDisabled ( true );

	mVendor = new QListBox  ( select );
	mName   = new QListBox  ( select );

	QHBox* btns = new QHBox ( mDialog );
	mRemove = new QPushButton ( mText ["removetablet"],btns );
	QLabel* nope = new QLabel ( btns );
	btns -> setStretchFactor ( mRemove, 0 );
	btns -> setStretchFactor ( nope, 10 );

	// create tab dialog widgets...
	// -----------------------------
	//-----------------------------------
	// General...
	//===================================
	QVBox* general = new QVBox ( mTop );
	QButtonGroup* toolmode = new QButtonGroup (
		1,Horizontal,mText["mode"],general
	);
	QButtonGroup* tooloptions = new QButtonGroup (
		1,Horizontal,mText["tabletopts"],general
	);
	mToolModeRelative = new QRadioButton ( mText["relative"], toolmode );
	mToolModeAbsolute = new QRadioButton ( mText["absolute"], toolmode );
	mToolModeAbsolute -> setChecked ( true );
	mToolTabletOptions = new QListBox ( tooloptions );
	mToolTabletOptions -> setSelectionMode ( QListBox::Multi );
	general -> setMargin ( 10 );

	// add tab dialogs to toplevel...
    // ------------------------------
	mTop -> addTab ( general,mText["general"] );

	mTop -> setOkButton      ( mText["Ok"] );
    mTop -> setCancelButton  ( mText["Cancel"] );

	// disable remove button
	// ---------------------
	mRemove -> setDisabled ( true );

	// connect me to the world
	// -----------------------
	QObject::connect (
		mVendor  , SIGNAL ( clicked    (QListBoxItem *) ),
        this     , SLOT   ( slotVendor (QListBoxItem *) )
	);
	QObject::connect (
		mVendor  , SIGNAL ( returnPressed (QListBoxItem *)),
		this     , SLOT   ( slotVendor    (QListBoxItem *))
	);
	QObject::connect (
		mVendor  , SIGNAL ( highlighted (QListBoxItem *)),
		this     , SLOT   ( slotVendor  (QListBoxItem *))
	);
	QObject::connect (
		mName    , SIGNAL ( clicked    (QListBoxItem *) ),
		this     , SLOT   ( slotName   (QListBoxItem *) )
	);
	QObject::connect (
		mName    , SIGNAL ( highlighted (QListBoxItem *) ),
		this     , SLOT   ( slotName    (QListBoxItem *) )
	);
	QObject::connect (
		mRemove  , SIGNAL ( clicked    (void) ),
		this     , SLOT   ( slotRemove (void) )
	);
	QObject::connect (
		mPen     , SIGNAL ( clicked              (void) ),
		this     , SLOT   ( slotPenCheck         (void) )
	);
	QObject::connect (
		mEraser  , SIGNAL ( clicked              (void) ),
		this     , SLOT   ( slotEraserCheck      (void) )
	);
	QObject::connect (
		mTop      , SIGNAL ( applyButtonPressed  (void) ),
		this      , SLOT   ( slotTopOk           (void) )
	);
	QObject::connect (
		mTop      , SIGNAL ( cancelButtonPressed (void) ),
		this      , SLOT   ( slotTopCancel       (void) )
	);
	QObject::connect (
		mEraserConfigure , SIGNAL ( clicked      (void) ),
		this             , SLOT   ( slotEraser   (void) )
	);
	QObject::connect (
		mPenConfigure    , SIGNAL ( clicked      (void) ),
		this             , SLOT   ( slotPen      (void) )
	);
	QObject::connect (
		mToolTabletOptions , SIGNAL ( clicked        (QListBoxItem*) ),
		this               , SLOT   ( slotToolOption (QListBoxItem*) )
	);
    QObject::connect (
		mTabletOptions     , SIGNAL ( clicked        (QListBoxItem*) ),
		this               , SLOT   ( slotOption     (QListBoxItem*) )
	);

	// add widgets to the layout...
	// ----------------------------
	layer2 -> addWidget  ( pixtablet );
	layer3 -> addWidget  ( SelectAndToolBox );
	layer3 -> addSpacing ( 10 );
	layer3 -> addWidget  ( btns );
	layer1 -> setMargin  ( 10 );

	initPage ();

	// add dialog to the widget stack
	// -------------------------------
	mStack -> addWidget  ( mDialog , mIndex );
}

//=====================================
// XTablet slotIntro...
//-------------------------------------
void XTablet::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> input;
	XData*      data;
	bool        gotTablet = FALSE;

	// wrap file pointer...
	// ---------------------
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText["tablet_header"];
	info    -> setText (message);
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
		if (QString(input["InputFashion"]) != "Tablet") {
		continue;
		}
		if (n > 0) {
		message += "<tr>";
		QTextOStream (&idents) 
			<< "<td>" << input["Vendor"] << " " << input["Name"] << "</td>";
		message += idents;
		message += "</tr>";
		gotTablet = TRUE;
		// ...
		// more than one tablet does not make 
		// any sense I think, therefore we will break now
		// ---
		break;
		}
	}
	if (! gotTablet) {
		message += "<tr>";
		QTextOStream (&idents) 
			<< "<td>" << mText["notConfigured"] << "</td>";
		message += idents;
		message += "</tr>";
	}
	message += "</table>";
	info -> setText (message);
	}
}
