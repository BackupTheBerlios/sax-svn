/**************
FILE          : opengl.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : header definitions for opengl.c
              :
STATUS        : Status: Up-to-date
**************/
#ifndef OPENGL_H
#define OPENGL_H 1

#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>

#include "../intro.h"

//=====================================
// Enumarators...
//-------------------------------------
enum {
	OpenGLPage1 = OpenGL,
};

//=====================================
// Class Headers...
//-------------------------------------
class XOpenGL : public XTemplate {
	Q_OBJECT

	private:
	QCheckBox*    mCheck3D;
	bool          mCheck3DStatusChanged;

	private:
	QFrame*       mDialog;
	XIntro*       mIntro;
	QDict<XFile>* mFilePtr;

    public:
	XOpenGL (XFrame*,XIntro*);
	XOpenGL (void);
	void addTo (XFrame* xf,XIntro* xi);

	private:
	void dialogCreate (void);
	void resetPage    (int = PAGE_NOLOAD);
	void pageCalled   (int);
	void initPage     (void);

	protected:
	bool slotRun   (int);
	void slotIntro (int);

	public slots:
	void slot3D    ( void );
};

#endif
