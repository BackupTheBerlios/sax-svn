/**************
FILE          : process.h
***************
PROJECT       : SaX2 - library interface [header]
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : SaX2 - SuSE advanced X11 configuration 
              : 
              :
DESCRIPTION   : native C++ class library to access SaX2
              : functionality. Easy to use interface for
              : //.../
              : - importing/exporting X11 configurations
              : - modifying/creating X11 configurations 
              : ---
              :
              :
STATUS        : Status: Development
**************/
#ifndef SAX_PROCESS_H
#define SAX_PROCESS_H 1

//====================================
// Includes...
//------------------------------------
#include <qprocess.h>
#include <qregexp.h>

#include "storage.h"
#include "exception.h"

//====================================
// Defines...
//------------------------------------
#define ISAX               "/usr/X11R6/lib/sax/tools/isax"
#define SYSP               "/usr/X11R6/lib/sax/sysp.pl"
#define PROF               "/var/lib/sax/createPRO.pl"

#define CDBDIR             "/usr/X11R6/lib/sax/api/data/cdb/"
#define DATADIR            "/usr/X11R6/lib/sax/api/data/"
#define CDBCARDS           CDBDIR "Cards"
#define CDBMONITORS        CDBDIR "Monitors"
#define CDBPOINTERS        CDBDIR "Pointers"
#define CDBPENS            CDBDIR "Pens"
#define CDBTABLETS         CDBDIR "Tablets"
#define CDBTOUCHERS        CDBDIR "Touchscreens"
#define CDBCARDMODULES     DATADIR "CardModules"
#define CDBTABLETMODULES   DATADIR "TabletModules"

#define SAX_CARD           4
#define SAX_DESKTOP        5
#define SAX_POINTERS       6
#define SAX_KEYBOARD       7
#define SAX_LAYOUT         8
#define SAX_PATH           9
#define SAX_EXTENSIONS     10
#define SAX_ISAX           11
#define SAX_SYSP           12
#define SAX_CDB            13
#define SAX_PROF           14

#define CDB_CARDS          15
#define CDB_MONITORS       16
#define CDB_PENS           17
#define CDB_POINTERS       18
#define CDB_TABLETS        19
#define CDB_TOUCHERS       20
#define CDB_CARDMODULES    21
#define CDB_TABLETMODULES  22

#define SYSP_MOUSE         30
#define SYSP_KEYBOARD      31
#define SYSP_CARD          32
#define SYSP_DESKTOP       33
#define SYSP_3D            34

#define MAX_LINE_LENGTH    8192

//====================================
// Interface class for dlopen ability
//------------------------------------
class SaXProcessIF : public SaXStorage {
	public:
	virtual void start ( QList<char>, int = SAX_ISAX ) = 0;
	virtual void start ( int ) = 0;

	public:
	virtual ~SaXProcessIF ( void ) { }
};
//====================================
// Class SaXProcess...
//------------------------------------
class SaXProcess : public SaXProcessIF {
	private:
	QProcess*     mProc;

	private:
	void storeData     ( void );
	void storeDataSysp ( void );
	void storeDataCDB  ( int  );
	void storeDataSYS  ( int  );

	public:
	void start ( QList<char>, int = SAX_ISAX );
	void start ( int );

	public:
	SaXProcess ( void );
};

#endif