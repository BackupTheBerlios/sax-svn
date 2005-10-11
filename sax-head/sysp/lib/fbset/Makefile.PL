use ExtUtils::MakeMaker;
WriteMakefile(
  'NAME'     => 'FBSet',
  'LIBS'     => ['-lm'],
  'OBJECT'   => 'FBSet_wrap.o fbset.o',
  'OPTIMIZE' => '-O2',
  'CC'       => 'g++',
  'LD'       => 'g++'
);
