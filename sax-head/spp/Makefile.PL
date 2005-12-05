use ExtUtils::MakeMaker;
use Env;

$QTDIR=$ENV{QTDIR};

WriteMakefile(
  'NAME'     => 'SPP',
  'LIBS'     => ["-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L$QTDIR/lib -L$QTDIR/lib64 -lm -lfl -lreadline -lqt-mt -lncurses"],
  'OBJECT'   => 'SPP_wrap.o lex.yy.o spp.tab.o spp.o',
  'INC'      => "-I/usr/X11R6/include -I$QTDIR/include",
  'OPTIMIZE' => '-O2',
  'CC'       => 'g++'
);

