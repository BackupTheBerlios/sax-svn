use ExtUtils::MakeMaker;
WriteMakefile(
  'NAME'     => 'PLog',
  'LIBS'     => ['-lfl'],
  'OBJECT'   => 'plog.o PLog_wrap.o ../../sysp/lib/libsysp.a',
  'DEFINE'   => '-DX_BYTE_ORDER=X_LITTLE_ENDIAN -DNEW_INPUT -DNDEBUG',
  'INC'      => '',
  'OPTIMIZE' => '-O2'
); 

