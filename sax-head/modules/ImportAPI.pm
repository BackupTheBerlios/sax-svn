# /.../
# Copyright (c) 2001 SuSE GmbH Nuernberg, Germany.  All rights reserved.
# Author: Marcus Schaefer <sax@suse.de>, 2001
#
# ImportAPI.pm configuration level 2
# provide read functions to incorp the API variable settings
# into the global var hash of the configuration 
#
# CVS ID:
# --------
# Status: Up-to-date
#
use lib '/usr/share/sax/modules/detect';
use lib '/usr/X11R6/lib/xfine/modules';
use lib '/usr/share/sax/modules';

use strict;
use XFineControl;
use Fonts;

#========================================
# Globals...
#----------------------------------------
my $ImportXFineCache = "no";

#========================================
# module spanned globals...
#----------------------------------------
our %import;
our %spec;

#========================================
# define the minimum specifications to 
# use AutoDetectFontPath()
#----------------------------------------
if (! defined %spec) {
	my $dataDir = "/usr/share/sax/api/data";
	$spec{StaticFontPathList}  = "$dataDir/StaticFontPathList";
	$spec{LangFirstPath}       = "$dataDir/LangFirstPath";
	$spec{LangCodes}           = "$dataDir/LangCodes";
	$spec{RcConfig}            = "/etc/rc.config";
}

#---[ ApiInit ]-----#
sub ApiInit {
#------------------------------------------
# this function is called to initialize the
# Api import filter
#
	$import{ApiFile}    = "/var/cache/sax/files/apidata";
	$import{Xmode}      = "/sbin/xmode";
	$import{Sysp}       = "/sbin/sysp";
	$import{FbSet}      = "fbset";
}

#---[ ApiRead ]-----#
sub ApiRead {
#--------------------------------------------
# this function is called to read parts of the
# api file to be handled from the import 
# functions
#
	my $filename = $import{ApiFile};
	my %result;
	my $line;
	my $name;
	my $key;
	my $value;
	my @file;

	@file = ();
	open ("DATA",$filename);
	while ($line=<DATA>) {
		chomp($line);
		push(@file,$line);
	}
	close (DATA);
	foreach $line (@file) {
	if (($line =~ /^#/) || ($line eq "")) {
		next;
	}
	if ($line =~ /(.*)\{/) {
		# /.../
		# get the topic line...
		# ---
		$name   = $1;
		$name   =~ s/^ +//g; $name   =~ s/ +$//g;
	} elsif ($line =~ /(.*)=(.*)/) {
		# /.../
		# get the values behind this topic...
		# ---
		$key   = $1;
		$value = $2;
		$key   =~ s/^ +//g; $key   =~ s/ +$//g;
		$value =~ s/^ +//g; $value =~ s/ +$//g;
		if (defined $name) {
			$result{$name}{$key} = $value;
		}
	}
	}
	return(%result);
}

#---[ ApiImportPath ]------# 
sub ApiImportPath {
#--------------------------------------------
# import path settings to create the
# Files Modules and ServerFlags sections
#
	my %api = %{$_[0]};
	my ($value,$key,$card);
	my (@list,@opt,@pair);
	my %var;

	foreach (keys %{$api{Path}}) {
	$value = $api{Path}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];
	SWITCH: for ($key) {
		/^FontPath/      && do {
		if ($value eq "YaST2") {
		%var = AutoDetectFontPath(\%spec,\%var);
		} else {
		$var{Files}{0}{FontPath} = $value;
		}
		last SWITCH;
		};

		/^RgbPath/       && do {
		$var{Files}{0}{RgbPath} = $value;
		last SWITCH;
		};

		/^ModulePath/    && do {
		$var{Files}{0}{ModulePath} = $value;
		last SWITCH;
		};

		/^ModuleLoad/    && do {
		$var{Module}{0}{Load} = $value;
		last SWITCH;
		};

		/^Extmod/        && do {
		@list = split(/\\n/,$value);
		foreach (@list) {
			@opt = split(/,/,$_);
			$var{Module}{0}{$opt[0]}{Option} = $opt[1];
		}
		last SWITCH;
		};

		/^SpecialFlags/  && do {
		@list = split(/\\n/,$value);
		foreach (@list) {
			@opt = split(/,/,$_);
			$var{ServerFlags}{0}{$opt[0]} = $opt[1];
		}
		last SWITCH;
		};

		/^ServerFlags/   && do {
		$var{ServerFlags}{0}{Option} = $value;
		last SWITCH;
		}; 
	}
	}
	return(%var);
}

#---[ ApiImportExtensions ]------# 
sub ApiImportExtensions {
#--------------------------------------------
# import Extensions settings to create the
# Extensions section
#
	my %api = %{$_[0]};
	my ($value,$key,$card);
	my (@list,@opt,@pair);
	my %var;
	
	foreach (keys %{$api{Extensions}}) {
	$value = $api{Extensions}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];
	SWITCH: for ($key) {
		/^SpecialExtensions/  && do {
		@list = split(/\\n/,$value);
		foreach (@list) {
			@opt = split(/,/,$_);
			$var{Extensions}{0}{$opt[0]} = $opt[1];
		}
		last SWITCH;
		};

		/^Extensions/   && do {
		$var{Extensions}{0}{Option} = $value;
		last SWITCH;
		};
	}
	}
	return(%var);
}

#---[ ApiImportKeyboard ]------#
sub ApiImportKeyboard {
#--------------------------------------------
# import keyboard settings to create the
# according InputDevice section
#
	my %api = %{$_[0]};
	my (@list,@opt,@pair);
	my ($value,$key,$card);
	my %var;

	foreach (keys %{$api{Keyboard}}) {
	$value = $api{Keyboard}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];
	SWITCH: for ($key) {

		/^Identifier/     && do {
		$var{InputDevice}{0}{Identifier} = $value;    
		last SWITCH;
		};

		/^Driver/         && do {
		$var{InputDevice}{0}{Driver} = $value; 
		last SWITCH;
		};

		/^Protocol/       && do {
		$var{InputDevice}{0}{Option}{Protocol} = $value;
		last SWITCH;
		};

		/^MapName/        && do {
		$var{InputDevice}{0}{Option}{MapName} = $value;
		last SWITCH;
		};

		/^XkbKeycodes/    && do {
		$var{InputDevice}{0}{Option}{XkbKeycodes} = $value;
		last SWITCH;
		};

		/^XkbGeometry/    && do {
		$var{InputDevice}{0}{Option}{XkbGeometry} = $value;
		last SWITCH;
		};

		/^XkbSymbols/     && do {
		$var{InputDevice}{0}{Option}{XkbSymbols} = $value;
		last SWITCH;
		};

		/^XkbRules/       && do {
		$var{InputDevice}{0}{Option}{XkbRules} = $value;
		last SWITCH;
		};

		/^XkbModel/       && do {
		$var{InputDevice}{0}{Option}{XkbModel} = $value;
		last SWITCH;
		};

		/^XkbLayout/      && do {
		$var{InputDevice}{0}{Option}{XkbLayout} = $value;
		last SWITCH;
		};

		/^XkbVariant/     && do {
		$var{InputDevice}{0}{Option}{XkbVariant} = $value;
		last SWITCH;
		};

		/^XkbOptions/     && do {
		$var{InputDevice}{0}{Option}{XkbOptions} = $value;
		last SWITCH;
		};

		/^AutoRepeat/     && do {
		if ($value ne "50 5") {
			$var{InputDevice}{0}{Option}{AutoRepeat} = $value;
		}
		last SWITCH;
		};

		/^Xleds/          && do {
		$var{InputDevice}{0}{Option}{Xleds} = $value;
		last SWITCH;
		};
             
		/^XkbDisable/     && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{0}{Option}{XkbDisable} = "on";
		} else {
			$var{InputDevice}{0}{Option}{XkbDisable} = "";
		}
		last SWITCH;
		};
        
		/^VTSysReq/       && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{0}{Option}{VTSysReq} = "on";
		} else {
			$var{InputDevice}{0}{Option}{VTSysReq} = "";
		}
		last SWITCH;
		};
          
		/^VTInit/         && do {
		$var{InputDevice}{0}{Option}{VTInit} = $value;
		last SWITCH;
		};
            
		/^ServerNumLock/  && do {     
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{0}{Option}{ServerNumLock} = "on";
		} else {
		$var{InputDevice}{0}{Option}{ServerNumLock} = "off";
		}
		last SWITCH;
		};

		/^LeftAlt/        && do {
		$var{InputDevice}{0}{Option}{LeftAlt} = $value;
		last SWITCH;
		};
           
		/^RightAlt/       && do {
		$var{InputDevice}{0}{Option}{RightAlt} = $value;
		last SWITCH;
		};
          
		/^ScrollLock/     && do {
		$var{InputDevice}{0}{Option}{ScrollLock} = $value;
		last SWITCH;
		};
        
		/^RightCtl/       && do {
		$var{InputDevice}{0}{Option}{RightCtl} = $value;
		last SWITCH;
		};
          
		/^XkbKeyCodes/    && do {
		if ($value !~ /default/i) {
			$var{InputDevice}{0}{Option}{XkbKeyCodes} = $value;
		}
		last SWITCH;
		};
   
		/^XkbCompat/    && do {
		$var{InputDevice}{0}{Option}{XkbCompat} = $value;
		last SWITCH;
		};

		/^XkbKeymap/    && do {
		$var{InputDevice}{0}{Option}{XkbKeymap} = $value;
		last SWITCH;
		};
	}
	}
	return(%var);
}

#---[ ApiImportMouse ]------#
sub ApiImportMouse {
#--------------------------------------------
# import mouse settings to create the
# according InputDevice sections
#
	my %api = %{$_[0]};
	my (@list,@opt,@pair,@optlist,@special);
	my ($value,$key,$card,$opt,$optname,$optval);
	my %var;

	foreach (keys %{$api{Mouse}}) {
	$value = $api{Mouse}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];

	SWITCH: for ($key) {
		/^Identifier/               && do {
		$var{InputDevice}{$card}{Identifier} = $value;
		last SWITCH;
		};

		/^Driver/                   && do {
		$var{InputDevice}{$card}{Driver} = $value;
		last SWITCH;
		};

		/^Protocol/                 && do {
		if ($value !~ /none/i) {
			$var{InputDevice}{$card}{Option}{Protocol} = $value;
		}
		last SWITCH;
		};

		/^Device/                   && do {
		$var{InputDevice}{$card}{Option}{Device} = $value;
		last SWITCH;
		};

		/^BaudRate/i                && do {
		if ($value != /none/i) {
			$var{InputDevice}{$card}{Option}{BaudRate} = $value;
		}
		last SWITCH;
		};

		/^Samplerate/               && do {
		if ($value > 60) {
			$var{InputDevice}{$card}{Option}{SampleRate} = $value;
		}
		last SWITCH;
		};

		/^Emulate3Buttons/          && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{$card}{Option}{Emulate3Buttons} = "on";
		} else {
			$var{InputDevice}{$card}{Option}{Emulate3Buttons} = "";
		}
		last SWITCH;
		};

		/^Emulate3Timeout/          && do {
		if ($var{InputDevice}{$card}{Option}{Emulate3Buttons} eq "on") {
			$var{InputDevice}{$card}{Option}{Emulate3Timeout} = $value;
		}
		last SWITCH;
		};

		/^ChordMiddle/              && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{$card}{Option}{ChordMiddle} = "on";
		} else {
			$var{InputDevice}{$card}{Option}{ChordMiddle} = "";
		}
		last SWITCH;
		};

		/^Option/                   && do {
		@optlist = split(/,/,$value);
		foreach $opt (@optlist) {
			$var{InputDevice}{$card}{Option}{$opt} = "on";
		}
		last SWITCH;
		};

		/^RawOption/                   && do {
		@optlist = split(/,/,$value);
		foreach $opt (@optlist) {
			@special = split(/ +/,$opt);
			$optname = shift (@special);
			$optname =~ s/\"+//g;
			$optval  = join (" ",@special);
			$var{InputDevice}{$card}{Option}{$optname} = $optval;
		}
		last SWITCH;
		};

		/^InputFashion/             && do {
		$var{InputDevice}{$card}{Option}{InputFashion} = $value;
		last SWITCH;
		};

		/^Load/                     && do {
		$var{InputDevice}{$card}{Option}{Load} = $value;
		last SWITCH;
		};

		/^MinX/                     && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MinX} = $value;
		}
		last SWITCH;
		};

		/^MinimumXPosition/         && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MinimumXPosition} = $value;
		}
		last SWITCH;
		};

		/^MaxX/                     && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MaxX} = $value;
		}
		last SWITCH;
		};

		/^MaximumXPosition/         && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MaximumXPosition} = $value;
		}
		last SWITCH;
		};

		/^MinY/                     && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MinY} = $value;
		}
		last SWITCH;
		};

		/^MinimumYPosition/         && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MinimumYPosition} = $value;
		}
		last SWITCH;
		};

		/^MaxY/                     && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MaxY} = $value;
		}
		last SWITCH;
		};

		/^MaximumYPosition/    && do {
		if ($value >= 0) {
			$var{InputDevice}{$card}{Option}{MaximumYPosition} = $value;
		}
		last SWITCH;
		};

		/^TopX/                     && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{TopX} = $value;
		}
		last SWITCH;
		};

		/^TopY/                     && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{TopY} = $value;
		}
		last SWITCH;
		};

		/^BottomX/                  && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{BottomX} = $value;
		}
		last SWITCH;
		};

		/^BottomY/                  && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{BottomY} = $value;
		}
		last SWITCH;
		};

		/^Suppress/                 && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{Suppress} = $value;
		}
		last SWITCH;
		};

		/^Serial/                   && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{Serial} = $value;
		}
		last SWITCH;
		};

		/^ScreenNumber/             && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{ScreenNumber} = $value;
		}
		last SWITCH;
		};

		/^ScreenNo/                 && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{ScreenNo} = $value;
		}
		last SWITCH;
		};

		/^ReportingMode/            && do {
		if ($value !~ /none/i) {
			$var{InputDevice}{$card}{Option}{ReportingMode} = $value;
		}
		last SWITCH;
		};

		/^Rotation/                 && do {
		$var{InputDevice}{$card}{Option}{Rotation} = $value;
		last SWITCH;
		};

		/^TabletMode/               && do {
		if ($value !~ /none/i) {
			$var{InputDevice}{$card}{Option}{Mode} = $value;
		}
		last SWITCH;
		};

		/^TabletType|TouchType/     && do {
		if ($value !~ /none/i) {
			$var{InputDevice}{$card}{Option}{Type} = $value;
		}
		last SWITCH;
		};

		/^ButtonNumber/             && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{ButtonNumber} = $value;
		}
		last SWITCH;
		};

		/^Buttons/                  && do {
		if ($value > 0) {
			$var{InputDevice}{$card}{Option}{Buttons} = $value;
		}
		last SWITCH;
		};

		/^ButtonThreshold/          && do {
		if ($value > 10) {
			$var{InputDevice}{$card}{Option}{ButtonThreshold} = $value;
		} else {
			$var{InputDevice}{$card}{Option}{ButtonThreshold} = "";
		}
		last SWITCH;
		};

		/^SendCoreEvents/           && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{$card}{Option}{SendCoreEvents} = "on";
		} else {
			$var{InputDevice}{$card}{Option}{SendCoreEvents} = "";
		}
		last SWITCH;
		};

		/^ClearDTR/                 && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{$card}{Option}{ClearDTR} = "on";
		} else {
			$var{InputDevice}{$card}{Option}{ClearDTR} = "";
		}
		last SWITCH;
		};

		/^ClearRTS/                 && do {
		if ($value =~ /yes|on/i) {
			$var{InputDevice}{$card}{Option}{ClearRTS} = "on";
		} else {
			$var{InputDevice}{$card}{Option}{ClearRTS} = "";
		}
		last SWITCH;
		};

		/^ZAxisMapping/             && do {
		if (($value !~ /off/i) && ($value !~ /none/i)) {
			$var{InputDevice}{$card}{Option}{ZAxisMapping} = $value;
		}
		last SWITCH;
		};

		/^EmulateWheel/             && do {
		if ($value !~ /no/) {
			$var{InputDevice}{$card}{Option}{EmulateWheel} = "on";
			$var{InputDevice}{$card}{Option}{EmulateWheelButton} = $value;
		}
		last SWITCH;
		};

		/^Vendor/                   && do {
		$var{InputDevice}{$card}{Option}{Vendor} = $value;
		last SWITCH;
		};

		/^Name/                     && do {
		$var{InputDevice}{$card}{Option}{Name} = $value;
		last SWITCH;
		};
		}
	}
	return(%var);
}

#---[ ApiImportCard ]------#
sub ApiImportCard {
#--------------------------------------------
# import card settings to create the 
# according Device sections
#
	my %api = %{$_[0]};
	my (@list,@opt,@pair,@elements);
	my ($value,$key,$card,$va,$el,$count);
	my %var;

	foreach (keys %{$api{Card}}) {
	$value = $api{Card}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];

	SWITCH: for ($key) {
		/^Identifier/      && do {
		$var{Device}{$card}{Identifier} = $value;
		last SWITCH;
		};

		/^Driver/          && do {
		$var{Device}{$card}{Driver} = $value;
		last SWITCH;
		};

		/^Memory/          && do {
		if ($value !~ /auto/i) {
			$var{Device}{$card}{Videoram} = $value;
		}
		last SWITCH;
		};

		/^Rotate/          && do {
		$var{Device}{$card}{Special}{Rotate} = $value;
		last SWITCH;
		};

		/^BusID/           && do {
		if ($value !~ /single/i) {
			$var{Device}{$card}{BusID} = $value;
		}
		last SWITCH;
		};

		/^Vendor/          && do {
		$var{Device}{$card}{VendorName} = $value; 
		last SWITCH;
		};

		/^Name/            && do {
		$var{Device}{$card}{BoardName} = $value;
		last SWITCH;
		};

		/^DacChip/         && do {
		if ($value !~ /auto/i) {
			$var{Device}{$card}{Ramdac} = $value;
		}
		last SWITCH;
		};

		/^GraphicsChip/    && do {
		if ($value !~ /auto/i) {
			$var{Device}{$card}{Chipset} = $value;
		}
		last SWITCH;
		};
	
		/^ClockChip/       && do {
		if ($value !~ /auto/i) {
			$var{Device}{$card}{ClockChip} = $value;
		}
		last SWITCH;
		};

		/^Clocks/          && do {
		$var{Device}{$card}{Clocks} = $value;
		last SWITCH;
		};

		/^Option/          && do {
		$var{Device}{$card}{Option} = $value;
		last SWITCH;
		};

		/^RawData/         && do {
		@list = split(/,/,$value);
		@list = CheckSplit (@list);
		$count = 0;
		foreach (@list) {
			@elements = split(/ +/,$_);
			$el = $elements[0];
			$va = $_; $va =~ s/$el//;
			$var{Device}{$card}{Raw}{$count}{$el} = $va;
			$count++;
		}
		last SWITCH;
		};

		/^Screen/          && do {
		$var{Device}{$card}{Screen} = $value;
		last SWITCH;
		};
		}
	}
	return(%var);
}

#---[ ApiImportDesktop ]------#
sub ApiImportDesktop {
#--------------------------------------------
# import desktop settings to create the
# according Screen Monitor and Modes sections
#
	my %api    = %{$_[0]};
	my (@list,@opt,@pair,@modelist,@elements,@tmode,@newsync);
	my ($value,$key,$card,$res,$k,$el,$va,$name,$timing,$color,$count);
	my %var;
	my %data;

	foreach (keys %{$api{Desktop}}) {
	$value = $api{Desktop}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];

	SWITCH: for ($key) {
		/^MonitorOptions/     && do {
		$var{Monitor}{$card}{Option} = $value;
		last SWITCH;
		};

		/^Identifier/         && do {
		$var{Screen}{$card}{Identifier} = $value;   
		last SWITCH;
		};

		/^Device/             && do {
		$var{Screen}{$card}{Device} = $value;
		last SWITCH;
		};

		/^Monitor/            && do {
		$var{Monitor}{$card}{Identifier}  = $value;
		$var{Screen}{$card}{Monitor} = $value;
		last SWITCH;
		};

		/^VendorName/         && do {
		$var{Monitor}{$card}{VendorName} = $value;
		last SWITCH;
		};

		/^ModelName/          && do {
		$var{Monitor}{$card}{ModelName} = $value;
		last SWITCH;
		};

		/^Modes/              && do {
		$k = "$card Driver";
		$color = $key;
		$color =~ s/Modes://;
		$data{$card}{modes}{$value} = "";

		# bad hack for ppc, if fbdev is used we need the Modes line
		# it must not set to default in this case
		# ----------------------------------------
		my $arch = qx(/bin/arch);
		if (($api{Card}{$k} =~ /fbdev/i) && ($arch !~ /ppc/i)) {
			$var{Screen}{$card}{Depth}{$color}{Modes} = "default";
		} else {
			$var{Screen}{$card}{Depth}{$color}{Modes} = $value;
		}
		last SWITCH;
		};

		/^HorizSync/          && do {
		$var{Monitor}{$card}{HorizSync} = $value;
		$data{$card}{hsync} = $value;
		last SWITCH;
		};

		/^VertRefresh/        && do {
		$var{Monitor}{$card}{VertRefresh} = $value;
		$data{$card}{vsync} = $value;
		last SWITCH;
		};

		/^DisplaySize/        && do {
		$var{Monitor}{$card}{DisplaySize} = $value;
		last SWITCH;
		};

		/^Modelines/          && do {
		$data{$card}{readline} = $value;
		last SWITCH;
		};

		/^SpecialModeline/    && do {
		$data{$card}{specialline} = $value;
		last SWITCH;
		};

		/^ColorDepth/         && do {
		$data{$card}{depth} = $value;
		$var{Screen}{$card}{DefaultDepth} = $value;
		last SWITCH;
		};

		/^Virtual/            && do {
		$color = $key;
		$color =~ s/Virtual://;
		if ($value ne "") {
			$var{Screen}{$card}{Depth}{$color}{Virtual}  = $value;
		}
		last SWITCH;
		};
	
		/^Visual/             && do {
		$color = $key;
		$color =~ s/Visual://;
		if ($value ne "") {
			$var{Screen}{$card}{Depth}{$color}{Visual}   = $value;
		}
		last SWITCH;
		};
		
		/^ViewPort/           && do {
		$color = $key;
		$color =~ s/ViewPort://;
		if ($value ne "") {
			$var{Screen}{$card}{Depth}{$color}{ViewPort} = $value;
		}
		last SWITCH;
		};

		/^ScreenOptions/      && do {
		$color = $key;
		$color =~ s/ScreenOptions://;
		if ($value ne "") {
			$var{Screen}{$card}{Depth}{$color}{Option}   = $value;
		}
		last SWITCH;
		};

		/^ScreenRawLine/      && do {
		$color = $key;
		$color =~ s/ScreenRawLine://;
		@list = split(/,/,$value);
		$count = 0;
		foreach (@list) {
			@elements = split(/ +/,$_);
			$el = $elements[0];
			$va = $_; $va =~ s/$el//;
			$var{Screen}{$card}{Depth}{$color}{Raw}{$count}{$el} = $va;
			$count++;
		}
		last SWITCH;
		};

		/^CalcModelines/      && do {
		if ($value =~ /yes|on/i) {
			$data{$card}{calc} = "yes";
		} else {
			$data{$card}{calc} = "no";
		}
		last SWITCH;
		};

		/^ImportXFineCache/   && do {
		if ($value =~ /yes|on/i) {
			$ImportXFineCache = "yes";
		}
		last SWITCH;
		};

		/^CalcAlgorithm/      && do {
		$var{Monitor}{$card}{CalcAlgorithm} = $value;
		if ($value =~ /CheckDesktopGeometry/i) {
			$data{$card}{algorithm} = "secure";
		} elsif ($value =~ /IteratePrecisely/i) {
			$data{$card}{algorithm} = "normal";
		} elsif ($value =~ /UseFrameBufferTiming/i) {
			$data{$card}{algorithm} = "fbdev";
		} else {
			$data{$card}{algorithm} = "secure";
			$data{$card}{calc} = "no";
		}
		last SWITCH;
		};
		}
	}  

	# handle data hash to calculate the modelines...
	# -----------------------------------------------
	foreach (keys %data) {
		@modelist = ();
		foreach (keys %{$data{$_}{modes}}) {
			push(@modelist,$_);
		}
		$res   = join(",",@modelist);
		$card  = $_;
		@tmode = ();
		@modelist = GetModelines(
			$res,
			$data{$_}{hsync},
			$data{$_}{vsync},
			$data{$_}{readline},
			$data{$_}{specialline},
			$data{$_}{algorithm},
			$data{$_}{calc},
			$card
		);

		$count = 0;
		delete $var{Monitor}{$card}{Modeline};
		foreach (@modelist) {
			@elements = split(/ +/,$_);
			$name   = $elements[1]; 
			$name   =~ s/\"//g; 
			$timing = $_; $timing =~ s/$elements[0]//; $timing =~ s/$name//;
			$timing =~ s/\"//g;
			$timing =~ s/^ +//g;
			$timing =~ s/ +$//g;
			$var{Monitor}{$card}{Modeline}{$count}{$name} = $timing;
			push(@tmode,$timing);
			$count++;
		}

		my $method = $var{Monitor}{$card}{CalcAlgorithm};
		if (($method ne "IteratePrecisely") && ($method ne "XServerPool")) {
			@newsync = CheckSyncRange (
				@tmode, $var{Monitor}{$card}{VertRefresh}, 
				$var{Monitor}{$card}{HorizSync}
			);
			$var{Monitor}{$card}{VertRefresh} = "$newsync[0]-$newsync[1]"; 
			$var{Monitor}{$card}{HorizSync}   = "$newsync[2]-$newsync[3]";
		}
	}
	return(%var);
}

#---[ ApiImportLayout ]------#
sub ApiImportLayout {
#--------------------------------------------
# import the layout settings to create the
# according ServerLayout section
#
	my %api = %{$_[0]};
	my (@list,@opt,@pair,@geo);
	my ($value,$key,$card,$i,$count,$screen,$id);
	my %var;

	my %entity;
	$screen = 0;
	foreach (sort keys %{$api{Layout}}) {
	$value = $api{Layout}{$_};
	@pair = split(/ /,$_);
	$card = $pair[0];
	$key  = $pair[1];

	SWITCH: for ($key) {
		/^Identifier/       && do {
		$var{ServerLayout}{all}{Identifier} = $value;
		last SWITCH;
		};

		/^Keyboard/         && do {
		@list = split(/,/,$value);
		$var{ServerLayout}{all}{InputDevice}{0}{id}    = $list[0];
		$var{ServerLayout}{all}{InputDevice}{0}{usage} = "CoreKeyboard";

		$count = 2;
		for ($i=1;$i<@list;$i++) {
		$var{ServerLayout}{all}{InputDevice}{$count}{id}    = $list[$i];
		$var{ServerLayout}{all}{InputDevice}{$count}{usage} = "CoreKeyboard";
		$count+=2;
		}
		last SWITCH;
		};

		/^InputDevice/      && do {
		@list = split(/,/,$value);
		$var{ServerLayout}{all}{InputDevice}{1}{id}    = $list[0];
		$var{ServerLayout}{all}{InputDevice}{1}{usage} = "CorePointer";
		my $driver = $api{Mouse}{"1 Driver"};
		my $device = $api{Mouse}{"1 Device"};
		$entity{$driver}{$device} = 1;
		
		$count = 3;
		for ($i=1;$i<@list;$i++) {
			my $driver = $api{Mouse}{"$count Driver"};
			my $device = $api{Mouse}{"$count Device"};
			if ((! defined $entity{$driver}{$device}) || ($device =~ "ttyS")) {
				my $l = "ServerLayout";
				$var{$l}{all}{InputDevice}{$count}{id}    = $list[$i];
				$var{$l}{all}{InputDevice}{$count}{usage} = "SendCoreEvents";
			}
			$entity{$driver}{$device} = $count;
			$count+=2;
		}
		last SWITCH;
		};

		/^Xinerama/         && do {
		if ($value =~ /on/i) {
			$var{ServerLayout}{all}{Option}{Xinerama} = "on";
		} else {
			$var{ServerLayout}{all}{Option}{Xinerama} = "off";
		}
		last SWITCH;
		};

		/^Clone/            && do {
		if ($value =~ /on/i) {
			$var{ServerLayout}{all}{Option}{Clone} = "on";
		} else {
			$var{ServerLayout}{all}{Option}{Clone} = "off";
		}
		last SWITCH;
		};

		/^VNC/              && do {
		$var{ServerLayout}{all}{Option}{VNC} = $value;
		my @idlist = split (/ /,$value);
		if ($idlist[0] > 0) {
			my $mID = $idlist[0];
			$var{ServerLayout}{all}{InputDevice}{$mID}{id} = "Mouse[$mID]";
			$var{ServerLayout}{all}{InputDevice}{$mID}{usage} = "ExtraPointer";
		}
		if ($idlist[1] > 0) {
			my $kID = $idlist[1];
			$var{ServerLayout}{all}{InputDevice}{$kID}{id} = "Keyboard[$kID]";
			$var{ServerLayout}{all}{InputDevice}{$kID}{usage} = "ExtraKeyboard";
		}
		last SWITCH;
		};

		/^Screen/           && do {
		@geo = split(/ +/,$value);
		$id     = $key; $id =~ s/Screen://;
		if ($id =~ /Screen\[(.*)\]/) {
			$screen = $1;
		}
		$var{ServerLayout}{all}{Screen}{$screen}{id}     = $id;
		if ($geo[1] !~ /none/i) {
			#print STDERR "$screen : left -> $geo[1]\n";
			$var{ServerLayout}{all}{Screen}{$screen}{left}  = $geo[1];
		}
		if ($geo[0] !~ /none/i) {
			#print STDERR "$screen : right -> $geo[0]\n";
			$var{ServerLayout}{all}{Screen}{$screen}{right} = $geo[0];
		}
		if ($geo[3] !~ /none/i) {
			#print STDERR "$screen : top -> $geo[3]\n";
			$var{ServerLayout}{all}{Screen}{$screen}{top}   = $geo[3];
		}
		if ($geo[2] !~ /none/i) {
			#print STDERR "$screen : bottom -> $geo[2]\n";
			$var{ServerLayout}{all}{Screen}{$screen}{bottom}= $geo[2];
		}
		$screen++;
		last SWITCH;
		};
		}
	}
	return(%var);
}

#---[ GetModelines ]-----#
sub GetModelines {
#-------------------------------------------
# this function create a list of modelines
# according to the selected modes
#
	my $modes      = $_[0];     # list of resolutions
	my $hsync      = $_[1];     # horizontal sync range
	my $vsync      = $_[2];     # vertical sync range
	my $readline   = $_[3];     # eventually read modelines
	my $specialline= $_[4];     # special lines from a monitor selection
	my $modus      = $_[5];     # calculation modus secure | normal
	my $calc       = $_[6];     # calculate Modelines or not 
	my $card       = $_[7];     # card number

	my (@modeline,@res,@l,@param,@clocklist,@data);
	my ($x,$y,$hs,$vs,$i,$line,$calctool,$chs,$cvs,$oldline,$r,$vesafb,$fbtime);
	my ($zf,$rr,$hfl,$vfl,$dcf,$clocks,$clk,$hsmax,$fbset,$mode,$found);
	my %fbmodes;
	my %query;

	# set FbSet binary to use...
	# ---------------------------
	$fbset = $import{FbSet};

	# decide for a tool to calculate
	# -------------------------------
	if ($modus eq "secure") {
		$calctool = "$import{Xmode}";
	} else {
		$calctool = "$import{Xmode} -n";
	}

	# get the sync freq for calculation (max vsync is 140 Hz)
	# -------------------------------------------------------
	if ($hsync =~ /(.*)-(.*)/) {
		$hs = $2; 
		$hs =~ s/ +//g;
		$hs = $hs * 1e3;
	}
	if ($vsync =~ /(.*)-(.*)/) {
		$vs = $2; 
		$vs =~ s/ +//g;
		if ($vs > 140) {
		$vs = 140;
		}
		# /.../
		# if IteratePrecisely is used ad a 3Hz span to
		# enable xmode to calculate exact modelines
		# ---
		if ($modus eq "normal") {
			$vs += 3;
		}
	}

	sub number  { $a <=> $b; }
	sub sortline {
		my @list = @_;
		my %tmp;
		my @param;
		my $d;
		my $count;
		my @erg;
		$count = 0;
		foreach (@list) {
			@param = split(/ +/,$_);
			$tmp{$param[1]}{$count} = $_;
			$count++;
		}
		foreach (sort number keys %tmp) {
		foreach $d (keys %{$tmp{$_}}) {
			push(@erg,$tmp{$_}{$d});
		}
		}
		return(@erg);
	}

	if (($readline ne "") && ($calc eq "no")) {
		# /.../
		# use the modelines from the config file...
		# -------------------------------------------
		@l   = split(/,/,$readline);
		@l   = sortline(@l);
		@res = split(/,/,$modes);
		foreach $r (@res) { 
		foreach (@l) {
			$_ =~ s/Modeline//;
			$_ =~ s/^ +//; $_ =~ s/ +$//;
			@param = split(/ +/,$_);
			$param[0] =~ s/\"//g;
			if ($param[0] =~ /$r/i) {
				$dcf = $param[1]; $dcf = $dcf * 1e6;
				$hfl = $param[5]; 
				$vfl = $param[9];

				$zf = int ($dcf/$hfl); 
				$rr = int ($zf/$vfl);
				$zf = int ($zf / 1e3) * 1e3;

				if ($oldline ne $_) {
				if (($zf <= $hs) && ($rr <= $vs)) {
					push(@modeline,"Modeline $_");
				}
				}
				$oldline = $_;
			}
		}
		}
	} elsif ($calc ne "no") {
		# /.../
		# Calculate Modelines...
		# look up fixed clocks ask sysp...
		# ----------------------------------
		if ($modus eq "fbdev") {
			# UseFrameBufferTiming is active...
			# -----------------------------------
			$vesafb = GetFramebufferResolution($fbset);
			$fbtime = GetFramebufferTimings($fbset);
			@data   = split(/ /,$vesafb);
			$line   = "Modeline \"$data[0]x$data[1]\" $fbtime";
			push(@modeline,$line);

			%fbmodes = ReadFbModes();
			$mode    = $data[0]."x".$data[1];
			foreach (keys %{$fbmodes{$mode}}) {
			if ($_ < $vs) {
				push(@modeline,$fbmodes{$mode}{$_});
			}
			}

		} else {
			# IteratePrecisely or CheckDesktopGeometry is active...
			# ------------------------------------------------------
			%query = GetHotQuery("xstuff");
			foreach $i (sort keys %{$query{$card}}) {
			if ($i =~ /Clock/) {
				$clocks = $query{$card}{$i};
				@clocklist = split(/ +/,$clocks);
			}
			}
			# got fixed clocks...
			# --------------------
			if ($clocks ne "") {
			@res = split(/,/,$modes);
			foreach $i (@clocklist) {
			if ($i eq "") { next; }
			$clk = $i * 1e6;
			foreach (@res) {
				if ($_ =~ /(.*)x(.*)/i) {
				$x = $1;
				$y = $2;
				$hsmax = $hs / 1e3;
				for ($i=$vs;$i>=60;$i-=10) {
					$line = qx (
						$calctool -x $x -y $y -r $i -n -d $clk -s $hsmax
					);
					@l = split(/\n/,$line);
					$chs  = $l[0];
					$cvs  = $l[1];
					$line = $l[2];
					$found = 0;
					foreach (@modeline) {
					if ($_ eq $line) { 
						$found = 1; last; 
					}
					}
					if ($found == 0) {
					if (($chs <= $hs) && ($cvs <= $vs)) {
						push(@modeline,$line);
					}
					}
				}
				}
			}
			}
			}	
			# calculate standard modelines...
			# ---------------------------------
			@res = split(/,/,$modes);
			foreach (@res) {
			if ($_ =~ /(.*)x(.*)/i) {
				$x = $1;
				$y = $2;
				$hsmax = $hs / 1e3;
				for ($i=$vs;$i>=60;$i-=10) {
				if ($modus eq "secure") {
					# CheckDesktopGeometry...
					# ------------------------
					$line = qx($calctool -x $x -y $y -r $i -s $hsmax);
				} else {
					# IteratePrecisely...
					# Note: this algorithm will calculate the modeline 
					# according to the vertical sync specification. The
					# horizontal sync range which depends on the vertical
					# sync range my be out of the range specified by the 
					# user. In this case the modeline will be deleted
					# from the X-Server with 
					# (hsync out of range)...
					# ------------------------
					$line = qx($calctool -x $x -y $y -r $i -n);
				}
				@l = split(/\n/,$line);
				$chs  = $l[0];
				$cvs  = $l[1];
				$line = $l[2];
				$found = 0; 
				foreach (@modeline) {
					if ($_ eq $line) { $found = 1; last; }
				}
				if ($found == 0) {
				if (($chs <= $hs) && ($cvs <= $vs)) {
					push(@modeline,$line);
				}
				} 
				}
			}
			}
		}
	}
	# /.../
	# add the special lines if exist
	# ------------------------------
	if ($specialline ne "") {
	@l = split(/,/,$specialline);
	foreach (@l) {
		push(@modeline,"Modeline $_");
	}
	}
	return(@modeline);
}

#----[ GetFramebufferResolution ]-----#
sub GetFramebufferResolution {
#---------------------------------------------------
# this function check for the framebuffer settings
# and treat the result as Vesa mode
#
	my $fbset = $_[0];
	my $mode;     # Mode part of fbset -x
	my $data;     # Data part of fbset -x
	my $hsync;    # hsync value
	my $vsync;    # vsync value
	my @size;     # X and Y values

	$mode = qx($fbset -x | grep ^Mode);
	$data = qx($fbset -x | grep "# D");

	if ($mode =~ /.*\"(.*)\".*/) {
		$mode = $1;  $mode =~ s/ +//g;
		@size = split(/x/,$mode);
	} else {
		return;
	}

	if ($data =~ /.*H: (.*) kHz, V: (.*) Hz/) {
		$hsync = $1; $hsync =~ s/ +//g;
		$vsync = $2; $vsync =~ s/ +//g;
		$hsync = int($hsync);
		$vsync = int($vsync);
	} else {
		return;
	}
	return("@size $hsync $vsync");
}

#----[ GetFramebufferTimings ]-----#
sub GetFramebufferTimings {
#-------------------------------------------------
# get the X11 modeline timings for the 
# framebuffer mode
#
	my $fbset = $_[0];
	my $clock;
	my $ht;
	my $vt;
	my $flags;
	my $mode;

	$clock = qx($fbset -x | grep -i DotClock);
	$ht    = qx($fbset -x | grep -i HTimings);
	$vt    = qx($fbset -x | grep -i VTimings);
	$flags = qx($fbset -x | grep -i Flags);

	$clock =~ s/DotClock//i; $clock =~ s/\n//g;
	$clock =~ s/ +//g;
	$ht    =~ s/HTimings//i; $ht =~ s/\n//g;
	$ht    =~ s/^ +//g; $ht =~ s/ +$//g;
	$vt    =~ s/VTimings//i; $vt =~ s/\n//g;
	$vt    =~ s/^ +//g; $vt    =~ s/ +$//g;
	$flags =~ s/Flags//i; $flags =~ s/\n//g;
	$flags =~ /(.*)#.*/;
	if ($1 ne "") {
		$flags = $1;
	}
	$flags =~ s/^ +//g;
	$flags =~ s/ +$//g;
	$flags =~ s/\"//g;

	$mode  = "$clock $ht $vt $flags";
	return($mode);
}

#-----[ GetHotQuery ]-----#
sub GetHotQuery {
#------------------------------------------------#
# This function call sysp -q < querystr > to     #
# obtain information from the sysp prog          #
# the result is a list containing the parameter  #
# and variable values as X=Y string list         #
#------------------------------------------------#
#
	my $querystr = $_[0];    # Query string

	my $result;              # result of sysp call
	my @list;                # result list of call 
	my $i;                   # loop counter 
	my %erg;                 # result hash;
	my $id;                  # identifier number              
	my @line;                # one line of @list
	my $variable;            # Variable
	my $value;               # Value
	my $count;               # Vesa mode counter

	$result = qx($import{Sysp} -q $querystr);
	@list = split(/\n/,$result);

	$count = 0;
	foreach $i (@list) {
		$i =~ /(.*)=>(.*):(.*)/;
		$id = $1;
		$variable = $2;
		$value  = $3;

		$id =~ s/ +//g;
		$id = chop($id);
		$variable =~ s/ +//g;
		$value  =~ s/^ +//g;

		if ($variable =~ /Vesa/i) {
			$erg{$id}{$variable}{$count} = $value;
			$count++;
		} else {
			$erg{$id}{$variable} = $value;
		}
	}
	return(%erg);
}


#---[ CheckSyncRange ]----#
sub CheckSyncRange {
#--------------------------------------------
# this function is used to check the 
# sync range for the calculated modelines 
#
	my @timing = @_;
	my $vsr    = pop(@timing);
	my $hsr    = pop(@timing);

	my @param;
	my $zf;
	my $rr;
	my $dac;
	my $htotal;
	my $vtotal;
	my $minzf = 160;
	my $minrr = 140;
	my $maxrr = 0;
	my $maxzf = 0;
	my @oldzf;
	my @oldrr;
	my @result;

	# get old parameters...
	# ----------------------
	$vsr =~ s/ +//g;
	$hsr =~ s/ +//g;
	@oldzf = split(/-/,$vsr);
	@oldrr = split(/-/,$hsr);

	foreach (@timing) {
		@param = split(/ +/,$_);
		$dac    = $param[0];
		$htotal = $param[4];
		$vtotal = $param[8];

		if (($htotal > 0) && ($vtotal > 0)) {
			$zf     = $dac * 1e6 / $htotal;
			$rr     = $zf / $vtotal;
			$zf     = $zf / 1000;
		}
		$zf = int($zf);
		$rr = int($rr);

		# get maximum...
		# ---------------
		if ($zf > $maxzf) { $maxzf = $zf; }
		if ($rr > $maxrr) { $maxrr = $rr; }

		# get minimum...
		# ---------------
		if ($zf < $minzf) { $minzf = $zf; }
		if ($rr < $minrr) { $minrr = $rr; }
	}

	# prove hsync range...
	# ---------------------
	if ($minzf < $oldzf[0]) {
		$oldzf[0] = $minzf;
	}
	if ($maxzf > $oldzf[1]) {
		#$oldzf[1] = $maxzf;
	}

	# prove vsync range...
	# ---------------------
	if ($minrr < $oldrr[0]) {
		$oldrr[0] = $minrr;
	}
	if ($maxrr > $oldrr[0]) {
		#$oldrr[1] = $maxrr;
	}

	push(@result,$oldrr[0]);
	push(@result,$oldrr[1]);
	push(@result,$oldzf[0]);
	push(@result,$oldzf[1]);
	return(@result);
}

#----[ ReadFbModes ]------#
sub ReadFbModes {
#-----------------------------------------------------
# this function is used to read the /etc/fb.modes 
# file and save the data as X11 configured list
#
	my $file = "/etc/fb.modes";
	my $nr   = 0;

	my $line;      # one line from fb.modes
	my @data;      # data list splitted from geometry or timings
	my %record;    # fb data record used for conversion
	my $group;     # resolution group counter
	my %modes;     # modes hash
	my $mode;      # one mode line
	my $dac;       # ram dac
	my $hr;        # horizontal resolution
	my $sh1;       # hsync start
	my $sh2;       # hsync end
	my $hfl;       # horizontal frame length
	my $vr;        # vertical resolution
	my $sv1;       # vsync start
	my $sv2;       # vsync end
	my $vfl;       # vertical frame length
	my $frq;       # vertical sync frequency

	# open file for reading...
	# --------------------------
	open (FD,"$file") || return;
	while ($line = <FD>) {
	chomp($line);
	if ($line =~ /^#.*/) { next; }
	$line =~ s/^ +//g;
	$line =~ s/ +$//g;
	SWITCH: for ($line) {
		# geometry...
		# ------------
		/geometry/       && do {
		$nr++;
		@data = split(/ +/,$line);
		$record{$nr}{xres} = $data[1];
		$record{$nr}{yres} = $data[2];
		last SWITCH;
		};

		# timings...
		# -----------
		/timings/        && do {
		@data = split(/ +/,$line);
		$record{$nr}{dac}    = $data[1];
		$record{$nr}{left}   = $data[2];
		$record{$nr}{right}  = $data[3];
		$record{$nr}{upper}  = $data[4];
		$record{$nr}{lower}  = $data[5];
		$record{$nr}{hslen}  = $data[6];
		$record{$nr}{vslen}  = $data[7];
		last SWITCH;
		};

		# flags...
		# -----------
		/laced true/     && do {
		$record{$nr}{Interlace} = "Interlace"; 
		last SWITCH;
		};
		/hsync high/     && do {
		$record{$nr}{HSync}     = "+HSync";
		last SWITCH;
		};
		/hsync low/      && do {
		$record{$nr}{HSync}     = "-HSync";
		last SWITCH;
		};
		/vsync high/     && do {
		$record{$nr}{VSync}     = "+VSync";
		last SWITCH;
		};
		/vsync low/      && do {
		$record{$nr}{VSync}     = "-VSync";
		last SWITCH;
		};
	}
	}
	close(FD);

	$group   = 0;
	foreach $nr (keys %record) {
		$mode = $record{$nr}{xres}."x".$record{$nr}{yres};
		$dac  = int(1e6 / $record{$nr}{dac});
		$hr   = $record{$nr}{xres};
		$sh1  = $record{$nr}{right} + $hr;
		$sh2  = $record{$nr}{hslen} + $sh1;
		$hfl  = $record{$nr}{left}  + $sh2;
		$vr   = $record{$nr}{yres};
		$sv1  = $record{$nr}{lower} + $vr;
		$sv2  = $record{$nr}{vslen} + $sv1;
		$vfl  = $record{$nr}{upper} + $sv2;
		$frq  = $dac * 1e6 / $hfl;
		$frq  = int ($frq / $vfl);

		$line = "Modeline \"$mode\" $dac $hr $sh1 $sh2 $hfl $vr $sv1 $sv2 $vfl";
		if (defined $record{$nr}{Interlace}) {
		$line = "$line $record{$nr}{Interlace}";
		}
		if (defined $record{$nr}{HSync}) {
		$line = "$line $record{$nr}{HSync}";
		}
		if (defined $record{$nr}{VSync}) {
		$line = "$line $record{$nr}{VSync}";
		}
		$modes{$mode}{$frq} = $line;
		$group++;
	}
	return(%modes);
}

#---[ IncludeModelineChanges ]----#
sub IncludeModelineChanges {
#-----------------------------------------------------
# this function will look at the given directory and
# search for files containing modeline changes the naming 
# convention of the file is: [screen]:[X]x[Y]
# The changing information is than included into the
# given configuration file is possible
#
	my $config = $_[0];   # configuration file
	my $dir    = $_[1];   # directory with patch files

	# /.../
	# first read the modelines and build a 
	# hash structure around them
	# ---
	my %modelines = IncorpModelines ($config);
	my %tune      = CreateTuneHash (\%modelines);

	# /.../
	# next read the patch files
	# ---
	opendir (FD,$dir);
	my $updateNeeded = 0;
	my @patchFiles   = readdir (FD);
	closedir (FD);
	foreach my $file (@patchFiles) {
	if (-f "$dir/$file") {
		%tune  = UpdateModelinesOnScreen (\%tune,"$dir/$file");
		$updateNeeded = 1;
	}
	}

	my %changed = TransformTuneHashOnScreen (\%tune);
	if ((keys %modelines != 0) && ($updateNeeded == 1)) {
	my @section = CreateModesSection (\%changed);
	ExcorpModelines (
		@section,$config
	);
	}
}

#---[ ImportXFineCache ]----#
sub ImportXFineCache {
#------------------------------------------------------
# return current value of ImportXFineCache variable
# this function return either "yes" or "no"
#
	return ($ImportXFineCache);
}

#---[ CheckSplit ]----#
sub CheckSplit {
#------------------------------------------------------
# check if a splitted list with seperator [,] was
# splitted in a correct way. This function will check
# for the number ["] signs and make sure there was
# no split between optionslist which mustn't splitted
#
	my @list   = @_;
	my @result = ();
	my $count  = 0;
	my $joined;
	for (my $i=0;$i<@list;$i++) {
		my $signs = countDoubleQuote ($list[$i]);
		if ( ($signs == 0) || ($signs % 2 != 0) ) {
			$joined.=",$list[$i]";
		} else {
			$result[$count] = $list[$i];
			$count++;
			if ($joined ne "") {
				$joined =~ s/^,//;
				$result[$count] = $joined;
				$joined = "";
				$count++;
			}
		}
	}
	if ($joined ne "") {
		$joined =~ s/^,//;
		push (@result,$joined);
	}
	@list   = @result;
	@result = ();
	foreach (@list) {
	if (defined $_) {
		push (@result,$_);
	}
	}
	return @result;
}

#---[ countDoubleQuote ]----#
sub countDoubleQuote {
#---------------------------------------
# count double quotes in a string and
# return the result
#   
	my $str = $_[0];
	my $cnt = 0;
	while (length ($str)) {
		if (chop($str) eq "\"") {
			$cnt++;
		}
	}
	return $cnt;
}

1;
