/**************
FILE          : geometry.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : xapi (X11-Application-Interface)
              : setup the picture geometry of your desktop(s)
              : this will affect the modeline timings which are
              : saved to the configuration if the setup is finished
              :
              :
STATUS        : Status: Up-to-date
**************/
#include "geometry.h"

//===============================================
// XDisplayGeometry Constructor...
//-----------------------------------------------
XDisplayGeometry::XDisplayGeometry (void) {
}

//===============================================
// XDisplayGeometry Constructor...
//-----------------------------------------------
XDisplayGeometry::XDisplayGeometry (XFrame* xf,XIntro* xi) : XTemplate(xf,xi) {
    addTo (xf,xi);
}

//===============================================
// XDisplayGeometry add to mainframe...
//-----------------------------------------------
void XDisplayGeometry::addTo (XFrame* xf,XIntro* xi) {
    mIntro    = xi;
    mFrame    = xf;
    mInit     = mFrame -> getInit();
    mStatus   = mFrame -> statusBar();
	mFilePtr  = mIntro -> getFiles();
    mIndex    = Geometry;
    mPage     = mIndex;
    mLastPage = GeometryPage1;
    getConnected (xf);
    dialogCreate ();
}

//===============================================
// XDisplayGeometry create the dialog...
//-----------------------------------------------
void XDisplayGeometry::dialogCreate (void) {
    mDialog0 = new QFrame ( mStack );
    mDialog1 = new QFrame ( mStack );

    QBoxLayout* layout0 = new QVBoxLayout ( mDialog0 );
    QBoxLayout* layout1 = new QVBoxLayout ( mDialog1 );
    QLabel *label0 = new QLabel (
        "load Geometry page... done",mDialog0
    );
    QLabel *label1 = new QLabel (
        "not jet implemented... <next>",mDialog1
    );
    layout0 -> addWidget ( label0 );
    layout1 -> addWidget ( label1 );

    mStack -> addWidget ( mDialog0 , mIndex );
    mStack -> addWidget ( mDialog1 , GeometryPage1 );
}

//===============================================
// XDisplayGeometry slotIntro...
//-----------------------------------------------
void XDisplayGeometry::slotIntro (int index) {
	if (index == mIndex) {
	XWrapPointer< QDict<char> > mText (mTextPtr);
	char*       part = NULL;
	char*       line = NULL;
	XTextBrowser* info;
	QString     message;
	QString     idents;
	QString     resolution;
	QString     dac;
	QString     timing;
	QString     optm ("-m");
	QString     optd ("-d");
	QString     optf ("-f");

	// xquery information...
	// -----------------------
	QString modeline = qx(XQUERY,STDOUT,3,"%s %s %s",
		optd.ascii(),mFrame->getDisplay()->ascii(),optm.ascii()
	);
	XQuery qMode (modeline);
	QString freq = qx(XQUERY,STDOUT,3,"%s %s %s",
		optd.ascii(),mFrame->getDisplay()->ascii(),optf.ascii()
	);
	XQuery qFrq (freq);

	info    = mIntro -> getTextView();
	message = mText["geometry_header"];	
	message += "<br><br><table border=1 bgcolor=lightgrey ";
    message += "cellspacing=1 width=90%>";

	for (int n=0; n < qMode.getScreen(); n++) {
		line = (char*) malloc (sizeof(char) * 256);
		sprintf(line,"%s",qMode.getValue(n).ascii());
		part = strtok (line,"\"");
		resolution.sprintf("%s",part);
		part = strtok (NULL,"\"");
		timing.sprintf("%s",part);
		sprintf(line,"%s",timing.ascii());
		part = strtok (line," ");
		dac.sprintf("%s",part);
		timing.sprintf("%c",'\0');
		while (part) {
			part = strtok (NULL," ");
			timing.sprintf("%s %s",timing.ascii(),part);
		}
		free (line);
		// ...
		// set table data cell
		// ---
		message += "<tr>";
		idents.sprintf(
			"<td width=5%c align=center>( %d )</td>",'%',n
		);
        message += idents;
		idents.sprintf(
			"<td width=20%c>%s</td>",'%',resolution.ascii()
		);
		message += idents;
		idents.sprintf(
			"<td width=40%c>%s</td>",'%',qFrq.getValue(n).ascii()
		);
		message += idents;
		idents.sprintf(
			"<td width=25%c>%.0f ",'%',dac.toFloat()
		);
		message += idents;
		QTextOStream (&idents) << mText["mhz"] << "</td>";
		message += idents;
		message += idents;
		message += "</tr>";
	}

	message += "</table><br>";
	info -> setText (message);
	}
}
