/**************
FILE          : virtual.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure virtual resolutions for each desktop
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "virtual.h"

//=====================================
// XVirtual Constructor...
//-------------------------------------
XVirtual::XVirtual (void) {
}

//=====================================
// XVirtual Constructor...
//-------------------------------------
XVirtual::XVirtual (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	addTo (xf,xi);
}

//=====================================
// XVirtual add widget to mainframe...
//-------------------------------------
void XVirtual::addTo (XFrame* xf,XIntro* xi) {
	mIntro    = xi;
	mFrame    = xf;
	mInit     = mFrame -> getInit();
	mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
	mIndex    = Virtual;
	mPage     = mIndex;
	mLastPage = VirtualPage1;
	getConnected (xf);
	dialogCreate ();
}

//=====================================
// XVirtual create the dialog...
//-------------------------------------
void XVirtual::dialogCreate (void) {
	mDialog = new QFrame ( mStack );
	QBoxLayout* layer1 = new QVBoxLayout ( mDialog );

	mDesktop = new QComboBox ( FALSE,mDialog );
	mVirtualStack = new QWidgetStack ( mDialog );
	mVirtualStack -> setFixedWidth  ( 
		mStack->width() + mFrame->getComponentView()->width() - 10
	);
	mVirtualStack -> setFixedHeight (
		mStack->height() - mDesktop->height() - 10
	);

	layer1 -> addWidget  ( mDesktop );
	layer1 -> setSpacing ( 10 );
	layer1 -> addWidget  ( mVirtualStack );
	layer1 -> addStretch ( 20 );
	layer1 -> setMargin  ( 10 );

	QObject::connect (
		mDesktop  , SIGNAL ( activated    (int) ),
		this      , SLOT   ( slotDesktop  (int) ) 
	);
	QObject::connect (
		mFrame    , SIGNAL ( sigResize    (void)),
		this      , SLOT   ( slotResize   (void))
	);

	mStack -> addWidget ( 
		mDialog , mIndex 
	);
}

//=====================================
// XVirtual got resize event...
//-------------------------------------
void XVirtual::slotResize ( void ) {
	mVirtualStack -> setFixedWidth  ( mStack->width()  - 10 );
	mVirtualStack -> setFixedHeight ( 
		mStack->height() - mDesktop->height() - 10 
	);
	int count = 0;
	while (1) {
		QWidget* w = NULL;
		w = mVirtualStack -> widget (count);
		if (! w) {
			break;
		}
		w -> setFixedWidth  ( 
			mVirtualStack->width() 
		);
		w -> setFixedHeight ( 
			mVirtualStack->height() 
		);
		count++;
	}
}

//=====================================
// XVirtual slotIntro...
//-------------------------------------
void XVirtual::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	XData*      sysData;
	QDict<char> layout;

	// wrap file pointer...
	// ----------------------
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);

	info    = mIntro -> getTextView();
	message = mText["virtual_header"];
	info    -> setText (message);
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
		sysData = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! sysData) {
			continue;
		}
		QDict<char> desktopInfo = sysData -> getData();
		QString defaultColor (desktopInfo["ColorDepth"]);
		QString reskey; reskey.sprintf("Virtual:%s",defaultColor.ascii());
		message += "<tr>";
		if (desktopInfo[reskey]) {
			XStringList res (desktopInfo[reskey]);
			res.setSeperator (" ");
			QString vres;
			vres.sprintf("X=%s , Y=%s",
				res.getList().at(0),res.getList().at(1)
			);
			idents.sprintf(
				"<td width=10%c align=center>( %d )</td>",'%',n
			);
			message += idents;
			idents.sprintf (
				"<td width=80%c>%s</td>",'%',vres.ascii()
			);
		} else {
			QTextOStream (&idents)
				<< "<td width=10% align=center>( " << n << " )</td>"
				<< "<td width=80%>" << mText["notConfigured"] << "</td>";
		}
		message += idents;
		message += "</tr>";
	}
	message += "</table>";
	info -> setText (message);
	}
}
