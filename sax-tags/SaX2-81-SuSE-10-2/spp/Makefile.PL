use ExtUtils::MakeMaker;
use Env;

$QTDIR=$ENV{QTDIR};
$CFLAGS = $ENV{RPM_OPT_FLAGS};
$CFLAGS =~ s/-Wall//;

WriteMakefile(
  'NAME'     => 'SPP',
  'LIBS'     => ["-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L$QTDIR/lib -L$QTDIR/lib64 -lm -lfl -lreadline -lqt-mt -lncurses"],
  'OBJECT'   => 'SPP_wrap.o lex.yy.o spp.tab.o spp.o',
  'INC'      => "-I/usr/include/X11 -I$QTDIR/include",
  'OPTIMIZE' => "-O2 $CFLAGS",
  'CC'       => 'g++'
);

