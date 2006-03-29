use ExtUtils::MakeMaker;
use Env;

$QTDIR=$ENV{QTDIR};

WriteMakefile(
  'NAME'     => 'SaX',
  'LIBS'     => ["-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L$QTDIR/lib -L$QTDIR/lib64 -lm -lxf86config -lqt-mt -lxkbfile"],
  'dynamic_lib' => { 'OTHERLDFLAGS' => '-L../.. -lsax' },
  'OBJECT'   => 'SaX_wrap.o',
  'INC'      => "-I/usr/X11R6/include -I/usr/include/xorg -I$QTDIR/include",
  'OPTIMIZE' => '-O2'
);

