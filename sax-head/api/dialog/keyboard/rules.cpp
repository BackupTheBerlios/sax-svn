#include <locale.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <libintl.h>
#include <qdir.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>
#include <X11/extensions/XKBgeom.h>
#include <X11/extensions/XKM.h>
#include <X11/extensions/XKBfile.h>
#include <X11/extensions/XKBui.h>

#include "rules.h"
#include "../../config.h"
#include "../../common/qx.h"

const QString X11_DIR( "/usr/X11R6/lib/X11/" );

//=========================================
// XKeyRules constructor
//-----------------------------------------
XKeyRules::XKeyRules (QString rule): mLayouts(80) {
	setlocale (LC_ALL,"");
	bindtextdomain ("sax", TDOMAIN);
	textdomain ("sax");
	loadRules (X11_DIR + QString("xkb/rules/%1").arg(rule));
	loadEncodings (X11_DIR + QString("locale/locale.alias"));
}

//=========================================
// loadRules
//-----------------------------------------
void XKeyRules::loadRules(QString file) {
	static struct {
		const char * locale;
		const char * layout;
	} fixedLayouts[] = {
		{ "ben", gettext("Bengali") },
		{ "ar" , gettext("Arabic")  },
		{ "ir" , gettext("Farsi")   },
		{ 0    , 0 }
	};

	char* locale = setlocale (LC_ALL,NULL);
	XkbRF_RulesPtr rules = XkbRF_Load (
		QFile::encodeName(file).data(),
		locale, true, true
	);
	if (! rules) {
		fprintf (stderr,"Unable to load rules");
		return;
	}

	int i;
	for (i = 0; i < rules->models.num_desc; ++i) {
		mModels.replace (
		rules->models.desc[i].name,
		gettext(qstrdup( rules->models.desc[i].desc))
		);
	}
	for (i = 0; i < rules->layouts.num_desc; ++i) {
		mLayouts.replace (
		rules->layouts.desc[i].name,
		gettext(qstrdup( rules->layouts.desc[i].desc))
		);
	}
	for (i = 0; i < rules->options.num_desc; ++i) {
		mOptions.replace (
		rules->options.desc[i].name,
		gettext(qstrdup( rules->options.desc[i].desc))
		);
	}
	XkbRF_Free(rules, true);
	// ...
	// Knowing which variants we have won't help us
	// We want to know which particular variants are available for
	// each language
	// ---
	for(int i=0; fixedLayouts[i].layout != 0; i++ ) {
		mLayouts.replace (fixedLayouts[i].locale, fixedLayouts[i].layout);
	}
}

//=========================================
// loadEncodings
//-----------------------------------------
void XKeyRules::loadEncodings (QString) {
	// some handcoded ones, because the X11 locale file
	// doesn't get them correctly, or in case the locale file
	// wasn't found
	static struct {
		const char * locale;
		const char * encoding;
		unsigned int initialGroup;
	} encs[] = {
		{ "ar", "ISO8859-6", 1 },
		{ "be", "ISO8859-1", 0 },
		{ "ben", "ISO10646-1", 0 },
		{ "br", "ISO8859-1", 0 },
		{ "bg", "ISO8859-5", 1 },
		{ "by", "CP 1251", 1 },
		{ "ca", "ISO8859-1", 0 },
		{ "cs", "ISO8859-2", 0 },
		{ "cz", "ISO8859-2", 0 },
		{ "de", "ISO8859-1", 0 },
		{ "de_CH", "ISO8859-1", 0 },
		{ "dev", "ISO8859-1", 1 },
		{ "dk", "ISO8859-1", 0 },
		{ "ee", "ISO8859-15", 0 },
		{ "en_US", "ISO8859-1", 0 },
		{ "el", "ISO8859-7", 1 },
		{ "es", "ISO8859-1", 0 },
		{ "eo", "ISO8859-3", 0 },
		{ "fi", "ISO8859-1", 0 },
		{ "fr", "ISO8859-1", 0 },
		{ "fr_CH", "ISO8859-1", 0 },
		{ "he", "ISO8859-8-i", 1 },
		{ "hu", "ISO8859-2", 0 },
		{ "hr", "ISO8859-2", 0 },
		{ "il", "ISO8859-8-i", 1 },
		{ "ir", "UTF-8", 1 },
		{ "it", "ISO8859-1", 0 },
		{ "kl", "ISO8859-1", 0 },
		{ "lt", "ISO8859-13", 1 },
		{ "lv", "ISO8859-13", 0 },
		{ "mk", "ISO8859-5", 1 },
		{ "nl", "ISO8859-1", 0 },
		{ "no", "ISO8859-1", 0 },
		{ "pl", "ISO8859-2", 0 },
		{ "pt", "ISO8859-1", 0 },
		{ "ro", "ISO8859-2", 0 },
		{ "ru", "KOI8-R", 1 },
		{ "ru_UA", "KOI8-U", 0 },
		{ "se", "ISO8859-1", 0 },
		{ "sk", "ISO8859-2", 0 },
		{ "th", "ISO8859-11", 1 },
		{ "us", "ISO8859-1", 0 },
		{ "ua", "KOI8-U", 1 },
		{ 0, 0, 0 }
	};
	int i = 0;
	while ( encs[i].encoding != 0 ) {
		mInitialGroup.insert(encs[i].locale, &encs[i].initialGroup);
		i++;
	}
}

//=========================================
// rules
//-----------------------------------------
QStringList XKeyRules::rules(QString path) {
	QStringList result;

	if (path.isEmpty()) {
		path = X11_DIR + "xkb/rules";
	}
	QDir dir(path);
	dir.setFilter(QDir::Files);
	QStringList list = dir.entryList();
	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
	if ((*it).right(4) != ".lst") {
		result << *it;
	}
	}
	return result;
}

//=========================================
// getVariants
//-----------------------------------------
QStringList XKeyRules::getVariants(const QString& layout) {
	if ( layout.isEmpty() || ! getLayouts().find(layout) ) {
		return QStringList();
	}
	QStringList* result1 = mVarLists[layout];
	if( result1 ) {
		return *result1;
	}
	QStringList* result = new QStringList();

	QString file = X11_DIR + "xkb/symbols/" + layout;
	QFile f(file);
	if (f.open(IO_ReadOnly)) {
		QTextStream ts(&f);

		QString line;
		QString prev_line;

		while (!ts.eof()) {
			prev_line = line;
			line = ts.readLine().simplifyWhiteSpace();

			if (line[0] == '#' || line.left(2) == "//" || line.isEmpty()) {
				continue;
			}
			int pos = line.find("xkb_symbols");
			if (pos < 0) {
				continue;
			}
			if ( prev_line.find("hidden") >=0 ) {
				continue;
			}
			pos = line.find('"', pos) + 1;
			int pos2 = line.find('"', pos);
			if ( pos < 0 || pos2 < 0 ) {
				continue;
			}
			QString* translation = new QString (line.mid(pos, pos2-pos));
			char* transtable = gettext( translation->ascii() );
			result->append (
				QString::fromLocal8Bit ( transtable )
			);
		}

		f.close();
	}
	mVarLists.insert(layout, result);
	return *result;
}

//=========================================
// parseVariants
//-----------------------------------------
void XKeyRules::parseVariants (
	const QStringList& vars, QDict<char>& variants
) {
	static const char* LAYOUT_PATTERN = "[a-z0-9_]*";
	static const char* VARIANT_PATTERN = "\\([a-z0-9_]*\\)";
	for (QStringList::ConstIterator it = vars.begin(); it != vars.end(); ++it) {
		QString varLine = (*it).stripWhiteSpace();
		QRegExp rx(LAYOUT_PATTERN);
		int pos = rx.search(varLine, 0);
		int len = rx.matchedLength();
		if ( pos < 0 || len < 2 ) {
			continue;
		}
		QString layout = varLine.mid(pos, len);
		rx.setPattern(VARIANT_PATTERN);
		pos = rx.search(varLine, pos+len);
		len = rx.matchedLength();
		if ( pos < 2 || len < 3 ) {
			continue;
		}
		QString variant_ = varLine.mid(pos+1, len-2);
		QStringList addVars = getVariants(layout);
		if ( !variant_.isEmpty() && addVars.contains(variant_) ) {
			variants.insert(layout, strdup(variant_.latin1()));
		}
	}
}

const QDict<char>& XKeyRules::getModels  (void) {
	return mModels;
}
const QDict<char>& XKeyRules::getLayouts (void) {
	return mLayouts;
}
const QDict<char>& XKeyRules::getOptions (void) {
	return mOptions;
}
const QDict<const unsigned int>& XKeyRules::getInitialGroup (void) {
	return mInitialGroup;
}
const QDict<char>& XKeyRules::getCurrentOptions (void) {
	return mCurrentOptions;
}
