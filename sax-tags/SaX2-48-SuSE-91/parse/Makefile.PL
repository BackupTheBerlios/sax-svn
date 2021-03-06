use ExtUtils::MakeMaker;
WriteMakefile(
  'NAME'     => 'XFree',
  'LIBS'     => ['-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lm -lxf86config'],
  'OBJECT'   => 'parse.o XFree_wrap.o',
  'DEFINE'   => '-DX_BYTE_ORDER=X_LITTLE_ENDIAN -DNEW_INPUT -DNDEBUG',
  'INC'      => '-I/usr/X11R6/include',
  'OPTIMIZE' => '-O2'
); 
