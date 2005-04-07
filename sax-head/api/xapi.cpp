/**************
FILE          : xapi.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : main program initializing all dialog objects,
              : log handler and option handler
              :
              :
STATUS        : Status: Up-to-date
**************/
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <qapplication.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <qpainter.h>
#include <math.h>
#include <qlist.h>
#include <qtimer.h>
#include <qprogressdialog.h>
#include <qobjectlist.h>
#include <getopt.h>

#include "xapi.h"
#include "xinfo.h"
#include "xidle.h"

//=====================================
// Globals...
//-------------------------------------
char* globalStyle    = NULL;
XIntro* introPointer = NULL;
XFrame* xapi         = NULL;
int global3DActive   = 0;
int globalFrameWidth = 0;

//=====================================
// main program...
//-------------------------------------
int main(int argc,char*argv[]) {
	//=====================================
	// init variables...
	//-------------------------------------
	bool uniFont    = false;
	bool noBorder   = false;
	bool fastSetup  = false;
	bool withDialog = false;
	bool yastMode   = false;
	bool useHwData  = false;
	bool fullScreen = false;
	bool setInfo    = false;
	bool set3D      = false;
	bool setXIdle   = false;
	bool checkPacs  = false;
	for (int n=0;n<argc;n++) {
	if (strcmp(argv[n],"-style") == 0) {
		globalStyle = (char*) malloc (sizeof(char) * 128);
		sprintf(globalStyle,"%s",argv[n+1]);
		break;
	}
	}

	//=====================================
	// check for root priviliges...
	//-------------------------------------
	if (! accessAllowed()) {
		fprintf (stderr,"xapi: only root can do this\n");
		usage();
	}

	//=====================================
	// start logging...
	//-------------------------------------
	logInit();

	//=====================================
	// allocate main qt object...
	//-------------------------------------
	for (int i=0;i<argc;i++) {
	QString item (argv[i]);
	if ((item == "-h") || (item == "--help")) {
		logExit(); usage(); exit(0);
	}
	}
	QApplication app(argc,argv);

	//=====================================
	// get additional options...
	//-------------------------------------
	QString* idlePID     = NULL;
	QString* cardName3D  = NULL;
	QString* popUpDialog = NULL;
	QString* cardDriver  = NULL;
	while (1) {
	int option_index = 0;
	static struct option long_options[] =
	{
		{"usehwdata"  , 0 , 0 , 'u'},
		{"fullscreen" , 0 , 0 , 'l'},
		{"info"       , 0 , 0 , 'i'},
		{"yast"       , 0 , 0 , 'y'},
		{"checkpacs"  , 0 , 0 , 'c'},
		{"noborder"   , 0 , 0 , 'n'},
		{"3d"         , 1 , 0 , 'D'},
		{"driver"     , 1 , 0 , 'd'},
		{"frameWidth" , 1 , 0 , 'w'},
		{"dialog"     , 1 , 0 , 'O'},
		{"unifont"    , 0 , 0 , 'U'},
		{"xidle"      , 0 , 0 , 'x'},
		{"pid"        , 1 , 0 , 'p'},
		{"fast"       , 0 , 0 , 'f'},
		{"help"       , 0 , 0 , 'h'},
		{0            , 0 , 0 , 0  }
	};
	int c = getopt_long (
		argc, argv, "uicD:fhxlw:d:yp:nUO:",long_options, &option_index
	);
	if (c == -1)
	break;

	switch (c) {
	case 0:
		fprintf (stderr,"xapi: option %s", long_options[option_index].name);
		if (optarg)
		fprintf (stderr," with arg %s", optarg);
		fprintf (stderr,"\n");
	break;

	case 'f':
		fastSetup  = true;
	break;

	case 'U':
		uniFont  = true;
	break;

	case 'O':
		withDialog  = true;
		popUpDialog = new QString (optarg);
	break;

	case 'n':
		noBorder = true;
	break;

	case 'y':
		yastMode = true;
	break;

	case 'w':
		globalFrameWidth = atoi (optarg);
	break;

	case 'u':
		useHwData  = true;
	break;

	case 'l':
		fullScreen = true;
	break;

	case 'i':
		setInfo    = true;
	break;

	case 'c':
		checkPacs  = true;
	break;

	case 'D':
		set3D      = true;
		cardName3D = new QString (optarg);
	break;

	case 'p':
		idlePID    = new QString (optarg);
	break;

	case 'd':
		cardDriver = new QString (optarg);
	break;

	case 'x':
		setXIdle   = true;
	break;

	case 'h':
		logExit(); usage();
	break;
	default:
		logExit(); exit(1);
	}
	}

	//=====================================
	// check if xapi should act as xidle
    //-------------------------------------
	if (setXIdle) {
		XTimeElapsed* idleTimer = new XTimeElapsed ( idlePID );
		app.setMainWidget (idleTimer);
		idleTimer->show();
		return app.exec();
	}

	//=====================================
	// check md5 sum of current config
	//-------------------------------------
	if ((! setInfo) && (! set3D) && (! checkPacs) &&
		(! useHwData) && (! fastSetup)
	) {
	bool createMD5sum = false;
	QFileInfo xc (MD5CONFIG);
	if (xc.exists()) {
		QString mD5Sum = qx(MD5,STDOUT,1,"%s",XCONFIG);
		QFile mHandle (MD5CONFIG); 
		if (mHandle.open(IO_ReadOnly)) {
		QString compare;
		mHandle.readLine(compare,MAX_LINE_LENGTH);
		mHandle.close();
		compare = compare.stripWhiteSpace();
		if (compare == mD5Sum) {
			fastSetup = true;
		} else {
			fastSetup = false;
		}
		} 
	} else {
		fastSetup = false;
	}
	if (createMD5sum) {
		QString mD5Sum = qx(MD5,STDOUT,1,"%s",XCONFIG);
		QFile mHandle (MD5CONFIG);
		if (mHandle.open(IO_WriteOnly)) {
			mHandle.writeBlock (mD5Sum.ascii(),mD5Sum.length());
			mHandle.close();
		}
	}
	} 

	//=====================================
	// check option combinations...
	//-------------------------------------
	if (useHwData == true) {
		fastSetup = false;
	}

	//=====================================
	// set root window cursor to the watch
	//-------------------------------------
	setMouseCursor ("watch");

	//=====================================
	// init program...
	//-------------------------------------
	QWidget::WFlags wflags = Qt::WType_TopLevel;
	if (noBorder) {
		wflags |= Qt::WStyle_Customize | Qt::WStyle_NoBorder;
	}
	if (uniFont) {
		QFont currentFont = QFont( "Helvetica", 12 );
		currentFont.setStyleHint( QFont::SansSerif, QFont::PreferBitmap );
		currentFont.setRawName (UNIFONT);
		qApp->setFont (currentFont);
	}
	xapi = new XFrame (
		globalFrameWidth, wflags
	);
	if (setInfo) {
		signal (SIGHUP ,gotKill);
		signal (SIGKILL,gotKill);
		signal (SIGABRT,gotKill);
		signal (SIGTERM,gotKill);
		XInfo infoBox (xapi);
		infoBox.showIntroBox();
		logExit();
		setMouseCursor ();
		exit (
			infoBox.returnCode()
		);
	}
	if (set3D) {
		XInfo infoBox (xapi);
		infoBox.show3DBox (*cardName3D,cardDriver);
		printf("%d\n",infoBox.returnCode());
		logExit();
		setMouseCursor ();
		exit (
			infoBox.returnCode()
		);
	}
	if (fastSetup) {
		xapi->fastSetup  = true;
	}
	if (useHwData) {
		xapi->useHwData  = true;
	}
	if (fullScreen) {
		xapi->fullScreen = true;
	}
	if (yastMode) {
		xapi->yastMode   = true;
	}

	//=====================================
	// check 3D environment
	//-------------------------------------
	XStringList packageInfo ( qx (GET3D,STDOUT) );
	packageInfo.setSeperator (":");
	QString package = packageInfo.getList().at(0);
	QString real3d  = packageInfo.getList().at(1);
	QString soft3d  = packageInfo.getList().at(2);
	QString general = packageInfo.getList().at(3);
	QString active  = packageInfo.getList().at(4);
	QString answer  = packageInfo.getList().at(5);
	QString flag    = packageInfo.getList().at(6);
	global3DActive  = active.toInt();
	QString status1 ("1");
	if (checkPacs) {
		// ...
		// if the 3D Answer was no, there is no need
		// to check for any packages or scripts
		// ---
		if (answer == "no") {
			setMouseCursor (); exit (0);
		}
		// ...
		// install the missing packages using YaST2
		// sw_single mode
		// ---
		if (package != "<none>") {
		if (setMessage("InstallPackage",xapi)) {
		status1 = qx (
			GETINSTALLED,STDOUT,1,"%s",package.ascii()
		);
		}
		}
		setMouseCursor ();
		exit (0);
	}

	//=====================================
	// remove lilo code file...
	//-------------------------------------
	unlink (LILOCODE);

	//=====================================
	// init main frame...
	//-------------------------------------
	if (! xapi->frameInitialized()) {
		xapi->setFrame ();
	}

	//=====================================
	// set signal handler...
	//-------------------------------------
	signal (SIGUSR2,SIG_IGN);
	signal (SIGINT ,gotInterrupted);

	//=====================================
	// init dialogs...
	//-------------------------------------
	XIntro*			intro    = new XIntro();
	XAccessX*		xaccess  = new XAccessX();
	XMouse*			mouse    = new XMouse();
	XCard*			card     = new XCard();
	XDesktop*		desktop  = new XDesktop();
	XDisplayGeometry*	geo  = new XDisplayGeometry();
	XKeyboard*		keyboard = new XKeyboard();
	XLayout*		layout   = new XLayout();
	XMonitor*		monitor  = new XMonitor();
	XMultihead*		multi    = new XMultihead();
	XTablet*		tablet   = new XTablet();
	XTouchScreen*	toucher  = new XTouchScreen();
	XOpenGL*        opengl   = new XOpenGL();
	XVirtual*       virtuals = new XVirtual();
	Xvnc*           vnc      = new Xvnc();

	intro     -> addTo (xapi);
	xaccess   -> addTo (xapi,intro);
	mouse     -> addTo (xapi,intro);
	card      -> addTo (xapi,intro);
	desktop   -> addTo (xapi,intro);
	geo       -> addTo (xapi,intro);
	keyboard  -> addTo (xapi,intro);
	layout    -> addTo (xapi,intro);
	monitor   -> addTo (xapi,intro);
	multi     -> addTo (xapi,intro);
	tablet    -> addTo (xapi,intro);
	toucher   -> addTo (xapi,intro);
	opengl    -> addTo (xapi,intro);
	virtuals  -> addTo (xapi,intro);
	vnc       -> addTo (xapi,intro);

	//=====================================
	// save intro pointer...
	//-------------------------------------
	introPointer = intro;

	//=====================================
	// make sure the serverlayout structure
	// is created at least one time
	//-------------------------------------
	XLayout* layoutDialog;
	layoutDialog = (XLayout*) intro -> retrieve (Layout);
	layoutDialog -> setupLayout();

	//=====================================
	// make sure the card dialog was build
	// in init stage
	//-------------------------------------
	XCard* cardDialog;
	cardDialog = (XCard*) intro -> retrieve (Card);
	cardDialog -> showSetupWindow (false);
	cardDialog -> slotRun (Card);
	cardDialog -> showSetupWindow (true);

	//=====================================
	// call resetPage of the desktop dialog
	// to check for framebuffer usage
	//-------------------------------------
	XDesktop* desktopDialog;
	desktopDialog = (XDesktop*) intro -> retrieve (Colors);
	desktopDialog -> resetPage ( PAGE_RELOAD );

	//=====================================
	// add margin around pushbutton texts 
	// this is only needed for QT3 
	//------------------------------------- 
	#if 1
	QObject* obj;
	QObjectList* l = xapi->queryList( "QPushButton" );
	QObjectListIt it( *l );
	QDict<char>* mText = xapi->getTextPointer();
	while ( (obj=it.current()) != 0 ) {
	++it;
	QPushButton* btn = (QPushButton*) obj;
	QString text = btn->text();
	if (! text.isNull()) {
		QDictIterator<char> n (*mText);
		for (; n.current(); ++n) {
		if (n.current() == text) {
			QString* newText = new QString();
			QTextOStream (newText) << "  " << text << "  ";
			mText->replace (n.currentKey(),*newText);
			break;
		}
		}
		QString addText;
		QTextOStream (&addText) << "  " << text << "  ";
		btn->setText (addText);
	}
	}
	delete l;
	// ...
	// special keys which are used dynamically on widgets
	// with initially other texts
	// ---
	QList<char> keyList;
	keyList.append ( "finish" );
	keyList.append ( "Next" );
	QListIterator<char> io ( keyList );
	for (; io.current(); ++io) {
		QString key ( io.current() );
		QString curText (mText->operator[](key));
		QString* newText = new QString();
		QTextOStream (newText) << "  " << curText << "  ";
		mText->replace (key,*newText);
	}
	#endif

	//=====================================
	// remove xfine cache directory...
	//-------------------------------------
	removeXFineCache();

	//=====================================
    // go to main event loop...
    //-------------------------------------
	app.setMainWidget(xapi);
	xapi -> show();
	xapi -> enterEvent ( 0 );
	xapi -> activateFirstItem();
	setMouseCursor();
	if ((! withDialog) && (useHwData)) {
		intro -> checkDetected();
	} else {
		if ( *popUpDialog == "Monitor" ) {
			xapi -> runDialog (Monitor);
		} else
		if ( *popUpDialog == "Card") {
			xapi -> runDialog (Card);
		} else {
			fprintf (stderr,"xapi: no such dialog: %s\n",popUpDialog->ascii());
			exit (1);
		}
	}
	return app.exec();
}

//=====================================
// gotInterupted...
//-------------------------------------
void gotInterrupted (int) {
	if (xapi) {
		xapi -> slotFastExit();
	}
}

//=====================================
// gotKill...
//-------------------------------------
void gotKill (int) {
	setMouseCursor ();
	exit(2);
}

//=====================================
// remove XFine2 cache...
//-------------------------------------
void removeXFineCache (void) {
	struct dirent* entry = NULL;
	DIR* cacheDir = NULL;
	cacheDir = opendir (XFINECACHE);
	while (1) {
		entry = readdir (cacheDir);
		if (! entry) {
			break;
		}
		QString file (entry->d_name);
		if ((file == ".") || (file == "..")) {
			continue;
		}
		unlink (file.ascii());
	}
}

//=====================================
// set message popup...
//-------------------------------------
bool setMessage (const QString& text,XFrame* parent) {
	parent -> setFrame();
	XWrapPointer< QDict<char> > mText (
		parent -> getTextPointer()
	);
	XBox mb (
		mText["hint"],mText[text],
		XBox::Information,
		XBox::Yes,
		XBox::No, 0,0,
		globalFrameWidth
	);
	mb.setButtonText (
		XBox::Yes, mText["Yes"]
	);
	mb.setButtonText (
		XBox::No, mText["No"]
	);
	switch (mb.exec()) {
	case XBox::No:
		return (false);
	break;
	case XBox::Yes:
		return (true);
	break;
	}
	return (true);
}

//=====================================
// check root privileges...
//-------------------------------------
bool accessAllowed (void) {
	if (getuid() == 0) { return (true); }
	return (false);
}

//=====================================
// set cursor for root window...
//-------------------------------------
void setMouseCursor (const QString& cursorName) {
	QString optc ("-cursor_name");
	QString optd ("-display");
	QString display ( DisplayString ( QApplication::desktop()->x11Display() ));
	qx (XSETROOT,STDNONE,4,"%s %s %s %s",
		optc.ascii(),cursorName.ascii(),optd.ascii(),display.ascii()
	);
}

//=====================================
// usage...
//-------------------------------------
void usage (void) {
	printf ("Linux SaX2 Version 7.1 startup level (xapi)\n");
	printf ("(C) Copyright 2002 SuSE GmbH\n");
	printf ("\n");
	printf ("usage: xapi [ options ]\n");
	printf ("options:\n");
	printf ("[ -u | --usehwdata ]\n");
	printf ("   allways call ISaX to use the configuration data scanned\n");
	printf ("   via init.pl. Usefull if SaX2 start its own X-Server\n");
	printf ("[ -U | --unifont ]\n");
	printf ("   force usage of Uni-Code font\n");
	printf ("[ -O | --dialog <nane> ]\n");
	printf ("   start SaX2 with the given dialog name. Available are\n");
	printf ("   the following names: Monitor,Card\n");
	printf ("[ -f | --fast ]\n");
	printf ("   fast startup, prevent ISaX calls and read\n");
	printf ("   the currently existing setup files\n");
	printf ("[ -l | --fullscreen ]\n");
	printf ("   start in fullscreen mode, use one screen if\n");
	printf ("   Xinerama is in use\n");
	printf ("[ -w | --frameWidth ]\n");
	printf ("   set frame width used for mainframe and toplevel\n");
	printf ("   windows like message boxes...\n");
	printf ("--\n");
	exit (1);
}
