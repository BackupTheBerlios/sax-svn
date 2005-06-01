/**************
FILE          : data.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for data.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XDATA_H
#define XDATA_H 1

#include <qdict.h>
#include <qlist.h>

//====================================
// Enumerators...
//------------------------------------
enum CDB {
    Cards     = 1,
    Monitors  = 2,
    Keyboards = 3,
    Pointers  = 4,
    Tablets   = 5,
    Touchers  = 6
};

enum Type {
    List      = 7,
    Hash      = 8
};

//====================================
// Class headers...
//------------------------------------
class XApiData {
    private:
    int mFileType;
    QList<char> mList;
    QDict<char> mHash;

    public:
    XApiData (int = List);
	char* operator[] (const char*);
	char* operator[] (const int);
    int  getFileType (void);
    void setFileType (int);
    void setList (const char*);
	void setHash (const QString &, const char*);
	QDict<char>* getHash (void);
	QList<char>* getList (void);
};

class XDb {
    private:
    int mFileType;
    QString mVendor;
    QString mName;
    QDict<char> mProperties;

    public:
    XDb (int = List);
    int  getFileType (void);
    QString getName (void);
    QString getVendor (void);
    QDict<char>* getHash (void);
    void setFileType (int);
    void setName (QString);
    void setVendor (QString);
	void setHash (const QString &,const char*);
	void setHash (QDict<char>);
};

class XData {
	private:
	int mID;
	QString  mSection;
	QDict<char> mData;

	public:
	XData (int = 0);
	QDict<char>  getData (void);
	QDict<char>* getDataPtr (void);
	int getID  (void);
	void setID (int);
	QString getSection (void);
	void setSection (const QString &);
	void setPair (const QString &,const char*);
	char* operator[] (const char*);
	void clear (void);
};

#endif
