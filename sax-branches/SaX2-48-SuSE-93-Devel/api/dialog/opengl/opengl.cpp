/**************
FILE          : opengl.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure the 3D OpenGL subsystem
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "opengl.h"

//=====================================
// external globals
//-------------------------------------
extern int global3DActive;

//=====================================
// XOpenGL Constructor...
//-------------------------------------
XOpenGL::XOpenGL (void) {
	mCheck3DStatusChanged = false;
}

//=====================================
// XOpenGL Constructor...
//-------------------------------------
XOpenGL::XOpenGL (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	mCheck3DStatusChanged = false;
	addTo (xf,xi);
}

//=====================================
// XOpenGL add widget to mainframe...
//-------------------------------------
void XOpenGL::addTo (XFrame* xf,XIntro* xi) {
	mIntro    = xi;
	mFrame    = xf;
	mInit     = mFrame -> getInit();
	mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
	mIndex    = OpenGL;
	mPage     = mIndex;
	mLastPage = OpenGLPage1;
	getConnected (xf);
	dialogCreate ();
}

//=====================================
// XOpenGL create the dialog...
//-------------------------------------
void XOpenGL::dialogCreate (void) {
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
	QLabel* pix3d = new QLabel ( mDialog );
	pix3d -> setPixmap (
		QPixmap (mInit["pix_intro_3d"])
	);
	QFontMetrics metrics ( font() );
	QTextView* infoline = new QTextView ( mDialog );
	infoline -> setVScrollBarMode (QScrollView::AlwaysOff);
	infoline -> setFixedHeight (40);
	infoline -> setFixedWidth  ( metrics.width (mText["openglsetup"]) );
	infoline -> setPaper ( QBrush(mDialog->backgroundColor()) );
	infoline -> setFrameStyle ( QFrame::NoFrame );
	infoline -> setText (mText["openglsetup"]);
	QButtonGroup* modus = new QButtonGroup (
		1,Horizontal,mDialog
	);
	mCheck3D = new QCheckBox ( 
		mText["enable3d"],modus 
	);

	// add widgets to layout and the widget stack
	// ------------------------------------------
	layer2 -> addWidget  ( pix3d );
	layer2 -> addSpacing ( 10 );
	layer2 -> addWidget  ( infoline );
	layer2 -> addStretch ( 10 ); 
	layer3 -> addWidget  ( modus );

	// connect me to the world...
	// ---------------------------
	QObject::connect (
		mCheck3D  , SIGNAL ( clicked () ),
		this      , SLOT   ( slot3D  () )
	);

	initPage ();
	mStack -> addWidget  ( mDialog , mIndex );
}

//=====================================
// XOpenGL slotIntro...
//-------------------------------------
void XOpenGL::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> layout;

	info    = mIntro -> getTextView();
	message = mText["opengl_header"];
	info    -> setText (message);
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	message += "<tr>";
	if (global3DActive) {
		QTextOStream (&idents) << "<td>" << mText["on"] << "</td>";
	} else {
		QString noscan ("noscan");
		XStringList packageInfo (
			qx (GET3D,STDOUT,1,"%s",noscan.ascii())
		);
		packageInfo.setSeperator (":");
		QList<char> result = packageInfo.getList();
		QString answer3D = result.at(5);
		if (answer3D == "yes") {
			QTextOStream (&idents)
			<< "<td>" << mText["unavailable"] << "</td>";
		} else {
			QTextOStream (&idents)
			<< "<td>" << mText["notConfigured"] << "</td>";
		}
	}
	message += idents;
	message += "</tr>";
	message += "</table>";
	info -> setText (message);
	}
}
