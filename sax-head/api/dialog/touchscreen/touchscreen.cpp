/**************
FILE          : touchscreen.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure touchscreens as input devices mostly
              : used from elographics
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "touchscreen.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XTouchScreen Constructor...
//-------------------------------------
XTouchScreen::XTouchScreen (void) {
	mSelected = -1;
}

//=====================================
// XTouchScreen Constructor...
//-------------------------------------
XTouchScreen::XTouchScreen (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	mSelected = -1;
    addTo (xf,xi);
}

//=====================================
// XTouchScreen add widget to mainframe...
//-------------------------------------
void XTouchScreen::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Toucher;
    mPage     = mIndex;
    mLastPage = TouchScreenPage1;
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XTouchScreen create the dialog...
//-------------------------------------
void XTouchScreen::dialogCreate (void) {
	mDialog = new QFrame ( mStack );
	mTop    = new XTabDialog ( mFrame,"",TRUE,0,globalFrameWidth );
	XWrapPointer< QDict<char> > mText (mTextPtr);

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
	// -----------------
	QFontMetrics metrics ( font() );
	QTextView* topic = new QTextView ( mDialog );
	topic -> setVScrollBarMode (QScrollView::AlwaysOff);
	topic -> setFixedHeight (40);
	topic -> setFixedWidth  ( metrics.width (mText["curtouch"]) );
	topic -> setPaper ( QBrush(mDialog->backgroundColor()) );
	topic -> setFrameStyle ( QFrame::NoFrame );
	topic -> setText (mText["curtouch"]);

	touchList     = new QListBox    ( mDialog );
	mSetup        = new QPushButton ( mText["conftouch"], mDialog );
	mAdd          = new QPushButton ( mText["addtouch"], mDialog );
	mDel          = new QPushButton ( mText["removetouch"], mDialog );
	QVBox* layer6 = new QVBox       ( mTop );

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

	// add tab dialogs to toplevel...
    // ------------------------------
	mTop -> addTab ( layer6,mText["general"] );
	mTop -> setOkButton      ( mText["Ok"] );
	mTop -> setCancelButton  ( mText["Cancel"] );

	// disable delete and setup button...
	// -----------------------------------
	mDel   -> setDisabled ( TRUE );
	mSetup -> setDisabled ( TRUE );

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
		mVendor   , SIGNAL ( highlighted (QListBoxItem *)),
		this      , SLOT   ( slotVendor  (QListBoxItem *))
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
		touchList, SIGNAL ( clicked    (QListBoxItem *) ),
		this     , SLOT   ( slotSelect (QListBoxItem *) )
	);
	QObject::connect (
		touchList, SIGNAL ( selectionChanged (QListBoxItem *) ),
		this     , SLOT   ( slotSelect       (QListBoxItem *) )
	);
	QObject::connect (
		mTop     , SIGNAL ( applyButtonPressed() ),
		this     , SLOT   ( slotTopOk() )
    );
	QObject::connect (
		mTop     , SIGNAL ( cancelButtonPressed() ),
		this     , SLOT   ( slotTopCancel() )
	);
	QObject::connect (
		mAdd     , SIGNAL ( clicked() ),
		this     , SLOT   ( slotAdd() )
	);
	QObject::connect (
		mDel     , SIGNAL ( clicked() ),
		this     , SLOT   ( slotRemove() )
	);
	QObject::connect (
		mSetup   , SIGNAL ( clicked() ),
		this     , SLOT   ( slotSetup() )
	);

	// add widgets to the layout...
	// ----------------------------
	layer2 -> addWidget  ( topic );
	layer2 -> addStretch ( 10 );
	layer4 -> addWidget  ( touchList );
	layer5 -> addWidget  ( mSetup );
	layer5 -> addSpacing ( 20 );
	layer5 -> addWidget  ( mAdd );
	layer5 -> addSpacing ( 5 );
	layer5 -> addWidget  ( mDel );
	layer5 -> addStretch ( 50 );

	initPage ();

	// add dialog to the widget stack
	// -------------------------------
	mStack -> addWidget  ( mDialog , mIndex );
}

//=====================================
// XTouchScreen slotIntro...
//-------------------------------------
void XTouchScreen::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> input;
	XData*      data;
	bool        gotScreen = FALSE;

	// wrap file pointer...
	// ---------------------
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText["touchscreen_header"];
	info    -> setText ( message );
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
		if (QString(input["InputFashion"]) != "Touchpanel") {
		continue;
		}
		if (n > 0) {
		message += "<tr>";
		XStringList product (input["Name"]);
		product.setSeperator (";");
		QString vendor = product.getList().at(0);
		QString name   = product.getList().at(1);
		QTextOStream (&idents) 
			<< "<td>" << vendor << " " << name << "</td>";
		message += idents;
		message += "</tr>";
		gotScreen = TRUE;
		}
	}
	if (! gotScreen) {
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
