/**************
FILE          : multihead.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure the multihead mode (clone,traditional,xinerama)
              : if the environment is multiheaded of course
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "multihead.h"

//=====================================
// XMultihead Constructor...
//-------------------------------------
XMultihead::XMultihead (void) {
}

//=====================================
// XMultihead Constructor...
//-------------------------------------
XMultihead::XMultihead (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
    addTo (xf,xi);
}

//=====================================
// XMultihead add widget to mainframe...
//-------------------------------------
void XMultihead::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Modus;
    mPage     = mIndex;
    mLastPage = MultiheadPage1;
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XMultihead create the dialog...
//-------------------------------------
void XMultihead::dialogCreate (void) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
    mDialog = new QFrame ( mStack );
    QBoxLayout* layer1 = new QVBoxLayout ( mDialog );
	layer1 -> setMargin  ( 10 );
	QBoxLayout* layer2 = new QHBoxLayout ( layer1  );
	layer1 -> addSpacing ( 10 );
	QBoxLayout* layer3 = new QVBoxLayout ( layer1  );
	layer1 -> addStretch ( 10 );

	// create widgets...
	// -----------------
	QLabel* pixlayout = new QLabel ( mDialog );
    pixlayout -> setPixmap (
        QPixmap (mInit["pix_intro_card"])
    );
	QFontMetrics metrics ( font() );
	QTextView* infoline = new QTextView ( mDialog );
	infoline -> setVScrollBarMode (QScrollView::AlwaysOff);
	infoline -> setFixedHeight (40);
	infoline -> setFixedWidth  ( metrics.width (mText["modusMultihead"]) );
	infoline -> setPaper ( QBrush(mDialog->backgroundColor()) );
	infoline -> setFrameStyle ( QFrame::NoFrame );
	infoline -> setText (mText["modusMultihead"]);
	QButtonGroup* modus = new QButtonGroup ( 
		1,Horizontal,mDialog
	);
	mTraditional = new QRadioButton ( mText["modusTraditional"],modus );
	mClone       = new QRadioButton ( mText["modusClone"],modus );
	mXinerama    = new QRadioButton ( mText["modusXinerama"],modus );

	// add widgets to layout and the widget stack
	// ------------------------------------------
	layer2 -> addWidget  ( pixlayout );
	layer2 -> addSpacing ( 10 );
	layer2 -> addWidget  ( infoline );
	layer2 -> addStretch ( 10 );
	layer3 -> addWidget  ( modus );

	initPage ();
    mStack -> addWidget  ( mDialog , mIndex );
}

//=====================================
// XMultihead slotIntro...
//-------------------------------------
void XMultihead::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> layout;
	XData*      data;
	int         cards;

	// is multihead available...
	// --------------------------
	mAvailable = TRUE;

	// wrap file pointer...
	// ---------------------
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText["multihead_header"];
	info    -> setText (message);
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";
	
	data = mFiles["sys_LAYOUT"] -> getDevice(0);
	if (! data) {
		return;
	}
	layout = data -> getData();
	cards  = mFiles["sys_CARD"]->getDeviceCount();
	if (cards > 1) {
		message += "<tr>";
		if (QString(layout["Clone"]) == "on") {
		QTextOStream (&idents) << "<td>" << mText["cloned"] << "</td>";
		} else if (QString(layout["Xinerama"]) == "on") {
		QTextOStream (&idents) << "<td>" << mText["xineramafied"] << "</td>";
		} else {
		QTextOStream (&idents) << "<td>" << mText["traditional"] << "</td>";
		}
		message += idents;
		message += "</tr>";
	} else {
		message += "<tr>";
		QTextOStream (&idents) << "<td>" << mText["nomulti"] << "</td>";
		message += idents;
		message += "</tr>";
		mAvailable = FALSE;
	}

	message += "</table>";
	info -> setText (message);
	}
}
