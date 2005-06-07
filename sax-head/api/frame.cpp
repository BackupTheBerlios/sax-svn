/**************
FILE          : frame.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : SaX2 GUI system using libsax to provide
              : configurations for a wide range of X11 capabilities
              : //.../
              : - frame.h: SaXGUI::SCCFrame header definitions
              : - frame.cpp: builds basic layout structure
              : ----
              :
STATUS        : Status: Development
**************/
#include "frame.h"

namespace SaXGUI {
//=====================================
// Constructor
//-------------------------------------
SCCFrame::SCCFrame (
	bool fullscreen, int mode, bool setinfo, bool checkpacs,
	bool yastmode , bool xidle, QString* xidlePID, bool middle, WFlags wflags
) : QWidget ( 0,0, wflags ) {
	// .../
	// create main frame for the complete application. This
	// constructor will first load all translations check the
	// given parameters and create the application without
	// displaying it first
	// ----
	mYaSTMode = yastmode;
	mTextPtr  = new QDict<QString>;
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());
	loadText();

	//=====================================
	// show information box if set
	//-------------------------------------
	if (setinfo) {
		SCCMessage* mMessageBox = new SCCMessage (
			this, getTextPtr(), SaXMessage::INTRO,
			"Suggestion","SuggestionCaption"
		);
		QString result = mMessageBox -> showMessage();
		if (result == mText["Cancel"]) {
			exitSaX (2);	
		}
		if (result == mText["Ok"]) {
			exitSaX (0);
		}
		exitSaX (1);
	}

	//=====================================
	// check for missing packages
	//-------------------------------------
	if (checkpacs) {
		QProcess* proc = new QProcess ();
		proc -> addArgument ( GET3D );
		if ( ! proc -> start() ) {
			return;
		}
		while (proc->isRunning()) {
			usleep (1000);
		}
		QByteArray data = proc->readStdout();
		QStringList lines = QStringList::split (":",data);
		QString answer,package;
		int count = 0;
		for (QStringList::Iterator it=lines.begin(); it != lines.end(); ++it) {
			QString token (*it);
			switch (count) {
				case 0: package = token; break;
				case 5: answer  = token; break;
				default:
				break;
			}
			count++;
		}
		//=====================================
		// 3D Answer was no: no check needed
		//-------------------------------------
		if (answer == "no") {
			exitSaX();
		}
		if (package != "<none>") {
			SCCMessage* mMessageBox = new SCCMessage (
				this, getTextPtr(), SaXMessage::YES_NO,
				"InstallPackage","InstallPackageCaption"
			);
			QString result = mMessageBox -> showMessage();
			if (result == mText["Yes"]) {
				QProcess* proc = new QProcess (); 
				proc -> addArgument ( GETINSTALLED );
				proc -> addArgument ( package );
				if ( ! proc -> start() ) { 
					return;
				}
				while (proc->isRunning()) {
					usleep (1000);
				}
			}
		}
		exitSaX();
	}

	//=====================================
	// show idle timer id requested
	//-------------------------------------
	if (xidle) {
		SCCIdleTimeElapsed* idleTimer = new SCCIdleTimeElapsed ( xidlePID );
		qApp->setMainWidget (idleTimer);
		idleTimer->show();
		qApp->exec();
	}

	//=====================================
	// set application geometry
	//-------------------------------------
	int width  = qApp->desktop()->width();
	int height = qApp->desktop()->height();
	if (! fullscreen) {
		if (middle) {
			QProcess* proc = new QProcess ();
			proc -> addArgument ( XQUERY );
			proc -> addArgument ( "-M" );
			if ( ! proc -> start() ) {
				return; 
			}
			while (proc->isRunning()) {
				usleep (1000);
			}
			QByteArray data = proc->readStdout();
			QStringList lines = QStringList::split ("\n",data);
			QStringList position = QStringList::split (" ",lines.first());
			QStringList reslist  = QStringList::split (":",position.last());
			int posx = reslist.first().toInt();
			int posy = reslist.last().toInt();
			posx = posx - 400;
			posy = posy - 300;
			setGeometry ( posx,posy,800,600 );
		} else {
			resize ( 800,600 );
		}
	} else {
		setGeometry ( 0,0,width,height );
	}

	//=====================================
	// install event filter
	//-------------------------------------
	installEventFilter (this);

	//=====================================
	// create layout
	//-------------------------------------
	QGridLayout* baseLayout = new QGridLayout ( this,1,1 );
	mMainFrame = new QFrame ( this );
	baseLayout -> addWidget ( mMainFrame,0,0 );

	//=====================================
	// create seperator
	//-------------------------------------
	#if 1
	mSeperator = new QFrame ( mMainFrame );
	mSeperator -> setFixedHeight (2);
	mSeperator -> setFrameStyle (
		QFrame::HLine | QFrame::Raised
	);
	#endif

	//=====================================
	// create area panels
	//-------------------------------------
	mMainLayout = new QVBoxLayout  ( mMainFrame );
	mMainLayout -> setMargin  (10);
	mTitleLayout= new QHBoxLayout ( mMainLayout );
	mWorkLayout = new QHBoxLayout ( mMainLayout );
	mMainLayout -> addSpacing (10);
	#if 1
	mMainLayout -> addWidget      ( mSeperator  );
	mMainLayout -> addSpacing (5);
	#endif
	mDoneLayout = new QHBoxLayout ( mMainLayout );

	//=====================================
	// create title bar (mTitleLayout)
	//-------------------------------------
	#if 0
	mTitleBox = new QVBox ( mMainFrame );
	QHBox* mTitleHBox  = new QHBox  ( mTitleBox );
	QLabel* titleSpace = new QLabel ( mTitleBox );
	titleSpace -> setFixedHeight ( 10 );
	QPixmap titleBarGradientPixmap = QPixmap ( TITLE_GRADIENT );
	QPixmap titleBarIconPixmap = QPixmap ( TITLE_PIXMAP );
	mTitleHBox -> setPaletteBackgroundPixmap ( titleBarGradientPixmap );
	mTitleHBox -> setMargin (5);
	mTitleHBox -> setFrameStyle (
		QFrame::Panel | QFrame::Raised
	);
	QFont mTitleFont ("Sans",20,QFont::Normal,true);
	QLabel* mIcon = new QLabel ( mTitleHBox );
	mIcon -> setPixmap ( titleBarIconPixmap );
	QLabel* mTitle  = new QLabel (" " + mText["TitleLeft"],mTitleHBox);
	mTitleHBox -> setStretchFactor ( mTitle, 20 );
	QLabel* mVendor = new QLabel ( mText["TitleRight"],mTitleHBox );
	mTitle  -> setFont (mTitleFont);
	mVendor -> setFont (mTitleFont);
	#endif

	//=====================================
	// create module listbox (mWorkLayout)
	//-------------------------------------
	mModuleBox  = new QHBox ( mMainFrame );
	mModuleList = new QListBox ( mModuleBox );
	QLabel* moduleSpace = new QLabel ( mModuleBox );
	moduleSpace -> setFixedWidth ( 10 );
	mModuleList -> setMinimumWidth ( 210 );
	mModuleList -> setFrameStyle (
		QFrame::Box | QFrame::Raised
	);

	//=====================================
	// create dialog vbox (mWorkLayout)
	//-------------------------------------
	QVBox* mDialogBox = new QVBox ( mMainFrame );
	mDialogBox -> setFrameStyle (
		QFrame::Box | QFrame::NoFrame
		#if 0
		QFrame::Box | QFrame::Raised
		#endif
	);
	#if 0
	mModuleTitle = new QListBox ( mDialogBox );
	mModuleTitle -> setFrameStyle (
		QFrame::Box | QFrame::NoFrame
	);
	mModuleTitle -> setVScrollBarMode (QScrollView::AlwaysOff);
	mModuleTitle -> setSelected ( 0,true );
	mModuleTitle -> setFocusPolicy ( QWidget::NoFocus );
	#else
	mModuleTitleBox   = new QHBox ( mDialogBox );
	mModuleTitleLabel = new QVBox ( mModuleTitleBox );
	mModuleTitleBox -> setStretchFactor ( mModuleTitleLabel, 20 );
	mModuleTitle = new QLabel ( mModuleTitleLabel );
	QLabel* mTitleSeperator = new QLabel ( mModuleTitleLabel );
	mTitleSeperator -> setFixedHeight ( 50 );
	QLabel* mTitleIcon = new QLabel ( mModuleTitleBox );
	QFont mTitleFont ("Serif",17,QFont::Normal,false);
	QPixmap titleBarIconPixmap = QPixmap ( TITLE_PIXMAP );
	mModuleTitle -> setFont ( mTitleFont );
	mTitleIcon   -> setPixmap ( titleBarIconPixmap );
	#endif

	//=====================================
	// create widget stack (mWorkLayout)
	//-------------------------------------
	mDialogStack = new QWidgetStack ( mDialogBox );
	mDialogBox -> setStretchFactor  ( mDialogStack,20 );

	//=====================================
	// create bottom btn bar (mDoneLayout)
	//-------------------------------------
	mHelper = new QPushButton ( mText["Help"]  , mMainFrame );
	mFinish = new QPushButton ( mText["Ok"]    , mMainFrame );
	mCancel = new QPushButton ( mText["Cancel"], mMainFrame );
	QPushButton* completionWidgets[] = {
		mHelper,mFinish,mCancel
	};
	int fixedWidth = 0;
	for (int i=0;i<3;i++) {
	if (completionWidgets[i]->width() > fixedWidth) {
		fixedWidth = completionWidgets[i]->width();
	}
	}
	for (int i=0;i<3;i++) {
		completionWidgets[i]->setFixedWidth (fixedWidth);
	}

	//=====================================
	// add widgets to the layout structure
	//-------------------------------------
	#if 0
	mTitleLayout -> addWidget  ( mTitleBox );
	#endif
	mWorkLayout  -> addWidget  ( mModuleBox );
	mWorkLayout  -> addWidget  ( mDialogBox );
	mWorkLayout  -> setStretchFactor ( mDialogBox,3 );
	mDoneLayout  -> addWidget  ( mHelper );
	mDoneLayout  -> addStretch ( 100 );
	mDoneLayout  -> addWidget  ( mFinish );
	mDoneLayout  -> addSpacing ( 10 );
	mDoneLayout  -> addWidget  ( mCancel );

	//=====================================
	// connect widgets
	//-------------------------------------
	QObject::connect(
		mCancel  , SIGNAL (clicked   ()),
		this     , SLOT   (slotCancel())
	);
	QObject::connect(
		mFinish  , SIGNAL (clicked   ()),
		this     , SLOT   (slotFinish())
	);
	QObject::connect(
		mHelper  , SIGNAL (clicked   ()),
		this     , SLOT   (slotHelp  ())
	);
	QObject::connect(
		mModuleList, SIGNAL (selectionChanged ( QListBoxItem* )),
		this      , SLOT   (slotSelected     ( QListBoxItem* ))
	);

	//=====================================
	// set window title
	//-------------------------------------
	setCaption (mText["GUICaption"]);

	//=====================================
	// set import mode
	//-------------------------------------
	mGUIMode = mode;

	//=====================================
	// setup modules
	//-------------------------------------
	setupModules();

	//=====================================
	// setup main focus
	//-------------------------------------
	mFinish -> setFocus();

	// disabled until SuSE's help strategy is clear
	mHelper -> setDisabled ( true );
}

//=====================================
// setupModules
//-------------------------------------
void SCCFrame::setupModules ( void ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());

	//=====================================
	// insert modules for selection
	//-------------------------------------
	mModuleList -> insertItem (
		QPixmap(MONITORS_PIXMAP), mText["MonitorModule"]
	);
	mModuleList -> insertItem (
		QPixmap(POINTERS_PIXMAP), mText["MouseModule"]
	);
	mModuleList -> insertItem (
		QPixmap(KEYBOARD_PIXMAP), mText["KeyboardModule"]
	);
	mModuleList -> insertItem (
		QPixmap(XTABLETS_PIXMAP), mText["TabletModule"]
	);
	mModuleList -> insertItem (
		QPixmap(TOUCHERS_PIXMAP), mText["ToucherModule"]
	);
	mModuleList -> insertItem (
		QPixmap(VNCSERVE_PIXMAP), mText["VNCModule"]
	);

	//=====================================
	// import all configuration data
	//-------------------------------------
	loadApplication ();

	//=====================================
	// create dialog widgets
	//-------------------------------------
	mMonitor  = new SCCMonitor     ( mDialogStack,getTextPtr(),mSection,this );
	mMouse    = new SCCMouse       ( mDialogStack,getTextPtr(),mSection,this );
	mKeyboard = new SCCKeyboard    ( mDialogStack,getTextPtr(),mSection,this );
	mTablet   = new SCCTablet      ( mDialogStack,getTextPtr(),mSection,this );
	mToucher  = new SCCTouchScreen ( mDialogStack,getTextPtr(),mSection,this );
	mVNC      = new SCCVNC         ( mDialogStack,getTextPtr(),mSection,this );

	//=====================================
	// setup height for module title box
	//-------------------------------------
	#if 0
	mModuleTitle -> setFixedHeight (
		mModuleList -> item(0) -> height(mModuleTitle)
	);
	#endif

	//=====================================
	// start with monitor dialog
	//-------------------------------------
	mModuleList -> setSelected ( 0,true );
}

//=====================================
// slotSelected
//-------------------------------------
void SCCFrame::slotSelected ( QListBoxItem* item ) {
	// .../
	// if the selection has changed this method is called.
	// the selection will be copied to be shown as module title
	// and the runDialog() method is called to start the dialog
	// ----
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());

	//=====================================
	// prepare module title text/pixmap
	//-------------------------------------
	QPixmap pixmap;
	if (item->pixmap()) {
		pixmap = *item->pixmap();
	}
	#if 0
	if (mModuleTitle->count() == 0) {
		mModuleTitle -> insertItem ( pixmap,item->text(),0 );
	} else {
		mModuleTitle -> changeItem ( pixmap,item->text(),0 );
	}
	mModuleTitle -> setSelected ( 0,true );
	#else
	QString selection = item -> text();
	QString titletext = selection;
	if (selection == mText["MonitorModule"]) {
		titletext  = mText["MonitorModuleTopic"];
	}
	if (selection == mText["MouseModule"]) {
		titletext  = mText["MouseModuleTopic"];
	}
	if (selection == mText["KeyboardModule"]) {
		titletext  = mText["KeyboardModuleTopic"];
	}
	if (selection == mText["TabletModule"]) {	
		titletext  = mText["TabletModuleTopic"];
	}
	if (selection == mText["ToucherModule"]) {
		titletext  = mText["ToucherModuleTopic"];
	}
	if (selection == mText["VNCModule"]) {
		titletext  = mText["VNCModuleTopic"];
	}
	mModuleTitle -> setText ( titletext );
	#endif

	//=====================================
	// run the selected dialog
	//-------------------------------------
	runDialog ( item );
}

//=====================================
// slotRunDialog
//-------------------------------------
void SCCFrame::runDialog ( QListBoxItem* item ) {
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());

	//=====================================
	// check selected item and raise dialog
	//-------------------------------------
	QString selection = item -> text();
	if (selection == mText["MonitorModule"]) {
		mDialogStack -> raiseWidget ( mMonitor -> getDialogID() );
		if ( mMonitor -> needInit() ) {
			mMonitor -> init();
		}
		if ( mMonitor -> needImport() ) {
			mMonitor -> import();
		}
	}
	if (selection == mText["MouseModule"]) {
		mDialogStack -> raiseWidget ( mMouse -> getDialogID() );
		if ( mMouse -> needInit() ) {
			mMouse -> init();
		}
		if ( mMouse -> needImport() ) {
			mMouse -> import();
		}
	}
	if (selection == mText["KeyboardModule"]) {
		mDialogStack -> raiseWidget ( mKeyboard -> getDialogID() );
		if ( mKeyboard -> needInit() ) {
			mKeyboard -> init();
		}
		if ( mKeyboard -> needImport() ) {
			mKeyboard -> import();
		}
	}
	if (selection == mText["TabletModule"]) {
		mDialogStack -> raiseWidget ( mTablet -> getDialogID() );
		if ( mTablet -> needInit() ) {
			mTablet -> init();
		}
		if ( mTablet -> needImport() ) {
			mTablet -> import();
		}
	}
	if (selection == mText["ToucherModule"]) {
		mDialogStack -> raiseWidget ( mToucher -> getDialogID() );
		if ( mToucher -> needInit() ) {
			mToucher -> init();
		}
		if ( mToucher -> needImport() ) {
			mToucher -> import();
		}
	}
	if (selection == mText["VNCModule"]) {
		mDialogStack -> raiseWidget ( mVNC -> getDialogID() );
		if ( mVNC -> needInit() ) {
			mVNC -> init();
		}
		if ( mVNC -> needImport() ) {
			mVNC -> import();
		}
	}
}

//=====================================
// slotCancel
//-------------------------------------
void SCCFrame::slotCancel ( void ) {
	if ( ! mYaSTMode ) {
		SCCWrapPointer< QDict<QString> > mText (getTextPtr());
		SCCMessage* mMessageBox = new SCCMessage (
			this, getTextPtr(), SaXMessage::YES_NO, "ExitSaX"
		);
		QString result = mMessageBox -> showMessage();
		if (result == mText["Yes"]) {
			exitSaX();
		}
	} else {
		exitSaX();
	}
}

//=====================================
// closeEvent (WM close event)
//-------------------------------------
void SCCFrame::closeEvent ( QCloseEvent* ) {
	slotCancel();
}

//=====================================
// exitSaX
//-------------------------------------
void SCCFrame::exitSaX ( int status ) {
	exit ( status );
}

//=====================================
// hideframe
//-------------------------------------
void SCCFrame::hideFrame ( int frame ) {
	// .../
	// hide the given frame. Mostly used to hide controls
	// which are not needed if only a specific configuration
	// dialog should be opened
	// ----
	switch ( frame ) {
		case FRAME_MAIN:
			mMainFrame -> hide();
		break;
		#if 0
		case FRAME_TITLE:
			mTitleBox -> hide();
		break;
		#endif
		case FRAME_MODULES:
			mModuleBox -> hide();
		break;
		#if 0
		case FRAME_TOPIC:
			mModuleTitle -> hide();
		break;
		#endif
		default:
		break;
	}
}

//=====================================
// setMainFrame
//-------------------------------------
void SCCFrame::loadApplication ( void ) {
	// .../
	// load the configuration data refering to the contents of
	// mGUIMode this call will use libsax to read all data and
	// store the SaXImport pointers within the dictionary named
	// mSection.
	// ----
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());
	QProgressDialog mProgress (
		mText["ImportDataFiles"],mText["Cancel"],8,this,"progress",true
	);
	mProgress.setFixedWidth ( 350 );
	mProgress.setCaption (mText["ImportDataCaption"]);
	mProgress.setLabelText (mText["InitCache"]);
	mProgress.setProgress ( 1 );
	qApp->processEvents();
	//=====================================
	// init cache if AUTO probed data used
	//-------------------------------------
	if ( mGUIMode == ISAX_AUTO ) {
		#if 0
		SaXInit init;
		if (init.needInit()) {
			log (L_INFO,"initialize cache...\n");
			init.doInit();
		}
		#endif
	}
	//=====================================
	// import all sections
	//-------------------------------------
	int importID[7] = {
		SAX_CARD,
		SAX_DESKTOP,
		SAX_POINTERS,
		SAX_KEYBOARD,
		SAX_LAYOUT,
		SAX_PATH,
		SAX_EXTENSIONS
	};
	mConfig = new SaXConfig;
	for (int id=0; id<7; id++) {
		mProgress.setProgress ( id + 1 );
		qApp->processEvents();
		if ( mProgress.wasCanceled() ) {
			exitSaX();
		}
		SaXImport* import = new SaXImport ( importID[id] );
		import -> setSource ( SAX_SYSTEM_CONFIG );
		if (mGUIMode == ISAX_AUTO) {
			import -> setSource ( SAX_AUTO_PROBE );
		}
		import  -> doImport();
		mConfig -> addImport (import);
		mProgress.setLabelText (
			mText["ImportDataFiles"]+": "+import->getSectionName()
		);
		log (L_INFO,"Imported: %s\n",
			import->getSectionName().ascii()
		);
		mSection.insert (
			import->getSectionName(),import
		);
	}
	mProgress.setProgress ( 8 );
}

//=====================================
// slotFinish
//-------------------------------------
void SCCFrame::slotFinish ( void ) {
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());
	SCCMessage* mMessageBox = new SCCMessage (
		this, getTextPtr(), SaXMessage::FINISH,
		"FinalSaX","MessageCaption"
	);
	QString result = mMessageBox -> showMessage();
	if (result == mText["FinalSave"]) {
		saveConfiguration();
	}
	if (result == mText["FinalTest"]) {
		testConfiguration();
	}
}

//=====================================
// prepareConfiguration
//-------------------------------------
void SCCFrame::prepareConfiguration ( void ) {
	if ( ! mMonitor -> needInit() ) {
		mMonitor -> exportData();
	}
	if ( ! mMouse -> needInit() ) {
		mMouse -> exportData();
	}
	if ( ! mKeyboard -> needInit() ) {
		mKeyboard -> exportData();
	}
	if ( ! mTablet -> needInit() ) {
		mTablet -> exportData();
	}
	if ( ! mToucher -> needInit() ) {
		mToucher -> exportData();
	}
	if ( ! mVNC -> needInit() ) {
		mVNC -> exportData();
	}
}

//=====================================
// saveConfiguration
//-------------------------------------
void SCCFrame::saveConfiguration ( void ) {
	prepareConfiguration();
	mConfig -> setMode (SAX_NEW);
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());
	if ( ! mConfig -> createConfiguration() ) {
		QString message;
		QString library = mConfig->errorString();
		QString parser1 = mConfig->getParseError();
		QString parser2 = mConfig->getParseErrorValue();
		if (library.isEmpty()) {
			library = "No information";
		}
		if (parser1.isEmpty()) {
			parser1 = "No information";
		}
		QTextOStream (&message) <<
			mText["SavingFailed"] << "<br><br>" <<
			"SaX library: " << library << "<br>" <<
			"X configuration: " << parser1 << " " << parser2;
		SCCMessage* mMessageBox = new SCCMessage (
			this, getTextPtr(), SaXMessage::OK,
			message,"MessageCaption",SaXMessage::Critical
		);
		mMessageBox -> showMessage();
	} else {
		installConfiguration();
		SCCMessage* mMessageBox = new SCCMessage (
			this, getTextPtr(), SaXMessage::YES_NO,
			"SavingDone","MessageCaption"
		);
		QString result = mMessageBox -> showMessage();
		if (result == mText["Yes"]) {
			exitSaX();
		}
	}
}

//=====================================
// testConfiguration
//-------------------------------------
void SCCFrame::testConfiguration ( void ) {
	prepareConfiguration();
	mConfig -> setMode (SAX_NEW);
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());
	qApp->setOverrideCursor ( Qt::forbiddenCursor );
	mModuleList -> setDisabled ( true );
	mFinish -> setDisabled ( true );
	mCancel -> setDisabled ( true );
	SCCTestThread testConfiguration ( mConfig );
	testConfiguration.start();
	while (1) {
		usleep (1000);
		qApp->processEvents();
		if (! testConfiguration.running()) {
			break;
		}
	}
	qApp->restoreOverrideCursor();
	mModuleList -> setDisabled ( false );
	mFinish -> setDisabled ( false );
	mCancel -> setDisabled ( false );
	int status = testConfiguration.status();
	if ( status == -1 ) {
		QString message;
		QString library = mConfig->errorString();
		QString parser1 = mConfig->getParseError();
		QString parser2 = mConfig->getParseErrorValue();
		if (library.isEmpty()) {
			library = "No information";
		}
		if (parser1.isEmpty()) {
			parser1 = "No information";
		}
		QTextOStream (&message) <<
			mText["TestingFailed"] << "<br><br>" <<
			"SaX library: " << library << "<br>" <<
			"X configuration: " << parser1 << " " << parser2;
		SCCMessage* mMessageBox = new SCCMessage (
			this, getTextPtr(), SaXMessage::OK,
			message,"MessageCaption",SaXMessage::Critical
		);
		mMessageBox -> showMessage();
	}
	if ( status == 0 ) {
		installConfiguration();
		SCCMessage* mMessageBox = new SCCMessage (
			this, getTextPtr(), SaXMessage::YES_NO,
			"SavingDone","MessageCaption"
		);
		QString result = mMessageBox -> showMessage();
		if (result == mText["Yes"]) {
			exitSaX();
		}
	}
}

//=====================================
// installConfiguration
//-------------------------------------
void SCCFrame::installConfiguration ( void ) {
	#if 0
	mConfig -> commitConfiguration();
	#endif
}

//=====================================
// evaluateAutoDetection
//-------------------------------------
void SCCFrame::evaluateAutoDetection ( void ) {
	// .../
	// check the imported data if there is something important
	// missing. Checked values are DisplaySize and Monitor Name
	// ----
	// TODO
}

//=====================================
// slotHelp
//-------------------------------------
void SCCFrame::slotHelp ( void ) {
	// TODO
}

//=====================================
// startDialog
//-------------------------------------
bool SCCFrame::startDialog ( const QString& dialog ) {
	// .../
	// start the GUI with the given dialog on top and don't
	// allow switching to other dialogs
	// ----
	//=====================================
	// get translation pointer
	//-------------------------------------
	SCCWrapPointer< QDict<QString> > mText (getTextPtr());

	//=====================================
	// hide unneeded frames
	//-------------------------------------
	hideFrame ( FRAME_MODULES );
	hideFrame ( FRAME_TOPIC );

	//=====================================
	// raiseDialog
	//-------------------------------------
	QListBoxItem* mItem = 0;
	if (dialog == "Monitor") {
		mItem = mModuleList -> findItem ( mText["MonitorModule"] );
	}
	if (dialog == "Mouse") {
		mItem = mModuleList -> findItem ( mText["MouseModule"] );
	}
	if (dialog == "Keyboard") {
		mItem = mModuleList -> findItem ( mText["KeyboardModule"] );
	}
	if (dialog == "Tablet") {
		mItem = mModuleList -> findItem ( mText["TabletModule"] );
	}
	if (dialog == "Touchscreen") {
		mItem = mModuleList -> findItem ( mText["ToucherModule"] );
	}
	if (dialog == "VNC") {
		mItem = mModuleList -> findItem ( mText["VNCModule"] );
	}
	if ( mItem ) {
		mModuleList -> setSelected ( mItem,true );
		return true;
	}
	return false;
}

//=====================================
// loadText
//-------------------------------------
void SCCFrame::loadText ( void ) {
	// .../
	// load the xapi.gtx file and translate the GTX/gettext
	// keys according to the currently used locale setting
	// ----
	SCCFile gtxHandle (GTX_FILE);
	setlocale (LC_ALL,"");
	bindtextdomain ("sax", TDOMAIN);
	textdomain ("sax");

	QDict<QString> gtx = gtxHandle.readDict();
	QDictIterator<QString> it (gtx);
	for (; it.current(); ++it) {
		QString* translation = new QString (
			gettext((char*)it.current()->data())
		);
		mTextPtr->insert (
			it.currentKey(),translation
		);
	}
}

//=====================================
// getText
//-------------------------------------
QDict<QString>* SCCFrame::getTextPtr ( void ) {
	// .../
	// return a pointer to the translation dictionary
	// ----
	return mTextPtr;
}

//=====================================
// Set global event filter
//-------------------------------------
bool SCCFrame::eventFilter ( QObject* obj, QEvent* event ) {
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
} // end namespace
