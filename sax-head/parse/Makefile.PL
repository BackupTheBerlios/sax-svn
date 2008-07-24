use ExtUtils::MakeMaker;

$CFLAGS = $ENV{RPM_OPT_FLAGS};
$CFLAGS =~ s/-Wall//;

$FLAG="HAVE_EXTENSIONS_SECTION";

WriteMakefile(
  'NAME'     => 'XFree',
  'LIBS'     => ['-L/usr/X11R6/lib -L/usr/X11R6/lib64 -lm -lxf86config'],
  'OBJECT'   => 'parse.o XFree_wrap.o',
  'DEFINE'   => "-DX_BYTE_ORDER=X_LITTLE_ENDIAN -DNEW_INPUT -DNDEBUG -D$FLAG",
  'INC'      => '-iquote /usr/X11R6/include -iquote /usr/include/xorg -I.',
  'OPTIMIZE' => "-O2 $CFLAGS"
); 
