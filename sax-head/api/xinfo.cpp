/**************
FILE          : xinfo.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xinfo QT message box: 
              : used for xc.pl to indicate the automatic
              : suggestion made via SaX2
              :
              :
STATUS        : Status: Up-to-date
**************/

#include <qapplication.h>
#include <qregexp.h>
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

#include "xinfo.h"

//=====================================
// external globals
//-------------------------------------
extern int globalFrameWidth;

//====================================
// XInfo::Constructor
//------------------------------------
XInfo::XInfo (XFrame* xapi) {
	mParent = xapi;
	mParent -> setFrame();
	mText.init (mParent -> getTextPointer());
	mCode = 0;
}

//====================================
// XInfo::show intro message box
//------------------------------------
void XInfo::showIntroBox (void) {
	XBox mb (
		mText["suggesttopic"],
		mText["suggest"],
		XBox::Information,
		XBox::Ok,
		XBox::Cancel,
		XBox::Ignore,
		mParent,
		globalFrameWidth
	);
	mb.setButtonText (
		XBox::Ok,mText["Ok"]
	);
	mb.setButtonText (
		XBox::Ignore,mText["Start"]
	);
	mb.setButtonText (
		XBox::Cancel,mText["Cancel"]
	);
	switch (mb.exec()) {
	case XBox::Ignore:
		mCode = 1;
	break;
	case XBox::Ok:
		mCode = 0;
	break;
	case XBox::Cancel:
		mCode = 2;
	break;
	}
}

//====================================
// XInfo::show 3D message box
//------------------------------------
void XInfo::show3DBox (const QString& cardName,QString* cardDriver) {
	QString D3Text;
	QString textKey ("question3d");
	if (cardDriver) {
	if ((*cardDriver == "nv") || (*cardDriver == "nvidia")) {
		textKey.sprintf("question3dnvidia");
	}
	}
	D3Text = mText[textKey];
	D3Text.replace (
		QRegExp("\%1"),cardName
	);
	XBox mb (
		mText["topic3d"],
		D3Text,
		XBox::Information,
		XBox::Yes,
		XBox::No, 0,0,
		globalFrameWidth
	);
	if (textKey == "question3dnvidia") {
		mb.setMinimumHeight (370);
	}
	mb.setButtonText (
		XBox::Yes,mText["Yes"]
	);
	mb.setButtonText (
		XBox::No,mText["No"]
	);
	switch (mb.exec()) {
	case XBox::No:
		mCode = 1;
	break;
	case XBox::Yes:
		mCode = 0;
	break;
    }
}

//====================================
// XInfo::return code from mBox
//------------------------------------
int XInfo::returnCode (void) {
	return (mCode);
}
