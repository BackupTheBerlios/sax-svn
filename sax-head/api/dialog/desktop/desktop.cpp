/**************
FILE          : desktop.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure colors and resolution for each desktop
              : bound to a graphics card
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "desktop.h"
#include <qevent.h>
#include <qnamespace.h>

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XDesktop Constructor...
//-------------------------------------
XDesktop::XDesktop (void) {
	mFramebufferUsed = false;
	mSelected = -1;
}

//=====================================
// XDesktop Constructor...
//-------------------------------------
XDesktop::XDesktop (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	mFramebufferUsed = false;
	mSelected = -1;
    addTo (xf,xi);
}

//=====================================
// XDesktop add widget to mainframe...
//-------------------------------------
void XDesktop::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Colors;
    mPage     = mIndex;
    mLastPage = DesktopPage1;
	mIntro -> store (this,mIndex);
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XDesktop get main Desktop ListBox...
//-------------------------------------
QListBox* XDesktop::getList (void) {
	return (mDesktopList);
}

//=====================================
// XDesktop create the dialog...
//-------------------------------------
void XDesktop::dialogCreate (void) {
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
	topic -> setFixedWidth  ( metrics.width (mText["curdesktop"]) );
	topic -> setPaper ( QBrush(mDialog->backgroundColor()) );
	topic -> setFrameStyle ( QFrame::NoFrame );
	topic -> setText (mText["curdesktop"]);

	mDesktopList  = new QListBox    ( mDialog );
	mSetup        = new QPushButton ( mText["confdesktop"],mDialog );
	
	// create layouts for the tab dialog
	// -----------------------------------
	layer6  = new QVBox ( mTop );
	layer7  = new QVBox ( mTop );
    layer8  = new QVBox ( mTop );
	QHBox* layer9  = new QHBox ( layer8 );
	QHBox* layer10 = new QHBox ( layer8 );

	// create tab dialog widgets...
	// ----------------------------
	// ----------------------------
	// Colors
	// ============================
	QButtonGroup* color = new QButtonGroup (
		1,Horizontal,mText["colorsetup"],layer6
	);
	layer6 -> setMargin (10);
	QLabel* pixcolor = new QLabel ( color );
	pixcolor -> setPixmap (
		QPixmap (mInit["pix_desktop_color"])
	);
	mColor = new QComboBox (FALSE,color);

	// ----------------------------
	// Resolution
	// ============================
	mResolution = new QListView ( layer7 );
	mResolution -> setRootIsDecorated( TRUE );
	mResolution -> setSorting ( -1 );
	mResolution -> addColumn (mText["resselect"],340);
	QLabel* seperator = new QLabel (layer7);
	seperator -> setFixedHeight (10);
	QLabel* start = new QLabel (layer7);
	start -> setText (mText["startres"]);
	mInfo = new QLabel (layer7);
	mInfo -> setFrameStyle (QFrame::StyledPanel|QFrame::Sunken);
	layer7 -> setMargin (10);

	// ----------------------------
	// Expert
	// ============================
	QTextView* owntip = new QTextView ( layer8 );
	owntip -> setVScrollBarMode (QScrollView::AlwaysOff);
	owntip -> setPaper ( QBrush(mDialog->backgroundColor()) );
	owntip -> setFrameStyle ( QFrame::NoFrame );
	owntip -> setText (mText["owntip"]);
	QButtonGroup* expertx  = new QButtonGroup (
		1,Horizontal,mText["addx"],layer9
	);
	layer9->setSpacing ( 10 );
	QButtonGroup* experty  = new QButtonGroup (
		1,Horizontal,mText["addy"],layer9
	);
	QLabel* sep1 = new QLabel ( layer8 );
	mNewX = new QSpinBox (320,3600,10,expertx);
	mNewY = new QSpinBox (200,3600,10,experty);
	layer8 -> setSpacing (10);
	mAdd  = new QPushButton ( mText["addres"],layer10 );
	mAdd -> setDisabled (false);
	QLabel* sep2 = new QLabel ( layer10 );
	layer8 -> setMargin (10);
	layer10 -> setStretchFactor ( mAdd,0 );
	layer10 -> setStretchFactor ( sep2,100 );
	layer8 -> setStretchFactor ( layer9,0 );
	layer8 -> setStretchFactor ( layer10,0 );
	layer8 -> setStretchFactor ( sep1,100 );

	// add tabs to toplevel...
	// -----------------------
	mTop -> addTab ( layer6,mText["colortab"] );
	mTop -> addTab ( layer7,mText["resolutiontab"] );
	mTop -> addTab ( layer8,mText["experttab"] );

	// set OK/Cancel buttons...
	// ------------------------
	mTop -> setOkButton      ( mText["Ok"] );
	mTop -> setCancelButton  ( mText["Cancel"] );

	// disable configure button...
	// ---------------------------
	// mSetup -> setDisabled ( TRUE );

	// add widgets to the layout(s)
    // -----------------------------
	layer2 -> addWidget  ( topic );
	layer2 -> addStretch ( 10 );
	layer4 -> addWidget  ( mDesktopList );
	layer5 -> addWidget  ( mSetup );
	layer5 -> addStretch ( 50 );

	// connect me to the world...
	// ---------------------------
	QObject::connect (
		mDesktopList, SIGNAL (clicked (QListBoxItem *)),
		this, SLOT (slotSelect (QListBoxItem *))
	);
	QObject::connect (
		mDesktopList, SIGNAL (doubleClicked (QListBoxItem *)),
		this, SLOT (slotDoubleSelect (QListBoxItem *))
	);
	QObject::connect (
		mDesktopList, SIGNAL (selectionChanged (QListBoxItem *)),
		this, SLOT (slotSelect (QListBoxItem *))
	);
	QObject::connect (
		mSetup, SIGNAL (clicked (void)),
		this, SLOT (slotSetup (void))
	);
	QObject::connect (
		mColor, SIGNAL (activated (int)),
		this, SLOT (slotColor (int))
	);
	QObject::connect (
		mResolution, SIGNAL (clicked (QListViewItem*)),
		this, SLOT (slotUsed (QListViewItem*))
	);
	#if 0
	QObject::connect (
		mResolution, SIGNAL (returnPressed (QListViewItem*)),
		this, SLOT (slotUsed (QListViewItem*))
	);
	#endif
	QObject::connect (
		mAdd, SIGNAL (clicked (void)),
		this, SLOT (slotAdd (void))
	);
	QObject::connect (
		mNewX , SIGNAL (valueChanged (int)),
		this, SLOT (slotXChanged (int))
	);
	QObject::connect (
		mNewY, SIGNAL (valueChanged (int)),
		this, SLOT (slotYChanged (int))
	);
	QObject::connect (
		mTop, SIGNAL (applyButtonPressed()),
		this, SLOT (slotTopOk())
	);
	QObject::connect (
		mTop, SIGNAL (cancelButtonPressed()),
		this, SLOT   (slotTopCancel())
	);

	initPage ();
    mStack -> addWidget ( mDialog , mIndex );
}

//=====================================
// XDesktop slotIntro...
//-------------------------------------
void XDesktop::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	XData*      sysData;
	QString     message;
	QString     idents;

	// wrap file pointer...
	// ----------------------
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText["desktop_header"];
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
		sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! sysData) {
			continue;
		}
		QDict<char> desktopInfo = sysData -> getData();
		QString defaultColor (desktopInfo["ColorDepth"]);
		QString reskey; reskey.sprintf("Modes:%s",defaultColor.ascii());
		XStringList res (desktopInfo[reskey]);
		res.setSeperator (",");
		QString primary = res.getList().at(0);
		message += "<tr>";
		idents.sprintf(
			"<td width=10%c align=center>( %d )</td>",'%',n
		);
		message += idents;
		idents.sprintf (
			"<td width=40%c>%s</td>",'%',primary.ascii()
		);
		message += idents;
		double colors = pow (2.0,(double)defaultColor.toInt());
		QString colorString;
		if (defaultColor.toInt() == 24) {
			colors = colors / 1e6;
			colorString.sprintf ("%2.1f Mio.",colors);
		} else {
			colorString.sprintf ("%.0f",colors);
		}
		idents.sprintf (
			"<td width=40%c>%s Bit (%s)</td>",'%',
			defaultColor.ascii(),colorString.ascii()
		);
		message += idents;
		message += "</tr>";
	}
	
	message += "</table><br>";
	info -> setText (message);
	}
}

//=====================================
// XCheckListItem constructor...
//-------------------------------------
XCheckListItem::XCheckListItem (
	QListView *parent, const QString & text, QCheckListItem::Type type,
	XIntro* xi
) : QCheckListItem ( parent,text,type ) {

	mResolution = parent;
	mIntro      = xi;
}

//=====================================
// XCheckListItem state changed...
//-------------------------------------
void XCheckListItem::stateChange ( bool ) {
	if ((mResolution->isVisible()) && (isSelected ())) {
		XDesktop* desktopDialog;
		desktopDialog = (XDesktop*) mIntro -> retrieve (Colors);
		desktopDialog -> slotUsed (NULL);
	}
}
