/**************
FILE          : data.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : data will provide the data structure
              : objects to be able to save all the
              : configuration information
              :
STATUS        : Status: Up-to-date
**************/
#include <qapplication.h>
#include <qfile.h>
#include <qlist.h>
#include <qdict.h>

#include "data.h"

//====================================
// XApiData implementation...
//------------------------------------
XApiData::XApiData (int t) {
	mFileType = t;
}
int XApiData::getFileType (void) {
	return(mFileType);
}
void XApiData::setFileType (int t) {
	mFileType = t;
}
void XApiData::setList (const char* element) {
	mList.append(element);
}
void XApiData::setHash (const QString & key,const char* val) {
	mHash.insert(key,val);
}
char* XApiData::operator[] (const char* key) {
	return(mHash[key]);
}
char* XApiData::operator[] (const int index) {
	return(mList.at(index));
} 
QDict<char>* XApiData::getHash (void) {
	return(&mHash);
}
QList<char>* XApiData::getList (void) {
	return(&mList);
}

//====================================
// XDb implementation...
//------------------------------------
XDb::XDb (int t) {
	mFileType   = t;
}
int XDb::getFileType (void) {
	return(mFileType);
}
QString XDb::getName (void) {
	return(mName);
}
QString XDb::getVendor (void) {
	return(mVendor);
}
QDict<char>* XDb::getHash (void) {
	return(&mProperties);
}
void XDb::setFileType (int t) {
	mFileType = t;
}
void XDb::setName (QString name) {
	mName = name;
}
void XDb::setVendor (QString vendor) {
	mVendor = vendor;
}
void XDb::setHash (const QString & key,const char* val) {
	mProperties.insert(key,val);
}
void XDb::setHash (QDict<char> hash) {
	mProperties = hash;
}

//====================================
// XData implementation...
//------------------------------------
XData::XData (int section) {
	mID = section;
}
void XData::setPair (const QString & key,const char* val) {
	if (val) {
	if (mData.find (key)) {
		mData.replace (key,val);
	} else {
		mData.insert (key,val);
	}
	}
}
QDict<char> XData::getData (void) {
	return(mData);
}
QDict<char>* XData::getDataPtr (void) {
	return(&mData);
}
int XData::getID  (void) {
	return(mID);
}
void XData::setID (int id) {
	mID = id;
}
QString XData::getSection (void) {
	return(mSection);
}
void XData::setSection (const QString & name) {
	mSection = name;
}
char* XData::operator[] (const char* key) {
    return(mData[key]);
}
void XData::clear (void) {
	mData.clear();
}
