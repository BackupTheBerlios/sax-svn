/**************
FILE          : card.c
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
              :
STATUS        : Status: Up-to-date
**************/
#include "card.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XCard Constructor...
//-------------------------------------
XCard::XCard (void) {
	mSelected = -1;
	mShowSetupWindow = true;
	mCheckType = true;
}

//=====================================
// XCard Constructor...
//-------------------------------------
XCard::XCard (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	mSelected = -1;
	mShowSetupWindow = true;
	mCheckType = true;
    addTo (xf,xi);
}

//=====================================
// XCard add widget to mainframe...
//-------------------------------------
void XCard::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Card;
    mPage     = mIndex;
    mLastPage = CardPage1;
	mIntro -> store (this,mIndex);
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XCard create the dialog...
//-------------------------------------
void XCard::dialogCreate (void) {
	mDialog = new QFrame ( mStack );
	mTop    = new XTabDialog ( mFrame,"",TRUE,0,globalFrameWidth );
	XWrapPointer< QDict<char> > mText (mTextPtr);

	// create global string objects...
	// --------------------------------
	mCardRawOptions = new QString ();
	mCardOptions    = new QString ();
	mModuleList     = new QString ();
	m3D             = new QString ();

	// create layout structure...
	// --------------------------
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
	topic -> setFixedWidth  ( metrics.width (mText["curcard"]) );
	topic -> setPaper ( QBrush(mDialog->backgroundColor()) );
	topic -> setFrameStyle ( QFrame::NoFrame );
	topic -> setText (mText["curcard"]);

	mCardList = new QListBox    ( mDialog );
	mSetup    = new QPushButton ( mText["confcard"], mDialog );
	mAdd      = new QPushButton ( mText["addcard"], mDialog );
	mDel      = new QPushButton ( mText["removecard"], mDialog );

	// create tab dialog widgets...
	// -----------------------------
	//-----------------------------------
	// General...
	//===================================
	layer6 = new QVBox ( mTop );
	QButtonGroup* select = new QButtonGroup (
		1,Vertical,mText["vnselect"],layer6
	);
	layer6 -> setMargin ( 10 );
	mVendor = new QListBox ( select );
	mName   = new QListBox ( select );

	//-----------------------------------
	// Expert...
	//===================================
	layer7 = new QVBox ( mTop );
	QHBox* layer8 = new QHBox ( layer7 );
	mPixExpert = new QLabel ( layer8 );
	mPixExpert -> setPixmap (
		QPixmap (mInit["pix_intro_card"])
	);
	layer8 -> setSpacing ( 20 );
	mCheck3D  = new QCheckBox ( mText["enable3d"], layer8 );
	QLabel* nope = new QLabel ( layer8 );
	layer8 -> setStretchFactor ( nope, 10 );

	QHBox* layer9  = new QHBox ( layer7 );
	QVBox* layer10 = new QVBox ( layer9 );
	layer9 -> setSpacing ( 10 );
	QVBox* layer11 = new QVBox ( layer9 );

	mRotate = new QButtonGroup (
        1,Horizontal,mText["rotate"],layer11
    );
	mRotateNone  = new QRadioButton ( mText["rotateNone"],mRotate );
	mRotateLeft  = new QRadioButton ( mText["rotateLeft"],mRotate );
	mRotateRight = new QRadioButton ( mText["rotateRight"],mRotate );
	QButtonGroup* busid = new QButtonGroup (
		1,Vertical,mText["busid"],layer10
	);
	mBusID = new QComboBox ( FALSE, busid );
	QButtonGroup* screenid = new QButtonGroup (
		1,Vertical,mText["screenid"],layer10
	);
	mScrID = new QComboBox ( FALSE, screenid );
	mScrID -> insertItem ("Primary Screen [0]");
	mScrID -> insertItem ("Secondary Screen [1]");
	QButtonGroup* options = new QButtonGroup (
        1,Vertical,mText["options"],layer10
    );
    mOptions = new QListBox ( options );
    mOptions -> setSelectionMode ( QListBox::Multi );
    layer7 -> setSpacing ( 10 );
	layer7 -> setMargin  ( 10 );

	// add tab dialogs to toplevel...
	// ------------------------------
	mTop -> addTab ( layer6,mText["general"] );
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
	layer4 -> addWidget  ( mCardList );
	layer5 -> addWidget  ( mSetup );
	layer5 -> addSpacing ( 20 );
	layer5 -> addWidget  ( mAdd );
	layer5 -> addSpacing ( 5 );
	layer5 -> addWidget  ( mDel );
	layer5 -> addStretch ( 50 );

	// connect me to the world...
	// ---------------------------
	QObject::connect ( 
		mCardList , SIGNAL ( clicked    (QListBoxItem *) ),
		this      , SLOT   ( slotSelect (QListBoxItem *) )
	);
	QObject::connect (
		mCardList , SIGNAL ( selectionChanged (QListBoxItem *) ),
		this      , SLOT   ( slotSelect       (QListBoxItem *) )
	);
	QObject::connect ( 
		mAdd      , SIGNAL ( clicked() ),
		this      , SLOT   ( slotAdd() )
	);
	QObject::connect ( 
		mDel      , SIGNAL ( clicked() ), 
		this      , SLOT   ( slotRemove() )
	);
	QObject::connect ( 
		mSetup    , SIGNAL ( clicked() ),
		this      , SLOT   ( slotSetup() )
	);
	QObject::connect (
		mTop      , SIGNAL ( applyButtonPressed() ),
		this      , SLOT   ( slotTopOk() )
	);
	QObject::connect (
		mTop      , SIGNAL ( cancelButtonPressed() ),
		this      , SLOT   ( slotTopCancel() )
	);
	QObject::connect (
		mVendor   , SIGNAL ( clicked    (QListBoxItem *) ),
		this      , SLOT   ( slotVendor (QListBoxItem *) )
	);
	QObject::connect (
		mVendor   , SIGNAL ( returnPressed (QListBoxItem *)),
		this      , SLOT   ( slotVendor    (QListBoxItem *))
	);
	QObject::connect (
		mVendor   , SIGNAL ( highlighted (QListBoxItem *)),
		this      , SLOT   ( slotVendor  (QListBoxItem *))
	);
	QObject::connect ( 
		mName     , SIGNAL ( clicked  (QListBoxItem *) ),
		this      , SLOT   ( slotName (QListBoxItem *) )
	);
	QObject::connect (
		mName     , SIGNAL ( highlighted  (QListBoxItem *) ),
		this      , SLOT   ( slotName     (QListBoxItem *) )
	);
	QObject::connect (
		mOptions  , SIGNAL ( clicked    (QListBoxItem *) ),
		this      , SLOT   ( slotOption (QListBoxItem *) )
	);
	QObject::connect (
		mBusID    , SIGNAL ( activated  (int) ),
		this      , SLOT   ( slotBusID  (int) )
	);
	QObject::connect (
		mScrID    , SIGNAL ( activated    (int) ),
		this      , SLOT   ( slotScreenID (int) )
	);
	QObject::connect (
		mCheck3D  , SIGNAL ( clicked () ),
		this      , SLOT   ( slot3D  () )
	);

	// ...
	// the 3D dialog has moved to a seperate dialog which
	// controls the functions implemented at this point. Therefore
	// we will not show the controls located in layer8
	// ---
	layer8 -> hide();

	initPage ();

	// add dialog to the widget stack
	// -------------------------------
	mStack -> addWidget ( mDialog , mIndex );
}

//=====================================
// XCard slotIntro...
//-------------------------------------
void XCard::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QDict<char> card;
	QString     message;
	QString     idents;
	XData*      data;

	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText ["card_header"];
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		data = mFiles["sys_CARD"] -> getDevice(n);
		if (! data) {
			continue;
		}
		card = data -> getData();
		message += "<tr>";
		idents.sprintf(
			"<td width=10%c align=center>( %d )</td>",'%',n
		);
		message += idents;
		idents.sprintf(
			"<td width=60%c>%s %s</td>",'%',card["Vendor"],card["Name"]
		);
		message += idents;
		idents.sprintf(
			"<td width=20%c>%s</td>",'%',card["Driver"]
		);
		message += idents;
		message += "</tr>";
	}

	message += "</table><br>";
	info -> setText (message);
	}
}

//=====================================
// XCard get selected card...
//-------------------------------------
int XCard::getSelected (void) {
	return (mSelected);
}
