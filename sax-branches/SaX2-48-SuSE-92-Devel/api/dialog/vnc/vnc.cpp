/**************
FILE          : vnc.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure VNC for X11. The VNC protocol provides
              : the possibility to export the X-Server desktop
              : for remote access
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "vnc.h"

//=====================================
// Xvnc Constructor...
//-------------------------------------
Xvnc::Xvnc (void) {
	noInputConnection = true;
	VNCPrepared = false;
}

//=====================================
// Xvnc Constructor...
//-------------------------------------
Xvnc::Xvnc (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	noInputConnection = true;
	VNCPrepared = false;
	addTo (xf,xi);
}

//=====================================
// Xvnc add widget to mainframe...
//-------------------------------------
void Xvnc::addTo (XFrame* xf,XIntro* xi) {
	mIntro    = xi;
	mFrame    = xf;
	mInit     = mFrame -> getInit();
	mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
	mIndex    = XVnc;
	mPage     = mIndex;
	mLastPage = XVncPage1;
	getConnected (xf);
	dialogCreate ();
}

//=====================================
// Xvnc create the dialog...
//-------------------------------------
void Xvnc::dialogCreate (void) {
	XWrapPointer< QDict<char> > mText (mTextPtr);

	mDialog = new QFrame ( mStack );
	QBoxLayout* layer1 = new QVBoxLayout ( mDialog );
	QBoxLayout* layer2 = new QVBoxLayout ( layer1  );
	layer1 -> addSpacing ( 10 );
	QBoxLayout* layer3 = new QVBoxLayout ( layer1  );
	layer1 -> addSpacing ( 10 );
	layer1 -> setStretchFactor (layer3,50);

	QHBox* topic = new QHBox ( mDialog ); 
	QLabel* pixvnc = new QLabel ( topic );
	pixvnc -> setPixmap (
		QPixmap (mInit["pix_xcontrol"])
	);
	topic -> setSpacing (10);
	QLabel* infoline = new QLabel ( topic );
	infoline -> setText ( mText["infovnc"] );
	topic -> setStretchFactor (infoline,50);

	mCheckVNC = new QCheckBox ( mText["enablevnc"], mDialog );

	mOptions = new QButtonGroup (
		1,Horizontal,mText["vncoptions"],mDialog
	);
	mOptions -> setDisabled (true);
	mCheckPWD = new QCheckBox ( mText["pwdprotect"], mOptions );

	QHBox* space = new QHBox ( mOptions );
	QLabel* hidden1 = new QLabel ( space );
	hidden1 -> setFixedWidth ( 22 );
	mEntries = new QVBox ( space );
	mEntries -> setSpacing (5);
	mEntries -> setDisabled ( true );

	QVBox* pwd1 = new QVBox ( mEntries );
	QLabel* pwd1text = new QLabel ( pwd1 );
	pwd1text -> setText ( mText["password"] );
	mPWD1 = new QLineEdit ( pwd1 );
	mPWD1 -> setText ( mText["newpwd"] );
	mPWD1 -> setMaximumWidth ( 400 );

	QVBox* pwd2 = new QVBox ( mEntries );
	QLabel* pwd2text = new QLabel ( pwd2 );
	pwd2text -> setText ( mText["passwordverify"] );
	mPWD2 = new QLineEdit ( pwd2 );
	mPWD2 -> setEchoMode ( QLineEdit::Password );
	mPWD2 -> setMaximumWidth ( 400 );

	QLabel* hidden2 = new QLabel ( mEntries );
	hidden2 -> setFixedHeight ( 5 );
	
	mCheckViewOnly = new QCheckBox ( mText["vncviewonly"], mOptions );
	mCheckShared   = new QCheckBox ( mText["vncshared"], mOptions );

	mCheckHTTP = new QCheckBox ( mText["vnchttp"], mOptions );

	QHBox* space2 = new QHBox ( mOptions );
	QLabel* hidden3 = new QLabel ( space2 );
	hidden3 -> setFixedWidth ( 22 );
	mHTTPEntries = new QVBox ( space2 );
	mHTTPEntries -> setSpacing (5);
	mHTTPEntries -> setDisabled ( true );

	QVBox* http = new QVBox ( mHTTPEntries );
	QLabel* httptext = new QLabel ( http );
	httptext -> setText ( mText["httpport"] );
	mHTTP = new QSpinBox (5000,10000,1,http);
	mHTTP -> setValue ( 5800 );
	mHTTP -> setMaximumWidth ( 400 );

	// connect me to the world...
	// --------------------------
	QObject::connect (
		mCheckVNC , SIGNAL ( toggled       ( bool ) ),
		this      , SLOT   ( slotState     ( bool ) )
	);
	QObject::connect (
		mCheckPWD , SIGNAL ( toggled       ( bool ) ),
		this      , SLOT   ( slotPassword  ( bool ) )
	);
	QObject::connect (
		mCheckHTTP , SIGNAL ( toggled       ( bool ) ),
        this       , SLOT   ( slotHTTP      ( bool ) )
    );

	// add widgets to the layout...
	// ----------------------------
	layer2 -> addWidget  ( topic );
	layer2 -> addSpacing ( 10 );
	layer2 -> addWidget  ( mCheckVNC );
	layer3 -> addWidget  ( mOptions );
	layer1 -> setMargin  ( 10 );

	initPage ();

	// add widgets to the stack...
	// ---------------------------
	mStack -> addWidget ( mDialog , mIndex );
}

//=====================================
// Xvnc slotIntro...
//-------------------------------------
void Xvnc::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	XData*      data;
	QDict<char> card;

	info    = mIntro -> getTextView();
	message = mText["xvnc_header"];

	// check if VNC configuration is present
	bool active = false;
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	for (int n=0; n<=mFiles["sys_CARD"]->getDeviceCount(); n++) {
		data = mFiles["sys_CARD"] -> getDevice(n);
		if (! data) {
			continue;
		}
		card = data -> getData();
		if ((card["Option"]) && (strstr(card["Option"],"usevnc") != NULL)) {
			active = true;
			break;
		}
		if ((card["RawData"]) && (strstr(card["RawData"],"usevnc") != NULL)) {
			QString rawopt = qx (
				GETRAWLIST,STDOUT,1,"%s",card["RawData"]
			);
			XStringList cur;
			cur.setText (rawopt);
			cur.setSeperator ("|");
			QList<char> strlist = cur.getList();
			QListIterator<char> it (strlist);
			QList<char> cleanOptionList;
			for (; it.current(); ++it) {
				int count = 0;
				QStringList optlist = QStringList::split ( "\"", it.current() );
				for ( QStringList::Iterator
					in = optlist.begin(); in != optlist.end(); ++in
				) {
					QString* result = new QString (*in);
					*result = result->stripWhiteSpace();
					if (result->isEmpty()) {
						continue;
					}
					if (*result == "Option") {
						count++; continue;
					}
					cleanOptionList.append (*result);
					count++;
				}
			}
			QListIterator<char> io (cleanOptionList);
			bool vncOption = false;
			for (; io.current(); ++io) {
				if ( (vncOption) && (QString(io.current()) == "yes") ) {
					active = true;
					break;
				}
				if (QString(io.current()) == "usevnc") {
					vncOption = true;
				}
			}
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
		<<"<td width=200>"<< mText["vncexport"] <<" "<< mText["yes"] <<"</td>";
	} else {
	QTextOStream (&idents)
		<<"<td width=200>"<< mText["vncexport"] <<" "<< mText["no"] <<"</td>";
	}
	message += idents;
	message += "</tr>";

	// end table...
	message += "</table>";
	info -> setText (message);
	}
}
