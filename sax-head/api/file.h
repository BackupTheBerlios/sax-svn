/**************
FILE          : file.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for file.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XFILE_H
#define XFILE_H 1

#include <qapplication.h>
#include <qregexp.h>
#include <qwidget.h>
#include <stdio.h>
#include <qdict.h>
#include <qlist.h>
#include <qdict.h>
#include <qlist.h>
#include <qfile.h>

#include "data.h"
#include "frame.h"

class XFile : public QFile {
	private:
	XProgressDialog *mProgress;
	QString* mFileName;
	QString* mFileKey;
	QString  mD5Sum;
	QFile*   mHandle;
	int      mFileSize;
	char*    mRaw;
	XDb*     mDB;
	XData*   mDD;
	QString* mCnr;
	QString* mKey;
	QString* mVal;
	QString* mOldKey;
	QString* mOldVal;

	private:
	QList<XDb>*   cdb;  // CDB data files
	XApiData      api;  // api data files
	QDict<char>   gtx;  // gettext key file
	QList<XData>* sys;  // ISaX list files

	private:
	void fileOpen (void);

	public:
	XFile (const char* name,const QString& = "");
	~XFile (void);
	QDict<char> gtxRead (void);
	XApiData apiRead    (void);
	QList<XDb>* cdbRead (void);
	QList<char> cdbGetNameList   (QString);
	QList<char> cdbGetVendorList (void);
	void cdbAddXDbSection (const QString& ,const QString& ,QDict<char>);
	XDb* cdbGetSpec (const QString &,const QString &);
	int fileSize    (void);
	int filePos     (void);
	void enableProgressDialog  (XProgressDialog*,XFrame*);
	void disableProgressDialog (void);
	XApiData getAPI       (void);
	QDict<char> getGTX    (void);
	QList<XData>* sysRead (void);
	XData* getDevice   (int);
	int getDeviceCount (void);
	void deleteDevice  (int);
	XData* addDevice   (void);
	XData* addDevice   (int);
	bool sysSerialize  (void);
	bool sysMove       (int,int);
	bool isModified    (XFrame*);
};

#endif
