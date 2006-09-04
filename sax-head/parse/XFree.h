#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "xf86Parser.h"

extern XF86ConfigPtr ReadConfigFile (char *filename);

/* Files Section... */
extern char* GetModulePath (XF86ConfigPtr conf);
extern char* GetFontPath (XF86ConfigPtr conf);
extern char* GetRgbPath (XF86ConfigPtr conf);
extern char* GetLogFile (XF86ConfigPtr conf);

/* Module Section... */
extern char* GetModuleSpecs (XF86ConfigPtr conf);
extern char* GetModuleSubSpecs (XF86ConfigPtr conf);

/* InputDevice Section... */
extern char* GetInputSpecs (XF86ConfigPtr conf);

/* ServerFlags Section... */
extern char* GetFlags (XF86ConfigPtr conf);

/* Extensions Section... */
extern char* GetExtensions (XF86ConfigPtr conf);

/* ServerLayout Section... */
extern char* GetLayoutSpecs (XF86ConfigPtr conf);

/* Device Section... */
extern char* GetDeviceSpecs (XF86ConfigPtr conf);

/* Monitor Section... */
extern char* GetMonitorSpecs (XF86ConfigPtr conf);
extern char* GetModesSpecs   (XF86ConfigPtr conf);

/* Screen Section... */
extern char* GetDisplaySpecs (XF86ConfigPtr conf);


