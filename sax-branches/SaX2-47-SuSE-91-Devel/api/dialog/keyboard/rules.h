/**************
FILE          : rules.h
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the XFree86 license
              :
DESCRIPTION   : header definition for rules.c
              : providing XKB file information
              :
              :
STATUS        : Status: Up-to-date
**************/
#ifndef RULES_H
#define RULES_H 1

#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>

class XKeyRules {
	public:
	XKeyRules (QString rule="xfree86");
	const QDict<char>& getModels  (void);
	const QDict<char>& getLayouts (void);
	const QDict<char>& getOptions (void);
	const QDict<const unsigned int>& getInitialGroup (void);
	
	public:
	const QDict<char>& getCurrentOptions (void);
	void parseVariants (const QStringList& vars, QDict<char>& variants);
	static QStringList rules (QString path = QString::null);
	QStringList getVariants (const QString& layout);

	private:
	QDict<char> mCurrent;
	QDict<char> mCurrentOptions;
	QDict<char> mModels;
	QDict<char> mLayouts;
	QDict<char> mOptions;
	QDict<const unsigned int> mInitialGroup;
	QDict<QStringList> mVarLists;

	protected:
	void loadRules(QString filename);
	void loadEncodings(QString filename);
};

#endif
