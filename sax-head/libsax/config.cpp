/**************
FILE          : config.cpp
***************
PROJECT       : SaX2 - library interface
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : SaX2 - SuSE advanced X11 configuration 
              : 
              :
DESCRIPTION   : native C++ class library to access SaX2
              : functionality. Easy to use interface for
              : //.../
              : - importing/exporting X11 configurations
              : - modifying/creating X11 configurations 
              : ---
              :
              :
STATUS        : Status: Development
**************/
#include "config.h"

//====================================
// Globals...
//------------------------------------
SaXConfig* self = NULL;

//====================================
// Constructor...
//------------------------------------
SaXConfig::SaXConfig (int mode) {
	// .../
	//! An object of this type is used to create and verify
	//! the basic X11 configuration file /etc/X11/xorg.conf
	// ----
	configMode = mode;
	gotCard       = false;
    gotDesktop    = false;
    gotPointers   = false;
    gotKeyboard   = false;
    gotLayout     = false;
    gotPath       = false;
    gotExtensions = false;
	mDesktop      = NULL;
	self = this;
	// .../
	// remove all ISaX output files building apidata
	// ----
	if ( setLock() ) {
		unlink (ISAXCARD);
		unlink (ISAXDESKTOP);
		unlink (ISAXINPUT);
		unlink (ISAXKEYBOARD);
		unlink (ISAXLAYOUT);
		unlink (ISAXPATH);
		unlink (ISAXEXTENSIONS);
		unsetLock();
	}
	mParseErrorString = new QString();
	mParseErrorValue  = new QString();
}

//====================================
// addImport...
//------------------------------------
void SaXConfig::addImport ( SaXImport* in ) {
	// .../
	//! add previously imported data using a SaXImport reference
	//! multiple addImport() calls are possible to add all
	//! needed sections
	// ----
	int ID = in->getSectionID();
	switch (ID) {
		case SAX_CARD:
		if (gotCard) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotCard = true;
		break;
		case SAX_DESKTOP:
		if (gotDesktop) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotDesktop = true;
		mDesktop   = in;
		break;
		case SAX_POINTERS:
		if (gotPointers) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotPointers = true;
		break;
		case SAX_KEYBOARD:
		if (gotKeyboard) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotKeyboard = true;
		break;
		case SAX_LAYOUT:
		if (gotLayout) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotLayout = true;
		break;
		case SAX_PATH:
		if (gotPath) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotPath = true;
		break;
		case SAX_EXTENSIONS:
		if (gotExtensions) {
			excImportAlreadyAdded (ID);
			qError (errorString(),EXC_IMPORTALREADYADDED);
			return;
		}
		gotExtensions = true;
		break;
		default:
			excUnknownImport (in);
			qError (errorString(),EXC_UNKNOWNIMPORT);
			return;
		break;
	}
	mImportList.append (in);
}

//====================================
// enableXFineCache...
//------------------------------------
void SaXConfig::enableXFineCache (bool enable) {
	// .../
	//! private member function to enable the XFine cache.
	//! the cache is created when xfine is running. xfine is
	//! started while testing the new configuration. If the test
	//! has been finished using the "Save" button the XFine cache
	//! will be imported to incorp the modeline changes
	// ----
	if (! mDesktop) {
		return;
	}
	mDesktop -> setID (0);
	if (enable) {
		mDesktop -> setItem ("ImportXFineCache","yes");
	} else {
		mDesktop -> removeEntry ("ImportXFineCache");
	}
}

//====================================
// setMode...
//------------------------------------
void SaXConfig::setMode (int mode) {
	// .../
	//! set the ISAX mode for creating the configuration
	//! file. Possible modes are: SAX_NEW or SAX_MERGE (default).
	//! If SAX_NEW is specified a complete set of imports must
	//! be added to create a valid configuration file.
	// ----
	configMode = mode;
}

//====================================
// createConfiguration...
//------------------------------------
bool SaXConfig::createConfiguration (void) {
	// .../
	//! create the preliminary configuration file and verify the
	//! syntax of the file. If everything is ok the method will
	//! return true otherwise false
	// ----
	if (! setLock()) {
		return false;
	}
	QListIterator<SaXImport> it (mImportList);
	for (; it.current(); ++it) {
		SaXExport exportconfig (it.current(),this);
		if (! exportconfig.doExport()) {
			//excExportSectionFailed ();
			//qError (errorString(),EXC_EXPORTSECTIONFAILED);
			unsetLock();
			return false;
		}
	}
	QProcess* proc = new QProcess ();
	proc -> addArgument ( SAX_CREATE_API );
	if ( ! proc -> start() ) {
		excProcessFailed();
		qError (errorString(),EXC_PROCESSFAILED);
	}
	while (proc->isRunning()) {
		usleep (1000);
	}
	QFileInfo api (SAX_API_FILE);
	if (! api.exists()) {
		excNoAPIFileFound ();
		qError (errorString(),EXC_NOAPIFILEFOUND);
		unsetLock();
		return false;
	}
	proc -> clearArguments ();
	proc -> addArgument ( ISAX );
	proc -> addArgument ( "-f" );
	proc -> addArgument ( SAX_API_FILE );
	proc -> addArgument ( "-c" );
	proc -> addArgument ( SAX_API_CONFIG );
	
	if (configMode == SAX_MERGE) {
		proc -> addArgument ( "-m" );
	}
	if ( ! proc -> start() ) {
		excProcessFailed();
		qError (errorString(),EXC_PROCESSFAILED);
	}
	while (proc->isRunning()) {
		usleep (1000);
	}
	if (! xf86openConfigFile (CONFPATH,SAX_API_CONFIG,0)) {
		excFileOpenFailed (0);
		qError (errorString(),EXC_FILEOPENFAILED);
		unsetLock();
		return false;
	}
	if (! xf86readConfigFile()) {
		unsetLock();
		return false;
	}
	xf86closeConfigFile();
	unsetLock();
	return true;
}

//====================================
// commitConfiguration...
//------------------------------------
void SaXConfig::commitConfiguration (void) {
	// .../
	//! install the configuration file as /etc/X11/xorg.conf
	//! installing the file should only be done when
	//! createConfiguration() returned succesfully
	// ----
	QFile apiConfig (SAX_API_CONFIG);
	QFile curConfig (SAX_SYS_CONFIG);
	QFile secConfig (SAX_SYS_CSAVED);

	//====================================
	// read api created config file
	//------------------------------------
	QStringList al;
	if ( apiConfig.open( IO_ReadOnly ) ) {
		QTextStream stream( &apiConfig );
		QString line;
		while ( !stream.atEnd() ) {
			line = stream.readLine();
			al += line;
		}
		apiConfig.close();
	}
	//====================================
	// read current config file
	//------------------------------------
	QStringList cl;
	if ( curConfig.open( IO_ReadOnly ) ) {
		QTextStream stream( &apiConfig );
		QString line;
		while ( !stream.atEnd() ) {
			line = stream.readLine();
			cl += line;
		}
		curConfig.close();
	}
	//====================================
	// create a backup copy
	//------------------------------------
	if ( secConfig.open( IO_WriteOnly ) ) {
		QTextStream stream ( &secConfig );
		for (QStringList::Iterator it = cl.begin(); it != cl.end();++it) {
			stream << *it << "\n";
		}
		secConfig.close();
	}
	//====================================
	// install to system
	//------------------------------------
	if ( curConfig.open( IO_WriteOnly ) ) {
		QTextStream stream ( &curConfig );
		for (QStringList::Iterator it = al.begin(); it != al.end();++it) {
			stream << *it << "\n";
		}
		curConfig.close();
	}
	//====================================
	// create symbolic loader links
	//------------------------------------
	unlink  ("/usr/X11R6/bin/X");
	unlink  ("/var/X11R6/bin/X");
	symlink ("/var/X11R6/bin/X","/usr/X11R6/bin/X");
	symlink (SAX_X11_LOADER    ,"/var/X11R6/bin/X");
}

//====================================
// testConfiguration...
//------------------------------------
bool SaXConfig::testConfiguration (void) {
	// .../
	//! test the configuration by starting a new X-Server using
	//! the preliminary configuration file.
	// ----
	if (! createConfiguration()) {
		return false;
	}
	QProcess* test = new QProcess ();
	test -> addArgument ( SAX_TEST_CONFIG );
	test -> addArgument ( "-d"  );
	test -> addArgument ( ":99" );
	if ( ! test -> start() ) {
		excProcessFailed();
		qError (errorString(),EXC_PROCESSFAILED);
	}
	while (test->isRunning()) {
		usleep (1000);
	}
	int exitCode = 0;
	QByteArray data = test -> readStdout();
	QStringList lines = QStringList::split ("\n",data);
	for ( QStringList::Iterator
		in = lines.begin(); in != lines.end(); ++in
	) {
		QString line (*in);
		exitCode = line.toInt();
	}
	switch (exitCode) {
	case 0:
		//====================================
		// test has failed for some reasons
		//------------------------------------
		return false;
	break;
	case 2:
		//====================================
		// test successful + changes saved
		//------------------------------------
		enableXFineCache();
		if (! createConfiguration()) {
			enableXFineCache (false);
			return false;
		}
		return true;
	break;
	default:
		//====================================
		// test successful + changes canceled
		//------------------------------------
		return true;
	break;
	}
	return false;
}

//====================================
// setParseErrorValue...
//------------------------------------
void SaXConfig::setParseErrorValue (char* data) {
	// .../
	//! private member which stores the parse error value
	//! for later acces in getParseErrorValue()
	// ---
	QTextOStream (mParseErrorValue) << data;
}

//====================================
// setParseError...
//------------------------------------
void SaXConfig::setParseError (char* data) {
	// .../
	//! private member which stores the parse error
	//! for later acces in getParseError()
	// ----
	QTextOStream (mParseErrorString) << data;
}

//====================================
// getParseErrorValue...
//------------------------------------
QString SaXConfig::getParseErrorValue (void) {
	// .../
	//! obtain parse error value if there is any. If no
	//! error occured an empty string is returned
	// ----
	if (mParseErrorValue->isEmpty()) {
		return QString("");
	}
	return *mParseErrorValue;
}

//====================================
// getParseError...
//------------------------------------
QString SaXConfig::getParseError (void) {
	// .../
	//! obtain parse error if there is any. If no
	//! error occured an empty string is returned
	// ----
	if (mParseErrorString->isEmpty()) {
		return QString("");
	}
	return *mParseErrorString;
}

//====================================
// error functions for libxf86config...
//------------------------------------
extern "C" {
void VErrorF (const char *f, va_list args) {
	static int n,size = CONFERRORLINE;
	char* data = (char*)malloc (sizeof(char) * size);
	while (1) {
		n = vsnprintf (data, size, f, args);
		if ((n > -1) && (n < size)) {
			break;
		}
		if (n > -1) {
			size = n+1;
		} else {
			size *= 2;
		}
		data = (char*)realloc (data, size);
	}
	if (self) {
		self -> setParseErrorValue (data);
	}
}
void ErrorF (const char *f, ...) {
	static int n,size = CONFERRORLINE;
	char* data = (char*)malloc (sizeof(char) * size);
	va_list args;
	while (1) {
		va_start(args, f);
		n = vsnprintf (data, size, f, args);
		va_end (args);
		if ((n > -1) && (n < size)) {
			break;
		}
		if (n > -1) {
			size = n+1;
		} else {
			size *= 2;
		}
		data = (char*)realloc (data, size);
	}
	if (self) {
		self -> setParseError (data);
	}
}
}
