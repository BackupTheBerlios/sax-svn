use ExtUtils::MakeMaker;
WriteMakefile(
  'NAME'     => 'SPP',
  'LIBS'     => ['-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/lib/qt3/lib -L/usr/lib/qt3/lib64 -lm -lfl -lreadline -lqt-mt'],
  'OBJECT'   => 'SPP_wrap.o lex.yy.o spp.tab.o spp.o',
  'INC'      => '-I/usr/X11R6/include -I/usr/lib/qt3/include',
  'OPTIMIZE' => '-O2',
  'CC'       => 'g++'
);

