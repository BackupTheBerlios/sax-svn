#!/usr/bin/perl
# Copyright (c) 2004 SuSE AG Nuernberg, Germany.  All rights reserved.
#
# Author: Marcus Schaefer <sax@suse.de>, 2004
# libsax script: apply profile on specific device number
# --
#
# createPRO 
#   -p <ProfileFile> -d <DeviceNumber>
#
# DeviceNumber
# ============ 
# number of the device the profile 
# should be applied to
#
# ProfileFile
# ============
# file name of the profile
#
# CVS ID:
# --------
# Status: Development
#
use strict;
#==========================================
# Module path
#------------------------------------------
use lib '/usr/share/sax/modules';

#==========================================
# Includes
#------------------------------------------
use HashMap;
use ExportAPI;
use Getopt::Long;

#==========================================
# Globals
#------------------------------------------
my $Profile;
my $Device;
my %init;
my %import;

our %dialog;

#----[ init ]------#
sub init {
#--------------------------------------------------
# init options and variables with const information
#
	$init{BinaryFile}  = "/var/cache/sax/files/config";
	my $result = GetOptions(
		"profile|p=s" => \$Profile,
		"device|d=i"  => \$Device,
		"<>"          => \&usage
	);
	if ( $result != 1 ) {
		usage();
	}
	if (-s $init{BinaryFile}) {
		my $hashref = retrieve($init{BinaryFile});
		if (defined $hashref) {
			%import = %{$hashref};
			PrepareLayoutDefaults();
			PrepareLayout();
		}
	}
}

#---[ PrepareProfile ]---------#
sub PrepareProfile {
#--------------------------------------------------
# prepare profile. remove comments and special
# symbols like REMOVE from the input stream
#
	my @file;
	if (! open (FD,"$Profile")) {
		return @file;
	}
	while (my $line = <FD>) {
		if (($line =~ /^#/) || ($line =~ /^$/)) {
			next;
		}
		if ($line =~ /\[REMOVE\]/) {
			next;
		}
		chomp ($line);
		push  (@file,$line);
	}
	close (FD);
	return (@file);
}

#---[ ImportProfile ]-------#
sub ImportProfile {
#-----------------------------------------------
# import profile information and return the
# changes imported into the given hash
#
	my @data   = @{$_[0]};   # profile data
	my $device = $_[1];      # base device number 
	my %source = %{$_[2]};   # source hash
	my %dialog;

	my $new;       # new device number
	my @result;    # result as list
	my @add;       # new sections through the profile
	my $newsec;    # loop counter for new sections
	my $data;      # raw profile key
	my $line;      # raw profile data
	my @v;         # list of keys (->)
	my $search;    # formatted search string
	my $size;      # size of @v
	my $ViewValue; # Tree view
	my $ViewRef;   # Tree view reference
	my $key;       # equals $data
	my $value;     # equals $ViewValue
	my $str;       # join "->" @v

	if (! defined $device) {
		$device = 0;
	}

	# read profile data...
	# ---------------------
	foreach my $l (@data) {
		SWITCH: for ($l) {
		/^.*\[X\].*/         && do {
			$l =~ s/\[X\]/$device/g;
		};

		/^.*\[X\+([1-9]).*/  && do {
			$new = $device + $1;
			$l =~ s/\[X\+[1-9]\]/$new/g;
			push (@add,$new);
		};

		/^.*\[(.*)\].*/         && do {
			$device = $1;
		};
		
		/^.*\[(.*)\+([1-9]).*/  && do {
			$new = $1 + $2;
			$l =~ s/\[.*\+[1-9]\]/$new/g;
			push (@add,$new);
		};
		}
		push (@result,$l);
	}

	# /.../
	# include the data to the hash...
	# ---
	foreach (@result) {
	while ($_ =~ /{(.*?)}/) {
		$line      = $1;
		$line      =~ s/ +//g;
		@v         = split(/->/,$line);
		$search    = join ("->",@v);
		$size      = @v;
		$ViewValue = "";
		$ViewRef   = "";
		$ViewValue = HGetValue(\%import,$search);
		$ViewValue =~ s/^ +//g;
		$ViewValue =~ s/ +$//g;
		$_ =~ s/{.*?}/$ViewValue/;
	}
	if ($_ =~ /^(.*)=(.*)/) {
		$key       = $1;
		$value     = $2;
		$key       =~ s/ +//g;
		$value     =~ s/^ +//g;
		@v         = split(/->/,$key);
		$str       = join("->",@v);
		%dialog    = HSetValue(\%dialog,$str,$value);
	}
	}
	my $max = 0;
	foreach (keys %{$dialog{Device}}) {
	if ($_ > $max) {
		$max = $_ ;
	}
	}
	$Device = $max + 1;
	%dialog = HUpdateServerLayout (
		\%dialog,$Device
	);
	return (%dialog);
}

#---[ main ]-----#
sub main {
#----------------------------------------------------
# do the job now...
# create API interface file according to the
# imported %dialog data
#
	my @desktop;
	my @input;
	my @device;
	my @layout;
	my %result;

	my @data = PrepareProfile();
	%dialog  = ImportProfile (\@data,$Device,\%import);
	foreach my $section (keys %dialog) {
	SWITCH: for ($section) {
	# create Card section...
	# ------------------------
	/^Device/                  && do {
		@device  = CardGetDevice();
		pop (@device); shift(@device);
		foreach (@device) {
			chomp($_);
			$_ =~ s/^ +//;
			$_ =~ s/ / : /;
			$_ =~ s/=/:/;
			my $line = $_;
			my @list = split (/:/,$line);
			if ($list[1] =~ /Identifier/) {
				SWITCH: for ($line) {
				/Device\[.*\]/   && do {
					$line =~ s/Identifier/Device    /;
					last SWITCH;
				};
				}
			}
			$result{"$line:Card"} = "Card";
			#print "$line:Card\n";
		}
		last SWITCH;
	};

	# create Monitor and Screen sections...
	# --------------------------------------
	/Screen|Monitor|Desktop/  && do {
		if (defined @desktop) {
			last SWITCH;
		}
		@desktop = DesktopGetDesktop();
		pop(@desktop); shift(@desktop); 
		foreach (@desktop) {
			chomp($_);
			$_ =~ s/^ +//;
			$_ =~ s/ / : /;
			$_ =~ s/=/:/;
			my $line = $_;
			my @list = split (/:/,$line);
			if ($list[1] =~ /Identifier/) {
				SWITCH: for ($line) {
				/Monitor\[.*\]/  && do {
					$line =~ s/Identifier/Monitor   /;
					last SWITCH;
				};
				/Device\[.*\]/   && do {
					$line =~ s/Identifier/Device    /;
					last SWITCH;
				};
				}
			}
			#print "$line:Desktop\n";
			$result{"$line:Desktop"} = "Desktop";
		}
		last SWITCH;
	};
	# create InputDevice sections...
	# --------------------------------------
	/^InputDevice/                && do {
		@input = MouseGetInputDevice();
		pop(@input); shift(@input);
		foreach (@input) {
			chomp($_);
			$_ =~ s/^ +//;
			$_ =~ s/ / : /;
			$_ =~ s/=/:/;
			my $line = $_;
			my @list = split (/:/,$line);
			#print "$line:Mouse\n";
			$result{"$line:Mouse"} = "Mouse";
		}
		last SWITCH;
	};

	# create Layout section...
	# -------------------------
	/^ServerLayout/            && do {
		my @list;
		@layout  = LayoutGetServerLayout();
		pop (@layout); shift(@layout);
		foreach (@layout) {
			chomp($_);
			@list = split (/=/,$_);
			$list[1] =~ s/ +//g;
			if ($list[1] ne "") {
				$_ =~ s/^ +//;
				$_ =~ s/ / : /;
				$_ =~ s/=/:/;
				#print "$_:Layout\n";
				$result{"$_:Layout"} = "Layout";
			}
		}
		last SWITCH;
	};
	}
	}
	foreach (keys %result) {
		print "$_\n";
	}
}

#---[ PrepareLayoutDefaults ]----#
sub PrepareLayoutDefaults {
#----------------------------------------------------
# this function is called to set default values to 
# the ServerLayout section
#
	$dialog{ServerLayout}{all}{Identifier} = "Layout[all]";

	foreach my $id (keys %{$dialog{InputDevice}}) {
	my $input = $dialog{InputDevice}{$id}{Identifier};
	if ($id % 2 == 0) {
		#============================================
		# this are keyboard devices
		#--------------------------------------------
		$dialog{ServerLayout}{all}{InputDevice}{$id}{usage} = "CoreKeyboard";
		$dialog{ServerLayout}{all}{InputDevice}{$id}{id}    = $input
	} else {
		#============================================
		# this are mouse devices
		#--------------------------------------------
		my $event  = "CorePointer";
		if ($id != 1) {
			$event = "SendCoreEvents";
		}
		$dialog{ServerLayout}{all}{InputDevice}{$id}{usage} = $event;
		$dialog{ServerLayout}{all}{InputDevice}{$id}{id}    = $input
	}
	}
	# / ... /
	# look up Xinerama/and Clone mode...
	# -----------------------------------
	if (! defined $dialog{ServerLayout}{all}{Option}{Xinerama}) {
		$dialog{ServerLayout}{all}{Option}{Xinerama}     = "off";
	}
	if ($dialog{ServerLayout}{all}{Option}{Xinerama} eq "") {
		$dialog{ServerLayout}{all}{Option}{Xinerama}     = "off";
	}
	if (! defined $dialog{ServerLayout}{all}{Option}{Clone}) {
		$dialog{ServerLayout}{all}{Option}{Clone}        = "off";
	}
	if ($dialog{ServerLayout}{all}{Option}{Clone} eq "") {
		$dialog{ServerLayout}{all}{Option}{Clone}        = "off";
	}
}

#---[ PrepareLayout ]---#
sub PrepareLayout {
#----------------------------------------------------------
# prepare layout to know every neighbour within each screen 
# line. This will create redundand information but is easier
# to handle within SaX2 later
#
	my %layout;
	my %screen = %{$import{ServerLayout}{all}{Screen}};
	foreach my $id (keys %screen) {
		my $leftOf  = $screen{$id}{left};
		my $rightOf = $screen{$id}{right};
		my $above   = $screen{$id}{top};
		my $beneath = $screen{$id}{bottom};

		if ($leftOf  =~ /Screen\[(.*)\]/) {
			$layout{$id}{right} = $1;
			$layout{$1}{left} = $id;
		}
		if ($rightOf =~ /Screen\[(.*)\]/) {
			$layout{$id}{left} = $1;
			$layout{$1}{right} = $id;
		}
		if ($above   =~ /Screen\[(.*)\]/) {
			$layout{$id}{bottom} = $1;
			$layout{$1}{top} = $id;
		}
		if ($beneath =~ /Screen\[(.*)\]/) {
			$layout{$id}{top} = $1;
			$layout{$1}{bottom} = $id;
		}
	}
	foreach my $id (keys %screen) {
		$import{ServerLayout}{all}{Screen}{$id}{left}   = "<none>";
		$import{ServerLayout}{all}{Screen}{$id}{right}  = "<none>";
		$import{ServerLayout}{all}{Screen}{$id}{top}    = "<none>";
		$import{ServerLayout}{all}{Screen}{$id}{bottom} = "<none>";
		if (defined $layout{$id}{left}) {
			my $n= $layout{$id}{left};
			$import{ServerLayout}{all}{Screen}{$id}{left}   = "Screen[$n]";
		}
		if (defined $layout{$id}{right}) {
			my $n= $layout{$id}{right};
			$import{ServerLayout}{all}{Screen}{$id}{right}  = "Screen[$n]";
		}
		if (defined $layout{$id}{top}) {
			my $n= $layout{$id}{top};
			$import{ServerLayout}{all}{Screen}{$id}{top}    = "Screen[$n]";
		}
		if (defined $layout{$id}{bottom}) {
			my $n= $layout{$id}{bottom};
			$import{ServerLayout}{all}{Screen}{$id}{bottom} = "Screen[$n]";
		}
	}
}


#----[ usage ]----#
sub usage {
#------------------------------------------------------
# if you need help :-)
#
	exit (0);   
}

init();
main();
