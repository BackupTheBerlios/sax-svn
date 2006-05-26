use ExtUtils::MakeMaker;
WriteMakefile(
  'NAME'     => 'CVT',
  'LIBS'     => ['-lm'],
  'OBJECT'   => 'CVT_wrap.o cvt.o',
  'OPTIMIZE' => '-O2',
  'CC'       => 'gcc',
  'LD'       => 'gcc'
);
