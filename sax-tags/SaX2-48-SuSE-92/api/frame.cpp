/**************
FILE          : frame.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : frame will build the main frame for the 
              : SaX2 api and the connection between the
              : component listbox and the view
              :
STATUS        : Status: Up-to-date
**************/
#include <qapplication.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qfileinfo.h>
#include <qwidgetlist.h>
#include <qwidget.h>
#include <qpainter.h>
#include <math.h>
#include <qlist.h>
#include <qtimer.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qstatusbar.h>
#include <qkeycode.h>
#include <qfont.h>
#include <qsplitter.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qwhatsthis.h>
#include <qlistview.h>
#include <locale.h>
#include <libintl.h>
#include <X11/Xlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "frame.h"
#include "file.h"
#include "config.h"
#include "common/log.h"
#include "common/qx.h"
#include "common/xlock.h"
#include "xbox.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//=====================================
// pre declaration of xapi fct()...
//-------------------------------------
void removeXFineCache (void);

//=====================================
// XFrame Constructor...
//-------------------------------------
XFrame::XFrame (
	int x1,int y1,int x2,int y2,int w,WFlags wflags
) : QWidget (
	0,0,wflags 
) {
	setArguments();
	setDisplay();
	setGeometry(x1,y1,x2,y2);
	mFrameWidth = w;
	setInit(); 
}

//=====================================
// XFrame Constructor...
//-------------------------------------
XFrame::XFrame (int w,WFlags wflags) : QWidget (
	0,0,wflags
) {
	setArguments();
	setDisplay();
	setGeometry(20,20,630,420);
	mFrameWidth = w;
	setInit(); 
}

//=====================================
// XFrame argument setup...
//-------------------------------------
void XFrame::setArguments (void) {
	mSetFrameCalled = false;
	mAdjustable = true;
	fastSetup  = FALSE;
	useHwData  = FALSE;
	fullScreen = FALSE;
	yastMode   = FALSE;
	mExitCode  = -1;
}

//=====================================
// XFrame set data modified flag...
//-------------------------------------
void XFrame::setModified (bool flag, QString* filename) {
	int value = flag;
	if (mFileChange.find (*filename)) {
		mFileChange.replace (*filename,&value);
	} else {
		mFileChange.insert  (*filename,&value);
	}
	QDictIterator<int> it (mFileChange);
	for (; it.current(); ++it) {
	if (*it.current() == TRUE) {
		mDataChanged = TRUE; return;
	}
	}
	mDataChanged = FALSE;
}

//=====================================
// XFrame set display for object...
//-------------------------------------
void XFrame::setDisplay (void) {
	Display* dpy = x11Display();
	mDisplay = new QString;
    mDisplay -> sprintf ("%s",DisplayString (dpy));
	mWidth  = QApplication::desktop()->width();
	mHeight = QApplication::desktop()->height();

	#if 1
	XQueryServer adjustable;
	adjustable.setOption ("-j");
	QString* data = adjustable.run();
	QStringList adjustData = QStringList::split ( "\n", *data );
	for ( QStringList::Iterator 
		in = adjustData.begin(); in != adjustData.end(); ++in
	) {
		QString currentAdjustable ( *in );
		currentAdjustable = currentAdjustable.right(
			currentAdjustable.length() - 2
		);
		if (currentAdjustable == "no") {
			mAdjustable = false;
			break;
		}
	}
	#endif
}

//=====================================
// XFrame init paths and pixmaps...
//-------------------------------------
void XFrame::setInit (void) {
	mDataChanged = FALSE;

	mInit.insert("logo"				, LOGO);
	mInit.insert("paper"            , PAPER);
	mInit.insert("gtxfile"			, XAPI);
	mInit.insert("pix_geo"          , PIXGEO);
	mInit.insert("pix_xcontrol"     , PIXXCONTROL);
	mInit.insert("pix_desktop"      , PIXDESKTOP);
	mInit.insert("pix_card"         , PIXCARD);
	mInit.insert("pix_input"        , PIXINPUT);
	mInit.insert("pix_multi"        , PIXMULTI);
	mInit.insert("pix_intro_card"   , PIXINTROCARD);
	mInit.insert("pix_intro_3d"     , PIXINTRO3D);
	mInit.insert("pix_intro_mouse"  , PIXINTROMOUSE);
	mInit.insert("pix_intro_port"   , PIXINTROPORT);
	mInit.insert("pix_intro_kbd"    , PIXINTROKBD);
	mInit.insert("pix_intro_pen"    , PIXINTROPEN);
	mInit.insert("pix_intro_eraser" , PIXINTROERASER);
	mInit.insert("pix_intro_tablet" , PIXINTROTABLET);
	mInit.insert("pix_intro_toucher", PIXINTROTOUCH);
	mInit.insert("pix_desktop_color", PIXDESKTOPCOLOR);
	mInit.insert("pix_desktop_left" , PIXDESKTOPLEFT);
	mInit.insert("pix_desktop_right", PIXDESKTOPRIGHT);
	mInit.insert("pix_monitor_frq"  , PIXMONITORFRQ);
	mInit.insert("pix_kbd_layout"   , PIXKBDLAYOUT);
	mInit.insert("pix_kbd_delay"    , PIXKBDDELAY);
	mInit.insert("pix_kbd_rate"     , PIXKBDRATE);
	mInit.insert("pix_tablet"       , PIXTABLET);
	mInit.insert("pix_touchscreen"  , PIXTOUCHSCREEN);

	// CDB Hash file names...
	// -----------------------
	mInit.insert("cdb_CARDS"		, CARDS);
	mInit.insert("cdb_MONITORS"		, MONITORS);
	mInit.insert("cdb_POINTER"		, POINTER);
	mInit.insert("cdb_PENS"			, PENS);
	mInit.insert("cdb_TABLETS"		, TABLETS);
	mInit.insert("cdb_TOUCHER"		, TOUCHER);

	// QLists...
	// ----------
	mInit.insert("std_PDEVICE"		, PDEVICE);
	mInit.insert("std_MVISUAL"		, MVISUAL);
	mInit.insert("std_C3DMODULES"   , C3DMODULES);
	mInit.insert("std_MRESOLUTION"  , MRESOLUTION);
	
	// QDicts...
	// -----------
	mInit.insert("ext_MFBRESOLUTION", MFBRESOLUTION);
	mInit.insert("ext_KLANGUAGE"	, KLANGUAGE);
	mInit.insert("ext_CMODULES"		, CMODULES);
	mInit.insert("ext_TMODULES"     , TMODULES);
	mInit.insert("ext_MCOLORS"		, MCOLORS);

	// System Config...
	// ------------------
	mInit.insert("sys_CARD"			, ISAXCARD);
	mInit.insert("sys_DESKTOP"		, ISAXDESKTOP);
	mInit.insert("sys_INPUT"		, ISAXINPUT);
	mInit.insert("sys_KEYBOARD"		, ISAXKEYBOARD);
	mInit.insert("sys_LAYOUT"		, ISAXLAYOUT);
	mInit.insert("sys_PATH"			, ISAXPATH);
}

//=====================================
// XFrame text loader...
//-------------------------------------
void XFrame::loadText (void) {
	QDict<char> gtx;
	XFile gtxHandle (mInit["gtxfile"]);

	setlocale (LC_ALL,"");
	bindtextdomain ("sax", TDOMAIN);
	textdomain ("sax");

	gtx = gtxHandle.gtxRead();
	QDictIterator<char> it(gtx);
	for (; it.current(); ++it) {
		if (it.currentKey().contains("_info")) {
			QString* tip = new QString();
			QTextOStream (tip) 
				<< "<img src=\"" << PIXINFO << "\">"
				<< gettext((char*)it.current());
			mText.insert (it.currentKey(),*tip);
			continue;
		}
		if (it.currentKey().contains("_header")) {
			QString* tip = new QString();
			QTextOStream (tip)
				<< "<img src=\"" << ABOUT << "\">"
				<< gettext((char*)it.current());
			mText.insert (it.currentKey(),*tip);
			continue;
		}
		mText.insert (
			it.currentKey(),gettext((char*)it.current())
		);
	}
}

//=====================================
// XFrame set frame width...
//-------------------------------------
void XFrame::setFrameWidth (int w) {
	mFrameWidth = w;
}

//=====================================
// XFrame create widgets...
//-------------------------------------
void XFrame::setFrame (void) {
	//==============================================
	// set geomentry...
	//----------------------------------------------
	mSetFrameCalled = true;
	if ((! fullScreen) && (mWidth > 800)) {
		setGeometry (20,20,800,600);
	} else if (fullScreen) {
		setGeometry (0,0,qApp->desktop()->width(),qApp->desktop()->height());
	}
	//==============================================
	// install event filter
	//----------------------------------------------
	installEventFilter (this);

	//----------------------------------------------
	// create layout structure...
	//==============================================
	// topview...
	//=========================
	QGridLayout* layer0  = new QGridLayout (this,1,1);
	mainFrame = new QFrame ( this );
	mainFrame -> setFrameStyle (
		QFrame::Box|QFrame::Raised
	);
	mainFrame -> setLineWidth ( mFrameWidth - 1 );
	mainFrame -> setMidLineWidth ( mFrameWidth );
	layer0 -> addWidget (mainFrame,0,0);
	QBoxLayout*  layer1  = new QVBoxLayout (mainFrame);

	// menu and seperator...
	//=========================
	QBoxLayout*  layer2  = new QHBoxLayout (layer1);
	QBoxLayout*  layer3  = new QHBoxLayout (layer1);

	// view: for logo component and next view...
	//==========================================
	layer1 -> addSpacing ( 8 );
	QBoxLayout*  layer4  = new QHBoxLayout (layer1);
	layer4 -> addSpacing ( 10 );

	// logo...
	//=========================
	QBoxLayout*  layer5  = new QHBoxLayout (layer4);

	// component...
	//=========================
	QBoxLayout*  layer6  = new QHBoxLayout (layer4);

	// view: for dialog grid and status bar...
	//===========================================
	QBoxLayout*  layer7  = new QVBoxLayout (layer4);
	layer4 -> setStretchFactor ( layer7 , 5 );

	// dialog grid...
	//=========================
	layer8  = new QGridLayout (layer7,1,1);
	layer7 -> setStretchFactor ( layer8 , 5 );
	layer4 -> addSpacing ( 10 );
	layer1 -> addSpacing ( 8 );
	
	// seperator and buttons...
	//=========================
	QBoxLayout*  layer10 = new QHBoxLayout (layer1);
	layer1->addSpacing(8);
	QBoxLayout*  layer11 = new QHBoxLayout (layer1);
	layer1->addSpacing(5);

	// status bar...
	//=========================
	QBoxLayout*  layer9  = new QHBoxLayout (layer1);
	layer1 -> addSpacing ( 5 );

	//----------------------------------------------
	// load texts...
	//==============================================
	loadText();

	// ...
	// wrap text pointer for i18n support
	// ---
	xText.init (getTextPointer());

	//----------------------------------------------
	// Create widgets...
	//==============================================
	// Progressbar
	//-------------
	mProgress = new XProgressDialog (
		"Loading...","Cancel",100,0,"progress",TRUE,
		//Qt::WStyle_Customize |Qt::WStyle_NoBorderEx,
		0,globalFrameWidth
    );
	mProgress -> setFixedWidth (400);

	// Menu
	//------
	mMenu = new QFrame (mainFrame);
	QMenuBar*   mbar = new QMenuBar   (mMenu);
	QPopupMenu* file = new QPopupMenu (this);
	QPopupMenu* help = new QPopupMenu (this);
	file -> insertSeparator();
	file -> insertItem (xText["quit"],  this, SLOT(slotExit()));
	help -> insertItem (xText["about"], this, SLOT(slotAbout()));
	int HelpModuleID = help -> insertItem (
		xText["modules"], this, SLOT(slotModules())
	);
	// ...
	// At the time there is no help system implemented.
	// therefore the help about modules item is disabled here
	// ---
	help -> setItemEnabled (
		HelpModuleID,false
	);

	mbar -> insertItem (xText["file"], file);
	mbar -> insertItem (xText["help"], help);

	// whats this button on the right
	// ------------------------------
	QPushButton* whatsThis = new QPushButton (mainFrame);
    whatsThis -> setPixmap (QPixmap(HELP));
    whatsThis -> setFlat (true);

	// Logo scroll view
	//------------------
	QPixmap*  logo      = new QPixmap  (mInit["logo"]);
	mSview = new QScrollView (mainFrame);
    mImage = new QLabel      (mSview->viewport());
	mImage -> setPixmap (*logo); 
	//component -> setMaximumWidth (190);
	mImage -> adjustSize();
	mSview -> setHScrollBarMode (QScrollView::AlwaysOff);
	mSview -> setVScrollBarMode (QScrollView::AlwaysOff);
	mSview -> setFrameStyle(QFrame::NoFrame);
	mSview -> addChild(mImage);
	mSview -> setMaximumWidth (35);

	// component list
	//----------------
	QListView* component = new QListView (mainFrame);
	//component -> setMaximumWidth (190);
	component -> setFixedWidth   (200);
	component -> addColumn(xText["component"]);
	component -> setResizeMode (QListView::LastColumn);
	component -> setRootIsDecorated(true);
	component -> setSorting ( -1 );
	component -> setItemMargin ( 2 );

	IDxcontrol = new QListViewItem (
		component,xText["xaccesscontrol"]
	);
	IDinput = new QListViewItem (
		component,xText["input"]
	);
	IDmulti = new QListViewItem (
		component,xText["multi"]
	);
	IDdesktop = new QListViewItem (
		component,xText["desktop"]
	);

	IDgeo = NULL;
	if ((! useHwData) && (mAdjustable)) {
	IDgeo      = new QListViewItem (IDdesktop, xText["geometry"]);
	}
	ID3D       = new QListViewItem (IDdesktop , xText["opengl"]);
	IDvirtual  = new QListViewItem (IDdesktop , xText["virtual"]);
	IDcolor    = new QListViewItem (IDdesktop , xText["color"]);
	IDcard     = new QListViewItem (IDdesktop , xText["card"]);
	IDmonitor  = new QListViewItem (IDdesktop , xText["monitor"]);

	IDtoucher  = new QListViewItem (IDinput   , xText["toucher"]);
	IDtablet   = new QListViewItem (IDinput   , xText["tablet"]);
	IDkeyboard = new QListViewItem (IDinput   , xText["keyboard"]);
	IDmouse    = new QListViewItem (IDinput   , xText["mouse"]);

	IDlayout   = new QListViewItem (IDmulti   , xText["mlayout"]);
	IDmode     = new QListViewItem (IDmulti   , xText["modus"]);

	IDaccessx  = new QListViewItem (IDxcontrol, xText["xaccess"]);
	IDvnc      = new QListViewItem (IDxcontrol, xText["vnc"]);

	IDmulti -> setSelectable(false);
	IDmulti -> setPixmap ( 
		0,QPixmap( mInit["pix_multi"] ) 
	);
	IDinput -> setSelectable(false);
	IDinput -> setPixmap ( 
		0,QPixmap( mInit["pix_input"] ) 
	);
	IDdesktop -> setSelectable(false);
	IDdesktop -> setPixmap ( 
		0,QPixmap( mInit["pix_desktop"] ) 
	);
	IDxcontrol -> setSelectable(true);
	IDxcontrol -> setPixmap ( 
		0,QPixmap( mInit["pix_xcontrol"] ) 
	);

	mTree.insert ( xText["mouse"]   , "2" );
	mTree.insert ( xText["tablet"]  , "3" );
	mTree.insert ( xText["toucher"] , "4" );
	mTree.insert ( xText["keyboard"], "5" );
	mTree.insert ( xText["monitor"],  "6" );
	mTree.insert ( xText["color"],    "7" );
	mTree.insert ( xText["mlayout"],  "8" );
	mTree.insert ( xText["modus"],    "9" );
	mTree.insert ( xText["geometry"], "10");
	mTree.insert ( xText["card"],     "11");
	mTree.insert ( xText["xaccess"],  "12");
	mTree.insert ( xText["opengl"],   "13");
	mTree.insert ( xText["virtual"],  "14");
	mTree.insert ( xText["vnc"],      "15");

	component->show();
	mView = component;

	// seperators
	//------------	
	QFrame *seperator1 = new QFrame (mainFrame);
	QFrame *seperator2 = new QFrame (mainFrame);
	seperator1 -> setFixedHeight(2);
	seperator1 -> setFrameStyle (
		QFrame::HLine|QFrame::Raised
	);
    seperator2 -> setFixedHeight(2);
    seperator2 -> setFrameStyle (
		QFrame::HLine|QFrame::Raised
	);

	// button bar Cancel/Next/Back
	//-----------------------------
	mButtonBar1 = new QFrame (mainFrame);
	mButtonBar2 = new QFrame (mainFrame);
	QBoxLayout*  btnlayer1 = new QHBoxLayout (mButtonBar1);
	QBoxLayout*  btnlayer2 = new QHBoxLayout (mButtonBar2);

	QLabel* sustainer = new QLabel (mainFrame);
	sustainer -> setFixedHeight (35);
	sustainer -> setFixedWidth  (0);
	mCancel = new QPushButton (xText["Cancel"],mainFrame);
	mFinish = new QPushButton (xText["Finalize"],mButtonBar1);
	mStart  = new QPushButton (xText["Start"]   ,mButtonBar1);
	mBack   = new QPushButton (xText["Back"]    ,mButtonBar2);
	mNext   = new QPushButton (xText["Next"]    ,mButtonBar2);

	mFinish   -> setDefault (true);
	btnlayer1 -> addWidget  (mStart);
	btnlayer1 -> addSpacing (10);
	btnlayer1 -> addWidget  (mFinish);

	btnlayer2 -> addWidget  (mBack);
	btnlayer2 -> addSpacing (10);
	btnlayer2 -> addWidget  (mNext);

	// status bar
	//------------
	mStatus = new QStatusBar    ( mainFrame );
	if (fullScreen) {
		mStatus -> setSizeGripEnabled (false);
	}
	mStatus -> message (xText["dohint"]);

	//----------------------------------------------
	// connect widgets...
	//============================================== 
	QObject::connect(
		whatsThis, SIGNAL (clicked ()),
		this     , SLOT   (slotWhatsThis())
	);
	QObject::connect(
		component, SIGNAL (doubleClicked    (QListViewItem *)),
		this     , SLOT   (runDialog        (QListViewItem *))
	);
	QObject::connect(
		component, SIGNAL (
		mouseButtonClicked (int,QListViewItem*,const QPoint&,int)
		),
		this     , SLOT   (
		listClicked (int,QListViewItem*,const QPoint&,int)
		)
	);
	QObject::connect(
		component, SIGNAL (selectionChanged (QListViewItem*)),
		this     , SLOT   (slotChanged      (QListViewItem*))
	);
	QObject::connect(
		component, SIGNAL (spacePressed     (QListViewItem *)),
		this     , SLOT   (runDialog        (QListViewItem *))
	);
	QObject::connect(
		mCancel  , SIGNAL (clicked   ()),
		this     , SLOT   (slotCancel())
	);
	QObject::connect(
		mBack    , SIGNAL (clicked   ()),
		this     , SLOT   (slotBack  ())
	);
	QObject::connect(
		mNext    , SIGNAL (clicked   ()),
		this     , SLOT   (slotNext  ())
	);
	QObject::connect(
		mStart   , SIGNAL (clicked   ()),
		this     , SLOT   (slotStart ())
	);
	QObject::connect(
		mFinish  , SIGNAL (clicked   ()),
		this     , SLOT   (slotFinish())
	);

	//----------------------------------------------
	// add widgets to the layout...
	//==============================================
	QBoxLayout* mDl = new QHBoxLayout ();	

	layer2  -> setMargin     (6);
	layer2  -> addWidget     (mMenu);
	layer2  -> addWidget     (whatsThis);
	layer2  -> addSpacing    (5);
	layer2  -> setStretchFactor (mMenu,10);
	layer3  -> addSpacing    (10);
    layer3  -> addWidget     (seperator2);
    layer3  -> addSpacing    (10);
	layer5  -> addWidget     (mSview);
	layer6  -> addWidget     (component);
	layer8  -> addLayout     (mDl,0,0);
	layer9  -> addSpacing    (5);
	layer9  -> addWidget     (mStatus);
	layer9  -> addSpacing    (10);
	layer10 -> addSpacing    (10);
	layer10 -> addWidget     (seperator1);
	layer10 -> addSpacing    (10);
	layer11 -> addSpacing    (10);
	layer11 -> addWidget     (sustainer);
	layer11 -> addWidget     (mCancel);
	layer11 -> addStretch    (4);
	layer11 -> addWidget     (mButtonBar1);
	layer11 -> addWidget     (mButtonBar2);
	layer11 -> addSpacing    (10);

	mDialog = new QFrame (mainFrame);
	mWstack = new QWidgetStack (mDialog);
	mDl -> addWidget (mDialog);
	setCaption ("SaX2");

	//----------------------------------------------
	// set focus to finalize button
	//==============================================
	setActiveWindow();
	setFocusPolicy (QWidget::StrongFocus);
}

//=====================================
// XFrame got show event...
//-------------------------------------
void XFrame::showEvent ( QShowEvent * ) {
	QRect cell = layer8->cellGeometry(0,0);
	mWstack -> setFixedWidth  ( cell.width()  );
	mWstack -> setFixedHeight ( cell.height() );
	mDialog -> adjustSize();
}

//=====================================
// XFrame get repaint event...
//-------------------------------------
void XFrame::resizeEvent (QResizeEvent*) {
    mWstack -> setFixedWidth  (mDialog->width());
	mWstack -> setFixedHeight (mDialog->height());
	mDialogWidth = mDialog->width() - mView->width();
	mSview -> scrollBy (
		0, mImage->height()
	);
	sigResize();
}

//=====================================
// XFrame get progress dialog pointer...
//-------------------------------------
XProgressDialog* XFrame::getProgressDialog (void) {
	return (mProgress);
}

//=====================================
// XFrame get cancel button pointer...
//-------------------------------------
QPushButton* XFrame::cancelButton (void) {
	return(mCancel);
}

//=====================================
// XFrame get finish button pointer...
//-------------------------------------
QPushButton* XFrame::finishButton (void) {
	return(mFinish);
}

//=====================================
// XFrame get back button pointer...
//-------------------------------------
QPushButton* XFrame::backButton   (void) {
	return(mBack);
}

//=====================================
// XFrame get next button pointer...
//-------------------------------------
QPushButton* XFrame::nextButton   (void) {
	return(mNext);
}

//=====================================
// XFrame get start button pointer...
//-------------------------------------
QPushButton* XFrame::startButton (void) {
	return(mStart);
}

//=====================================
// XFrame get change/finish btn bar...
//-------------------------------------
QFrame* XFrame::buttonBar1 (void) {
	return(mButtonBar1);
}

//=====================================
// XFrame get next/back btn bar frame...
//-------------------------------------
QFrame* XFrame::buttonBar2 (void) {
	return(mButtonBar2);
}

//=====================================
// XFrame get status bar pointer...
//-------------------------------------
QStatusBar* XFrame::statusBar (void) {
	return(mStatus);
}

//=====================================
// XFrame get layout pointer...
//-------------------------------------
QWidgetStack* XFrame::getStack (void) {
	return(mWstack);
}

//=====================================
// XFrame get init copy...
//-------------------------------------
QDict<char> XFrame::getInit (void) {
	return(mInit);
}

//=====================================
// XFrame get translation copy...
//-------------------------------------
QDict<char> XFrame::getText (void) {
	return(mText);
}
QDict<char>* XFrame::getTextPointer (void) {
	return(&mText);
}

//=====================================
// XFrame slot getting selection...
//-------------------------------------
void XFrame::runDialog (QListViewItem *lvi) {
	if (lvi->isSelected()) {
	QString* selectedItem  = new QString(lvi->text(0));
	QString* selectedIndex = new QString(mTree[*selectedItem]);
	mComponent = selectedIndex->toInt();
	// ...
	// if the intro page is not visible there is
	// another dialog running and we have to ask
	// for an action
	// ---
	if ((mWstack -> id (mWstack->visibleWidget())) != Intro ) {
		XBox mb (
			xText["visibleinfo"],
			xText["visiblequestion"],
			XBox::Warning,
			XBox::Yes,
			XBox::No, 0,this,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,xText["Yes"]
		);
		mb.setButtonText (
			XBox::No, xText["No"]
		);
		switch (mb.exec()) {
		case XBox::No: {
			return;
		}
		}
		enterEvent ( 0 );
	}
	// ...
	// Ok, call sigRun, all dialogs visible will call
	// resetPage (PAGE_NOLOAD) before raising up if index
	// equals mComponent
	// ---
	sigRun (mComponent);
	mNext -> setFocus();
	}
}

//=====================================
// XFrame public runDialog member...
//-------------------------------------
void XFrame::runDialog (int module) {
	mComponent = module;
	switch (mComponent) {
	case Monitor:
		IDdesktop -> setOpen(true);
		mView -> setSelected (IDmonitor,true);
	break;
	case Card:
		mView -> setSelected (IDcard,true);
	break;
	}
	sigRun (mComponent);
}

//=====================================
// XFrame slot clicked for intro...
//-------------------------------------
void XFrame::runIntro (QListViewItem *lvi) {
	if (lvi) {
	if (lvi->isSelected()) {
	QString* selectedItem  = new QString(lvi->text(0));
	QString* selectedIndex = new QString(mTree[*selectedItem]);
	mStart -> setDisabled (FALSE);
	sigIntro (selectedIndex->toInt());
	return;
	}
	}
	mStart -> setDisabled (TRUE);
}

//=====================================
// XFrame clicked in list view...
//-------------------------------------
void XFrame::listClicked (int,QListViewItem* item,const QPoint&,int) {
	if (item) {
	QString* selectedItem  = new QString(item->text(0));
	QString* selectedIndex = new QString(mTree[*selectedItem]);
	mComponent = selectedIndex->toInt();
	if ((mWstack -> id (mWstack->visibleWidget())) != mComponent ) {
		sigIntro (Intro);
		runIntro (item);
	}
	}
}

//=====================================
// XFrame component selection changed
//-------------------------------------
void XFrame::slotChanged (QListViewItem* item) {
	if (item) {
	QPoint null;
	listClicked (0,item,null,0);
	}
}

//=====================================
// XFrame exit immediatelly...
//-------------------------------------
void XFrame::slotFastExit (void) {
	QString optc ("-cursor_name");
	QString optd ("-display");
	QString cursorName ("top_left_arrow");
	QString display ( DisplayString (x11Display()) );
	qx (XSETROOT,STDNONE,4,"%s %s %s %s",
		optc.ascii(),cursorName.ascii(),optd.ascii(),display.ascii()
	);
	logExit(); exit (EXIT_NOSAVE);
}

//=====================================
// XFrame slot exit program...
//-------------------------------------
void XFrame::slotExit (void) {
	mExitCode = -1; slotCancel();
}

//=====================================
// XFrame slot About box...
//-------------------------------------
void XFrame::slotAbout (void) {
	XBox mb (
		xText["aboutauthor"], xText["author"],
		XBox::Information, XBox::Ok,
		0,0,mainFrame,globalFrameWidth
	);
	mb.setButtonText (
		XBox::Ok,xText["Ok"]
	);
	mb.exec();
	enterEvent ( 0 );
}

//=====================================
// XFrame slot Module help...
//-------------------------------------
void XFrame::slotModules (void) {
	log (L_INFO,"XFrame::slotModules()...\n");
}

//=====================================
// XFrame enter whats this mode...
//-------------------------------------
void XFrame::slotWhatsThis (void) {
	QWhatsThis::enterWhatsThisMode();
	QWidget* widget = mWstack -> visibleWidget();
	QWhatsThis::remove (widget);
	switch (mComponent) {
	case Intro:
	break;
	case Mouse: 
		QWhatsThis::add (widget,xText["mouse_info"]);
	break;
	case Tablet:
		QWhatsThis::add (widget,xText["tablet_info"]);
	break;
	case Toucher:
		QWhatsThis::add (widget,xText["touchscreen_info"]);
	break;
	case Keyboard:
		QWhatsThis::add (widget,xText["keyboard_info"]);
	break;
	case Monitor:
		QWhatsThis::add (widget,xText["monitor_info"]);
	break;
	case Colors:
		QWhatsThis::add (widget,xText["desktop_info"]);
	break;
	case Layout:
		QWhatsThis::add (widget,xText["layout_info"]);
	break;
	case Modus:
		QWhatsThis::add (widget,xText["multihead_info"]);
	break;
	case Geometry:
		QWhatsThis::add (widget,xText["geometry_info"]);
	break;
	case Card:
		QWhatsThis::add (widget,xText["card_info"]);
	break;
	case Xaccess:
		QWhatsThis::add (widget,xText["accessx_info"]);
	break;
	case XVnc:
		QWhatsThis::add (widget,xText["xvnc_info"]);
	break;
	case OpenGL:
		QWhatsThis::add (widget,xText["opengl_info"]);
	break;
	case Virtual:
		QWhatsThis::add (widget,xText["virtual_info"]);
	break;
	default:
		QWhatsThis::add (this,xText["no_info"]);
	break;
	}
}

//=====================================
// XFrame slot next button...
//-------------------------------------
void XFrame::slotNext (void) {
	sigNext();
}

//=====================================
// XFrame slot back button...
//-------------------------------------
void XFrame::slotBack (void) {
	sigBack();
}

//=====================================
// XFrame slot start button...
//-------------------------------------
void XFrame::slotStart (void) {
	QListViewItem* item = mView -> currentItem();
	runDialog (item);
}

//=====================================
// XFrame slot finish button...
//-------------------------------------
void XFrame::slotCancel (void) {
	if ((mWstack -> id (mWstack->visibleWidget())) == Intro ) {
	// ...
	// we are looking at the introduction page and want 
	// to exit SaX2 now... are you sure
	// ---
	if (! yastMode) {
		XBox mb (
			xText["exitinfo"],xText["exitsax"],
			XBox::Information,
			XBox::Yes,
			XBox::No, 0,this,
			globalFrameWidth
		);
		mb.setButtonText (
			XBox::Yes,xText["Yes"]
		);
		mb.setButtonText (
			XBox::No, xText["No"]
		);
		switch (mb.exec()) {
		case XBox::No: {
			enterEvent ( 0 );
			mExitCode = -1; return;
		}
		case XBox::Yes: {
			if (mExitCode <= 0) {
				if (mDataChanged) { unlink (MD5CONFIG); }
				mExitCode = EXIT_NOSAVE;
			}
			logExit();
			exit ( mExitCode );
		}
		}
		} else {
			if (mExitCode <= 0) {
				if (mDataChanged) { unlink (MD5CONFIG); }
				mExitCode = EXIT_NOSAVE;
			}
			logExit();
			exit ( mExitCode );
		}
	} else {
	// ...
	// we are within a configuration dialog and want 
	// to abort this dialog now... are you sure :)
	// ---
	sigAbort (mComponent);
	}
}

//=====================================
// XFrame slot cancel button...
//-------------------------------------
void XFrame::slotFinish (void) {
	mExitCode = EXIT_NOSAVE;
	sigCheck (mComponent);
	// ...
	// If mView widget isn`t visible the user
	// has changed to a configuration dialog and we
	// will abort the finishing workflow
	// ---
	if (! mView -> isVisible()) {
		return;
	}
	bool isFBDevConfig = false;
	QString message = xText["finalsax"];
	QFileInfo lilocode (LILOCODE);
	if (lilocode.exists()) {
		isFBDevConfig = true;
		message = xText["finalsaxfbdev"];
	}
	XBox* mb = new XBox (
		xText["finalstep"],message,XBox::Information,
		TEST, SAVE, CANCEL, this,
		globalFrameWidth
	);
	if (isFBDevConfig) {
		mb->disableButton (0);
	}
	mb->setButtonText ( TEST  , xText["Test"] );
	mb->setButtonText ( SAVE  , xText["Save"] );
	mb->setButtonText ( CANCEL, xText["Cancel"] );
	mb->setButtonFocus (1);
	int choose = mb->exec();
	enterEvent ( 0 );
	delete (mb);
	switch (choose) {
	case TEST: {
		// ... /
		// 1) create config file using getconfig
		// 2) call xrun which will start a server and XFine2
		// 3) handle the return value from xrun
		//    0 error -> show message
		//    2 save  -> linkConfiguration() / EXIT_SAVE
		//    1 abort -> EXIT_NOSAVE
		// ---
		QString optd ("-d");
		QString dpy  (":99");
		qApp->processEvents();
		qApp->setOverrideCursor ( 
			Qt::forbiddenCursor 
		);
		usleep (200);
		updateComplete();
		qApp->processEvents();
		XCommand getConfig ( GETCONFIG );
		getConfig.runCommand();
		while (getproc(TESTCONFIG)) {
			usleep(500); // file still locked...
		}
		QString code = qx ( XRUN,STDOUT,2,"%s %s",
			optd.ascii(),dpy.ascii()
		);
		qApp->restoreOverrideCursor();
		switch (code.toInt()) {
		case 0:
			{
			QString logFile ( TEST_LOG );
			QString logData = qx (GETLOG,STDOUT,1,"%s",
				logFile.ascii()
			);
			QString logMsg;
			QTextOStream(&logMsg) << xText["testfailed"] << logData;
			XBox mb (
				xText["testinfo"],
				logMsg,
				XBox::Critical,
				XBox::Ok, 0,0,this,
				globalFrameWidth
			);
			mb.setButtonText (
				XBox::Ok,xText["Ok"]
			);
			mb.exec();
			enterEvent ( 0 );
			}
			return;
		break;
		case 2:
			{
			qApp->processEvents();
			qApp->setOverrideCursor ( Qt::forbiddenCursor );
			usleep (200);
			updateComplete();
			qApp->processEvents();
			XCommand getConfig ( GETCONFIG );
			getConfig.runCommand();
			while (getproc(TESTCONFIG)) {
				usleep(500); // file still locked...
			}
			qApp->restoreOverrideCursor();
			linkConfiguration();
			mExitCode = EXIT_SAVE;
			}
		break;
		default:
			removeXFineCache();
			mExitCode = EXIT_NOSAVE;
			return;
		break;
		}
		slotCancel();
		break;
	}
	case SAVE: {
		mExitCode = EXIT_SAVE;
		// ... /
		// 1) create config file using getconfig
		// 2) linkConfiguration()
		// 3) set info message that changes will take
		//   effect on next X-Server startup
		// ---
		qApp->processEvents();
		qApp->setOverrideCursor ( Qt::forbiddenCursor );
		usleep (200);
		updateComplete();
		qApp->processEvents();
		XCommand getConfig ( GETCONFIG );
		getConfig.runCommand();
		qApp->restoreOverrideCursor();
		linkConfiguration();
		logExit(); 
		exit ( 
			mExitCode 
		);
		break;
	}
	case CANCEL: {
		return;
	}
	}
}

//=====================================
// XFrame get display...
//-------------------------------------
QString* XFrame::getDisplay (void) {
	return (mDisplay);
}

//=====================================
// XFrame hide component view...
//-------------------------------------
void XFrame::hideListView (void) {
	if (mView -> isVisible()) {
	mView->hide();
	mDialogWidth = mDialog->width();
	mWstack -> setFixedWidth  (
		mDialogWidth + mView->width()
	);
	}
}

//=====================================
// XFrame show component view...
//-------------------------------------
void XFrame::showListView (void) {
	if (! mView -> isVisible()) {
	mView->show();
	mWstack -> setFixedWidth (
		mDialogWidth
	);
	}
}

//=====================================
// XFrame get close event from WM
//-------------------------------------
void XFrame::closeEvent (QCloseEvent*) {
	slotExit();
}

//=====================================
// XFrame start the test X-Server
//-------------------------------------
void XFrame::linkConfiguration (void) {
	// log (L_INFO,"XFrame::linkConfiguration()...\n");
	// ---
	// write configuration file
	// ---
	qx (GETLINKED,STDNONE);
	// ...
	// remove hardware changed flag file and create
	// a new md5sum for the written configuration
	// ---
	unlink (HWFLAG);
	QString mD5Sum = qx(MD5,STDOUT,1,"%s",XCONFIG);
	QFile mHandle (MD5CONFIG);
	if (mHandle.open(IO_WriteOnly)) {
		mHandle.writeBlock (mD5Sum.ascii(),mD5Sum.length());
		mHandle.close();
	}
	QFileInfo lilocode (LILOCODE);
	QString message = xText["savehint"];
	if (lilocode.exists()) {
		message = xText["savehintfb"];
	}
	// ...
	// inform the user about the configuration which
	// has been written now
	// ---
	XBox mb (
		xText["saveinfo"],message,
		XBox::Information,
		XBox::Ok, 0,0,this,
		globalFrameWidth
	);
	mb.setButtonText (
		XBox::Ok,xText["Ok"]
	);
	mb.exec();
	enterEvent ( 0 );
}

//=====================================
// XFrame disable application
//-------------------------------------
void XFrame::disableInteraction (void) {
	mView -> setDisabled (true);
	mMenu -> setDisabled (true);
	mButtonBar1 -> setDisabled (true);
	mButtonBar2 -> setDisabled (true);
	mCancel     -> setDisabled (true);
	setCursor ( 
		QCursor (forbiddenCursor) 
	);
}

//=====================================
// XFrame get component list view
//-------------------------------------
QListView* XFrame::getComponentView (void) {
	return (mView);
}

//=====================================
// XFrame enable application
//-------------------------------------
void XFrame::enableInteraction (void) {
	mView -> setDisabled (false);
	mMenu -> setDisabled (false);
	mButtonBar1 -> setDisabled (false);
	mButtonBar2 -> setDisabled (false);
	mCancel     -> setDisabled (false);
	setCursor ( 
        QCursor (arrowCursor)  
    );
}

//=====================================
// XFrame update all widgets
//-------------------------------------
void XFrame::updateComplete (void) {
	QWidgetList *list = QApplication::allWidgets();
	QWidgetListIt it( *list );
	QWidget * w;
	while ( (w=it.current()) != 0 ) {
		++it; w->update();
	}
	delete list;
}

//=====================================
// XFrame setFrame() already called ?
//-------------------------------------
bool XFrame::frameInitialized (void) {
	if (mSetFrameCalled) {
		return (true);
	}
	return (false);
}

//=====================================
// XFrame set input focus if enterd
//-------------------------------------
void XFrame::enterEvent ( QEvent * ) {
	XSetInputFocus (
		x11Display(),winId(),RevertToParent,CurrentTime
	);
}

//=====================================
// XFrame set global event filter
//-------------------------------------
bool XFrame::eventFilter ( QObject* obj, QEvent* event ) {
	if (event->type() == FocusIn) {
	if ((mView) && ((QListView*)obj != NULL)) {
		//======================================
		// handle QListView FocusIn events
		//--------------------------------------
		mView -> setFocus ();
		mView -> setCurrentItem ( IDdesktop );
		mView -> setSelected ( IDdesktop,true );
	}
	}
	if ((QPushButton*)obj != NULL) {
		QPushButton* btn = (QPushButton*)qApp->focusWidget();
		if (! btn) {
			return (false);
		}
		switch (event->type()) {
		//======================================
		// handle press event
		//--------------------------------------
		case 6:
		if (((QKeyEvent*)event)->key() == Key_Return) {
			QKeyEvent spacePress ( (QEvent::Type)6, Qt::Key_Space, 32 ,0 );
			QApplication::sendEvent ( (QObject*)btn,&spacePress );
			return (true);
		}
		break;
		//======================================
		// handle release event
		//--------------------------------------
		case 7:
		if (((QKeyEvent*)event)->key() == Key_Return) {
			QKeyEvent spaceRelease ( (QEvent::Type)7, Qt::Key_Space, 32 ,0 );
			QApplication::sendEvent ( (QObject*)btn,&spaceRelease );
			return (true);
		}
		break;
		default:
		break;
		}
	}
	return (false);
}

//=====================================
// activate first mView Item
//-------------------------------------
void XFrame::activateFirstItem (void) {
	mView -> setFocus();
	mView -> setCurrentItem ( IDdesktop );
	sigIntro ( Intro );
	runIntro ( IDdesktop );
	mFinish -> setFocus();
}

//=====================================
// XCommand Constructor...
//-------------------------------------
XCommand::XCommand (const QString & cmd,const QString & file) {
	char* line = NULL;
	char* call = NULL;
	int n      = 1;

	line = (char*) malloc (cmd.length() + 1);
	sprintf(line,"%s",cmd.ascii());

	call    = strtok(line,"\t ");
	pBase   = call;
	pArg[0] = call;
	n       = 1;
	while (call) {
		call = strtok(NULL,"\t ");
		pArg[n] = call;
		n++;
	}
	pArg[n] = NULL;
	mCount  = n - 2;
	mFile   = file;
}

//=====================================
// XCommand run the command...
//-------------------------------------
int XCommand::runCommand (void) {
	int pid = fork();
	switch(pid) {
	case -1:
	log(L_ERROR,"could not fork: %s",strerror(errno));
	return(-1);

	case 0:
	if (mFile != "") {
		freopen(mFile.ascii(),"w",stdout);
	}
	execv(pBase,pArg);
	log(L_ERROR,"could not execute command %s: %s\n",
		pBase,strerror(errno)
	);
	exit(1);

	default:
	waitpid(pid, NULL,0);
	}
	return(0);
}

//=====================================
// XCommand add option to command...
//-------------------------------------
void XCommand::addOption (const QString & option) {
	char* add = (char*) malloc (option.length() + 1);
	sprintf(add,"%s",option.ascii());
	pArg[mCount + 1] = add;
	pArg[mCount + 2] = NULL;
	mCount++;
}
