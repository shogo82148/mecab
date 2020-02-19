use ExtUtils::MakeMaker;

sub run {
    my $cmd = shift;
    my $ret = `$cmd`;
    if ($? != 0) {
        die "failed to ececute $cmd: $ret";
    }
    return $ret;
}

WriteMakefile(
    'NAME'    => 'MeCab',
    'CC'      => run('mecab-config --cxx'),
    'LD'      => run('mecab-config --cxx'),
    'INC'     => run('mecab-config --cflags'),
    'LIBS'    => run('mecab-config --libs'),
    'VERSION' => run('mecab-config --version'),
    'OBJECT'  => 'MeCab_wrap.o',
);
