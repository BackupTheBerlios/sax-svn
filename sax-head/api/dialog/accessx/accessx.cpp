/**************
FILE          : accessx.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure accessx for XFree86. accessx provide
              : the possibility to handle the mouse via the
              : number pad
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "accessx.h"

//=====================================
// XAccessX Constructor...
//-------------------------------------
XAccessX::XAccessX (void) {
}

//=====================================
// XAccessX Constructor...
//-------------------------------------
XAccessX::XAccessX (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
    addTo (xf,xi);
}

//=====================================
// XAccessX add widget to mainframe...
//-------------------------------------
void XAccessX::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Xaccess;
    mPage     = mIndex;
    mLastPage = XAccessPage1;
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XAccessX create the dialog...
//-------------------------------------
void XAccessX::dialogCreate (void) {
	int initSpeed = 20;
	XWrapPointer< QDict<char> > mText (mTextPtr);

    mDialog = new QFrame ( mStack );
	QBoxLayout* layer1 = new QVBoxLayout ( mDialog );
	QBoxLayout* layer2 = new QHBoxLayout ( layer1  );
	layer1 -> addSpacing ( 10 );
	QBoxLayout* layer3 = new QVBoxLayout ( layer1  );
	layer1 -> addSpacing ( 10 );
	QBoxLayout* layer4 = new QHBoxLayout ( layer1  );

	QLabel* pixaccess = new QLabel ( mDialog );
	pixaccess -> setPixmap (
		QPixmap (mInit["pix_intro_card"])
	);
	QFontMetrics metrics ( font() );
	QTextView* infoline = new QTextView ( mDialog );
	infoline -> setVScrollBarMode (QScrollView::AlwaysOff);
	infoline -> setFixedHeight ( 40 );
	infoline -> setFixedWidth  ( metrics.width (mText["infoaccessx"]) );
	infoline -> setPaper       ( QBrush(mDialog->backgroundColor()) );
	infoline -> setFrameStyle  ( QFrame::NoFrame );
	infoline -> setText        ( mText["infoaccessx"] );

	QButtonGroup* activate = new QButtonGroup (
		1,Horizontal,mDialog
	);
	activate -> setFrameStyle(QFrame::NoFrame);
	mDisable= new QRadioButton ( mText["disableacx"],activate );
	mEnable = new QRadioButton ( mText["enableacx"],activate );
	mDisable -> setChecked   ( TRUE );
	mControl = new QButtonGroup ( 
		1,Horizontal,mText["speed"],mDialog 
	);
	QHBox* speed = new QHBox   ( mControl );
	QLabel* nope = new QLabel  ( mControl );
	nope -> setFixedHeight  ( 20 );  
	mSpeed = new QSlider       ( 
		10,500,5,initSpeed,QSlider::Horizontal,speed 
	);
	mSpeed -> setTickmarks    ( QSlider::Below );
	mSpeed -> setTickInterval ( 20 );
	speed  -> setSpacing      ( 10 );

	mLCD = new QLCDNumber   ( 3,speed );
	mLCD -> setSegmentStyle ( QLCDNumber::Filled );
	mLCD -> setPalette      ( QPalette( Qt::darkGray, Qt::darkGray ) );
	mLCD -> setFixedWidth   ( 60 );
	mLCD -> setFixedHeight  ( 30 );
	mLCD -> display         ( initSpeed );

	QLabel* applytip = new QLabel ( mText["applytip"],mDialog );
	mApply  = new QPushButton ( mText["apply"],mDialog );

	// connect me to the world...
	// --------------------------
	QObject::connect ( 
		mSpeed  , SIGNAL ( valueChanged  ( int ) ),
		this    , SLOT   ( slotLCD       ( int ) )
	);
	QObject::connect (
		mApply  , SIGNAL ( clicked       ( void ) ),
		this    , SLOT   ( slotApply     ( void ) )
	);
	QObject::connect (
		mEnable , SIGNAL ( toggled       ( bool ) ),
		this    , SLOT   ( slotState     ( bool ) )
	);

	// add widgets to the layout...
	// ----------------------------
	layer2 -> addWidget  ( pixaccess );
	layer2 -> addSpacing ( 10 );
	layer2 -> addWidget  ( infoline  );
	layer2 -> addStretch ( 50 );
	layer3 -> addWidget  ( activate );
	layer3 -> addSpacing ( 10 );
	layer3 -> addWidget  ( mControl );
	layer4 -> addWidget  ( applytip );
	layer4 -> addStretch ( 50 );
	layer4 -> addWidget  ( mApply );
	layer1 -> setMargin  ( 10 );
	layer1 -> addStretch ( 50 );

	initPage ();

	// add widgets to the stack...
	// ---------------------------
    mStack -> addWidget ( mDialog , mIndex );
}

//=====================================
// XAccessX slotIntro...
//-------------------------------------
void XAccessX::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
    QString     message;
	QString     idents;
	bool        active = FALSE;

	info    = mIntro -> getTextView();
    message = mText["accessx_header"];

	XkbGetControls(x11Display(), XkbAllControlsMask, xkb);
	if (xkb->ctrls == NULL) {
		active = FALSE;
	} else {
		if (XACCESS_CODE(xkb)) {
		active = TRUE;
		} else {
		active = FALSE;
		}
	}

	// begin table
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";
	message += idents;

	// first row
	message += "<tr>";
	if (active) {
	QTextOStream (&idents)
		<<"<td width=200>"<< mText["xaccess"] <<" "<< mText["on"] <<"</td>";
	} else {
	QTextOStream (&idents)
		<<"<td width=200>"<< mText["xaccess"] <<" "<< mText["off"] <<"</td>";
	}
	message += idents;
	message += "</tr>";

	// end table...
	message += "</table>";
	info -> setText (message);
	}
}
