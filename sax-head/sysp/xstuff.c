/**************
FILE          : xstuff.c
***************
PROJECT       : SaX2 - SuSE advanced X configuration
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
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
	ddc      = 1;
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
	ddc      = 1;
	strcpy(card,cardnr);
	strcpy(cardopt,copt);
}

//======================================
// ScanXStuff: Enable DDC...
//--------------------------------------
void ScanXStuff::EnableDDC (void) {
	ddc = 1;
}

//======================================
// ScanXStuff: disable DDC...
//--------------------------------------
void ScanXStuff::DisableDDC (void) {
	ddc = 0;
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
	if ((m[i].bus == bus) && (m[i].slot == slot) && (m[i].func == func)) {
		return(i);
	}
	}
	return(-1); 
}

//======================================
// ScanXStuff: hw scan of StuffData
//--------------------------------------
void ScanXStuff::Scan (void) {
	MsgDetect display;
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
 
	display = MonitorGetData();
	VBEmem  = MemorySize();

	srvmsg.SetFile(SERVER_STUFF_DATA);
	server.SetFile(SERVER_DATA);
	// ...
	// if no server scan data is present, scan it
	// -------------------------------------------
	if (server.Read() < 0) {
		server.Scan();
	}
	// ...
	// read the server scan data and save it
	// to graphics...
	// ---------------
	for (int i = server.Count(); i > 0; i--) {
		graphics[card] = server.Pop();
		card++;
	}
	precard = card;

	// ...
	// it is not sure to get any server message data
	// this depend on some items which you can see in the
	// server.c code. If no server message data is present
	// we had to run a server and parse the messages. If
	// there is only one card in the system we will obtain
	// the information from the libhd without starting an
	// X-Server...
	// ------------
	if ((graphics.size() == 1) && (graphics[0].module != "vmware")) {
		parse[0].dtype  = display.displaytype;
		parse[0].model  = display.model;
		parse[0].vendor = display.vendor;
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
		// ...
		// the following values could not be detected via libhd
		// without starting an X-Server. Some defaults are set
		// now
		parse[0].dacspeed = 220;
		parse[0].hsmax    = 33;
		parse[0].vsmax    = 72;
		parse[0].chipset  = "<undefined>";
		parse[0].dpix     = 0;
		parse[0].dpiy     = 0;
		parse[0].vmdepth  = 0;

	} else {

		if (srvmsg.Read() < 0) {
			str config;
			sprintf(config,"%s-%d",TMP_CONFIG,getpid());
			map<int,string> section;
			ScanMouse mouse;
			ofstream handle(config);

			// open startup config...
			// -----------------------
			if (! handle) {
				cout << "ScanXStuff: could not create file: ";
				cout << TMP_CONFIG << endl;
				return;
			}
 
			// scan the mouse...
			// -------------------
			mouse.Scan(); MouseData mdata = mouse.Pop();
			srvmsg.SetMouseProperties(mdata.protocol,mdata.device);

			// create base sections...
			// ------------------------
			section[0] = srvmsg.DoFilesSection();
			section[1] = srvmsg.DoModuleSection();
			section[2] = srvmsg.DoInputDeviceSection();

			srvmsg.SetSectionID(card);
			section[3] = srvmsg.DoServerLayoutSection();

			// create dynamic sections...
			// ---------------------------
			for (int n=0;n<card;n++) {
			srvmsg.SetSectionID(n);
			if ((srvmsg.SetDriver(graphics[n].module)) == 1) {
				cout << "SaX: sorry could not open /dev/fb0... abort" << endl;
				exit(1);
			}
			srvmsg.SetBus (graphics[n].bus,graphics[n].slot,graphics[n].func);
			srvmsg.SetDeviceOption (graphics[n].option);

			section[4] = section[4] + "\n" + srvmsg.DoMonitorSection();
			section[5] = section[5] + "\n" + srvmsg.DoScreenSection();
			section[6] = section[6] + "\n" + srvmsg.DoDeviceSection();
			section[7] = section[7] + "\n" + srvmsg.DoServerFlagsSection();
			}

			// write sections to file...
			// ---------------------------
			handle << section[0] << endl;
			handle << section[1] << endl;
			handle << section[7] << endl;
			handle << section[2] << endl;
			handle << section[4] << endl;
			handle << section[5] << endl;
			handle << section[6] << endl;
			handle << section[3] << endl;

			handle.close();
			srvmsg.CallXF86Loader(config); 
			unlink(config);
		}
	}
	// ...
	// read the parse data from the X11 log
	// and save it to parse...
	// -------------------------
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
	// ...
	// lookup framebuffer timing for primary device
	// ---------------------------------------------
	ifstream handle(FB_DEV);
	FbData *fb = NULL;
	if (handle) {
		handle.close();
		fb = FbGetData();
		sprintf(fbstring,"\"%dx%d\" %.2f %s %s %s",
			fb->x,fb->y,fb->clock,fb->ht,fb->vt,fb->flags
		);
	}
	// ...
	// merge all the data. If only one card is detected
	// we will call the MonitorGetData() function to use
	// the VBE interface for getting more information 
	// via DDC[1/2]...
	// ----------------
	str current = "";
	str primary = "";
	for (int i=0;i<card;i++) {
		mapnr = FindParseData(parse,
			graphics[i].bus,graphics[i].slot,graphics[i].func
		); 
		sprintf(current,"%02d-%02d-%d",
		graphics[i].bus,graphics[i].slot,graphics[i].func);
		sprintf(primary,"%02d-%02d-%d",
		parse[mapnr].pbus,parse[mapnr].pslot,parse[mapnr].pfunc);  

		stuff[i].model  = "<undefined>";
		stuff[i].vendor = "<undefined>";
		if (parse[mapnr].model != "") {
			stuff[i].model = parse[mapnr].model;
		}
		if (parse[mapnr].vendor != "") {
			stuff[i].vendor = parse[mapnr].vendor;
		}
		stuff[i].vmdepth   = parse[mapnr].vmdepth;
		stuff[i].ddc       = parse[mapnr].ddc;
		stuff[i].dtype     = parse[mapnr].dtype;
		stuff[i].primary   = primary;
		stuff[i].chipset   = parse[mapnr].chipset;
		stuff[i].hsync     = parse[mapnr].hsmax; 
		stuff[i].vsync     = parse[mapnr].vsmax;
		stuff[i].dacspeed  = parse[mapnr].dacspeed;
		stuff[i].videoram  = parse[mapnr].videoram;
		stuff[i].current   = current; 
		stuff[i].raw       = graphics[i].raw;
		stuff[i].option    = graphics[i].option;
		stuff[i].extension = graphics[i].extension;
		if (TvSupport()) {
		if (stuff[i].extension == "") {
			stuff[i].extension = "v4l";
		} else {
			stuff[i].extension += ",v4l";
		}
		}
		stuff[i].driver    = graphics[i].module;
		stuff[i].dpix      = parse[mapnr].dpix;
		stuff[i].dpiy      = parse[mapnr].dpiy;

		if (
		(ddc == 1) && (
			(string(parse[i].ddc) != "<undefined>") || 
			(string(display.ddc)  != "00000000")
		)
		) {
			stuff[i].vesacount = parse[mapnr].modecount;
			stuff[i].vesa      = parse[mapnr].modes;
		} else {
			stuff[i].vesacount = -1;
			stuff[i].vesa[0].x = 640;
			stuff[i].vesa[0].y = 480;
			stuff[i].vesa[0].hsync = 25;
			stuff[i].vesa[0].vsync = 60;
		}

		// save fbtiming only for primary device...
		// -----------------------------------------
		if ((i==0) && (stuff[0].dtype == "CRT")) {
		if ((stuff[i].vesacount == -1) && (fb != NULL) && (ddc == 1)) {
			stuff[i].fbtiming  = fbstring;
			stuff[i].vesacount = 1;
			stuff[i].hsync     = (int)fb->hsync + 1;
			stuff[i].vsync     = (int)fb->vsync + 1;
			stuff[i].vesa[0].x = fb->x;
			stuff[i].vesa[0].y = fb->y;
			stuff[i].vesa[0].hsync = (int)fb->hsync;
			stuff[i].vesa[0].vsync = (int)fb->vsync;
			ddc = 0;
		} else if ((stuff[i].vesacount == -1) && (fb == NULL)) {
			stuff[i].vesacount = 1;
			stuff[i].vesa[0].x = 640;
			stuff[i].vesa[0].y = 480;
			stuff[i].vesa[0].hsync = 25;
			stuff[i].vesa[0].vsync = 60;
		}
		}
	}
	// ...
	// set the VBE DDC probed values if valid
	// --------------------------------------
	if (ddc == 1) {
	if (graphics.size() == 1) {
	if ((string(display.ddc) != "") && (string(display.ddc) != "00000000")) {
		stuff[0].ddc = display.ddc;
	}
	if (display.hsync_max > 0) { 
		stuff[0].hsync = display.hsync_max; 
	}
	if (display.vsync_max > 0) { 
		stuff[0].vsync = display.vsync_max; 
	}
	if ((display.dpix > 0) && (display.dpix < 100))     { 
		stuff[0].dpix  = display.dpix; 
	}
	if ((display.dpiy > 0) && (display.dpiy < 100))     { 
		stuff[0].dpiy  = display.dpiy; 
	}
	stuff[0].vesacount = display.vesacount;
	if (display.vesacount > 0) {
		int vesaCount = 0;
		for (int n=0;n<=display.vesacount;n++) {
		if (display.vmodes[n].x > 0) {
			XMode mode;
			mode.x = display.vmodes[n].x;
			mode.y = display.vmodes[n].y;
			// ...
			// check if the detected resolutions are part
			// of the following list
			// ---
			XMode vesaStandard[16];
			vesaStandard[0].x  = 640;  vesaStandard[0].y  = 480;
			vesaStandard[1].x  = 800;  vesaStandard[1].y  = 600;
			vesaStandard[2].x  = 1024; vesaStandard[2].y  = 768;
			vesaStandard[3].x  = 1152; vesaStandard[3].y  = 864;
			vesaStandard[4].x  = 1280; vesaStandard[4].y  = 768;
			vesaStandard[5].x  = 1280; vesaStandard[5].y  = 800;
			vesaStandard[6].x  = 1280; vesaStandard[6].y  = 960;
			vesaStandard[7].x  = 1280; vesaStandard[7].y  = 1024;
			vesaStandard[8].x  = 1440; vesaStandard[8].y  = 900;
			vesaStandard[9].x  = 1400; vesaStandard[9].y  = 1050;
			vesaStandard[10].x = 1600; vesaStandard[10].y = 1000;
			vesaStandard[11].x = 1600; vesaStandard[11].y = 1024;
			vesaStandard[12].x = 1600; vesaStandard[12].y = 1200;
			vesaStandard[13].x = 1680; vesaStandard[13].y = 1050;
			vesaStandard[14].x = 1900; vesaStandard[14].y = 1200;
			vesaStandard[15].x = 1920; vesaStandard[15].y = 1200;
	
			int isStandard = false;
			for (int i=0;i<16;i++) {
			if ((mode.x == vesaStandard[i].x)&&(mode.y == vesaStandard[i].y)) {
				isStandard = true;
				break;
			} 
			}
			if (! isStandard) {
				continue;
			}
			mode.hsync = display.vmodes[n].hsync;
			mode.vsync = display.vmodes[n].vsync;
			// ...
			// if mode has more than 100 Hz skip it
			// ---
			if (mode.vsync > 100) {
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
			if (mode.hsync > stuff[0].hsync) {
				stuff[0].hsync = mode.hsync;
			}
			if (mode.vsync > stuff[0].vsync) {
				stuff[0].vsync = mode.vsync;
			}
			stuff[0].vesa[vesaCount] = mode;
			vesaCount++;
			stuff[0].vesacount = vesaCount;
		}
		}
	}
	}
	}
	// ...
	// save the stuff result...
	// -------------------------
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
		strcpy(part1.ddc       , data.ddc.c_str());
		strcpy(part1.primary   , data.primary.c_str());
		strcpy(part1.chipset   , data.chipset.c_str());
		strcpy(part1.current   , data.current.c_str());
		strcpy(part1.raw       , data.raw.c_str());
		strcpy(part1.option    , data.option.c_str());
		strcpy(part1.extension , data.extension.c_str());
		strcpy(part1.driver    , data.driver.c_str());
		strcpy(part1.fbtiming  , data.fbtiming.c_str());
		strcpy(part1.dtype     , data.dtype.c_str());
		strcpy(part1.model     , data.model.c_str());
		strcpy(part1.vendor    , data.vendor.c_str());
		part1.vmdepth          = data.vmdepth;
		part1.hsync            = data.hsync;
		part1.vsync            = data.vsync;
		part1.vesacount        = data.vesacount;
		part1.dacspeed         = data.dacspeed;
		part1.videoram         = data.videoram;
		part1.dpix             = data.dpix;
		part1.dpiy             = data.dpiy;
		handle.write((char*)&part1,sizeof(part1));
		// ...
		// save the mode structs...
		// -------------------------
		for (int i=0;i<data.vesacount;i++) {
			XMode part2;
			part2.x     = data.vesa[i].x;
			part2.y     = data.vesa[i].y;
			part2.hsync = data.vesa[i].hsync;
			part2.vsync = data.vesa[i].vsync;
			handle.write((char*)&part2,sizeof(part2));
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
		data.ddc       = part1.ddc;
		data.dtype     = part1.dtype;
		data.model     = part1.model;
		data.vendor    = part1.vendor;
		data.primary   = part1.primary;
		data.chipset   = part1.chipset;
		data.hsync     = part1.hsync;
		data.vsync     = part1.vsync;
		data.vesacount = part1.vesacount;
		data.dacspeed  = part1.dacspeed;
		data.videoram  = part1.videoram;
		data.current   = part1.current;
		data.raw       = part1.raw;
		data.option    = part1.option;
		data.extension = part1.extension;
		data.driver    = part1.driver;
		data.fbtiming  = part1.fbtiming;
		data.dpix      = part1.dpix;
		data.dpiy      = part1.dpiy;
		data.vmdepth   = part1.vmdepth;
		// ...
		// read mode list...
		// -------------------
		for (int i=0;i<part1.vesacount;i++) {
			XMode part2;
			handle.read((char*)&part2,sizeof(part2));
			data.vesa[i] = part2;
		}
		if (handle.eof()) {
			break;
		}
		Push(data);
	}
	handle.close(); 
	return(0);
}
