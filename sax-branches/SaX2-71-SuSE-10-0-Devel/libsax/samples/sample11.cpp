#include "../sax.h"

int main (void) {
	SaXImportProfile* pProfile = new SaXImportProfile (
		"/usr/X11R6/lib/sax/profile/firegl"
	);
	pProfile -> doImport();
	SaXImport* mImport = pProfile -> getImport();
	QDict<QString> table = mImport -> getTable ( 0 );

	QDictIterator<QString> it (table);
	for (;it.current();++it) {
		printf ("%s : %s\n",it.currentKey().ascii(),it.current()->ascii());
	}
}
