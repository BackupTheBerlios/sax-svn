/**************
FILE          : layout.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure the server layout if a multihead
              : environment is available
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "layout.h"
#include "xplot.h"
#include "xfig.h"

//=====================================
// XLayout Constructor...
//-------------------------------------
XLayout::XLayout (void) {
}

//=====================================
// XLayout Constructor...
//-------------------------------------
XLayout::XLayout (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
    addTo (xf,xi);
}

//=====================================
// XLayout add widget to mainframe...
//-------------------------------------
void XLayout::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Layout;
    mPage     = mIndex;
    mLastPage = LayoutPage1;
	mIntro -> store (this,mIndex);
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XLayout create the dialog...
//-------------------------------------
void XLayout::dialogCreate (void) {
    mDialog = new QFrame ( mStack );
	XWrapPointer< QDict<char> > mText (mTextPtr);

	// create layout structure...
	// --------------------------
	QBoxLayout* layer1 = new QVBoxLayout ( mDialog );
	QBoxLayout* layer2 = new QHBoxLayout ( layer1 );
    QBoxLayout* layer3 = new QHBoxLayout ( layer1 );

	// create topic...
	// -----------------
	QFontMetrics metrics ( font() );
	QTextView* topic = new QTextView ( mDialog );
	topic -> setVScrollBarMode (QScrollView::AlwaysOff);
	topic -> setFixedHeight (40);
	topic -> setFixedWidth  ( metrics.width (mText["curlayout"]) );
	topic -> setPaper ( QBrush(mDialog->backgroundColor()) );
	topic -> setFrameStyle ( QFrame::NoFrame );
	topic -> setText (mText["curlayout"]);

	// create Matrix...
	// ----------------
	mMatrix = new XPlot ( mDialog,FIGURE_SQUARE );
	QObject::connect (
		mMatrix, SIGNAL  (sigFigure (int)),
		this   , SLOT    (gotFigure (int))
	);
	mMatrix -> buildMatrix();

	// add widgets to the layout...
	// ----------------------------
	layer2 -> addWidget  ( topic );
	layer2 -> addStretch ( 10 );
    layer3 -> addWidget  ( mMatrix );
	
	initPage ();
    mStack -> addWidget ( mDialog , mIndex );
}

//=====================================
// XLayout slotIntro...
//-------------------------------------
void XLayout::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> layout;
	XData*      data;
	int         cards;

	// is multihead available...
	// -------------------------
	mAvailable = TRUE; 

	info    = mIntro -> getTextView();
	message = mText["layout_header"];
	info    -> setText (message);
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	data = mFiles["sys_LAYOUT"] -> getDevice(0);
	if (! data) {
		return;
	}
	layout = data -> getData();
	cards  = mFiles["sys_CARD"]->getDeviceCount();
	if (cards > 1) {
		message += "<tr>";
		QTextOStream (&idents) 
			<< "<td>" << cards << " " << mText["nlayout"] << "</td>";
		message += idents;
		message += "</tr>";
	} else {
		message += "<tr>";
		QTextOStream (&idents) 
			<< "<td>" << mText["nomulti"] << "</td>";
		message += idents;
		message += "</tr>";
		mAvailable = FALSE;
	}

	message += "</table>";
	info -> setText (message);
	}
}
