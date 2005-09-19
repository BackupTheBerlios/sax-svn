/**************
FILE          : process.cpp
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
#include "process.h"

namespace SaX {
//====================================
// Constructor...
//------------------------------------
SaXProcess::SaXProcess ( void ) {
	// .../
	//! An object of this type is used to read information
	//! from one of the interfaces ISAX CDB SYSP or PROFILE.
	//! Depending what information should be retrieved a new
	//! process is forked calling isax sysp or createPRO.
	//! The CDB information is based on simple file reading
	// ----
	mProc = new SaXProcessCall ();
}

//====================================
// start...
//------------------------------------
void SaXProcess::start ( QList<char> args, int prog ) {
	// .../
	//! This start method will check if (prog) is a valid
	//! program and call it by adding the options set in
	//! args. After the call the appropriate evaluation
	//! method is called to store the data
	// ----
	switch (prog) {
		case SAX_ISAX:
			mProc -> addArgument ( ISAX );
		break;
		case SAX_SYSP:
			mProc -> addArgument ( SYSP );
		break;
		case SAX_PROF:
			mProc -> addArgument ( PROF );
		break;
		case SAX_META:
			mProc -> addArgument ( META );
		break;
		default:
			mProc -> addArgument ( ISAX );
		break;
	}
	QListIterator<char> it (args);
	for (; it.current(); ++it) {
		mProc->addArgument ( it.current() );
	}
	if ( ! mProc -> start() ) {
		excProcessFailed();
		qError (errorString(),EXC_PROCESSFAILED);
	}
	switch (prog) {
		case SAX_ISAX:
			storeData();
		break;
		case SAX_SYSP:
			storeDataSysp();
		break;
		case SAX_PROF:
			storeData();
		break;
		case SAX_META:
			storeData();
		break;
		default:
			storeData();
		break;
	}
}

//====================================
// start...
//------------------------------------
void SaXProcess::start ( int fileID ) {
	// .../
	//! This start method will check if the given fileID
	//! points to a valid file and call the appropriate
	//! evaluation method to the store the data
	// ----
	switch (fileID) {
		case CDB_CARDMODULES:
			storeDataSYS (fileID);
		break;
		case CDB_TABLETMODULES:
			storeDataSYS (fileID);
		break;
		default:
			storeDataCDB (fileID);
		break;
	}
}

//====================================
// storeDataCDB...
//------------------------------------
void SaXProcess::storeDataCDB (int fileID) {
	// .../
	//! Store CDB based group records into the CDB
	//! dictionary. The given file ID must be able to
	//! become resolved into a valid CDB file
	// ----
	QString file;
	switch (fileID) {
		case CDB_CARDS:
			file = CDBCARDS;
		break;
		case CDB_MONITORS:
			file = CDBMONITORS;
		break;
		case CDB_PENS:
			file = CDBPENS;
		break;
		case CDB_POINTERS:
			file = CDBPOINTERS;
		break;
		case CDB_TABLETS:
			file = CDBTABLETS;
		break;
		case CDB_TOUCHERS:
			file = CDBTOUCHERS;
		break;
		default:
			excCDBFileFailed ();
			qError (errorString(),EXC_CDBFILEFAILED);
		break;
	}
	QFile* handle = new QFile (file);
	if (! handle -> open(IO_ReadOnly)) {
		excFileOpenFailed ( errno );
		qError (errorString(),EXC_FILEOPENFAILED);
		return;
	}
	QString line;
	QString group,key,val;
	while ((handle->readLine (line,MAX_LINE_LENGTH)) != 0) {
		line.truncate(line.length()-1);
		if ((line[0] == '#') || (line.isEmpty())) {
			if (handle->atEnd()) {
				break;
			}
			continue;
		}
		int bp = line.find('{');
		if (bp >= 0) {
			QStringList tokens = QStringList::split ( ":", line );
			QString vendor = tokens.first();
			QString name   = tokens.last();
			name.truncate(
				name.find('{')
			);
			name   = name.stripWhiteSpace();
			vendor = vendor.stripWhiteSpace();
			group = vendor+":"+name;
		} else {
			bp = line.find('}');
			if (bp >= 0) {
				continue;
			}
			QStringList tokens = QStringList::split ( "=", line );
			key = tokens.first();
			val = tokens.last();
			val = val.stripWhiteSpace();
			key = key.stripWhiteSpace();
			// ... /
			// CDB keys and ISAX keys are not the same,
			// check this and adapt to ISAX keys
			// ---
			if (key == "Hsync") {
				key = "HorizSync";
			}
			if (key == "Vsync") {
				key = "VertRefresh";
			}
			if (key == "Modeline") {
				key = "SpecialModeline";
			}
			addGroup (group,key,val);
		}
		if (handle->atEnd()) {
			break;
		}
	}
	handle -> close();
}

//====================================
// storeDataSYS...
//------------------------------------
void SaXProcess::storeDataSYS (int fileID) {
	// .../
	//! Store CDB based data which is manually maintained
	//! within the SaX2 source files. Currently only files
	//! of the Format key=value are supported
	// ----
	QString file;
	switch (fileID) {
		case CDB_CARDMODULES:
			file = CDBCARDMODULES;
		break;
		case CDB_TABLETMODULES:
			file = CDBTABLETMODULES;
		break;
		default:
			excCDBFileFailed ();
			qError (errorString(),EXC_CDBFILEFAILED);
		break;
	}
	QFile* handle = new QFile (file);
	if (! handle -> open(IO_ReadOnly)) {
		excFileOpenFailed ( errno );
		qError (errorString(),EXC_FILEOPENFAILED);
		return;
	}
	QString line;
	QString group,key,val;
	while ((handle->readLine (line,MAX_LINE_LENGTH)) != 0) {
		line.truncate(line.length()-1);
		if ((line[0] == '#') || (line.isEmpty())) {
			if (handle->atEnd()) {
				break;
			}
			continue;
		}
		QStringList tokens = QStringList::split ( "=", line );
		group  = tokens.first();
		if (group.contains("Format:(")) {
			continue;
		}
		tokens = QStringList::split ( " ", tokens.last() );
		for (QStringList::Iterator it=tokens.begin(); it!=tokens.end();++ it) {
			QString value (*it);
			QStringList tokens = QStringList::split ( ":", value );
			key = tokens.first();			
			val = tokens.last();
			addGroup (group,key,val);
		}
		if (handle->atEnd()) {
			break;
		}
	}
	handle -> close();
}

//====================================
// storeDataSysp...
//------------------------------------
void SaXProcess::storeDataSysp (void) {
	// .../
	//! Store data which has been written to STDOUT after
	//! a previous sysp process call
	// ----
	QList<QString> data = mProc->readStdout();
	QListIterator<QString> in (data);
	for (; in.current(); ++in) {
		int id = 0;
		QString line (*in.current());
		QStringList tokens = QStringList::split ( "=>", line );
		QString idstr = tokens.first();
		QString data  = tokens.last();
		QStringList datalist = QStringList::split ( ":", data );
		QString key = datalist.first();
		QString val = datalist.last();
		QRegExp idExp ("(\\d+)");
		int rpos = idExp.search (idstr,0);
		if (rpos >= 0) {
			id = idExp.cap().toInt();
		}
		addID (id);
		val = val.stripWhiteSpace();
		key = key.stripWhiteSpace();
		if ((! key.isEmpty()) && (! val.isEmpty())) {
			setItem (key,val);
		}
	}
}

//====================================
// storeData...
//------------------------------------
void SaXProcess::storeData (void) {
	// .../
	//! Store data which has been written to STDOUT after
	//! a previous isax process call
	// ----
	QList<QString> data = mProc->readStdout();
	QListIterator<QString> in (data);
	for (; in.current(); ++in) {
		QString line (*in.current());
		QString cnr;
		QString key;
		QString val;
		int index = 0;
		QStringList tokens = QStringList::split ( ":", line );
		for ( QStringList::Iterator
			in = tokens.begin(); in != tokens.end(); ++in
		) {
			QString item (*in);
			item = item.stripWhiteSpace();
			switch (index) {
			case 0:
				cnr = item;
			break;
			case 1:
				key = item;
			break;
			case 2:
				val = item;
			break;
			default:
				bool isNumber = false;
				if (key == "Screen") {
					QRegExp idExp ("^(\\d+)$");
					if (idExp.search (val,0) >= 0) {
						isNumber=true;
					}
				}
				if (
					((key == "Screen") && (!isNumber)) ||
					 (key == "Modes")||(key == "Virtual")
				) {
					key = key+":"+val;
					val = item;
				} else {
					val = val+":"+item;
				}
			break;
			}
			index++;
		}
		if (val.isEmpty()) {
			continue;
		}
		addID   (cnr.toInt());
		setItem (key,val);
	}
}
} // end namespace
