/**************
FILE          : monitor.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : configure the monitor properties including
              : sync ranges DDC capabilities and DPMS rules
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "monitor.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// XMonitor Constructor...
//-------------------------------------
XMonitor::XMonitor (void) {
	mSelected = -1;
}

//=====================================
// XMonitor Constructor...
//-------------------------------------
XMonitor::XMonitor (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
	mSelected = -1;
    addTo (xf,xi);
}

//=====================================
// XMonitor add widget to mainframe...
//-------------------------------------
void XMonitor::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Monitor;
    mPage     = mIndex;
    mLastPage = MonitorPage1;
    getConnected (xf);
    dialogCreate ();
}

//=====================================
// XMonitor create the dialog...
//-------------------------------------
void XMonitor::dialogCreate (void) {
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
	topic -> setFixedWidth  ( metrics.width (mText["curmonitor"]) );
	topic -> setPaper ( QBrush(mDialog->backgroundColor()) );
	topic -> setFrameStyle ( QFrame::NoFrame );
	topic -> setText (mText["curmonitor"]);

	monitorList = new QListBox    ( mDialog );
	mSetup      = new QPushButton ( mText["confmonitor"],mDialog );
	mRemove     = new QPushButton ( mText["removemonitor"],mDialog );

	// create tab dialog widgets...
	// ----------------------------
	// ----------------------------
	// Monitor
	// ============================
	QVBox* layer6 = new QVBox ( mTop );
	QButtonGroup* select = new QButtonGroup (
		1,Vertical,mText["vnselect"],layer6
	);
	mVendor  = new QListBox ( select );
	mName    = new QListBox ( select );
	layer6 -> setSpacing ( 10 );
	QHBox* layer7      = new QHBox ( layer6 );

	QButtonGroup* aspect = new QButtonGroup (
		1,Vertical,mText["aspect"],layer7
	);
	mTraversal = new QComboBox ( aspect );
	mRatio     = new QComboBox ( aspect );

	QLabel* mSep = new QLabel ( layer7 );
	mDisk      = new QPushButton ( layer7 );
	mDisk  -> setText    ( mText["disk"] );
	layer7 -> setStretchFactor ( mDisk,0 );
	layer7 -> setStretchFactor ( mSep,100 );
	layer6 -> setMargin  ( 10 );

	// ----------------------------
	// Frequency
	// ============================
	QVBox* layer8   = new QVBox ( mTop );
	QButtonGroup* frequency = new QButtonGroup (
		1,Horizontal,mText["frqsetup"],layer8
	);
	QHBox* pixbox = new QHBox  ( frequency );
	QLabel* nope  = new QLabel ( pixbox );
	nope -> setFixedWidth (5);
	QLabel* pixfrq  = new QLabel ( pixbox );
	pixfrq -> setPixmap (
		QPixmap (mInit["pix_monitor_frq"])
	);
	QHBox* layer9 = new QHBox  ( frequency );
	QLabel* hnope = new QLabel ( layer9 );
	hnope -> setFixedWidth (10);
	QLabel* HLabel  = new QLabel ( mText["horizontal"] , layer9 );
	HLabel -> setFixedWidth ( 120 );
	layer9 -> setSpacing ( 10 );
	mHSpinMin = new QSpinBox (20,300,1,layer9);
	layer9 -> setSpacing ( 5 );
	QLabel* hbar = new QLabel (layer9);
	hbar -> setText ("-");
	layer9 -> setSpacing ( 5 );
	mHSpinMax = new QSpinBox (30,300,1,layer9);
	layer9 -> setSpacing ( 10 );
	QLabel* HUnit   = new QLabel ( mText["khz"], layer9 );
	HUnit  -> setFixedWidth ( 50 );
	layer9 -> setStretchFactor ( HLabel,0 );
	layer9 -> setStretchFactor ( mHSpinMin,50);
	layer9 -> setStretchFactor ( mHSpinMax,50);
	layer9 -> setStretchFactor ( HUnit,0);
	QHBox* layer10 = new QHBox  ( frequency );
	QLabel* vnope  = new QLabel ( layer10 );
	vnope -> setFixedWidth (10);
	QLabel* VLabel  = new QLabel ( mText["vertical"] , layer10 );
	VLabel -> setFixedWidth ( 120 );
	layer10 -> setSpacing ( 10 );
    mVSpinMin = new QSpinBox (40,300,1,layer10);
	layer10 -> setSpacing ( 5 );
	QLabel* vbar = new QLabel (layer10);
	vbar -> setText ("-");
	layer10 -> setSpacing ( 5 );
	mVSpinMax = new QSpinBox (60,300,1,layer10);
	layer10 -> setSpacing ( 10 );
    QLabel* VUnit   = new QLabel ( mText["hz"], layer10 );
	VUnit  -> setFixedWidth ( 50 );
	layer10 -> setStretchFactor ( VLabel,0 );
	layer10 -> setStretchFactor ( mVSpinMin,50);
	layer10 -> setStretchFactor ( mVSpinMax,50);
	layer10 -> setStretchFactor ( VUnit,0);
	layer8 -> setMargin ( 10 );

	// add a note...
	// --------------
	QTextView* note = new QTextView ( frequency );
	note -> setVScrollBarMode (QScrollView::AlwaysOff);
	note -> setPaper ( QBrush(mDialog->backgroundColor()) );
	note -> setFrameStyle ( QFrame::NoFrame );
	note -> setText (mText["frqnote"]);

	// ----------------------------
	// Expert
	// ============================
	layer11   = new QVBox ( mTop );
	QButtonGroup* algorithm = new QButtonGroup (
		1,Horizontal,mText["algorithm"],layer11
	);
	mMode = new QComboBox ( FALSE,algorithm );
	layer11 -> setSpacing ( 10 );
	QButtonGroup* size = new QButtonGroup (
		1,Vertical,mText["monitorsize"],layer11
	);
	QHBox* layer12 = new QHBox ( size );
	QLabel* X = new QLabel ( mText["x"], layer12 );
	layer12 -> setSpacing ( 10 );
	mXspin = new QSpinBox ( 0,1000,1,layer12 );
	layer12 -> setSpacing ( 20 );
	QLabel* Y = new QLabel ( mText["y"], layer12 );
	layer12 -> setSpacing ( 10 );
    mYspin = new QSpinBox ( 0,1000,1,layer12 );
	layer12 -> setStretchFactor ( X,0 );
    layer12 -> setStretchFactor ( Y,0);
    layer12 -> setStretchFactor ( mXspin,100);
	layer12 -> setStretchFactor ( mYspin,100);
	layer11  -> setSpacing ( 20 );
	mDPMS     = new QCheckBox ( mText["dpms"],layer11 );
	layer11  -> setMargin  ( 10 );
	

	// add tabs to toplevel...
	// -----------------------
	mTop -> addTab ( layer6,mText["monitormodel"] );
	mTop -> addTab ( layer8,mText["freq"] );
	mTop -> addTab ( layer11,mText["experttab"] );
    
	// set OK/Cancel buttons...
	// ------------------------
	mTop -> setOkButton     ( mText["Ok"] );
	mTop -> setCancelButton ( mText["Cancel"] );

	// disable configure button...
	// ---------------------------
	// mSetup -> setDisabled ( TRUE );

	// add widgets to the layout(s)
	// -----------------------------
	layer2 -> addWidget  ( topic );
	layer2 -> addStretch ( 10 );
	layer4 -> addWidget  ( monitorList );
	layer5 -> addWidget  ( mSetup  );
	layer5 -> addSpacing ( 5 );
	layer5 -> addWidget  ( mRemove );
	layer5 -> addStretch ( 50 );

	// connect me to the world...
	// ---------------------------
	QObject::connect (
		monitorList, SIGNAL (clicked (QListBoxItem *)),
		this, SLOT (slotSelect (QListBoxItem *))
	);
	QObject::connect (
		monitorList, SIGNAL (doubleClicked (QListBoxItem *)),
		this, SLOT (slotDoubleSelect (QListBoxItem *))
	);
	QObject::connect (
		monitorList, SIGNAL (selectionChanged (QListBoxItem *)),
		this, SLOT (slotSelect (QListBoxItem *))
	);
	QObject::connect (
		mSetup, SIGNAL (clicked (void)),
		this, SLOT (slotSetup (void))
	);
	QObject::connect (
		mRemove, SIGNAL (clicked (void)),
		this, SLOT (slotRemove (void))
	);
	QObject::connect (
		mVendor, SIGNAL (clicked (QListBoxItem *)),
		this, SLOT (slotVendor (QListBoxItem *))
	);
	QObject::connect (
		mVendor, SIGNAL (returnPressed (QListBoxItem *)),
		this, SLOT (slotVendor (QListBoxItem *))
	);
	QObject::connect (
		mVendor, SIGNAL (highlighted (QListBoxItem *)),
		this, SLOT (slotVendor (QListBoxItem *))
	);
	QObject::connect (
		mName, SIGNAL (clicked (QListBoxItem *)),
		this, SLOT (slotName (QListBoxItem *))
	);
	QObject::connect (
		mName, SIGNAL (highlighted (QListBoxItem *)),
		this, SLOT (slotName (QListBoxItem *))
	);
	QObject::connect (
		mDisk, SIGNAL (clicked (void)),
		this, SLOT (slotDisk (void))
	);
	QObject::connect (
		mRatio, SIGNAL (activated (int)),
		this, SLOT (slotRatio (int))
	);
	QObject::connect (
		mTraversal, SIGNAL (activated (int)),
		this, SLOT (slotTraversal (int))
	);
	QObject::connect (
		mHSpinMin, SIGNAL (valueChanged (int)),
		this, SLOT (slotHsyncMin (int))
	);
	QObject::connect (
		mHSpinMax, SIGNAL (valueChanged (int)),
		this, SLOT (slotHsyncMax (int))
	);
	QObject::connect (
		mVSpinMin, SIGNAL (valueChanged (int)),
		this, SLOT (slotVsyncMin (int))
	);
	QObject::connect (
		mVSpinMax, SIGNAL (valueChanged (int)),
		this, SLOT (slotVsyncMax (int))
	);
	QObject::connect (
		mMode, SIGNAL (activated (int)),
		this, SLOT (slotAlgorithm (int))
	);
	QObject::connect (
		mXspin, SIGNAL (valueChanged (int)),
		this, SLOT (slotXSize (int))
	);
	QObject::connect (
		mYspin, SIGNAL (valueChanged (int)),
		this, SLOT (slotYSize (int))
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
// XMonitor slotIntro...
//-------------------------------------
void XMonitor::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QDict<char> desktop;
	XData*      data;

	// wrap file ponter...
	// --------------------
	XWrapFile < QDict<XFile> > mFiles (mFilePtr);
	
	info    = mIntro -> getTextView();
	message = mText["monitor_header"];
	message += "<br><br><table border=1 bgcolor=lightgrey ";
	message += "cellspacing=1 width=90%>";

	for (int n=0; n<mFiles["sys_DESKTOP"]->getDeviceCount(); n++) {
		data = mFiles["sys_DESKTOP"] -> getDevice(n);
		if (! data) {
			continue;
		}
		desktop = data -> getData();
		message += "<tr>";
		idents.sprintf(
			"<td width=10%c align=center>( %d )</td>",'%',n
		);
		message += idents;
		QString model (desktop["VendorName"]);
		QString name  (desktop["ModelName"]);
		if (model != name) {
			model += " "; model += name;
		}
		if (model == "Unknown") {
			model = mText["unknwonMonitor"];
		}
		QTextOStream (&idents)
			<< "<td width=80%>" << model << "</td>";
		message += idents;
		message += "</tr>";
	}

	message += "</table><br>";
	info -> setText (message);
	}
}
