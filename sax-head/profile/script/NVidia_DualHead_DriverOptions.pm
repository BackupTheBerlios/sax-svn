#!/usr/bin/perl

sub getDualDisplaySize {
	my $x = 0;
	my $y = 0;
	my $size = qx (/usr/sbin/hwinfo --monitor | grep Size: | cut -f2 -d:);
	if ($size =~ /(\d+)x(\d+) cm/) {
		$x = $1;
		$y = $2;
		$x = $x * 10 * 2;
		$y = $y * 10;
	}
	return ($x,$y);
}

1;
