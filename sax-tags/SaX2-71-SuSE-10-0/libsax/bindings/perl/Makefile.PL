use ExtUtils::MakeMaker;
WriteMakefile(
  'NAME'     => 'SaX',
  'LIBS'     => ['-L/usr/X11R6/lib -L/usr/X11R6/lib64 -L/usr/lib/qt3/lib -L/usr/lib/qt3/lib64 -L../../ -lm -lxf86config -lqt-mt -lsax -lxkbfile'],
  'OBJECT'   => 'SaX_wrap.o',
  'INC'      => '-I/usr/X11R6/include -I/usr/lib/qt3/include',
  'OPTIMIZE' => '-O2'
);

