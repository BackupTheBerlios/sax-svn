use ExtUtils::MakeMaker;
use Env;

$QTDIR=$ENV{QTDIR};

WriteMakefile(
  'NAME'     => 'SaX',
  'LIBS'     => ["-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L$QTDIR/lib -L$QTDIR/lib64 -lm -lxf86config -lqt-mt -lxkbfile"],
  'dynamic_lib' => { 'OTHERLDFLAGS' => '-L../.. -lsax' },
  'OBJECT'   => 'SaX_wrap.o',
  'INC'      => "-I$QTDIR/include",
  'OPTIMIZE' => '-O2'
);

