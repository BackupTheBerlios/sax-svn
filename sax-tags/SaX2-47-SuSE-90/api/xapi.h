/**************
FILE          : xapi.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header file for main program
              : 
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XAPI_H
#define XAPI_H 1

#include <qthread.h>
#include <qtextstream.h>

#include "frame.h"
#include "file.h"
#include "data.h"
#include "common/log.h"
#include "dialog/intro.h"
#include "xbox.h"

//===================================
// Globals...
//-----------------------------------
extern char* globalStyle;

//===================================
// Templates...
//-----------------------------------
template <class T>
class XWrapPointer {
	private:
    T* pM;

	public:
	XWrapPointer (void) { }
	XWrapPointer (T* typ) { 
		pM = typ; 
	}
	void init (T* typ) {
		pM = typ;
	}
	#if 0
	char* operator[] (const char* key) {
		return(pM->operator[](key));
	}
	#endif
	QString operator[] (const char* key) {
		return(QString::fromLocal8Bit(pM->operator[](key)));
	}
	QDict<char> getData (void) {
		return(pM->getData());
	}
	int getID  (void) {
		return(pM->getID());
	}
	QString getSection (void) {
		return(pM->getSection());
	}
	void setSection (const QString & section) {
		pM->setSection(section);
	}
	void setPair (const QString & key,const char* val) {
		pM->setPair (key,val);
	}
	void clear (void) {
		pM->clear();
	}
};

template <class T>
class XWrapFile {
	private:
	T* pM;

	public:
	XWrapFile (T* typ) {
		pM = typ;
	}
	XFile* operator[] (const char* key) {
		return(pM->operator[](key));
	}
};

//===================================
// Thread classes...
//-----------------------------------
class XRunXFine : public QThread {
	private:
	int code;

	public:
	XRunXFine (void) { 
		code = 0;
	}
	virtual void run ( void ) {
		QString optst ("-style");
		if (globalStyle) {
			QString command;
			command.sprintf ("%s %s %s",XFINE,optst.ascii(),globalStyle);
			code = system (command);
		} else {
			code = system (XFINE);
		}
		exit();
	}
	int getReturnCode ( void ) {
		return (code);
	}
};

//===================================
// Prototypes...
//-----------------------------------
void usage (void);
bool accessAllowed (void);
bool setMessage (const QString&,XFrame* = 0);
void removeXFineCache (void);
void setMouseCursor (const QString& = "top_left_arrow");
void gotInterrupted (int);
void gotKill (int);

#endif
