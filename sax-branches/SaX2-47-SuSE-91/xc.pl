#!/usr/bin/perl
# Copyright (c) 1996 SuSE GmbH Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2000
# xc.pl configuration level 2 after sax startup level 1 (init)
#
# CVS ID:
# --------
# Status: Up-to-date
#
use lib   '/usr/X11R6/lib/sax/modules';
use lib   '/usr/X11R6/lib';

# strict checking disabled because this cause problems 
# with SuSE 7.0 in combination with the Env module
# -------------------------------------------------
use strict;
use Env;

use FileHandle;
use CreateSections;
use CheckServer;
use ParseConfig;
use Getopt::Long;
use Storable;
use FileHandle;
use XFree; 

no strict "subs";
no strict "refs";

#==================================================
# this is the global var hash which build the 
# fundamental basics of the whole configuration
#--------------------------------------------------
my %var;                   # hold all the configuration parameters
my %spec;                  # hold specifications for xc.pl
my $EnableAutoConf;        # option variable: enable auto config
my $YaSTMode;              # called from YaST2
my $StartWithSystemConfig; # option start with system config
my $EnableXF86AutoMode;    # option variable: enable auto modeline
my $Display;               # option variable: set Display 
my $TmpDir;                # secure dir name
my %tune;                  # tune hash for changed mode timings
my $logHandle;             # log file handle
my $FullScreen;            # full screen mode
my $NoCheckPacs;           # do not check package requirements
my $NoIntro;               # prevent intro appearance
my $NoBorder;              # suppress border around main window
my $UniFont;               # force usage of UniCode font
my $FrameWidth = 0;        # default frame width for xapi
my $DialogToStartWith;     # dialog to start with (xapi option)

main();
#==================================================
# next there is the main function which has the
# following tasks:
#
# 0 -> init all the needed variables
# 1 -> open the configuration hash with the 
#      autodetection data
# 2 -> open LOG handler
# 3 -> create auto configuration
#      0 -> save this configuration
#      1 -> proceed with X-Server check
# 4 -> if there is no X-Server running start
#      an own X-Server otherwhise try to access
#      the running one
# 5 -> start SaX2
#
#--------------------------------------------------
#----[ main ]--------#
sub main {
#--------------------------------------------------
# start point with main 
#
	my $exit;     # exit codes
	my $final;    # name of final config file
	my @part0;    # configuration part 0
	my @part1;    # configuration part 1
	my @part2;    # configuration part 2
	my @part3;    # configuration part 3
	my @part4;    # configuration part 4
	my @part5;    # configuration part 5
	my @part6;    # configuration part 6
	my @part7;    # configuration part 7
	my @part8;    # configuration part 8
	my @part9;    # configuration part 9
	my @part10;   # configuration part 10

	my $disp       = ":0.0";
	my $haveServer = 0;

	# some global xc specifications...
	# ---------------------------------
	init();

	# open configuration hash...
	# ---------------------------
	if ((prepare()) == -1) {
		print "xc: level 2... abort\n";
		Exit(0);
	}

	# open LOG file handler...
	# -------------------------
	$logHandle = OpenLog();

	# first create the automatic configuration file and
	# write the result configuration to the LOG...
	# ------------------------------------------------
	my $crt = "glx,dri,fgl1";
	@part0  = CreateHeaderSection ();
	@part1  = CreateFilesSection  (\%var);
	if (defined $EnableAutoConf) {
		@part2 = CreateModuleSection (\%var);
	} else {
		@part2 = CreateModuleSection (\%var,$crt);
	}
	@part3 = CreateServerFlagsSection (\%var);
	@part4 = CreateInputDeviceSection (\%var);
	if (defined $EnableXF86AutoMode) {
		@part5 = CreateMonitorSection (\%var,"yes");
	} else {
		@part5 = CreateMonitorSection (\%var,"no");
		@part6 = CreateModesSection (\%var,"no"); 
	}
	@part7  = CreateDeviceSection (\%var);
	@part8  = CreateScreenSection (\%var);        
	@part9  = CreateServerLayoutSection (\%var);  
	@part10 = CreateDRISection ();                

	# write new AutoConf file...
	# ----------------------------
	open (HANDLE,">$spec{AutoConf}");
	print HANDLE @part0;  print HANDLE "\n";
	print HANDLE @part1;  print HANDLE "\n";
	print HANDLE @part2;  print HANDLE "\n";
	print HANDLE @part3;  print HANDLE "\n";
	print HANDLE @part4;  print HANDLE "\n";
	print HANDLE @part5;  print HANDLE "\n";
	print HANDLE @part6;  print HANDLE "\n";
	print HANDLE @part7;  print HANDLE "\n";
	print HANDLE @part8;  print HANDLE "\n";
	print HANDLE @part9;  print HANDLE "\n";
	print HANDLE @part10; print HANDLE "\n";
	close(HANDLE);

	if (defined $EnableAutoConf) {
		# /.../
		# Auto configuration is enabled. This will save the auto
		# detected configuration file as final config file
		# ---
		$final = LinkConfiguration();
		print "Automatic configuration is done\n";
		print "A file $final has been written\n";
		CleanTmp();
		Exit(0);
	}

	# /.../
	# auto configuration is disabled...
	# test if there is another server active, if yes run
	# there otherwise start a new one
	# ---
	my @displayStatus;
	my @xpid = GetPids();
	if (@xpid > 0) {
	#============================================
	# check on local system according to pid's
	#--------------------------------------------
	@displayStatus = GetDisplay (@xpid,$spec{Corner});
	$disp = $displayStatus[0];
	if ($displayStatus[1] eq "grant") {
		$haveServer = 1;
	}
	} else {
	#============================================
	# check for remote access, x11 forward
	#--------------------------------------------
	@displayStatus = X11ForwardEnabled();
	if ($displayStatus[1] eq "grant") {
		$disp = $displayStatus[0];
		$haveServer = 1;
	}
	}

	# /.../
	# add log message to SaX.log
	# ---
	Logger ("Startup...",$logHandle);
	if ($haveServer) {
	Logger ("Startup on already running Server:
		$displayStatus[0] -> $displayStatus[1]",
		$logHandle
	);
	} else {
	Logger ("Startup on new Server: <$disp>",
		$logHandle
	);
	Logger ($spec{AutoConf},
		$logHandle
	);
	}

	# /.../
	# if ! haveServer start a new server using xw.pl.
	# Note: this server allways runs on terminal provided via: 
	# GetVirtualTerminal()
	# ---
	$spec{haveServer} = $haveServer;
	if ($haveServer == 0) {
		my $vt = GetVirtualTerminal();
		my $XLoader = "$spec{X} $spec{Xmsg} -br -xf86config $spec{AutoConf}";
		my $XFlags  = "-allowMouseOpenFail -verbose 10 +accessx $vt";
		system("$XLoader $XFlags $disp >$spec{XpidFile}");
		if (! open(FD,"$spec{XpidFile}")) {
			die "xc: could not open file: $spec{XpidFile}";
		}
		while (<FD>) { 
			$spec{Xpid} = $_; 
		}
		close(FD);
		unlink($spec{XpidFile});

		# save terminal settings...
		# --------------------------
		my $terminal = 7;
		my $tty = Getty();
		unlink($spec{XTerminal});
		open (FD,">$spec{XTerminal}");
		print FD "TTyX:$terminal\n";
		print FD "TTyT:$tty\n";
		close(FD);

		# Hmm, is the server up... 
		# -------------------------
		if (CheckPID($spec{Xpid}) != 0) {
		# /.../
		# look if there are packages missing...
		# --------------------------------------
		my $install = "<none>";
		my $data = qx($spec{Sysp} -q 3d);
		if ($data =~ /Card3D0.*Install.*:(.*)\n.*/) {
			$install = $1;
			$install =~ s/ +//g;
		}
		if ($install ne "<none>") {
			print "xc: missing base package:\n";
			print "    please install the following package(s):\n";
			print "\n";
			print "    $install\n";   
			print "\n";
			print "xc: abort...\n";
		} else {
			# /.../
			# all packages there something serious happened...
			# -------------------------------------------------
			print "\n";
			print "xc: sorry could not start configuration server\n";
			print "xc: this could have the following reasons:\n";
			print "\n";
			print "  - the card is still not supported. To get\n";
			print "    further information about the driver status\n";
			print "    please have a look at:\n";
			print "\n";
			print "    http://www.xfree86.org/4.2.0/Status.html\n";
			print "\n";
			print "  - the card was not detected correctly. In\n"; 
			print "    this case please get the information which\n"; 
			print "    XFree86 module provide support for the card\n";
			print "    and set the module using the following\n"; 
			print "    command:\n";
			print "\n";
			print "    sax2 -m 0=<name of the module>\n";
			print "\n";
			print "    Note: 0 is a digit not a letter\n";
			print "    Note: Information about modules are\n"; 
			print "          provided on the mentioned WWW-Page\n";
			print "\n";
			print "  - the card was detected but the server fails\n";
			print "    to start in spite of the detection. In this\n";
			print "    case something more serious had happened.\n";
			print "    Please have a look at the log file:\n"; 
			print "\n";
			print "    $spec{LogFile}\n";
			print "\n";   
			print "xc: if you can not determine the cause\n"; 
			print "    please get in contact with:\n";
			print "\n";
			print "    <support\@suse.de>\n";
			print "\n";
			print "    Please include the log file and a detailed\n";
			print "    description into your e-mail\n";
			print "\n";
			print "xc: abort...\n";
		}
		Logger($spec{Xmsg},$logHandle);
		unlink($spec{Xmsg});
		Exit(0);
		}
		sleep (1);
	}
	# /.../
	# check package information via xapi and create the
	# initial option line used for startup
	# [sysp -q 3d]
	# ------------
	my @parent = getParentName();
	system("xsetroot -cursor_name watch -d $disp");
	my $apiopt = "-style platinum -w $FrameWidth";
	# 1)
	#=======================================
	# Check if we are called from YaST2
	#---------------------------------------
	if (defined $YaSTMode) {
		$apiopt = "$parent[1] -w $FrameWidth -y";
	}
	# 2)
	#=======================================
	# Check if we are called from SaX2
	#---------------------------------------
	if (($haveServer) && ($parent[0] eq "sax")) {
		$apiopt = "";
	}
	# 3)
	#=======================================
	# Check for package check flag
	#--------------------------------------- 
	if (! defined $NoCheckPacs) {
		system ("$spec{Xapi} $apiopt --checkpacs -display $disp");
	}

	# /.../
	# add the config file and the XFree86 log file
	# to the SaX.log file
	# ---
	if ($haveServer == 0) {
		my $glxinfo = qx (/usr/X11R6/bin/glxinfo -display $disp);
		Logger ($spec{Xmsg},$logHandle);
		Logger ("GLX Info:\n$glxinfo",$logHandle);
		unlink ($spec{Xmsg});
	}

	# /.../
	# run the do you like it program...
	# ----
	if (defined $FullScreen) {
		$apiopt = $apiopt." --fullscreen";
	}
	if (defined $NoBorder) {
		$apiopt = $apiopt." --noborder";
	}
	if (defined $UniFont) {
		$apiopt = $apiopt." --unifont";
	}
	if (defined $NoIntro) {
		$exit = 1;
	} else {
	if ($haveServer == 0) {
		system ("$spec{Xapi} $apiopt --info -display $disp");
		$exit = $? >> 8;
		my @xpid = GetPids();
		if (@xpid > 0) {
			@displayStatus = GetDisplay (@xpid,$spec{Corner});
			$disp = $displayStatus[0];
			if ($displayStatus[1] ne "grant") {
				$exit = 2;
			}
		} else {
			$exit = 2;
		}
	} else {
		$exit = 1;
	}
	}

	# /.../
	# handle the pseudo exit code from the do you
	# like it call
	# ---
	my $exitCode = 0;
	SWITCH: for ($exit) {
	/^1/          &&  do {
	#==========================
	# Run the X11 manager...
	#--------------------------
	if (open (OPT,">$spec{StartOptions}")) {
		print OPT $apiopt;
		close OPT;
	}
	if (defined $DialogToStartWith) {
		$apiopt = "$apiopt -O $DialogToStartWith";
	}
	if ($haveServer == 0) {
		if ((defined $StartWithSystemConfig) && (HeaderOK())) {
		system ("$spec{Xapi} $apiopt -display $disp");
		} else {
		system ("$spec{Xapi} $apiopt --usehwdata -display $disp");
		}
	} else {
		if (-f $spec{HWFlag}) {
		system ("$spec{Xapi} $apiopt --usehwdata -display $disp");
		} else {
		system ("$spec{Xapi} $apiopt -display $disp");
		}
	}
	$exitCode = $? >> 8;
	last SWITCH;
	};

	/^0/          &&  do {
	#==========================
	# Save...
	#--------------------------
	# recreate Module Section because some modues not used
	# for initial configuration are needed for final 
	# configuration
	# -------------
	@part2  = CreateModuleSection(\%var);
	open (HANDLE,">$spec{AutoConf}");
	print HANDLE @part0;  print HANDLE "\n";
	print HANDLE @part1;  print HANDLE "\n";
	print HANDLE @part2;  print HANDLE "\n";
	print HANDLE @part3;  print HANDLE "\n";
	print HANDLE @part4;  print HANDLE "\n";
	print HANDLE @part5;  print HANDLE "\n";
	print HANDLE @part6;  print HANDLE "\n";
	print HANDLE @part7;  print HANDLE "\n";
	print HANDLE @part8;  print HANDLE "\n";
	print HANDLE @part9;  print HANDLE "\n";
	print HANDLE @part10; print HANDLE "\n";
	close(HANDLE);

	$final = LinkConfiguration();
	$exitCode = 1;
	last SWITCH;
	};

	/^2/          &&  do {
	#==========================
	# Abort...
	#--------------------------
	last SWITCH;
	};
	}

	# kill the server...
	# ------------------
	if ($haveServer == 0) {
		Kill ($spec{Xpid});
	}
	CleanTmp();
	Exit ($exitCode);
}

#==================================================
# next there are all the functions needed to work
# in the main routine
#--------------------------------------------------
#---[ CreateSecureDir ]-----#
sub CreateSecureDir {
#----------------------------------------------
# this function create a secure tmp directory
# and return the name of the directory
#
	my $saxdir = "sax2-$$";
	my $result = mkdir("/tmp/$saxdir",0700);
	if ($result == 0) {
		print "xc: could not create tmp dir... abort\n";
		Exit(0);
	}
	return($saxdir);
}

#---[ CleanTmp ]----#
sub CleanTmp {
#----------------------------------------------
# this function send a signal to the process
# itself and forces removement of the tmp dir
#
	kill(15,$$);
}

#----[ init ]----#
sub init {
#------------------------------------
# init some data and test for root
#
	autoflush STDOUT 1;

	# secure directory and signals...
	# --------------------------------
	$TmpDir = CreateSecureDir();

	$SIG{HUP}   = "HandleTmpSignal";
	$SIG{INT}   = "HandleTmpSignal";
	$SIG{QUIT}  = "HandleTmpSignal";
	$SIG{ABRT}  = "HandleTmpSignal";
	$SIG{SEGV}  = "HandleTmpSignal";
	$SIG{PIPE}  = "HandleTmpSignal";
	$SIG{TERM}  = "HandleTmpSignal";

	$spec{WrapMouse}   = "/usr/X11R6/lib/sax/tools/wrap";
	$spec{Sysp}        = "/usr/X11R6/lib/sax/sysp.pl"; 
	$spec{DbmFile}     = "/var/cache/sax/files/config"; 
	$spec{HWFlag}      = "/var/cache/sax/files/hardware.chg";
	$spec{LogFile}     = "/var/log/SaX.log";
	$spec{XpidFile}    = "/var/log/SaX.pid";
	$spec{AutoConf}    = "/tmp/$TmpDir/XF86Config";
	$spec{Xmsg}        = "/var/log/xlog";
	$spec{X}           = "/usr/X11R6/lib/sax/xw.pl";
	$spec{Xapi}        = "/usr/X11R6/lib/sax/xapi";
	$spec{Intro}       = "/usr/X11R6/lib/sax/intro.pl";
	$spec{Corner}      = "/usr/X11R6/lib/sax/tools/corner";
	$spec{Xpid}        = undef;
	$spec{NewServer}   = undef;
	$spec{IntroPid}    = undef;
	$spec{Demo}        = "/usr/X11R6/lib/sax/tools/demo.sh";
	$spec{XTerminal}   = "/var/log/xvt";
	$spec{Twm}         = "/usr/X11R6/bin/twm";
	$spec{TwmRc}       = "/usr/X11R6/lib/sax/api/data/twmrc";
	$spec{StartOptions}= "/var/cache/sax/files/xapi.opt";

	# get options...
	# --------------
	undef ($YaSTMode);
	undef ($EnableAutoConf);
	undef ($Display);
	undef ($FullScreen);
	undef ($NoCheckPacs);
	undef ($NoBorder);
	undef ($UniFont);
	undef ($EnableXF86AutoMode);
	undef ($StartWithSystemConfig);
	undef ($DialogToStartWith);
	undef (%tune);

	my $result = GetOptions(
		"auto|a"         => \$EnableAutoConf,
		"yast|y"         => \$YaSTMode,
		"xmode|x"        => \$EnableXF86AutoMode,
		"sysconfig|s"    => \$StartWithSystemConfig,
		"fullscreen|f"   => \$FullScreen,
		"nocheckpacs|p"  => \$NoCheckPacs,
		"noborder"       => \$NoBorder,
		"unifont"        => \$UniFont,
		"nointro|n"      => \$NoIntro,
		"framewidth|w=i" => \$FrameWidth,
		"dialog|O=s"     => \$DialogToStartWith,
		"help|h"         => \&usage,
		"<>"             => \&usage
	);
	if ( $result != 1 ) {
		usage();
	}

	#--------------#
	# test on root #
	#--------------#
	my $user = qx(whoami);
	if ($user !~ /root/i) {
		print "xc: only root can do this\n";
		exit(0);
	}
}

#----[ reaper ]----#
sub reaper {
#------------------------------------------
# prevent us from zombies, using wait
#
	my $waitedpid = wait;
	$SIG{CHLD} = \&reaper;
}

#----[ prepare ]----#
sub prepare {
#------------------------------------------
# read saved hash data from file
#
	my $hashref;
	if (! -s $spec{DbmFile}) {
	print "xc: could not open tree: $spec{DbmFile}\n";
	return(-1);
	}
	eval {
		$hashref = retrieve($spec{DbmFile});
	};
	if (! defined $hashref) {
	print "xc: could not open tree: $spec{DbmFile}\n";
	return(-1);
	}
	%var = %{$hashref};
	return(0);
}


#----[ save ]--------#
sub save {
#-------------------------------------------
# save hash data as file $spec{DbmFile}
#
	unlink($spec{DbmFile});
	if(!store(\%var,$spec{DbmFile})) {
		print "init: could not create tree: $spec{DbmFile}\n";
		return(-1);
	}
	return(0);
}

#---[ HandleTmpSignal ]-----#
sub HandleTmpSignal {
#----------------------------------------------
# signal function to remove tmp stuff
#
	qx(rm -rf /tmp/$TmpDir);
	qx(rm -rf /tmp/sysp-*);
}

#---[ Logger ]-------#
sub Logger {
#----------------------------------------------------- 
# append log file data to LOG handler...
#
	my $data   = $_[0];
	my $handle = $_[1];
	my $date   = qx (
		LANG=POSIX /bin/date "+%d-%h %H:%M:%S"
	);
	$date =~ s/\n$//;
	if (! defined $handle) {
		$handle = STDOUT;
	}
	if (-f $data) {
		#===============================
		# got file for logging
		#-------------------------------
		print $handle "$date <X> Logging File contents: $data [\n";
		my $xf = new FileHandle;
		if ($xf->open("$data")) {
		while (my $line=<$xf>) {
			$line =~ s/^\n//;
			$line =~ s/\n$//;
			$line =~ s/^\t+//g;
			print $handle "\t$line\n";
		}
		}
		print $handle "]\n";
		$xf->close;
	} else {
		#===============================
		# got string data for logging
		#-------------------------------
		print $handle "$date <X> ";
		my $lineWrap = 0;
		$data =~ s/^\n//;
		$data =~ s/\n$//;
		if ($data =~ /\n+/) {
			$lineWrap = 1;
		}
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
}

#--[ LinkConfiguration ]-----#
sub LinkConfiguration {
#-----------------------------------------------------
# this function is called to copy the temporar config
# file to /etc/X11/XF86Config and create the symbolic 
# X links
# 
	my $final  = "/etc/X11/XF86Config";
	my $save   = "/etc/X11/XF86Config.saxsave";
	my $server = "/usr/X11R6/bin/XFree86";

	if (-f $final) {
		qx(cp $final $save);
	}

	# save configuration to /etc/X11...
	# ----------------------------------
	qx(cp $spec{AutoConf} $final);

	# create links...
	# ----------------
	if (-f $server) {
		qx(rm -f /usr/X11R6/bin/X);
		qx(rm -f /var/X11R6/bin/X);
		qx(ln -s /var/X11R6/bin/X /usr/X11R6/bin/X);
		qx(ln -s $server /var/X11R6/bin/X);
	}
	unlink ($spec{HWFlag});
	return ($final);
}

#----[ Exit ]--------#
sub Exit {
#------------------------------------------
# exit function wrapper which close the
# LOG handler first...
#
	my $code = $_[0];
	CloseLog ($logHandle);
	qx (rm -rf $TmpDir);
	exit ($code);
}

#----[ Getty ]-------#
sub Getty {
#--------------------------------------------------
# look for the terminal ID and return it. Please
# note this function has to return a number
#
	my $tty;
	$tty = qx(tty | cut -f2 -dy);
	return($tty);
}

#----[ CheckPID ]----#
sub CheckPID {
#--------------------------------------------------
# check if the given process id still exist in
# the process table
#
	my $pid = $_[0];
	my $exit;

	if ($pid eq "") {
		return(1);
	}
	sleep(2); qx(ps $pid);
	$exit = $? >> 8;
	return($exit);
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
		die "xc: could not create LogFile: $spec{LogFile}";
	}

	my $LOG = $io->fileno;
	open(STDERR,">&$LOG") ||
	die "could not duplicate LOG on stderr";

	binmode($io, ':unix');
	return ($io);
}

#----[ CloseLog ]----#
sub CloseLog {
#--------------------------------------------------
# close current log file descriptor...
#
	my $io = $_[0];
	if (defined $io) {
		$io->close;
	}
}

#---[ Kill ]-------#
sub Kill {
#--------------------------------------
# my own kill function to make sure
# the process is really killed
#
	my $pid = $_[0];
	my $err;

	if ($pid eq "") {
		return;
	}
	$err = system("kill -15 $pid");
	$err = $? >> 8;
	if ($err > 0) {
		Logger ("die hard on pid: $pid",$logHandle);
		sleep(1);
		system("kill -9 $pid");
	}
}

#---[ headerOK ]------#
sub headerOK {
#-------------------------------------------
# check the XF86Config header if the given
# file was created using SaX2 / ISaX
#
	my $file = $_[0];
	my $hunk = "SaX generated XFree86 config file";

	# /.../
	# first check if the file exist and could
	# be opened
	# ---
	if (! -f $file) {
		return (0);
	}
	if (! open (FD,$file)) {
		return (0);
	}
	while (<FD>) {
	if ($_ =~ /$hunk/) {
		close (FD);
		return (1);
	}
	}
	close (FD);
	return (0);
}

#---[ getParentName ]------#
sub getParentName {
#-------------------------------------------
# get the name of the parent process calling
# this script
#
	my @return;
	my $parent = getppid();
	my $name = qx ( cat /proc/$parent/cmdline );
	$name =~ /\/.*\/+(.*)/;
	my @list = split(/\./,$1);
	my $base = shift (@list);
	my $cmdline = qx (
		cat /proc/$parent/cmdline
	);
	chomp ($base);
	push(@return,$base);

	if ($cmdline =~ /.*-font(.*)-iso10646-1/) {
		push(@return,"-fn $1-iso10646-1");
	}
	return (
		@return
	);
}

#---[ HeaderOK ]----#
sub HeaderOK {
#--------------------------------------------
# check if the header of the config file is
# create from SaX2/ISaX... otherwhise this
# file shouldn't be read
#
	my $config = "/etc/X11/XF86Config";
	open (FD,$config) || return 0;
	my $check = <FD>;
	chomp ($check);
	close (FD);
	if ($check eq "# /.../") {
		return (1);
	}
	return (0);
}

#---[ usage ]------#
sub usage {
#--------------------------------
# if you need help :-)
#
	print "Linux SaX Version 4.8 level (xc) (2002-02-19)\n";
	print "(C) Copyright 2002 - SuSE GmbH\n";
	print "\n";

	print "usage: xc [ options ]\n";
	print "options:\n";
	print "[ -a | --auto ]\n";
	print "   enable automatic configuration\n";
	print "[ -x | --xmode ]\n";
	print "   do not use sax calculated Modelines,\n";
	print "   use the server generated once instead\n";
	print "[ -s | --sysconfig ]\n";
	print "   this option will only take effect if xc\n";
	print "   has to start its own X-Server. It will than\n";
	print "   read the system installed config file\n";
	print "   instead of the HW detection data\n" ;
	print "--\n";
	Exit(0);
}
