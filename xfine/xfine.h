/**************
FILE          : xfine.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Sch�fer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : XFine2 (display fine tuning tool)
              : header definitions for xfine.c
              : 
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XFINE_H
#define XFINE_H 1

#include "config.h"
#include "../api/common/log.h"

//===================================
// Prototypes...
//-----------------------------------
void usage (void);
bool accessAllowed (void);
void signalActivateSave (int);
void signalQuit (int);
void signalSave (int);
void signalReset (int);

//===================================
// Templates...
//-----------------------------------
template <class T>
class XWrapText {
	private:
	T* pM;

	public:
	XWrapText (void) { }
	XWrapText (T* typ) {
		pM = typ;
	}
	void init (T* typ) {
		pM = typ;
	}
	QString operator[] (const char* key) {
		return(QString::fromLocal8Bit(pM->operator[](key)));
	}
};

#endif
