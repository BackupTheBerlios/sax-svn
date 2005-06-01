#include "../sax.h"

int main (void) {
	SaXException().setDebug (true);
	QDict<SaXImport> section;
	int importID[] = {
		SAX_CARD,
		SAX_DESKTOP,
		SAX_PATH,
	};
	printf ("Importing data...\n");
	SaXConfig* config = new SaXConfig;
	for (int id=0; id<3; id++) {
		SaXImport* import = new SaXImport ( importID[id] );
		import -> setSource ( SAX_SYSTEM_CONFIG );
		import -> doImport();
		config -> addImport (import);
		section.insert (
			import->getSectionName(),import
		);
	}
	printf ("Setting up resolution...\n");
	SaXManipulateDesktop mDesktop (
		section["Desktop"],section["Card"],section["Path"]
	);
	if (mDesktop.selectDesktop (0)) {
		mDesktop.addResolution (24,1600,1200);
	}
	printf ("Writing configuration\n");
	config -> setMode (SAX_MERGE);
	if ( ! config -> createConfiguration() ) {
		printf ("%s\n",config->errorString().ascii());
		printf ("%s\n",config->getParseErrorValue().ascii());
		return 1;
	}
	return 0;
}
