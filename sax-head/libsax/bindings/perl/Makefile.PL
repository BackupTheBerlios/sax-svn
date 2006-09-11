use ExtUtils::MakeMaker;
use Env;

$QTDIR=$ENV{QTDIR};
$CFLAGS = $ENV{RPM_OPT_FLAGS};
$CFLAGS =~ s/-Wall//;

$INCPATH="-I$QTDIR/include";
if (-f "/usr/X11R6/include/xf86Parser.h") {
	$INCPATH=$INCPATH." -iquote /usr/X11R6/include -I.";
} else {
	$INCPATH=$INCPATH." -iquote /usr/include/xorg -I.";
}

WriteMakefile(
  'NAME'     => 'SaX',
  'LIBS'     => ["-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L$QTDIR/lib -L$QTDIR/lib64 -lm -lxf86config -lqt-mt -lxkbfile"],
  'dynamic_lib' => { 'OTHERLDFLAGS' => '-L../.. -lsax' },
  'OBJECT'   => 'SaX_wrap.o',
  'INC'      => $INCPATH,
  'OPTIMIZE' => "-O2 $CFLAGS"
);

