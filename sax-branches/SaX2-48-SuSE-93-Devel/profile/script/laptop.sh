#!/usr/bin/perl

#
# Config
#
$scriptname = $0;
$sysp = "/usr/sbin/sysp";
$card = "Card0";
$xorg = "/usr/X11R6/bin/Xorg";
$profiledir = "/usr/X11R6/lib/sax/profile";

$tmpc = $ENV{"HOME"}."/sax2_laptop_xorg.conf";
$tmpl = $ENV{"HOME"}."/sax2_laptop_xorg.log";


#
# Subroutines
#
local $optnum = 242;

# Probe X for BIOS and driver specific information
# arg1: module
# arg2: added to screen section
sub probe_x11 {
    my $mod = $_[0];
    my $scr = $_[1];
    unlink $tmpc;
    open F, ">$tmpc" or die "cannot write to temporary config file $tmpc";
    print F <<"EO_CONF" ;
Section "ServerLayout"
  Identifier    "srv"
  Screen        "scr"
EndSection
Section "Screen"
  Identifier    "scr"
  Device        "dev"
  $scr
EndSection
Section "Device"
  Identifier    "dev"
  Driver        "$mod"
EndSection
EO_CONF
    close F;
    print STDERR "...probing...\n";
    unlink $tmpl;
    system "$xorg :98 -probeonly -logfile $tmpl -config $tmpc -quiet -logverbose 255 2>/dev/null";
    local $/;
    open F, "<$tmpl" or die "cannot open Xorg log file $tmpl";
    my $res = <F>;
    close F;
    return $res;
}

# find matching modes from list of known beamer sizes (thus no 1400x1050)
# Currently no PAL & NTSC beamer included (848x480, 720x576 etc)
# max width x max height is always included
# arg1: max width
# arg2: max height
sub find_modes {
    my $w=$_[0];
    my $h=$_[1];
    my @xa=(1920, 1600, 1280, 1280, 1280, 1024, 800, 640);
    my @ya=(1080, 1200, 1024,  960,  720,  768, 600, 480);
    my @r=();
    my $x, $y;
    while (@xa > 0) {
        $x = shift @xa;
	$y = shift @ya;
	if ($x <= $w && $y <= $h) {
	    push @r, "${x}x${y}";
	}
    }
    if ($r[0] ne "${w}x${h}") {
	unshift @r, "${w}x${h}";
    }
    return @r;
}

# send config option string to sax2
# arg1: module
# arg2: option
# arg3: parameter
sub send_stringoption {
    #print "\t$_[0]: Option \"$_[1]\" \"$_[2]\"\n";
    print P "Device->[X]->Raw->$optnum->Option = \"$_[1]\" \"$_[2]\"\n";
    $optnum++;
}

# send metamode line to sax2
# arg1: module
# arg2: option
# arg3: seperator mon1<>mon2
# arg4: seperator res1 res2 res3...
# arg5: list of modes
sub send_metamodes {
    my $r="";
    foreach $m (@{$_[4]}) {
	$r="$r$_[3]$m$_[2]$m";
    }
    $r =~ s/^$_[3]//;
    #print "\t$_[0]: Option \"$_[1]\" \"$r\"\n";
    print P "Device->[X]->Raw->$optnum->Option = \"$_[1]\" \"$r\"\n";
    $optnum++;
}

# arg1: module
# arg2: option
# arg3: list of modes
sub send_modes {
    my $r="";
    #foreach $m (@{$_[2]}) {
	#$r="$r \"$m\"";
    #}
    #$r =~ s/^ //;
    #print "\t$_[0]: $_[1] $r\n";
    foreach $m (@{$_[2]}) {
	$r="$r,$m";
    }
    $r =~ s/^,//;
    print P "Screen->[X]->Depth->8->Modes  = $r\n";
    print P "Screen->[X]->Depth->15->Modes = $r\n";
    print P "Screen->[X]->Depth->16->Modes = $r\n";
    print P "Screen->[X]->Depth->24->Modes = $r\n";
    print P "Screen->[X]->Depth->32->Modes = $r\n";
}


#
# Start
#

if ($> != 0) {
    print STDERR "Please run me as root.\n";
    exit 1;
}

($profile = $scriptname) =~ s%^.*/([^/]+)\.[^/.]+$%$1%;
$profile = "$profiledir/$profile.tmp";
open P, ">$profile" or die "Cannot write profile data $profile";

$sysp_server = `$sysp -q server`;
$sysp_xstuff = `$sysp -q xstuff`;

#$sysp_server =~ /^$card\s*=>\s*VID\s*:\s*([0-9a-fA-Fx]+)\s*$/m;
$sysp_server =~ /^$card\s*=>\s*Module\s*:\s*(.*?)\s*$/m;
$module      = $1;
$sysp_server =~ /^$card\s*=>\s*Vendor\s*:\s*(.*?)\s*$/m;
$vendorname  = $1;

print STDERR "Configuring laptop mode for $vendorname:\n";


if ($module eq "nvidia") {

#
# NVIDIA
#

    $_ = $xorg_probe = probe_x11 ($module, "");

    if (/^\(..\) NVIDIA\([0-9]+\): EDID reported maximum dimensions for display device DFP(-[0-9]+)?:\s*.*?width\s*:\s*([0-9]+)\s*.*?height\s*:\s*([0-9]+)\s*$/ms) {
        print STDERR "Maximum resolution: $2x$3\n";
	($maxwidth, $maxheight) = ($2, $3);
    } elsif (/^\(..\) NVIDIA\([0-9]+\): Virtual screen size determined to be\s*([0-9]+)\s*x\s*([0-9]+)\s*$/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } else {
	print STDERR "***** Cannot determine maximum resolution, assuming 1024x768\n";
	($maxwidth, $maxheight) = (1024, 768);
    }
    if (/^\(..\) NVIDIA\([0-9]+\): Boot display device\(?s?\)?: (.*?)\s*$/m) {
	($bootdev=$1) =~ s/,.*$//;
    } else {
	$bootdev="DFP";
    }
    if (/^\(..\) NVIDIA\([0-9]+\): Supported display device\(?s?\)?: (.*?)\s*$/m) {
	($devs=$1) =~ s/$bootdev//;
    } else {
	$devs="CRT";
    }

    if ($bootdev =~ /DFP/) {
	if ($devs =~ /DFP/) {
	    $connected = "$bootdev,AUTO";
	    print STDERR "***** Secondary output might be flat panel as well.\n";
	} elsif ($devs =~ /CRT/) {
	    $connected = "$bootdev,CRT";
	} else {
	    $connected = "$bootdev,AUTO";
	    print STDERR "***** No secondary output found.\n";
	}
    } else {
	$connected="AUTO,AUTO";
	print STDERR "***** Device not booted into DFP panel.\n";
    }
    print STDERR "Selecting $connected as monitor configuration, you might want to\nchange this in /etc/X11/xorg.conf (Option \"ConnectedMonitor\")\nif it does not behave as anticipated.\n";

    @modes = find_modes ($maxwidth, $maxheight);
    send_stringoption ($module, "TwinView",                 "yes");
    send_stringoption ($module, "ConnectedMonitor",         $connected);
    send_stringoption ($module, "TwinViewOrientation",      "Clone");
    send_stringoption ($module, "SecondMonitorHorizSync",   "30-120");
    send_stringoption ($module, "SecondMonitorVertRefresh", "58-65");
    send_metamodes    ($module, "MetaModes", ",", ";",      \@modes);
    send_modes        ($module, "Modes",                    \@modes);


} elsif ($module eq "radeon") {

#
# ATI radeon
#

    $_ = $xorg_probe = probe_x11 ($module, "");

    if (/^\(..\) RADEON\([0-9]+\): Panel Size from BIOS:\s*([0-9]+)\s*x\s*([0-9]+)/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } elsif (/^\(..\) RADEON\([0-9]+\): Virtual screen is\s*([0-9]+)\s*x\s*([0-9]+)/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } else {
	print STDERR "***** Cannot determine maximum resolution, assuming 1024x768\n";
	($maxwidth, $maxheight) = (1024, 768);
    }

    @modes = find_modes ($maxwidth, $maxheight);
    send_stringoption ($module, "MergedFB",            "yes");
    send_stringoption ($module, "CRT2HSync",           "30-120");
    send_stringoption ($module, "CRT2VSync",           "58-65");
    send_stringoption ($module, "CRT2VRefresh",        "58-65");
    send_stringoption ($module, "IgnoreEDID",          "yes");
    send_metamodes    ($module, "MetaModes", "-", " ", \@modes);
    send_modes        ($module, "Modes",               \@modes);


} elsif ($module eq "fglrx") {

#
# ATI fglrx
#

    $_ = $xorg_probe = probe_x11 ($module, "DefaultDepth 24");

    if (/^\(..\) fglrx\([0-9]+\): Panel Size from BIOS:\s*([0-9]+)\s*x\s*([0-9]+)/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } elsif (/^\(..\) fglrx\([0-9]+\): Virtual screen is\s*([0-9]+)\s*x\s*([0-9]+)/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } else {
	print STDERR "***** Cannot determine maximum resolution, assuming 1024x768\n";
	($maxwidth, $maxheight) = (1024, 768);
    }

    $_ = $xorg_probe;
    s/^.*\(..\) fglrx\([0-9]+\): Primary head:\s*$//ms;
    s/^\S.*//ms;
    if (/^ Monitor\s*--\s*(\S+)\s*$/m) {
        $primary = $1;
    } else {
        print STDERR "***** Cannot determine primary monitor.\n";
        $primary = "AUTO";
    }
    $_ = $xorg_probe;
    s/^.*\(..\) fglrx\([0-9]+\): Secondary head:\s*$//ms;
    s/^\S.*//ms;
    $secondary="AUTO";
    if (/^ Connector\s*--\s*(\S+)\s*$/m) {
	$p = $1;
	if ($p =~ /VGA/) {
	    $secondary = "CRT";
	} elsif ($p =~ /DVI-D/) {
	    $secondary = "DLP";
	    print STDERR "***** Secondary output seems to be a flat panel as well.\n";
	}
    }
    if (/^ Monitor\s*--\s*(\S+)\s*$/m) {
	$p = $1;
        $secondary = $p if $p !~ /none/i;
    }
    if ($secondary eq "AUTO") {
        print STDERR "***** Cannot determine secondary monitor.\n";
    }
    $connected="$primary,$secondary";
    print STDERR "Selecting $connected as monitor configuration, you might want to\nchange this in /etc/X11/xorg.conf (Option \"MonitorLayout\")\nif it does not behave as anticipated.\n";

    @modes = find_modes ($maxwidth, $maxheight);
    $optnum = 16; # This is magic - see profile 'firegl'
    send_stringoption ($module, "DesktopLayout", "0x00000100"); #16
    send_stringoption ($module, "MonitorLayout", $connected);   #17
    send_stringoption ($module, "IgnoreEDID",    "no");         #18
    send_stringoption ($module, "HSync2",    "30-120");         #19
    send_stringoption ($module, "VRefresh2", "58-65");          #20 
    send_modes        ($module, "Modes",     \@modes);


} elsif ($module eq "i810") {

#
# INTEL
#

    $_ = $xorg_probe = probe_x11 ($module, "");

    if (/^\(..\) I8.0\([0-9]+\): Size of device LFP.*?is\s*([0-9]+)x([0-9]+)\s*$/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } elsif (/^\(..\) I8.0\([0-9]+\): Virtual size is\s*([0-9]+)x([0-9]+)/m) {
        print STDERR "Maximum resolution: $1x$2\n";
	($maxwidth, $maxheight) = ($1, $2);
    } else {
	print STDERR "***** Cannot determine maximum resolution, assuming 1024x768\n";
	($maxwidth, $maxheight) = (1024, 768);
    }
    
    %devs = ();
    %bootdevs = ();
    while (/^\(..\) I8.0\([0-9]+\): Display Info: ([^ :]+)[^:]*: attached: ([a-zA-Z]+), present: ([a-zA-Z]+)/mg) {
	my $d=$1, $a=$2, $p=$3;
	if ($p =~ /true/i) {
	    $devs{$d}=1;
	    $bootdevs{$d}=1 if $a =~ /true/i;
	}
    }
    if ($bootdevs{"LFP"}) {
	$primary="LFP";
    } elsif ($bootdevs{"DFP"}) {
	$primary="DFP";
    } elsif ($bootdevs{"CRT"}) {
	$primary="CRT";
	print STDERR "***** Device not booted into DFP panel.\n";
    } else {
	$primary="DFP";
	print STDERR "***** Cannot determine boot display.\n";
    }
    undef $devs{$primary};
    if ($devs{"LFP"}) {
	$primary="LFP";
	$secondary="CRT";
	print STDERR "***** Not booted into LFP, but connected. Weird. Config may not work.\n";
    } elsif ($devs{"DFP"}) {
	$secondary="DFP";
	print STDERR "***** Secondary output seems to be a flat panel as well.\n";
    } elsif ($devs{"CRT"}) {
	$secondary="CRT";
    } elsif ($devs{"DFP2"}) {
	$secondary="DFP";
	print STDERR "***** Secondary output seems to be a flat panel as well.\n";
    } elsif ($devs{"CRT2"}) {
	$secondary="CRT";
    } else {
	$secondary="CRT";
	print STDERR "***** No secondary output found. Config may not work.\n";
    }
    if ($xorg_probe =~ /^\(..\) I8.0\([0-9]+\): Primary Pipe is B,/m) {
	$connected="$secondary,$primary";
    } else {
	$connected="$primary,$secondary";
    }
    print STDERR "Selecting $connected as monitor configuration, you might want to\nchange this in /etc/X11/xorg.conf (Option \"MonitorLayout\")\nif it does not behave as anticipated.\n";

    @modes = find_modes ($maxwidth, $maxheight);
    if ($primary ne "" && $secondary ne "") {
	send_stringoption ($module, "MonitorLayout", $connected);
    }
    send_stringoption ($module, "Clone",        "true");
    send_stringoption ($module, "CloneRefresh", "60");
    send_modes        ($module, "Modes",        \@modes);


} else {

    print STDERR "\n********************\n";
    print STDERR "Unknown vendor for laptop mode: $vendorname (driver $module)\n";
    print STDERR "Continuing in standard configuration mode.\n";
    print STDERR "********************\n\n";
}


close P;
unlink $tmpc;
#unlink $tmpl;

#EOF
