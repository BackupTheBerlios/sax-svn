#!/usr/bin/perl
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2000
# init.pl configuration level 1
#
# CVS ID:
# --------
# Status: Up-to-date
#
use lib   '/usr/share/sax/modules';
use strict;
use Env;

use Getopt::Long;
use Storable;
use CheckServer;
use AutoDetect;
use HashMap;
use FileHandle;
use Term::ReadLine;

no strict "subs";
no strict "refs";

#==================================================
# this is the global var hash which build the 
# fundamental basics of the whole configuration
#--------------------------------------------------
my %var;                # hold configuration parameter
my %spec;               # hold specifications for init.pl
my $StartBatchMode;     # option variable: start batch mode y/n
my $CreateVESAProfile;  # create user profile according to -vesa option
my $IgnoreProfile;      # ignore all automatically added profiles
my $Debug;              # option variable: debugging
my $Virtual;            # option variable: virtual low resolution
my $ModuleList;         # option variable: module(s) to use   
my $AutoMode;           # option variable: auto modeline
my $UseDbmNew;          # use the DbmNew file instead of DbmFile
my $CardNumber;         # option variable: card to use
my $UseGPM;             # option variable: use gpm as repeater
my $MouseDevice;        # option variable: core pointer device
my $MouseProtocol;      # option variable: core pointer protocol
my @ProfileAddSections; # profile add section list
my @ProfileData;        # profile data
my $ProfileSize;        # size of profile
my $AbortBatch;         # abort batch mode
my $ViewValue;          # Tree view
my $ViewRef;            # Tree view reference
my $haveServer;         # Is there a server for later access
my $Suggest;            # create ycp suggestion files
my $Quiet;              # answer questions with [yes|no] [3D...]
my $HWScanNeeded = 1;   # indicate if scan(),detect() call is needed
my $logHandle;          # log handler
my $D3Answer = "no";    # answer to the 3D question
my $dpy      = ":0.0";  # display to use

our %mop;               # monitor profile hash
our %idp;               # input device profile hash
our %xdp;               # X11 driver profile hash

init();
#==================================================
# next we will start the hardware detection 
# (level 1) with the following tasks:
#
# 0 -> get options and test for privileges
# 1 -> set default configuration hash elements
# 2 -> start the HW-Scan
# 3 -> handle the scanned information (detect)
# 4 -> handle the given options and the
#      profile data
# 5 -> save the configuration hash
#
#--------------------------------------------------
#---[ init ]----#
sub init {
	my $depth;   # supported depths
	my $scr;     # screen list
	my $line;    # read line buffer
	my $param;   # gpm parameter
	my @plist;   # pointer list
	my @list;    # raw pointer list 
	my $m;       # helper variable
	my %tmpvar;  # tmp hash

	# /.../
	# get options... 
	# ---------------
	my $result = GetOptions(
		"module|m=s"        => \$ModuleList,
		"chip|c=s"          => \$CardNumber,             
		"batch|b:s"         => \$StartBatchMode,
		"vesa=s"            => \$CreateVESAProfile,
		"ignoreprofile|i"   => \$IgnoreProfile,
		"debug|d"           => \$Debug,
		"virtual|v"         => \$Virtual,
		"automode|u"        => \$AutoMode,
		"dbmnew"            => \$UseDbmNew,
		"type|t=s"          => \$MouseProtocol,
		"node|n=s"          => \$MouseDevice,
		"suggest"           => \$Suggest,
		"quiet|q=s"         => \$Quiet,
		"gpm|g"             => \$UseGPM,
		"help|h"            => \&usage,
		"<>"                => \&usage
	);
	if ( $result != 1 ) {
		usage();
	}
	if ($< != 0) {
		die "init: only root can do this";
	}
	# /.../
	# check for the Identity.map
	# --------------------------
	my $map = "/usr/share/sax/sysp/maps/Identity.map";
	my $api = "/usr/sbin/xapi";
	if ( ! -f $map ) {
		die "init: no Identity.map found";
	}
	if ( ! -f $api ) {
		print "SaX: GUI is missing, starting commandline interface\n";
		$StartBatchMode = "";
	}

	# /.../
	# check for the syntax of the ModuleList
	# --------------------------------------
	if ((defined $ModuleList) && ($ModuleList ne "none")) {
	if ($ModuleList !~ /^(\d=\w+)(,\d=\w+)*$/) {
		print "SaX: wrong module syntax...\n";
		print "SaX: syntax: -m CardNr=CardModule[,...]\n";
		exit 1;
	}
	}

	# /.../
	# global spec hash definitions...
	# -------------------------------
	$spec{StaticFontPathList}  = "/usr/share/sax/api/data/StaticFontPathList";
	$spec{LangFirstPath}       = "/usr/share/sax/api/data/LangFirstPath";
	$spec{LangCodes}           = "/usr/share/sax/api/data/LangCodes";
	$spec{InitFlag}            = "/var/cache/sax/files/reinit";
	$spec{DbmFile}             = "/var/cache/sax/files/config";
	$spec{DbmNew}              = "/var/cache/sax/files/config.new";
	$spec{DbmDir}              = "/var/cache/sax/files";
	$spec{SyspDir}             = "/var/cache/sax/sysp/rdbms";
	$spec{Sysp}                = "/usr/sbin/sysp";
	$spec{FbSet}               = "fbset";
	$spec{LogFile}             = "/var/log/SaX.log";
	$spec{Xmode}               = "/usr/sbin/xmode";
	$spec{Xmset}               = "/usr/sbin/xmset";
	$spec{RcConfig}            = "/etc/rc.config";
	$spec{RcSysConfigLang}     = "/etc/sysconfig/language";
	$spec{SysConfig}           = "/etc/sysconfig";
	$spec{Profile}             = "/usr/share/sax/pci.pl";
	$spec{ProfileDir}          = "/usr/share/sax/profile/";
	$spec{Monitors}            = "/usr/share/sax/api/data/cdb/Monitors";
	$spec{Corner}              = "/usr/sbin/corner";
	$spec{MD5}                 = "/usr/bin/md5sum";
	$spec{ISaX}                = "/usr/sbin/isax";
	$spec{HwUpdate}            = "/usr/sbin/hwupdate";

	# /.../
	# check if sysp was able to detect a card
	# if not exit (1)
	# ---
	my $syspCard = qx ($spec{Sysp} -c);
	if ($syspCard !~ /^Chip:.*/) {
		die "init: no graphics card(s) found";
	}

	# /.../
	# Init configuration hash...
	# the auto detection will add a lots of settings to this
	# default settings
	#
	#------------------------------------------#
	# Files specification                      #
	#------------------------------------------#
	$var{Files}{0}{LogFile}    = "";

	#------------------------------------------#
	# Module specification                     #
	#------------------------------------------#
	$var{Module}{0}{Load}      = "dbe,type1,freetype,extmod,glx,v4l";

	#------------------------------------------#
	# ServerFlags specification                #
	#------------------------------------------#
	$var{ServerFlags}{0}{Option}                = "AllowMouseOpenFail";
	$var{ServerFlags}{0}{"blank time"}          = "";
	$var{ServerFlags}{0}{"standby time"}        = "";
	$var{ServerFlags}{0}{"suspend time"}        = "";
	$var{ServerFlags}{0}{"off time"}            = "";

	#------------------------------------------#
	# Keyboard specification                   #
	#------------------------------------------#
	$var{InputDevice}{0}{Identifier}            = "Keyboard[0]";
	$var{InputDevice}{0}{Driver}                = "kbd";
	$var{InputDevice}{0}{Option}{Protocol}      = "Standard";
	$var{InputDevice}{0}{Option}{XkbRules}      = "xfree86";
	$var{InputDevice}{0}{Option}{XkbModel}      = "pc101";
	$var{InputDevice}{0}{Option}{XkbLayout}     = "us";
	$var{InputDevice}{0}{Option}{XkbVariant}    = "";
	$var{InputDevice}{0}{Option}{AutoRepeat}    = "";
	$var{InputDevice}{0}{Option}{Xleds}         = "";
	$var{InputDevice}{0}{Option}{XkbOptions}    = "";

	#------------------------------------------#
	# Mouse specification                      #
	#------------------------------------------#
	$var{InputDevice}{1}{Identifier}               = "Mouse[1]";
	$var{InputDevice}{1}{Driver}                   = "mouse";
	$var{InputDevice}{1}{Option}{Protocol}         = "Microsoft";
	$var{InputDevice}{1}{Option}{Device}           = "/dev/ttyS0";
	$var{InputDevice}{1}{Option}{SampleRate}       = "";
	$var{InputDevice}{1}{Option}{BaudRate}         = "";
	$var{InputDevice}{1}{Option}{Emulate3Buttons}  = "";
	$var{InputDevice}{1}{Option}{Emulate3Timeout}  = "";
	$var{InputDevice}{1}{Option}{ChordMiddle}      = "";
	$var{InputDevice}{1}{Option}{Buttons}          = "";
	$var{InputDevice}{1}{Option}{Resolution}       = "";
	$var{InputDevice}{1}{Option}{ClearDTR}         = "";
	$var{InputDevice}{1}{Option}{ClearRTS}         = "";
	$var{InputDevice}{1}{Option}{ZAxisMapping}     = "";
	$var{InputDevice}{1}{Option}{MinX}             = "";
	$var{InputDevice}{1}{Option}{MaxX}             = "";
	$var{InputDevice}{1}{Option}{MinY}             = "";
	$var{InputDevice}{1}{Option}{MaxY}             = "";
	$var{InputDevice}{1}{Option}{ScreenNumber}     = "";
	$var{InputDevice}{1}{Option}{ReportingMode}    = "";
	$var{InputDevice}{1}{Option}{ButtonThreshold}  = "";
	$var{InputDevice}{1}{Option}{Buttons}          = "";
	$var{InputDevice}{1}{Option}{SendCoreEvents}   = "";

	#------------------------------------------#
	# Monitor specification                    #
	#------------------------------------------#
	$var{Monitor}{0}{Identifier}                = "Monitor[0]";
	$var{Monitor}{0}{VendorName}                = "Unknown";
	$var{Monitor}{0}{ModelName}                 = "Unknown";
	$var{Monitor}{0}{HorizSync}                 = "29-70";
	$var{Monitor}{0}{VertRefresh}               = "40-90";
	$var{Monitor}{0}{Option}                    = "DPMS";
	$var{Desktop}{0}{CalcModelines}             = "on";

	#------------------------------------------#
	# Device specification                     #
	#------------------------------------------#
	$var{Device}{0}{Identifier}                 = "Device[0]";
	$var{Device}{0}{VendorName}                 = "Unknown";
	$var{Device}{0}{BoardName}                  = "Unknown";
	$var{Device}{0}{Videoram}                   = "";
	$var{Device}{0}{Driver}                     = "vga";
	$var{Device}{0}{Chipset}                    = "";
	$var{Device}{0}{Clocks}                     = "";
	$var{Device}{0}{BusID}                      = "";

	#------------------------------------------#
	# Screen specification                     #
	#------------------------------------------#
	$var{Screen}{0}{Identifier}                 = "Screen[0]";
	$var{Screen}{0}{Device}                     = $var{Device}{0}{Identifier};
	$var{Screen}{0}{Monitor}                    = $var{Monitor}{0}{Identifier};
	$var{Screen}{0}{DefaultDepth}               = "8";
	foreach $depth (8,16,24,32) {
	 $var{Screen}{0}{Depth}{$depth}{Modes}      = "640x480";
	 $var{Screen}{0}{Depth}{$depth}{ViewPort}   = "";
	 $var{Screen}{0}{Depth}{$depth}{Virtual}    = "";
	 $var{Screen}{0}{Depth}{$depth}{Visual}     = "";
	 $var{Screen}{0}{Depth}{$depth}{Weight}     = ""; # RGB
	 $var{Screen}{0}{Depth}{$depth}{Black}      = ""; # RGB 
	 $var{Screen}{0}{Depth}{$depth}{White}      = ""; # RGB 
	 $var{Screen}{0}{Depth}{$depth}{Option}     = ""; 
	}

	#------------------------------------------#
	# ServerLayout specification for Screen0   #
	#------------------------------------------#
	$var{ServerLayout}{all}{Identifier}            = "Layout[all]";
	$var{ServerLayout}{all}{InputDevice}{0}{id}    = "Keyboard[0]";
	$var{ServerLayout}{all}{InputDevice}{0}{usage} = "CoreKeyboard";
	$var{ServerLayout}{all}{InputDevice}{1}{id}    = "Mouse[1]";
	$var{ServerLayout}{all}{InputDevice}{1}{usage} = "CorePointer";
	$var{ServerLayout}{all}{Screen}{0}{id}         = "Screen[0]";
	$var{ServerLayout}{all}{Screen}{0}{top}        = "";
	$var{ServerLayout}{all}{Screen}{0}{bottom}     = "";
	$var{ServerLayout}{all}{Screen}{0}{left}       = "";
	$var{ServerLayout}{all}{Screen}{0}{right}      = "";

	# check if there is a server which will
	# be used for later access
	# -------------------------
	$haveServer = 0;
	my @xpid = GetPids();
	if (@xpid > 0) {
		my @displayStatus = GetDisplay (@xpid,$spec{Corner});
		$dpy = $displayStatus[0];
		if ($displayStatus[1] eq "grant") {
			$haveServer = 1;
			print ("SaX: access to your display has been granted\n");
		} else {
			print ("SaX: access to your display is denied\n");
		}
	} else {
		print ("SaX: no X-Server is running\n");
		print ("SaX: will start own server if needed\n");
	}

	# /.../
	# search and detect the installed hardware...
	# -------------------------------------------
	$logHandle = OpenLog();
	Logger ("Initializing...",$logHandle);
	qx (rm -f $spec{InitFlag});

	my $needUpdate = $ENV{HW_UPDATE};
	if ($needUpdate) {
		qx (rm -f $spec{SyspDir}/*);
		qx (touch $spec{InitFlag});
		scan(); qx (touch $spec{DbmDir}/hardware.chg);
	}
	# /.../
	# sysp -s 3dlib...
	# ------------------
	# call the sysp -s 3dlib stuff, but at init point
	# we will only prepare this data.
	# --
	# look at xc.pl to see what happened with the
	# scanned information
	# -------------------
	my $D3Stuff = qx ($spec{Sysp} -s 3d;echo);
	$D3Stuff =~ s/\n$//;
	if ($needUpdate) {
	Logger ("Sysp: 3D detection data\n$D3Stuff",
		$logHandle
	);
	} else {
	Logger ("Detection already done
		To perform a fresh detection use the --reinit option",
		$logHandle
	);
	}
	Debug ( $result );

	# /.../
	# close log file handler
	# ---
	CloseLog ( $logHandle );

	# /.../
	# call the autodetect functions to prepare the
	# X11 configuration data
	# ---
	detect();
	
	# /.../
	# test all the options defined begin with the
	# automode option...
	# ------------------
	if (defined $AutoMode) {
		foreach $depth (4,8,15,16,24,32) {
		foreach $scr (keys %{$var{Screen}}) {
		if ($scr !~ /HASH/i) {
			$var{Screen}{$scr}{Depth}{$depth}{Modes} = "Auto";
		}
		}
		}
	} 
	# /.../
	# handle options --node --type
	# ----------------------------
	if (defined $MouseDevice) {
		$var{InputDevice}{1}{Option}{Device}   = $MouseDevice;
	}
	if (defined $MouseProtocol) {
		$var{InputDevice}{1}{Option}{Protocol} = $MouseProtocol;
	}
	# /.../
	# handle option --gpm...
	# -----------------------
	if (defined $UseGPM) {
	# try to start gpm as repeater...
	# --------------------------------
	my %translate;
	$translate{'mouseman'}     = "mman";
	$translate{'microsoft'}    = "ms";
	$translate{'mousesystems'} = "msc";
	$translate{'logitech'}     = "logi";
	$translate{'busmouse'}     = "bm";
	$translate{'ps/2'}         = "ps2";
	$translate{'intellimouse'} = "ms3";
	$translate{'imps/2'}       = "imps2";
	# ...
	@plist = ();
	my $gpmType   = lc ($var{InputDevice}{1}{Option}{Protocol});
	my $gpmDevice = $var{InputDevice}{1}{Option}{Device};
	if (! defined $translate{$gpmType}) {
		die "could not translate X11 protocol to GPM protocol...";
	}
	qx(gpm -k 2>/dev/null);
	qx(gpm -t $translate{$gpmType} -m $gpmDevice -R);
	if ( ! defined (qx(pidof gpm 2>/dev/null)) ) {
		die "could start gpm as repeater...";
	}
	foreach $m (keys %{$var{InputDevice}}) {
	if ($m%2 != 0) {
		$var{InputDevice}{$m}{Option}{Protocol} = "MouseSystems";
		$var{InputDevice}{$m}{Option}{Device}   = "/dev/gpmdata";
	}
	}
	}
	# /.../
	# handle option --batch
	# ---------------------
	if (! defined $UseDbmNew) {
		# /.../
		# look if there is X11 driver profile defined...
		# ------------------------------------------------
		$ProfileSize = keys %{$xdp{DriverProfile}};
		if ($ProfileSize > 0) {
			@ProfileAddSections = ();
			@ProfileData = ();
			$ProfileSize = 0;
			@ProfileData = ReadProfile($CardNumber,"driver");
			$ProfileSize = @ProfileData;
			IncludeProfile();
		}
		# /.../
		# look if there is a profile defined for the card...
		# ----------------------------------------------------
		@ProfileAddSections = ();
		@ProfileData = ();
		$ProfileSize = 0;
		@ProfileData = ReadProfile($CardNumber,"card");
		$ProfileSize = @ProfileData;
		if ($ProfileSize > 0) {
			IncludeProfile();
		}
		# /.../
		# look if there is a profile defined for the monitor...
		# ------------------------------------------------------
		$ProfileSize = keys %{$mop{MonitorProfile}};
		if ($ProfileSize > 0) {
			@ProfileAddSections = ();
			@ProfileData = ();
			$ProfileSize = 0;
			@ProfileData = ReadProfile($CardNumber,"monitor");
			$ProfileSize = @ProfileData;
			IncludeProfile();
		}
		# /.../
		# look if there is a profile defined for the mouse...
		# ------------------------------------------------------
		$ProfileSize = keys %{$idp{MouseProfile}};
		if ($ProfileSize > 0) {
			@ProfileAddSections = ();
			@ProfileData = ();
			$ProfileSize = 0;
			@ProfileData = ReadProfile($CardNumber,"mouse");
			$ProfileSize = @ProfileData;
			IncludeProfile();
		}
	}
	# /.../
	# check if the -vesa option was set. If yes build a
	# profile and include it
	# ----------------------
	if (defined $CreateVESAProfile) {
	if ($CreateVESAProfile =~ /(.*):(.*)x(.*)\@(.*)/) {
		@ProfileData = ();
		@ProfileAddSections = ();
		my $id  = $1;  # Card ID
		my $rx  = $2;  # X resolution
		my $ry  = $3;  # Y resolution
		my $vs  = $4;  # Vertical Sync value
		my $hs  = int (($ry * $vs)/1000); $hs += 10;
		my $res = $rx."x".$ry;
		push (@ProfileData,"Monitor->$id->VertRefresh=50-$vs");
		push (@ProfileData,"Monitor->$id->HorizSync=20-$hs");
		push (@ProfileData,"Screen->$id->Depth->8->Modes=$res");
		push (@ProfileData,"Screen->$id->Depth->16->Modes=$res");
		push (@ProfileData,"Screen->$id->Depth->15->Modes=$res");
		push (@ProfileData,"Screen->$id->Depth->24->Modes=$res");
		push (@ProfileData,"Screen->$id->Depth->32->Modes=$res");
		$ProfileSize = @ProfileData;
		IncludeProfile();
	}
	}

	# /.../
	# check if we want to start the interactive
	# batch mode shell
	# ---------------- 
	if (defined $StartBatchMode) {
	if ($StartBatchMode ne "") {
		%var = SetBatchMode(\%var,$StartBatchMode);
		%var = HPrepareServerLayout(\%var);
	} else {
		%tmpvar = SetBatchMode(\%var,"interactive");  
		if ($AbortBatch ne "yes") {
			%var = %tmpvar;
		}
	}
	}

	# /.../
	# handle the module list (Option -m X=..) with the highest 
	# priority. Therefore we had to check the ModuleList again
	# because it may be modified via a profile
	# ---
	if ($ModuleList ne "none") {
	my @list;
	my %chiplist;
	my $count;
	my @pair;

	# create chiplist hash 
	# chip{OriginalNumber} = ConfigHashNumber
	# ----------------------------------------
	if ($CardNumber ne "all") {
		@list  = split (/,/,$CardNumber);
		$count = 0;
		foreach (@list) {
			$chiplist{$_} = $count;
			$count++;
		}
	} else {
		@list = split (/,/,$ModuleList);
		foreach (@list) {
			@pair = split (/=/,$_);
			$chiplist{$pair[0]} = $pair[0];
		}
	}
	# setup modules...
	# ----------------
	@list = split (/,/,$ModuleList);
	foreach (@list) {
		@pair = split (/=/,$_);
		$var{Device}{$chiplist{$pair[0]}}{Driver} = $pair[1];
	}
	}

	# /.../
	# we are ready now.
	# Save configuration hash as storable binary
	# output which is read from the interface again
	# ---------------------------------------------
	if (save() == -1) {
		die "init: level 1... abort";
	}

	# /.../
	# use ISaX to create the configuration files
	# used to provide the X11 auto config files to YaST2
	# and the X11 manager (xapi)
	# ---
	if (defined $Suggest) {
	if (($HWScanNeeded) || (! -f "$spec{DbmDir}/suggest.ycp")) {
	if (-f $spec{DbmFile}) {
		my $suggest = "$spec{DbmDir}/suggest.ycp";
		my @modlist = (
			"card",
			"desktop",
			"input",
			"keyboard",
			"layout",
			"path"
		);
		unlink $suggest;
		if (! open (FD,"> $suggest")) {
			die "init: could not create file: $suggest";
		}
		print FD "[\n";
		foreach my $module (@modlist) {
			my $imode;
			SWITCH: for ($module) {
			/^card/  && do {
				$imode = "Card";
			last SWITCH;
			};
			/^desktop/  && do {
				$imode = "Desktop";
			last SWITCH;
			};
			/^input/  && do {
				$imode = "Mouse";
			last SWITCH;
			};
			/^keyboard/  && do {
				$imode = "Keyboard";
			last SWITCH;
			};
			/^layout/  && do {
				$imode = "Layout";
			last SWITCH;
			};
			/^path/  && do {
				$imode = "Path";
			last SWITCH;
			};
			}
			my $xapiFile = "$spec{DbmDir}/$module";
			my $result;
			if (($HWScanNeeded) || 
				(-f "$spec{DbmDir}/hardware.chg") || 
				(! -f "/etc/X11/xorg.conf")
			) {
				$result = qx (
				$spec{ISaX} -l $imode -b -plusycp 2>$xapiFile
				);
			} else {
				$result = qx (
				$spec{ISaX} -l $imode -plusycp 2>$xapiFile
				);
			}
			if ($imode ne "Path") {
				$result =~ s/\n+$//g;
				$result = $result.",\n";
			}
			print FD $result;
		}
		print FD "]\n";
		close FD;
	}
	}
	}
	if ($AbortBatch ne "yes") {
		exit (0);
	} else {
		exit (1);
	}
}

#==================================================
# next there are all the functions needed to work
# in the init routine
#--------------------------------------------------
#---[ IncludeProfile ]----#
sub IncludeProfile {
#--------------------------------------------
# this function will include the profile
# for cards and monitors
#
	my $newsection;
	@ProfileAddSections = unique(@ProfileAddSections);
	# /.../
	# if a profile includes new sections we had to create 
	# space for this sections. Therefore the branch on the
	# old section is moved to the next section ( old +1 )
	# ------------------------------------------------------
	foreach $newsection (@ProfileAddSections) {
		print "SaX: profile add new section(s) -> moving: $newsection\n";
		%var = HMoveBranch(\%var,$newsection);
	}
	%var = SetBatchMode(\%var,"profile");
	%var = HPrepareServerLayout(\%var);
}


#----[ scan ]----#
sub scan {
#-----------------------------------------------------
# this function is used to call the system profiler
# it detects all the needed stuff for an automatic
# X11 setup. The result is saved into the DBM database
# called registry and is used again in the autodetect
# functions
#
	my $result;    # result of sysp -s call
	my $cards;     # grep result
	my $i;         # loop counter
	my @chiplist;  # list of chips given through option -c
	my $used;      # real used number of cards
	my $querystr;  # module name for sysp
	my $subject;   # topic line for log

	if (! defined $CardNumber) { $CardNumber = "all"; }
	if (! defined $ModuleList) { $ModuleList = "none"; }

	# /.../
	# remove the registry and
	# install the child signal handler...
	# ------------------------------------
	$SIG{"CHLD"} = sub { wait };

	# /.../	
	# sysp -s mouse...
	# ---
	$querystr = "mouse";
	$subject  = "Sysp: Mouse detection data";
	if ($haveServer == 1) {
		DisableMouse();
	}
	$result   = qx ($spec{Sysp} -s $querystr;echo);
	$result   =~ s/\n$//;
	Logger("$subject\n$result",$logHandle);
	Debug ($result);
	if ($haveServer == 1) {
		EnableMouse();
	}

	# /.../
	# sysp -s keyboard...
	# --------------------
	$querystr = "keyboard";
	$subject  = "Sysp: Keyboard detection data";
	$result   = qx ($spec{Sysp} -s $querystr;echo);
	$result   =~ s/\n$//;
	Logger("$subject\n$result",$logHandle);
	Debug ($result);

	# /.../
	# sysp -s server...
	# ------------------
	$querystr = "server";
	$subject  = "Sysp: Server detection data";
	if (defined $Quiet) {
		$result   = qx 
		($spec{Sysp} -s $querystr -C $CardNumber -M $ModuleList -A $Quiet;echo);
	} else {
		if ($haveServer == 1) {
		EnableMouse();
		$result   = qx
		($spec{Sysp} -s $querystr -C $CardNumber -M $ModuleList -x;echo);
		DisableMouse();
		} else {
		$result   = qx 
		($spec{Sysp} -s $querystr -C $CardNumber -M $ModuleList;echo);
		}
	}
	$result =~ s/\n$//;
	if ($result =~ /.*Flag.*:.*3D.*/) {
		$D3Answer = "yes";
	}
	Logger("$subject\n$result",$logHandle);
	my $serverLines = split("\n",$result);
	if ($serverLines < 3) {
		if ($haveServer == 1) { EnableMouse(); }
		die $result;
	}
	Debug ($result);

	# /.../
	# sysp -s xstuff...
	# ------------------
	$querystr = "xstuff";
	$subject  = "Sysp: XStuff detection data";
	if (defined $Virtual) {
		$result   = qx ($spec{Sysp} -s $querystr -d;echo);
	} else {
		$result   = qx ($spec{Sysp} -s $querystr;echo);
	}
	$result =~ s/\n$//;
	Logger("$subject\n$result",$logHandle);
	my $xstuffLines = split("\n",$result);
	if ($xstuffLines < 3) {
		if ($haveServer == 1) { EnableMouse(); }
		die $result;
	}
	Debug ($result);

	# /.../
	# switch back to the start terminal  
	# re-init vga registers...
	# -------------------------
	if ($haveServer == 0) {
		my $tty = Getty();
		qx (/usr/bin/chvt $tty);
	}
	if ($haveServer == 1) {
		EnableMouse();
	}

	# /.../
	# sync changes to disk
	# ---------------
	for ($i=0;$i<3;$i++) { 
		qx (/bin/sync);
	}
}

#----[ detect ]----#
sub detect {
#--------------------------------------------------------
# this is the detect function. it is called after the
# scan. detect read from the DBM files and save the 
# result into the hash which is saved using the storable
# modul.
#
	my $resolutionSet = 0;
	if (defined $UseDbmNew) {
	#===================================================
	# retrieve the DbmNew file if it exist instead of
	# using the sysp saved query information
	#---------------------------------------------------
	if (-f $spec{DbmNew}) {
		my $hashref = retrieve($spec{DbmNew});
		%var = %{$hashref};
		return;
	} else {
		print STDERR "init: could not open DBM file: $spec{DbmNew}\n";
		print STDERR "init: revert to standard protocol\n";
	}
	}
	#===================================================
	# use the detect functions to create the %var hash
	#---------------------------------------------------
	my $resolutionSet = 0;
	AutoDetectFontPath     (\%spec,\%var);
	AutoDetectMouse        (\%spec,\%var);
	AutoDetectKeyboard     (\%spec,\%var);
	AutoDetectServer       (\%spec,\%var);
	AutoDetectOptions      (\%spec,\%var);
	# /.../
	# if the monitor detection was able to detect the resolution 
	# for the monitor via the CDB we need to call the function
	# again after the resolution detection was done
	# ---
	$resolutionSet = AutoDetectSyncRanges  (\%spec,\%var);
	AutoDetectResolution   (\%spec,\%var);
	if ($resolutionSet) {
		ResetLDNr();
		AutoDetectSyncRanges  (\%spec,\%var);
	}
	AutoDetectServerLayout (\%spec,\%var);
}

#----[ save ]----#
sub save {
#----------------------------------------------
# this function is used to save the hash on 
# disk. The data in this file is called registry
#
	unlink($spec{DbmFile});
	if(! store(\%var,"$spec{DbmFile}")) {
		print "init: could not create tree: $spec{DbmFile}\n";
		return(-1);
	}
	return(0);
}

#-----[ Logger ]------#
sub Logger {
#------------------------------------------
# append log file data to LOG handler...
#
	my $data   = $_[0];
	my $handle = $_[1];
	my $date   = qx (
		LANG=POSIX /bin/date "+%d-%h %H:%M:%S"
	);
	my $lineWrap = 0;
	if (! defined $handle) {
		$handle = STDOUT;
	}
	$data =~ s/^\n//;
	$data =~ s/\n$//;
	$date =~ s/\n$//;
	if ($data =~ /\n+/) {
		$lineWrap = 1;
	}
	print $handle "$date <I> ";
	if ($lineWrap) {
		my @list  = split ("\n",$data);
		my $topic = shift (@list);
		print $handle "[ $topic\n";
		foreach (@list) {
			$_ =~ s/^\t+//g;
			print $handle "\t$_\n";
		}
		print $handle "]\n";
	} else {
		print $handle "$data\n";
	}
}

#---[ CreateTreeList ]----#
sub CreateTreeList {
#------------------------------------------------
# this function is called to create a tree list 
# from the current var setup 
#
	my (%var)     = %{$_[0]};
	my @tree;

	@tree = HGet(\%var);
	return(@tree);
}


#----[ SetBatchMode ]----#
sub SetBatchMode {
#------------------------------------------------
# enable the profile batch mode for interactive
# configurations
# 
	my (%var)     = %{$_[0]};   # config hash
	my ($profile) = $_[1];      # profile batch
	my $line;                   # stdinput line after \n
	my $index;                  # optinal list index given to variable
	my @tree;                   # option tree
	my @result;                 # grep result from tree
	my $param;                  # parameter behind a key 
	my $key;                    # key name
	my $value;                  # value to be set
	my @outlist;                # formatted output list
	my @plist;                  # profile list;
	my $k;                      # loop counter
	my $v;                      # loop counter
	my $ref;                    # hash reference
	my $search;                 # search string in hash
	my $id;                     # X -> id
	my $data;                   # profile variable data
	my @v;                      # data lines
	my $size;                   # size of @v
	my $term;                   # Readline object var
	my $str;                    # helper string
	my $expr;                   # helper string
	my $i;                      # loop counter
	my $x;                      # calc mode X
	my $y;                      # calc mode Y
	my $vsmax;                  # calc mode VS-max
	my $hsmax;                  # calc mode HS-max
	my $ml;                     # calc modeline string
	my @mode;                   # modeline as list
	my $res;                    # calc resolution
	my $tim;                    # calc timing

	if ($profile eq "interactive") {
		print "\n";
		print "Linux SaX Version 7.1 startup level (init) (2001-04-06)\n";
		print "(C) Copyright 2001 - SuSE GmbH\n";
		print "\n";
	}

	# create profile list if defined...
	# ----------------------------------
	SWITCH: for ($profile) {
	# automatic profile detection mode
	# ----------------------------------
	/^profile$/       && do {
		# ProfileData was defined using ReadProfile()...
		last SWITCH;
	};

	# interactive mode
	# ------------------
	/^interactive$/   && do {
		@ProfileData = ();
		$ProfileSize = 0;
		last SWITCH;
	};

	# default: use file as profile
	# ------------------------------
	@ProfileData = ();
	print "SaX: including profile: $profile...\n";
	#==========================================
	# Check if there is a profile script which
	# should run before the profile is opened
	#------------------------------------------
	if (-f "$profile.sh") {
		print "SaX: calling profile script: $profile.sh\n";
		qx ($profile.sh);
		$profile = "$profile.tmp";
	}
	open (FD,$profile) || die "init: could not open file: $profile";
	while(<FD>) {
		chomp($_);
		if ($_ =~ /^#/) {
			next;
		}
		SWITCH: for ($_) {
		/^.*\[X\].*/         && do {
			$_ =~ s/\[X\]/0/g;
		};

		/^.*\[X\+([1-9]).*/  && do {
			$id = $1;
			$_ =~ s/\[X\+[1-9]\]/$id/g;
		};

		/\[REMOVE\]/         && do {
		if ($_ =~ /(.*)=.*/) {
			$line    = $1;
			$line    =~ s/ +//g;
			@v       = split (/->/,$line);
			$search  = join("->",@v);
			%var = HRemoveValue (\%var,$search);
		}
		};
		}
		if ($_ !~ /\[REMOVE\]/) {
			push(@ProfileData,$_);
		}
	}
	close(FD);
	$ProfileSize = @ProfileData;
	$profile     = "profile";
	}

	if ($ProfileSize > 0) {
	foreach (@ProfileData) {
		# look for eventually defined variables in the
		# profile data stream...
		# -----------------------
		while ($_ =~ /{(.*?)}/) {
			$line      = $1;
			undef $index;
			if ($line =~ /(.*):(.*)/) {
				$line  = $1;
				$index = $2;
			}
			$line      =~ s/ +//g;
			@v         = split(/->/,$line);
			$search    = join ("->",@v);
			$size      = @v;
			$ViewValue = "";
			$ViewRef   = "";
			$ViewValue = HGetValue(\%var,$search);
			$ViewValue =~ s/^ +//g;
			$ViewValue =~ s/ +$//g;
			if (defined $index) {
				my @list = split (/,/,$ViewValue);
				$ViewValue = $list[$index];
			}
			$_ =~ s/{.*?}/$ViewValue/;
		}
		# push stream data...
		# --------------------
		push(@plist,$_);
	}
	push(@plist,"exit");
	}

	# /.../
	# read data via readline from /dev/tty or import
	# profile data given from file
	# ---
	while (1) {
		if ( $profile ne "profile" ) {
			# /.../
			# no profile file name specified, read from
			# STDIN using readline
			# ---
			my $prompt = "SaX> ";
			if (! defined $term) {
				$term = Term::ReadLine->new ( "SaX Profile Mode" );
			}
			last unless defined ( 
				$_ = $term->readline ($prompt) 
			);
			$term->addhistory($_) if /\S/;
		} else {
			# /.../
			# profile file contents saved in @plist
			# read lines from list (plist)
			# ---
			last unless defined ( 
				$_ = shift(@plist) 
			);
		}
		s/^\s+//;
		next if /^$/;
		$_ = 'h' if /^\s*\?/;
		if (/^(?:q(?:uit)?|bye|exit)$/i) {
			last;
		} else {
	 
		$line = $_;
		SWITCH: for ($line) {
		#-----------------------------------------#
		# get help...                             #
		# ----------------------------------------# 
		/^help/i  && do {
			help();
			last SWITCH;
		};

		#-----------------------------------------#
		# abort ( lost changes )...               #
		# ----------------------------------------#
		/^abort/i  && do {
			print "batch mode aborted...\n";
			$AbortBatch = "yes";
			return(%var);
			last SWITCH;
		};

		#-----------------------------------------#
		# set a variable use HSetValue function   #
		# ----------------------------------------#
		/^(.*)=(.*)/   &&  do {
			$key   = $1;
			$value = $2;
			$key   =~ s/ +//g;
			$value =~ s/^ +//g;
			@v     = split(/->/,$key);
			$str   = join("->",@v);
			%var   = HSetValue(\%var,$str,$value);
			if ($profile ne "profile") {
				$str =~ s/->/}{/g;
				print "\t\033[32m<set>:\033[m\017{$str} to \"$value\"\n\n";
			}
			last SWITCH;
		};

		#-----------------------------------------#
		# list the variable tree use HGet()...    #
		# ----------------------------------------# 
		/^list/i  &&  do {
			@tree = CreateTreeList(\%var); 
			if ($line =~ /(.*)\/(.*)\//) {
				$expr = $2; 
				@result = grep(/$expr/i,@tree);
			} else {
				@result = @tree;
			}
			foreach $i (@result) {
				print "$i\n";
			}
			print "\n";
			last SWITCH;
		};
	
		#-----------------------------------------#
		# calculate a modeline...                 #
		# ----------------------------------------#
		/^calc/i  && do {
			if ($line =~ /calc(.*)x(.*)->(.*)/) {
			$x     = $1;
			$y     = $2;
			$vsmax = $3;
			$x =~ s/ +//g;
			$y =~ s/ +//g;    
			$vsmax =~ s/ +//g;
			$ml = qx($spec{Xmode} -x $x -y $y -r $vsmax);
			@mode  = split(/\n/,$ml);
			$hsmax = $mode[0];
			$vsmax = $mode[1];
			$ml    = $mode[2];
			$ml    =~ /Modeline \"(.*)\" (.*)/;
			$res   = $1;
			$tim   = $2;
			print "Hsync:    $hsmax Khz\n";  
			print "Vsync:    $vsmax Hz\n";
			print "Modeline: $res\n";
			print "Timing:   $tim\n";
			print "\n";
			}
			last SWITCH;
		};

		#-----------------------------------------#
		# see a value use the HGetValue function  #
		# ----------------------------------------#
		/^see/i  && do {
			chomp($line);
			$line      =~ s/see//; 
			$line      =~ s/ +//g;
			@v         = split(/->/,$line);
			$size      = @v;
			$ViewValue = "";
			$ViewRef   = ""; 
			$search    = join("->",@v);

			$v = HGetValue(\%var,$search);
			if ($v eq "") {
			$ViewRef = HGetValue(\%var,$search,"GET_REF");
			if ($ViewRef eq "") {
				print "Value not defined\n\n";
			} else {
				while(($k,$v) = each(%{$ViewRef})) {
					printf("%-15s %-3s %-30s\n",$search,"->",$k);
				}
				print "\n";
			}
			} else {
				printf("%-15s %-3s %-30s\n\n",$search,"=",$v);
			}
			last SWITCH;
		};
		}
		}
	}
	return(%var);
}


#---[ ReadProfile ]-----#
sub ReadProfile {
#-------------------------------------------
# this function is called to read the 
# eventually given profile for the card XY
#
	my $cards = $_[0];   # the contents of the option -c if given
	my $kind  = $_[1];   # kind of profile (card or monitor)
	
	my $profile;
	my %data;
	my @list;
	my @line;
	my $l;
	my @result;
	my $file;
	my $new;
	my $c;

	my $topic = "Card";
	# get profiles filenames for all cards, 
	# save result to %data
	# --------------------
	if (($kind ne "monitor") && ($kind ne "mouse") && ($kind ne "driver")) {
	# create %data for a card...
	# ------------------------------
	$profile = qx($spec{Profile} -p);
	@list    = split(/\n/,$profile);
	foreach (@list) {
	$_ =~ s/Chip://g;
	@line = split(/:/,$_);
	if ($line[1] ne "unknown") {
		$data{$line[0]} = $line[1];
	}
	}
	} 
	else {
	if ($kind eq "monitor") {
		$topic = "Monitor";
		# create %data for a monitor...
		# ------------------------------
		foreach (keys %{$mop{MonitorProfile}}) {
		if ($mop{MonitorProfile}{$_} ne "unknown") {
			$data{$_} = $mop{MonitorProfile}{$_};
		}
		}
	}
	if ($kind eq "mouse") {
		$topic = "Mouse";
		# create %data for a mouse...
		# ------------------------------
		foreach (keys %{$idp{MouseProfile}}) {
		if ($idp{MouseProfile}{$_} ne "<undefined>") {
			$data{$_} = $idp{MouseProfile}{$_};
		}
		}
	}
	if ($kind eq "driver") {
		$topic = "Driver";
		# create %data for a device (driver)...
		# ------------------------------
		foreach (keys %{$xdp{DriverProfile}}) {
		if ($xdp{DriverProfile}{$_} ne "<undefined>") {
			$data{$_} = $xdp{DriverProfile}{$_};
		}
		}
	}
	}
	# use only profiles for defined chips
	# ------------------------------------
	if (! defined $IgnoreProfile) {
		foreach $c (keys %data) {
		# /.../
		# read all profiles and select the card number
		# it belongs to within the current card setup
		# ---------------------------------------------
		my @drvplist = split (/,/,$data{$c});
		foreach my $name (@drvplist) {
			$file = "$spec{ProfileDir}$name";
			print   "SaX: including [$topic:$c] profile: $name...\n";
			if (-f $file) {
				#==========================================
				# Check if there is a profile script which
				# should run before the profile is opened
				#------------------------------------------
				if (-f "$file.sh") {
					print "SaX: calling [$topic:$c] profile script: $name.sh\n";
					qx ($file.sh);
					$file = "$file.tmp";
				}
				#==========================================
				# Save profile information
				#------------------------------------------
				open (DATA,"$file");
				while ($l = <DATA>) {
					chomp($l);
					SWITCH: for ($l) {
					/^.*\[X\].*/         && do {
						$l =~ s/\[X\]/$c/g;
					};
					/^.*\[X\+([1-9]).*/  && do {
						$new = $c + $1;
						$l =~ s/\[X\+[1-9]\]/$new/g;
						push(@ProfileAddSections,$new);
					};
					}
					push(@result,$l);
				}
				close(DATA);
			}
		}
		}
	}
	return(@result);
}

#---[ unique ]----#
sub unique {
#--------------------------------------
# create unique list of members
#
	my @list = @_;
	my @result;
	my %map;

	sub num { $a <=> $b; };
	foreach (@list) {
	if ($_ =~ /[0-9]+/) {
		$map{$_} = $_;
	}
	}
	@result = sort num keys(%map);
	return(@result);
}

#----[ Getty ]----#
sub Getty {
#--------------------------------------------------
# look for the terminal ID and return it. Please
# note this function has to return a number
#
	my $tty;
	$tty = qx(tty | cut -f2 -dy);
	if ($tty =~ /.*dev.*/) {
		return(7);
	}
	return($tty);
}

#----[ OpenLog ]-----#
sub OpenLog {
#--------------------------------------------------
# remove old auto configuration file and open a 
# new log file descriptor...
#
	my $io;
	unlink($spec{AutoConf});

	$io = new FileHandle;
	if (! $io->open(">>$spec{LogFile}")) {
		die "init: could not create LogFile: $spec{LogFile}";
	}
	binmode($io, ':unix');
	return ($io);
}

#----[ CloseLog ]----#
sub CloseLog {
#--------------------------------------------------
# close current log file descriptor...
#
	my $io = $_[0];
	$io->close;
}

#----[ Debug ]----#
sub Debug {
#--------------------------------------------------
# print given information if $Debug ist
# defined
#
	my $result = $_[0];
	if (defined $Debug) {
		print "$result\n";
	}
}

#---[ EnableMouse ]----#
sub EnableMouse {
#-------------------------------------------
# enable the mouse pointer again after
# it was disabled using DisableMouse()
#
#	system("$spec{Xmset} -d $dpy --enable 2>/dev/null >/dev/null");
}

#---[ DisableMouse ]----#
sub DisableMouse {
#-------------------------------------------
# disable mouse to prevent us from haply
# mouse events
#
#	system("$spec{Xmset} -d $dpy --disable 2>/dev/null >/dev/null");
}

#----[ usage ]----#
sub usage {
#------------------------------------------------------
# if you need help :-)
#
	print "Linux SaX Version 7.1 level (init) (2001-03-22)\n";
	print "(C) Copyright 2001 - SuSE GmbH\n";
	print "\n";

	print "usage: init [ options ]\n";
	print "options:\n";
	print "[ -b | --batch [ filename ]]\n";
	print "   enable batch mode to set configuration options\n";
	print "   via the SaX2 shell. Optionally give a filename holding\n";
	print "   the profile data\n";
	print "\n";
	print "[ -v | --virtual ]\n";
	print "   This option enable the 640x480 virtual mode\n";  
	print "   You should prefere this mode if you don`t want\n"; 
	print "   to use DDC\n";
	print "\n";
	print "[ -V | --vesa ]\n";
	print "   This option will set a given resolution and vertical sync\n";
	print "   value (in Hz) as VESA standard resolution for a specific\n";
	print "   card. Format:  Card:XxY\@VSync";
	print "   Example: 0:1024x768\@85\n";
	print "\n";
	print "[ -m | --module ]\n";
	print "   This option set the modules to use for the single cards\n";
	print "   setting more than one module require a comma seperated list\n";
	print "\n";
	print "[ -c | --chip < number > ]\n";
	print "   Set the card/chip number(s) to use for the configuration\n";
	print "   If you have a card with more than one graphics chip onboard\n";
	print "   this option may usefull\n";
	print "\n";
	print "[ -d | --debug ]\n";
	print "   enable debug mode, print result to STDOUT\n";
	print "\n";
	print "[ -g | --gpm ]\n";
	print "   enable gpm mode, use gpm as repeater to report\n";
	print "   mouse events over /dev/gpmdata\n";
	print "\n";
	print "[ -n | --node ]\n";
	print "   set the device node to use for the core pointer\n";
	print "\n";
	print "[ -t | --type ]\n";
	print "   set the protocol to use for the core pointer\n";
	print "\n";
	print "[ -q | --quiet ]\n";
	print "   answer any questions with the string given through this\n";
	print "   option, for example the [3D...] question\n";
	print "\n";
	print "[ --suggest ]\n";
	print "   additionally create the configuration information\n";
	print "   usable for YaST2 and the X11 manager\n";
	print "\n";
	print "[ --help ]\n";
	print "   show this message\n";
	exit(0);
}

#----[ help ]------#
sub help {
#----------------------------------------------------
# help information for the profile batch mode
# using help within the shell
#
	print "variable format:\n";
	print "  var -> var ->...-> var = value\n";
	print "\n";
	print "commands:\n";
	print "  variable list:    list [ /<regexp>/ ]\n";
	print "  variable value:   see <variable format>\n";
	print "  modeline:         calc XxY->Vsync\n";
	print "  abort batchmode:  abort\n";
	print "\n";
} 
