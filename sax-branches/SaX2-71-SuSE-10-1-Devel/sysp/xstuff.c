/**************
FILE          : xstuff.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Sch�fer <ms@suse.de>
              :
BELONGS TO    : configuration tool for the X window system 
              : released under the GPL license
              :
DESCRIPTION   : server Stuff detection code 
              : 
              :
STATUS        : Status: Up-to-date
**************/

#include "server.h"
#include "config.h"
#include "cfg.h"
#include "mouse.h"
#include "xstuff.h"

//======================================
// ScanXStuff: constructor...
//--------------------------------------
ScanXStuff::ScanXStuff (void) {
	question = -1;
	withx    = -1;
	elements = 0;
	current  = 0;
	strcpy(card,"all");
	strcpy(cardopt,"");
}

//======================================
// ScanXStuff: constructor...
//--------------------------------------
ScanXStuff::ScanXStuff (int ask,int xavailable,str cardnr,str copt) {
	question = ask;
	withx    = xavailable;
	elements = 0;
	current  = 0;
	strcpy(card,cardnr);
	strcpy(cardopt,copt);
}

//======================================
// ScanXStuff: set file name for save
//--------------------------------------
void ScanXStuff::SetFile (str name) {
	file = name;
}

//======================================
// ScanXStuff: reset map counter
//--------------------------------------
void ScanXStuff::Reset (void) {
	current = 0;
}

//======================================
// ScanXStuff: find map nr in ParseData
//--------------------------------------
int ScanXStuff::
FindParseData (map<int,ParseData> m,int bus,int slot,int func) {
	for (int i=0;i<(int)m.size();i++) {
		if ((m[i].pbus==bus) && (m[i].pslot==slot) && (m[i].pfunc==func)) {
			return(i);
		}
		if ((m[i].bus==bus) && (m[i].slot==slot) && (m[i].func==func)) {
			return(i);
		}
	}
	return(-1); 
}

//======================================
// ScanXStuff: hw scan of StuffData
//--------------------------------------
void ScanXStuff::Scan (void) {
	MsgDetect* display;
	unsigned VBEmem;
	ScanServer server(question,withx,card,cardopt);
	XF86ConfigFile srvmsg;
	map<int,ParseData>  parse;
	map<int,ServerData> graphics;
	map<int,StuffData>  stuff;
	str fbstring = "";
	int mapnr    = 0;
	int precard  = 0;
	int card     = 0;

	//======================================
	// Prepare Xstuff Scan
	//-------------------------------------- 
	display = MonitorGetData();
	VBEmem  = MemorySize();
	srvmsg.SetFile(SERVER_STUFF_DATA);
	server.SetFile(SERVER_DATA);

	//======================================
	// retrieve VESA BIOS version
	//--------------------------------------
	char* vbios = vesaBIOS();

	//======================================
	// lookup framebuffer timing
	//--------------------------------------
	ifstream handle (FB_DEV);
	FbData *fb = NULL;
	if (handle) {
		handle.close();
		fb = FbGetData();
		if ((fb) && (fb->x >= 800) && (fb->y >= 600)) {
			sprintf(fbstring,"\"%dx%d\" %.2f %s %s %s",
				fb->x,fb->y,fb->clock,fb->ht,fb->vt,fb->flags
			);
		} else {
			fb = NULL;
		}
	}
	//======================================
	// call -s server of not scanned
	//--------------------------------------
	if (server.Read() < 0) {
		server.Scan();
	}
	//======================================
	// read the server scan data -> graphics
	//--------------------------------------
	for (int i = server.Count(); i > 0; i--) {
		graphics[card] = server.Pop();
		card++;
	}
	precard = card;

	// .../
	// it is not sure to get any server message data this depend on
	// some items which you can see in the server.c code. If no server
	// message data is present we had to run a server and parse the messages.
	// If there is only one card in the system we will obtain the information
	// from the libhd without starting an X-Server...
	// ----
	if ((graphics.size() == 1) && (graphics[0].module != "vmware")) {
		//======================================
		// use libhd data to setup parse
		//--------------------------------------
		if (display) {
			parse[0].dtype  = display->displaytype;
		}
		if (display) {
			parse[0].model  = display->model;
		}
		if (display) {
			parse[0].vendor = display->vendor;
		}
		parse[0].bus    = graphics[0].bus;
		parse[0].slot   = graphics[0].slot;
		parse[0].func   = graphics[0].func;
		parse[0].pbus   = graphics[0].bus;
		parse[0].pslot  = graphics[0].slot;
		parse[0].pfunc  = graphics[0].func;
		parse[0].ddc    = "<undefined>";
		if (VBEmem > 0) {
			parse[0].videoram = VBEmem;
		} else {
			parse[0].videoram = 4096;
		}
		// .../
		// the following values could not be detected via libhd
		// or are currently not part of the parse map. Setting
		// defaults for this values
		// ----
		parse[0].modecount = 0;
		parse[0].dacspeed  = 220;
		parse[0].hsmax     = 40;
		parse[0].vsmax     = 75;
		parse[0].chipset   = "<undefined>";
		parse[0].dpix      = 0;
		parse[0].dpiy      = 0;
		parse[0].vmdepth   = 0;
	} else {
		//======================================
		// call a server to setup parse
		//--------------------------------------
		if (srvmsg.Read() < 0) {
			str config;
			sprintf(config,"%s-%d",TMP_CONFIG,getpid());
			map<int,string> section;
			ScanMouse mouse;
			ofstream handle(config);
			//======================================
			// open startup config...
			//--------------------------------------
			if (! handle) {
				cout << "ScanXStuff: could not create file: ";
				cout << TMP_CONFIG << endl;
				return;
			}
 			//======================================
			// scan the mouse...
			//--------------------------------------
			mouse.Scan(); MouseData mdata = mouse.Pop();
			srvmsg.SetMouseProperties(mdata.protocol,mdata.device);
			//======================================
			// create base sections...
			//--------------------------------------
			section[0] = srvmsg.DoFilesSection();
			section[1] = srvmsg.DoModuleSection();
			section[2] = srvmsg.DoInputDeviceSection();

			srvmsg.SetSectionID(card);
			section[3] = srvmsg.DoServerLayoutSection();
			//======================================
			// create dynamic sections...
			//--------------------------------------
			for (int n=0;n<card;n++) {
				srvmsg.SetSectionID(n);
				if ((srvmsg.SetDriver(graphics[n].module)) == 1) {
					cout << "SaX: sorry could not open /dev/fb0... abort\n";
					exit(1);
				}
				srvmsg.SetBus (
					graphics[n].domain,graphics[n].bus,
					graphics[n].slot,graphics[n].func
				);
				srvmsg.SetDeviceOption (graphics[n].option);
				section[4] = section[4] + "\n" + srvmsg.DoMonitorSection();
				section[5] = section[5] + "\n" + srvmsg.DoScreenSection();
				section[6] = section[6] + "\n" + srvmsg.DoDeviceSection();
				section[7] = section[7] + "\n" + srvmsg.DoServerFlagsSection();
			}
			//======================================
			// write sections to file...
			//--------------------------------------
			handle << section[0] << endl;
			handle << section[1] << endl;
			handle << section[7] << endl;
			handle << section[2] << endl;
			handle << section[4] << endl;
			handle << section[5] << endl;
			handle << section[6] << endl;
			handle << section[3] << endl;
			//======================================
			// Call server creating a log file
			//--------------------------------------
			handle.close();
			srvmsg.CallXF86Loader(config); 
			unlink(config);
		}
	}
	//======================================
	// parse data from the X11 log -> parse
	//--------------------------------------
	if ((graphics.size() > 1) || (graphics[0].module == "vmware")) {
		card = 0;
		for (int i = srvmsg.Count(); i > 0; i--) {
			parse[card] = srvmsg.Pop();
			card++;
		}
	}
	if (precard > card) {
		cout << "SaX: ups lost card during probing... abort" << endl;
		exit(1);
	}
	// .../
	// merge the parse and the display data maps into
	// one result map named -> stuff. 
	// ----
	str current = "";
	str primary = "";
	for (int i=0;i<card;i++) {
		//======================================
		// Find card in the card database
		//--------------------------------------
		mapnr = FindParseData(parse,
			graphics[i].bus,graphics[i].slot,graphics[i].func
		);
		//======================================
		// Save PCI BusID
		//-------------------------------------- 
		sprintf (current,"%02d-%02d-%d",
			graphics[i].bus,graphics[i].slot,graphics[i].func
		);
		sprintf(primary,"%02d-%02d-%d",
			parse[mapnr].pbus,parse[mapnr].pslot,parse[mapnr].pfunc
		);
		//======================================
		// Save VBE port number -> 1
		//--------------------------------------
		stuff[i].port = 1;
		//======================================
		// Save vbios version
		//--------------------------------------
		stuff[i].vbios = "<undefined>";
		if (vbios) {
			stuff[i].vbios = vbios;
		}
		for (int p=0;p<4;p++) {
			//======================================
			// Save model and vendor from parse
			//--------------------------------------
			stuff[i].model[p]  = "<undefined>";
			stuff[i].vendor[p] = "<undefined>";
			if (parse[mapnr].model != "") {
				stuff[i].model[p] = parse[mapnr].model;
			}
			if (parse[mapnr].vendor != "") {
				stuff[i].vendor[p] = parse[mapnr].vendor;
			}
			//======================================
			// Save DisplayType and DDCID from parse
			//--------------------------------------
			stuff[i].dtype[p]  = parse[mapnr].dtype;
			stuff[i].ddc[p]    = parse[mapnr].ddc;
			//======================================
			// Save hsync/vsync ranges from parse
			//--------------------------------------
			stuff[i].hsync[p]  = parse[mapnr].hsmax;
			stuff[i].vsync[p]  = parse[mapnr].vsmax;
			//======================================
			// Save DisplaySize from parse
			//--------------------------------------
			stuff[i].dpix[p]   = parse[mapnr].dpix;
			stuff[i].dpiy[p]   = parse[mapnr].dpiy;
			//======================================
			// Save modes from parse
			//--------------------------------------
			if (parse[mapnr].modecount) {
				//======================================
				// Found vesa modes -> use it
				//--------------------------------------
				stuff[i].vesacount[p] = parse[mapnr].modecount;
				stuff[i].vesa[p]      = parse[mapnr].modes;
			}
		}
		//======================================
		// Save standard values from parse
		//--------------------------------------
		stuff[i].vmdepth   = parse[mapnr].vmdepth;
		stuff[i].chipset   = parse[mapnr].chipset;
		stuff[i].dacspeed  = parse[mapnr].dacspeed;
		stuff[i].videoram  = parse[mapnr].videoram;
		stuff[i].raw       = graphics[i].raw;
		stuff[i].option    = graphics[i].option;
		stuff[i].extension = graphics[i].extension;
		stuff[i].driver    = graphics[i].module;
		stuff[i].primary   = primary;
		stuff[i].current   = current;
		//======================================
		// Save v4l extension if TV card exists
		//--------------------------------------
		if (TvSupport()) {
		if (stuff[i].extension == "") {
			stuff[i].extension = "v4l";
		} else {
			stuff[i].extension += ",v4l";
		}
		}
	}
	// .../
	// set the VBE DDC probed values from libhd
	// the display map may provides multiple DDC information
	// per port.
	// ----
	for (MsgDetect* dpy=display; dpy; dpy=dpy->next) {
		//======================================
		// Get current port number
		//--------------------------------------
		int p = dpy->port;
		stuff[0].port = p + 1;
		//======================================
		// Save DDC id and Type per port
		//--------------------------------------
		if ((string(dpy->ddc) != "") && (string(dpy->ddc) != "00000000")) {
			stuff[0].ddc[p]   = dpy->ddc;
			stuff[0].dtype[p] = dpy->displaytype;
		} else {
			strcpy (dpy->ddc,"<undefined>");
		}
		//======================================
		// Save DisplaySize per port
		//--------------------------------------
		if (
			((dpy->dpix > 0) && (dpy->dpix < 100)) &&
			((dpy->dpiy > 0) && (dpy->dpiy < 100))
		) {
			stuff[0].dpix[p]  = dpy->dpix;
			stuff[0].dpiy[p]  = dpy->dpiy;
			stuff[0].ddc[p]   = dpy->ddc;
		}
		//======================================
		// Save hsync/vsync ranges per port
		//--------------------------------------
		if (dpy->hsync_max) { 
			stuff[0].hsync[p] = dpy->hsync_max;
		}
		if (dpy->vsync_max) {
			stuff[0].vsync[p] = dpy->vsync_max;
		}
		//======================================
		// Save vesa modes per port
		//--------------------------------------
		if ((dpy->vesacount) && (string(dpy->vendor) != "Generic")) {
			stuff[0].vesacount[p] = dpy->vesacount;
			int vesaCount = 0;
			for (int n=0;n<dpy->vesacount;n++) {
			if (dpy->vmodes[n].x > 0) {
				XMode mode;
				mode.x = dpy->vmodes[n].x;
				mode.y = dpy->vmodes[n].y;
				// ...
				// check if the detected resolutions are part
				// of the following list
				// ---
				XMode vesaStandard[17];
				vesaStandard[0].x  = 800;  vesaStandard[0].y  = 600;
				vesaStandard[1].x  = 1024; vesaStandard[1].y  = 768;
				vesaStandard[2].x  = 1152; vesaStandard[2].y  = 864;
				vesaStandard[3].x  = 1280; vesaStandard[3].y  = 768;
				vesaStandard[4].x  = 1280; vesaStandard[4].y  = 800;
				vesaStandard[5].x  = 1280; vesaStandard[5].y  = 960;
				vesaStandard[6].x  = 1280; vesaStandard[6].y  = 1024;
				vesaStandard[7].x  = 1440; vesaStandard[7].y  = 900;
				vesaStandard[8].x  = 1400; vesaStandard[8].y  = 1050;
				vesaStandard[9].x  = 1600; vesaStandard[9].y  = 1000;
				vesaStandard[10].x = 1600; vesaStandard[10].y = 1024;
				vesaStandard[11].x = 1600; vesaStandard[11].y = 1200;
				vesaStandard[12].x = 1680; vesaStandard[12].y = 1050;
				vesaStandard[13].x = 1900; vesaStandard[13].y = 1200;
				vesaStandard[14].x = 1920; vesaStandard[14].y = 1200;
				vesaStandard[15].x = 1024; vesaStandard[15].y = 600;
				vesaStandard[16].x = 1024; vesaStandard[16].y = 576;
			
				int isStandard = false;
				for (int i=0;i<17;i++) {
				if ((mode.x==vesaStandard[i].x)&&(mode.y==vesaStandard[i].y)) {
					isStandard = true;
					break;
				} 
				}
				if (! isStandard) {
					continue;
				}
				mode.hsync = dpy->vmodes[n].hsync;
				mode.vsync = dpy->vmodes[n].vsync;
				// ...
				// if mode has more than 100 Hz skip it
				// if mode is <= 0 Hz skip it
				// ---
				if ((mode.vsync > 100) || (mode.vsync <= 0)) {
					continue;
				}
				// ...
				// if mode has no hsync specification, estimate it
				// from the vsync value
				// ---
				if (mode.hsync <= 0) {
					mode.hsync = (int)((mode.vsync * mode.x) / 1224);
				}
				// ...
				// if mode hsync or vsync is greater than currently
				// used max hsync/vsync, overwrite it
				// ---
				if (mode.hsync > stuff[0].hsync[p]) {
					stuff[0].hsync[p] = mode.hsync;
				}
				if (mode.vsync > stuff[0].vsync[p]) {
					stuff[0].vsync[p] = mode.vsync;
				}
				stuff[0].vesa[p][vesaCount] = mode;
				vesaCount++;
				stuff[0].vesacount[p] = vesaCount;
			}
			}
			//======================================
			// Sort vesa resolutions
			//--------------------------------------
			for (int n=0;n<vesaCount;n++) {
			for (int l=n;l<vesaCount;l++) {
				XMode mode_n = stuff[0].vesa[p][n];
				XMode mode_l = stuff[0].vesa[p][l];
				if (mode_n.x > mode_l.x) {
					XMode save = mode_n;
					stuff[0].vesa[p][n] = mode_l;
					stuff[0].vesa[p][l] = save;
				}
			}
			}
			//======================================
			// set basic hsync/vsync if not set
			//--------------------------------------
			if ((! dpy->hsync_max) || (! dpy->vsync_max)) {
				stuff[0].hsync[p] = stuff[0].vesa[p][vesaCount-1].hsync;
				stuff[0].vsync[p] = stuff[0].vesa[p][vesaCount-1].vsync;
			}
			//======================================
			// Save Vendor and Model name per port
			//--------------------------------------
			stuff[0].model[p]  = dpy->model;
			stuff[0].vendor[p] = dpy->vendor;
		} else {
			//======================================
			// No DDC modes found check framebuffer
			//--------------------------------------
			if (fb != NULL) {
				//======================================
				// Use FbDev Timing
				//--------------------------------------
				stuff[0].fbtiming     = fbstring;
				stuff[0].vesacount[p] = 1;
				stuff[0].hsync[p]     = (int)fb->hsync + 1;
				stuff[0].vsync[p]     = (int)fb->vsync + 1;
				stuff[0].vesa[p][0].x = fb->x;
				stuff[0].vesa[p][0].y = fb->y;
				stuff[0].vesa[p][0].hsync = (int)fb->hsync;
				stuff[0].vesa[p][0].vsync = (int)fb->vsync;
			} else {
				//======================================
				// No modes set default
				//--------------------------------------
				stuff[0].vesacount[p] = 1;
				stuff[0].vesa[p][0].x = 800;
				stuff[0].vesa[p][0].y = 600;
				stuff[0].vesa[p][0].hsync = 38;
				stuff[0].vesa[p][0].vsync = 60;
			}
		}
	}
	//======================================
	// save the stuff result...
	//--------------------------------------
	for (int i=0;i<card;i++) {
		Push(stuff[i]);
	}
}

//======================================
// ScanXStuff: push StuffData struct
//--------------------------------------
void ScanXStuff::Push (StuffData e) {
	qX[elements] = e;
	elements++;
}

//======================================
// ScanXStuff: get StuffData struct
//--------------------------------------
StuffData ScanXStuff::Pop (void) {
	if (current >= elements) {
		throw ReadLastElement();
	}
	StuffData element(qX[current]);
	current++;
	return element;
}

//======================================
// ScanXStuff: save data to file
//--------------------------------------
int ScanXStuff::Save (void) {
	ofstream handle(file.c_str(),ios::binary);
	if (! handle) {
		cout << "ScanXStuff: could not create file: ";
		cout << file << endl;
		return(-1);
	}
	Reset();
	for (int i = Count(); i > 0; i--) {
		StuffData data = Pop();
		StuffData_S part1;
		// ...
		// save the base data...
		// ----------------------
		for (int n=0;n<data.port;n++) {
			strcpy(part1.ddc[n]   , data.ddc[n].c_str());
			strcpy(part1.dtype[n] , data.dtype[n].c_str());
			strcpy(part1.model[n] , data.model[n].c_str());
			strcpy(part1.vendor[n], data.vendor[n].c_str());
			part1.hsync[n]       = data.hsync[n];
			part1.vsync[n]       = data.vsync[n];
			part1.vesacount[n]   = data.vesacount[n];
			part1.dpix[n]        = data.dpix[n];
			part1.dpiy[n]        = data.dpiy[n];
		}
		strcpy(part1.primary   , data.primary.c_str());
		strcpy(part1.chipset   , data.chipset.c_str());
		strcpy(part1.current   , data.current.c_str());
		strcpy(part1.raw       , data.raw.c_str());
		strcpy(part1.option    , data.option.c_str());
		strcpy(part1.extension , data.extension.c_str());
		strcpy(part1.driver    , data.driver.c_str());
		strcpy(part1.fbtiming  , data.fbtiming.c_str());
		strcpy(part1.vbios     , data.vbios.c_str());
		part1.port             = data.port;
		part1.vmdepth          = data.vmdepth;
		part1.dacspeed         = data.dacspeed;
		part1.videoram         = data.videoram;
		handle.write((char*)&part1,sizeof(part1));
		// ...
		// save the mode structs...
		// -------------------------
		for (int n=0;n<data.port;n++) {
			for (int i=0;i<data.vesacount[n];i++) {
				XMode part2;
				part2.x     = data.vesa[n][i].x;
				part2.y     = data.vesa[n][i].y;
				part2.hsync = data.vesa[n][i].hsync;
				part2.vsync = data.vesa[n][i].vsync;
				handle.write((char*)&part2,sizeof(part2));
			}
		}
	}
	handle.close();
	return(0);
}

//======================================
// ScanXStuff: retrieve data from file
//--------------------------------------
int ScanXStuff::Read (void) {
	ifstream handle(file.c_str(),ios::binary); 
	if (! handle) {
		//cout << "ScanXStuff: could not open file: "
		//cout << file << endl;
		return(-1);
	}
	elements = 0;
	current  = 0;
	qX.clear();

	while(1) {
		StuffData data;
		StuffData_S part1;
		// ...
		// read base data...
		// -------------------
		handle.read((char*)&part1,sizeof(part1));
		for (int n=0;n<part1.port;n++) {
			data.ddc[n]       = part1.ddc[n];
			data.dtype[n]     = part1.dtype[n];
			data.hsync[n]     = part1.hsync[n];
			data.vsync[n]     = part1.vsync[n];
			data.vesacount[n] = part1.vesacount[n];
			data.dpix[n]      = part1.dpix[n];
			data.dpiy[n]      = part1.dpiy[n];
			data.model[n]     = part1.model[n];
			data.vendor[n]    = part1.vendor[n];
		}
		data.vbios     = part1.vbios;
		data.primary   = part1.primary;
		data.chipset   = part1.chipset;
		data.dacspeed  = part1.dacspeed;
		data.videoram  = part1.videoram;
		data.current   = part1.current;
		data.raw       = part1.raw;
		data.option    = part1.option;
		data.extension = part1.extension;
		data.driver    = part1.driver;
		data.fbtiming  = part1.fbtiming;
		data.vmdepth   = part1.vmdepth;
		data.port      = part1.port;
		// ...
		// read mode list...
		// -------------------
		for (int n=0;n<data.port;n++) {
			for (int i=0;i<part1.vesacount[n];i++) {
				XMode part2;
				handle.read((char*)&part2,sizeof(part2));
				data.vesa[n][i] = part2;
			}
		}
		if (handle.eof()) {
			break;
		}
		Push(data);
	}
	handle.close(); 
	return(0);
}
