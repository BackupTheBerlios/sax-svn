/**************
FILE          : xinfo.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xinfo QT message box: 
              : header file for xinfo.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XINFO_H
#define XINFO_H 1

#include <qapplication.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <qdict.h>
#include <locale.h>
#include <libintl.h>

#include "frame.h"
#include "xbox.h"

class XInfo : public QWidget {
	Q_OBJECT

	public:
	XInfo (XFrame*);

	private:
	int mCode;
	XFrame* mParent;
	XWrapText< QDict<char> > mText;

	public:
	void showIntroBox (void);
	void show3DBox    (const QString&,QString*);
	int  returnCode   (void);
};

#endif
