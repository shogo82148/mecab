---
title: "Mecab"
date: 2020-01-05T15:19:57+09:00
---

<H1>MECAB</H1>
Section: MeCab (1)<BR>Updated: July 2006<BR>[Index](#index)
[Return to Main Contents](/cgi-bin/man/man2html)<HR>

<A NAME="lbAB">&nbsp;</A>
## NAME

mecab - manual page for mecab of 0.92
<A NAME="lbAC">&nbsp;</A>
## SYNOPSIS

<B>mecab</B>

[<I>options</I>] <I>files</I>
<A NAME="lbAD">&nbsp;</A>
## DESCRIPTION

MeCab: Yet Another Part-of-Speech and Morphological Analyzer
<A NAME="lbAE">&nbsp;</A>
## COPYRIGHT

Copyright &#169; 2001-2006 Taku Kudo
<BR>

Copyright &#169; 2004-2006 Nippon Telegraph and Telephone Corporation
<DL COMPACT>
<DT><B>-r</B>, <B>--rcfile</B>=<I>FILE</I><DD>
use FILE as resource file
<DT><B>-d</B>, <B>--dicdir</B>=<I>DIR</I><DD>
set DIR  as a system dicdir
<DT><B>-u</B>, <B>--userdic</B>=<I>FILE</I><DD>
use FILE as a user dictionary
<DT><B>-l</B>, <B>--lattice-level</B>=<I>INT</I><DD>
lattice information level (default 0)
<DT><B>-a</B>, <B>--all-morphs</B><DD>
output all morphs (default false)
<DT><B>-O</B>, <B>--output-format-type</B>=<I>TYPE</I><DD>
set output format type (wakati,none,...)
<DT><B>-p</B>, <B>--partial</B><DD>
partial parsing mode
<DT><B>-F</B>, <B>--node-format</B>=<I>STR</I><DD>
use STR as the user-defined node format
<DT><B>-U</B>, <B>--unk-format</B>=<I>STR</I><DD>
use STR as the user-defined unk format
<DT><B>-B</B>, <B>--bos-format</B>=<I>STR</I><DD>
use STR as the user-defined bos format
<DT><B>-E</B>, <B>--eos-format</B>=<I>STR</I><DD>
use STR as the user-defined eos format
<DT><B>-b</B>, <B>--input-buffer-size</B>=<I>INT</I><DD>
set input buffer size (default BUF_SIZE)
<DT><B>-C</B>, <B>--allocate-sentence</B><DD>
allocate new memory for input sentence
<DT><B>-N</B>, <B>--nbest</B>=<I>INT</I><DD>
output N best results  (default 1)
<DT><B>-t</B>, <B>--theta</B>=<I>FLOAT</I><DD>
set temparature parameter theta (default 0.75)
<DT><B>-o</B>, <B>--output</B>=<I>FILE</I><DD>
set the output file name
<DT><B>-v</B>, <B>--version</B><DD>
show the version and exit.
<DT><B>-h</B>, <B>--help</B><DD>
show this help and exit.

</DL>

<HR>
<A NAME="index">&nbsp;</A>## Index
<DL>
<DT>[NAME](#lbAB)<DD>
<DT>[SYNOPSIS](#lbAC)<DD>
<DT>[DESCRIPTION](#lbAD)<DD>
<DT>[COPYRIGHT](#lbAE)<DD>
</DL>
<HR>
This document was created by
[man2html](/cgi-bin/man/man2html),
using the manual pages.<BR>
Time: 15:16:13 GMT, July 09, 2006
