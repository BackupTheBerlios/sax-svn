/**************
FILE          : file.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : file will provide reader functions to include
              : the gettext key files and the data files used
              : for the configuration
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

#include "common/log.h"
#include "common/qx.h"
#include "data.h"
#include "file.h"
#include "frame.h"
#include "config.h"
#include "xapi.h"

//====================================
// XFile constructor...
//------------------------------------
XFile::XFile (const char* name,const QString& key) {
	mFileName = new QString (name);
	mFileKey  = new QString (key);
	mHandle   = new QFile(*mFileName);
	mD5Sum    = qx(MD5,STDOUT,1,"%s",name);
	fileOpen ();
	mFileSize = mHandle->size();
	mProgress = NULL;
	cdb = NULL;
	sys = NULL;
}

//====================================
// XFile destructor...
//------------------------------------
XFile::~XFile (void) {
	mHandle -> close();
	if (cdb) {
		delete (cdb);
	}
	if (sys) {
		delete (sys);
	}
}

//====================================
// XFile check if file was changed...
//------------------------------------
bool XFile::isModified (XFrame* xf) {
	#if 0
	QString current = qx(MD5,STDOUT,1,"%s",mFileName->ascii());
	if (current != mD5Sum) {
		xf->setModified ( TRUE,mFileName );
		return (TRUE);
	}
	xf->setModified ( FALSE,mFileName );
	return (FALSE);
	#endif
	xf->setModified ( TRUE,mFileName );
	return (TRUE);
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
// XFile set progress dialog...
//------------------------------------
void XFile::enableProgressDialog (XProgressDialog *p,XFrame* frame) {
	QString* label = new QString();
	QDict<char>* textPtr = frame->getTextPointer();
	XWrapPointer< QDict<char> > text (textPtr);
	mProgress = p;
	XStringList nameList (*mFileName);
	nameList.setSeperator ("/");
	QList<char> baseName = nameList.getList();
	QString textKey;
	QString key = mFileKey->left(3);
	if (key == "cdb") {
		textKey = "loadingCDBInfo";
	}
	if (key == "std") {
		textKey = "loadingBaseInfo";
	}
	if (key == "sys") {
		textKey = "loadingSystemInfo";
	}
	QString translation = text [textKey];
	translation.replace (
		QRegExp("\%1"),baseName.getLast()
	);
	QTextOStream (label) << translation << "...";

	mProgress-> setLabelText (*label);
	mProgress-> setCancelButtonText (text["Cancel"]);
	mProgress-> setTotalSteps (mFileSize);
	mProgress-> showDialog (true);
	if (mFileSize < 5000 ) {
		mProgress -> showDialog (false);
	}
	mProgress-> setProgress  (0);
	mProgress-> setMinimumDuration (300);
	mProgress-> setAutoClose (FALSE);
	QObject::connect(
		mProgress, SIGNAL (cancelled    (void)),
		frame    , SLOT   (slotFastExit (void))
	);
}

//====================================
// XFile cancel progress dialog...
//------------------------------------
void XFile::disableProgressDialog (void) {
	if (mProgress) {
		mProgress->cancel();
	}
}


//====================================
// XFile serialize sys file...
//------------------------------------
bool XFile::sysSerialize (void) {
	QFileInfo file (*mHandle);
	QString base = file.baseName();
	if (sys) {
	if (! mHandle -> open(IO_WriteOnly)) {
		log (L_ERROR,
			"XFile::XFile: open failed on: %s -> %s\n",
			mFileName->ascii(),strerror(errno)
		);
		return (FALSE);
	}
	QListIterator<XData> io (*sys);
	for (; io.current(); ++io) {
	QDict<char> data = io.current()->getData();
	QDictIterator<char> it (data);
	int count = io.current()->getID();
	for (; it.current(); ++it) {
		if (QString(it.current()).isEmpty()) {
			continue;
		}
		QString line;
		line.sprintf ("%d : %-20s : %s\n",
			count,it.currentKey().ascii(),it.current()
		);
		mHandle -> writeBlock (
			line.ascii(),line.length()
		);
	}
	}
	mHandle -> close();
	return (TRUE);
	}
	return (FALSE);
}

//====================================
// XFile move device section ID`s...
//------------------------------------
bool XFile::sysMove (int from, int to) {
	XData* device = getDevice (from);
	if (! device) {
		return (FALSE);
	}

	device->setID (to);
	QRegExp identifier ("\\[.+\\]");

	QDict<char> data = device->getData();
	QDictIterator<char> it (data);
	for (; it.current(); ++it) {
	// QT3
	int rpos = identifier.search (it.current(),0);
	//int rpos = identifier.find (it.current(),0);
	if (rpos >= 0) {
		QString* line;
		QString newid;
		newid.sprintf("%d",to);
		line = new QString;
		line->sprintf("%s",it.current());
		line->replace (rpos+1,1,newid);
		device->setPair (it.currentKey(),line->ascii());
	}
	}
	return (TRUE);
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
	while ((mHandle->readLine(line,MAX_LINE_LENGTH)) != 0) {
	if (mProgress) {
		mProgress->setProgress (filePos());
	}
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
	if (mProgress) {
		mProgress-> setProgress (mProgress->totalSteps());
	}
	mHandle->close();
	return(gtx);
}

//====================================
// XFile read api info files...
//------------------------------------
XApiData XFile::apiRead (void) {
	fileOpen ();
	QString line;
	char* key;
	char* val;
	while ((mHandle->readLine(line,MAX_LINE_LENGTH)) != 0) {
	if (mProgress) {
		mProgress->setProgress (filePos());
	}
	line.truncate(line.length()-1);
	if ((line[0] == '[') || (line[0] == '#') || (line.isEmpty())) {
	if (mHandle->atEnd()) break;
	continue;
	}
	mRaw  = (char*) malloc (line.length() + 1);
	sprintf(mRaw,"%s",line.ascii());
	key   = strtok(mRaw,"=");
	val   = strtok(NULL,"=");
	mKey  = new QString(key);
	mVal  = new QString(val);
	if (line[0] == '\t') {
		*mKey = mKey->stripWhiteSpace();
		mOldVal->sprintf("%s,%s",mOldVal->ascii(),mKey->ascii());
		mKey = mOldKey;
		mVal = mOldVal;
	} else {
		*mKey   = mKey->stripWhiteSpace();
		*mVal   = mVal->stripWhiteSpace();
		mOldKey = new QString (key);
		mOldVal = new QString (val);
	}
	if ((val == NULL) && (line[0] != '\t')) {
		api.setList (mKey->ascii());
		api.setFileType (List);
	} else {
		api.setHash (mKey->ascii(),mVal->ascii());
		api.setFileType (Hash);
	}
	if (mHandle->atEnd()) {
		break;
    }
	}
	if (mProgress) {
		mProgress-> setProgress (mProgress->totalSteps());
	}
	mHandle->close();
	return(api);
}

//====================================
// XFile read CDB data files...
//------------------------------------
QList<XDb>* XFile::cdbRead (void) {
	fileOpen ();
	cdb = new QList<XDb>;
	QString line;
	char* vendor = NULL;
	char* name   = NULL;
	char* key    = NULL;
	char* val    = NULL;
	int step     = 0;
	int bp;
	while ((mHandle->readLine(line,MAX_LINE_LENGTH)) != 0) {
	if (mProgress) {
		if ((step == 100) && (mFileSize > 1000)) {
		mProgress->setProgress (filePos());
		step = 0;
		}
	}
	step++;
	line.truncate(line.length()-1);
	if ((line[0] == '#') || (line.isEmpty())) {
	if (mHandle->atEnd()) break;
	continue;
	}
	mRaw  = (char*) malloc (line.length() + 1);
	sprintf(mRaw,"%s",line.ascii());

	bp = line.find('{');
	if (bp >= 0) {
		mDB = new XDb();
		vendor = strtok(mRaw,":");
		name   = strtok(NULL,":");
		mKey   = new QString (vendor);
		mVal   = new QString (name);
		mVal->truncate(
			mVal->find('{')
		);
		*mVal = mVal->stripWhiteSpace();
		mDB -> setVendor (mKey->ascii());
		mDB -> setName   (mVal->ascii());
	} else {
		bp = line.find('}');
		if (bp >= 0) {
			cdb->append(mDB);
			continue;
		}
		key   = strtok(mRaw,"=");
        val   = strtok(NULL,"=");
		mKey  = new QString (key);
		mVal  = new QString (val);
		*mVal = mVal->stripWhiteSpace();
		*mKey = mKey->stripWhiteSpace();
		mDB -> setHash(mKey->ascii(),mVal->ascii());
	}
	if (mHandle->atEnd()) {
		break;
	}
	}
	if (mProgress) {
		mProgress-> setProgress (mProgress->totalSteps());
	}
	mHandle->close();
	return(cdb);
}

//====================================
// XFile read ISaX output lists...
//------------------------------------
QList<XData>* XFile::sysRead (void) {
	fileOpen ();
	sys = new QList<XData>;
	int card;
	QString line;
	char* cnr = NULL;
	char* key = NULL;
	char* val = NULL;
	QFileInfo file (*mHandle);
	while ((mHandle->readLine(line,MAX_LINE_LENGTH)) != 0) {
	if (mProgress) {
		mProgress->setProgress (filePos());
	}
	line.truncate(line.length()-1);
	if ((line[0] == '#') || (line.isEmpty())) {
	if (mHandle->atEnd()) break;
	continue;
	}
	mRaw  = (char*) malloc (line.length() + 1);
	sprintf(mRaw,"%s",line.ascii());

	// special case for Modes Keyword...
	// ----------------------------------
	bool isModes   = FALSE;
	bool isScreen  = FALSE;
	bool isVirtual = FALSE;
	if (strstr(mRaw,"Modes:") != NULL) {
		isModes  = TRUE;
	}
	if (strstr(mRaw,"Screen:") != NULL) {
		isScreen = TRUE;
	}
	if (strstr(mRaw,"Virtual:") != NULL) {
		isVirtual = TRUE;
	}
	cnr = strtok(mRaw,":");
	key = strtok(NULL,":");
	if ((isModes) || (isScreen) || (isVirtual)) {
		val = strtok(NULL,":");
	} else {
		val = strtok(NULL,"");
	}

	if ((isModes) || (isScreen) || (isVirtual)) {
		char *next  = strtok(NULL,":");
		char *modes = (char*) malloc (line.length() + 1);
		sprintf(modes,"%s:%s",key,val);
		key = modes;
		val = next;
	}

	// save card key value pairs as QStrings...
	// -------------------------------------------
	mCnr  = new QString (cnr);
	mKey  = new QString (key);
	mVal  = new QString (val);
	*mVal = mVal -> stripWhiteSpace();
	*mKey = mKey -> stripWhiteSpace();
	*mCnr = mCnr -> stripWhiteSpace();

	if (mVal->isEmpty()) {
		continue;
	}

	bool addSection = TRUE;
	card  = mCnr->toInt();
	QListIterator<XData> it (*sys);
	int listIndex = 0;
	for (; it.current(); ++it) {
	if (it.current()->getID() == card) {
		addSection = FALSE;
		break;
	}
	listIndex++;
	} 
	if (addSection) {
		mDD = new XData (card);
		mDD -> setSection (file.baseName());
		mDD -> setPair (mKey->ascii(),mVal->ascii());
		sys -> append (mDD);
	} else {
		mDD = sys -> at (listIndex);
		mDD -> setPair (mKey->ascii(),mVal->ascii());
	}
	if (mHandle->atEnd()) {
		break;
	}
	}
	if (mProgress) {
		mProgress-> setProgress (mProgress->totalSteps());
		//mProgress -> cancel();
	}
	mHandle -> close();
	return(sys);
}

//====================================
// XFile get CDB name list...
//------------------------------------
QList<char> XFile::cdbGetNameList (QString vendor) {
	QListIterator<XDb> it (*cdb);
	QList<char> names;
	for (; it.current(); ++it) {
	XDb *pCDB = it.current();
	if (pCDB->getVendor() == vendor) {
		names.append (pCDB->getName());
	}
	}
	return(names);
}

//====================================
// XFile get CDB vendor list...
//------------------------------------
QList<char> XFile::cdbGetVendorList (void) {
	QListIterator<XDb> it (*cdb);
	QList<char> vendors;
	QString currentVendor;
	bool found;
	for (; it.current(); ++it) {
		XDb *pCDB = it.current();
		currentVendor = pCDB->getVendor();
		found = FALSE;
		QListIterator<char> io (vendors);
		for (; io.current(); ++io) {
		if (io.current() == currentVendor) {
			found = TRUE;
			break;
		}
		}
		if (! found) {
		vendors.append (currentVendor);
		}
	}
	return(vendors);
}

//====================================
// XFile get CDB spec block for V/N...
//------------------------------------
XDb* XFile::cdbGetSpec (const QString & v,const QString & n) {
	QListIterator<XDb> it (*cdb);
	for (; it.current(); ++it) {
		XDb *pCDB = it.current();
		if ((pCDB->getVendor() == v) && (pCDB->getName() == n)) {
		return(pCDB);
		}
	}
	return(NULL);
}

//====================================
// XFile add XDb structure to cdb...
//------------------------------------
void XFile::cdbAddXDbSection (
	const QString& vendor,const QString& name,QDict<char> properties
) {
	if (! cdbGetSpec (vendor,name)) {
		XDb* data = new XDb (Hash);
		data -> setName (name); data -> setVendor (vendor);
		data -> setHash (properties);
		cdb -> append (data);
	}
}

//====================================
// XFile return API...
//------------------------------------
XApiData XFile::getAPI (void) {
	return(api);
}

//====================================
// XFile return GTX...
//------------------------------------
QDict<char> XFile::getGTX (void) {
	return(gtx);
}

//====================================
// XFile search XData section...
//------------------------------------
XData* XFile::getDevice (int index) {
	QListIterator<XData> it (*sys);
    for (; it.current(); ++it) {
	if (it.current()->getID() == index) {
		return(it.current());
	}
	}
	return(0);
}

//====================================
// XFile get number of devices...
//------------------------------------
int XFile::getDeviceCount (void) {
	return (sys->count());
}

//====================================
// XFile add new XData section...
//------------------------------------
XData* XFile::addDevice (int index) {
	QFileInfo file (*mHandle);
	XData *mNew = new XData (index);
	mNew -> setSection (file.baseName());

	bool exist = FALSE;
	int  count = 0;
	QListIterator<XData> it (*sys);
	for (; it.current(); ++it) {
	if (it.current()->getID() == index) {
		exist = TRUE;
		break;
	}
	count++;
	}
	if (exist) {
		sys -> remove (count);
		sys -> insert (count,mNew);
		log(L_WARN,
			"XFile::insert: overwrite %d at [QList<XData>* sys] pos %d\n",
			index,count
		);
	} else { 
		sys -> append (mNew); 
	}
	return(mNew);
}

//====================================
// XFile add new XData section...
//------------------------------------
XData* XFile::addDevice (void) {
	QFileInfo file (*mHandle);
	int index = 0;
	QListIterator<XData> it (*sys);
	for (; it.current(); ++it) {
	if (it.current()->getID() > index) {
		index =  it.current()->getID();
    }
	}
	XData *mNew = new XData (index);
	mNew -> setSection (file.baseName());
	sys -> append (mNew);
	return(mNew);
}

//====================================
// XFile delete XData section...
//------------------------------------
void XFile::deleteDevice (int index) {
	QListIterator<XData> it (*sys);
	int count = 0;
	for (; it.current(); ++it) {
	if (it.current()->getID() == index) {
		delete (it.current());
		sys -> remove (count);
		return;
    }
	count++;
    }
	log(L_WARN,
		"XFile::delete: no such index %d [QList<XData>* sys]\n",index
	);
}

