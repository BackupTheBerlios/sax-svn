use ExtUtils::MakeMaker;
use Env;

$QTDIR=$ENV{QTDIR};

WriteMakefile(
  'NAME'     => 'SaX',
  'LIBS'     => ["-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L$QTDIR/lib -L$QTDIR/lib64 -L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile"],
  'OBJECT'   => 'SaX_wrap.o',
  'INC'      => "-I/usr/X11R6/include -I$QTDIR/include",
  'OPTIMIZE' => '-O2'
);

