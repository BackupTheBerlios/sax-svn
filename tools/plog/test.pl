#!/usr/bin/perl

use PLogData;

$ptr = ParseLog("/var/log/XFree86.99.log");
$mem = GetVideoRam($ptr);
print "$mem\n";

@res = GetResolution($ptr);
print "@res\n";

$ddc = GetMonitorManufacturer($ptr);
print "$ddc\n";

$type = CheckDisplayType($ptr);
print "$type\n";

$vmd  = GetVMwareColorDepth($ptr);
print "$vmd\n";

$siz  = GetDisplaySize($ptr);
print "$siz\n";
