#!/usr/bin/perl

#
# Config
#
$scriptname = $0;
$card = "Card0";
$profiledir = "/usr/share/sax/profile";

$xorglogname = "/var/log/Xorg.99.log";
$xorglogname = "/var/log/Xorg.0.log" if ! -f $xorglogname;

#
# Start
#

if ($> != 0) {
    print STDERR "Please run me as root.\n";
    exit 1;
}

($profilename = $scriptname) =~ s%^.*/([^/]+)\.[^/.]+$%$1%;
{ local $/;
  open F, "<$profiledir/$profilename" or die "Cannot read profile $profiledir/$profile";
  $profile = <F>;
  close F;
  open F, "<$xorglogname" or die "Cannot read X.org log $xorglogname";
  $xorglog = <F>;
  close F;
}


#
# INTEL
#

$_ = $xorglog;

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
} elsif ($devs{"CRT"}) {
	$secondary="CRT";
} elsif ($devs{"DFP"}) {
	$secondary="DFP";
	print STDERR "***** Secondary output seems to be a flat panel as well.\n";
} elsif ($devs{"DFP2"}) {
	$secondary="DFP";
	print STDERR "***** Secondary output seems to be a flat panel as well.\n";
} elsif ($devs{"CRT2"}) {
	$secondary="CRT";
} else {
	$secondary="CRT";
	print STDERR "***** No secondary output found. Config may not work.\n";
}

# Intel chips *always* seem to have the internal display attached
# to pipe B.
# The following code *might* work well enough to find strange hardware
# with LDP attached to Pipe A. Might.
$connected="$secondary,$primary";
$_ = $xorglog;
if (s/^.*\(..\) I8.0\([0-9]+\): Currently active displays on Pipe A:\s*//s
	&& s/\(..\) I8.0\([0-9]+\): \S.*$//s) {
    if (/\sLFP\s/) {
	    $connected="$primary,$secondary";
    }
}
$_ = $xorglog;
if (/^\(..\) I[89].0\([0-9]+\): Primary Pipe is A,/m) {
	$connected="$primary,$secondary";
}
print STDERR "Checked $xorglogname...\nSelecting $connected as monitor configuration, you might want to\nchange this in /etc/X11/xorg.conf (Option \"MonitorLayout\")\nif it does not behave as anticipated.\n";

$profile =~ s/\[MONITORLAYOUT\]/$connected/;

#
# Output
#
open P, ">$profiledir/$profilename.tmp" or die "Cannot write profile data $profiledir/$profilename.tmp";
print P $profile;
close P;

#EOF
