/**************
FILE          : file.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Sch�fer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : XFine2 (display fine tuning tool)
              : file will provide reader functions to include
              : the gettext key files...
              :
              :
STATUS        : Status: Up-to-date
**************/
#include <qapplication.h>
#include <qfileinfo.h>
#include <qregexp.h>
#include <qfile.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include "file.h"

//====================================
// XFile constructor...
//------------------------------------
XFile::XFile (const char* name) {
	mFileName = new QString(name);
	mHandle   = new QFile(*mFileName);
	fileOpen ();
	mFileSize = mHandle->size();
}

//====================================
// XFile destructor...
//------------------------------------
XFile::~XFile (void) {
	mHandle -> close();
}

//====================================
// XFile open file...
//------------------------------------
void XFile::fileOpen (void) {
	if (! mHandle -> isOpen()) {
	if (! mHandle -> open(IO_ReadOnly)) {
	log (L_ERROR,
		"XFile::XFile: open failed on: %s -> %s\n",
		mFileName->ascii(),strerror(errno)
	);
	exit(1);
	}
	}
}

//====================================
// XFile get size of the file...
//------------------------------------
int XFile::fileSize (void) {
	return(mFileSize);
}

//====================================
// XFile get position in file...
//------------------------------------
int XFile::filePos (void) {
	return(mHandle->at());
}

//====================================
// XFile read translation keys...
//------------------------------------
QDict<char> XFile::gtxRead (void) {
	fileOpen ();
	QString line;
	QString *qVal;
	char* key = NULL;
	char* val = NULL;
	while ((mHandle->readLine(line,4096)) != 0) {
	line.truncate(line.length()-1);
	if ((line[0] == '#') || (line.isEmpty())) {
	if (mHandle->atEnd()) break;
	continue;
	}
	mRaw = (char*) malloc (line.length() + 1);
	sprintf(mRaw,"%s",line.ascii());
	key = strtok(mRaw,"=");
	val = strtok(NULL,"=");
	qVal = new QString (val);
	int bslash = qVal->find ('\\');
	if (bslash >= 0) {
		*qVal = qVal->replace (bslash,2,"\n");
		while ( 1 ) {
		bslash = qVal->find ('\\');
		if (bslash >= 0) {
			*qVal  = qVal->replace (bslash,2,"\n");
		} else {
			break;
		}
		}
	}
	gtx.insert(key,qVal->ascii());
	if (mHandle->atEnd()) {
		break;
	}
	}
	mHandle->close();
	return(gtx);
}

//====================================
// XFile return GTX...
//------------------------------------
QDict<char> XFile::getGTX (void) {
	return(gtx);
}
