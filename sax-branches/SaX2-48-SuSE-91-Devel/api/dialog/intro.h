/**************
FILE          : intro.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for intro.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef XINTRO_H
#define XINTRO_H 1

#include <qapplication.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <qframe.h>
#include <qwidgetstack.h>
#include <qtextview.h>
#include <qtextbrowser.h>

#include "../template.h"
#include "../file.h"
#include "../config.h"
#include "../common/log.h"
#include "../common/qx.h"
#include "../xapi.h"

//====================================
// Pre-class Definitions...
//------------------------------------
class XIntro;
class XTopicMimeFactory;
class XTextBrowser;

//====================================
// Dialog Includes...
//------------------------------------
#include "accessx/accessx.h"
#include "card/card.h"
#include "desktop/desktop.h"
#include "geometry/geometry.h"
#include "keyboard/keyboard.h"
#include "layout/layout.h"
#include "monitor/monitor.h"
#include "mouse/mouse.h"
#include "multihead/multihead.h"
#include "tablet/tablet.h"
#include "touchscreen/touchscreen.h"
#include "opengl/opengl.h"
#include "virtual/virtual.h"

//====================================
// Class XQuery...
//------------------------------------
class XQuery {
	private:
	QList<char> mValue;
	int mScreen;

	public:
	XQuery  (const QString&,bool truncate=TRUE);
	QString getValue (int);
	int getScreen (void);
};

//====================================
// Class XStringList...
//------------------------------------
class XStringList {
	private:
	QList<char> mList;
	QString*    mSeperator;
	QString*    mText;

	public:
	XStringList (const QString& = "");
	void setText (const QString&);
	void setSeperator (const QString&);
	QList<char> getList (void);
	void clear (void);
};

//====================================
// Class XIntro...
//------------------------------------
class XIntro : public XTemplate {
	Q_OBJECT

	private:
	QFrame*       mDialog;
	QListView*    mComponent;
	XTextBrowser* mTextView;

	private:
	int           mIndex;
	QString*      mHeaderText;
	XFrame*       mFrame;
	QDict<XFile>  mFiles;
	QIntDict<void> mStoreWidget;

	public:
	XIntro (XFrame*);
	XIntro (void);
	void addTo (XFrame* xf);
	QDict<XFile>* getFiles (void);
	void importFile (QString&);
	bool isVisible (void);
	XTextBrowser* getTextView (void);
	QString* getHeaderText (void);
	void checkDetected (void);
	void store (void*,int);
	void* retrieve (int);

	private:
	void dialogCreate (void);

	protected:
	void slotNext  (void);
	void slotBack  (void);
	bool slotRun   (int);
	void slotIntro (int);

	private slots:
	void slotMimeType ( const QString& );
};

//====================================
// Class XTextBrowser...
//------------------------------------
class XTextBrowser : public QTextBrowser {
	Q_OBJECT

	public:
	XTextBrowser ( QWidget* p ) : QTextBrowser (p) {}

	protected:
	void setSource ( const QString & name ) {
		linkSelected (name);
	}

	signals:
	void linkSelected ( const QString & );
};

#endif
