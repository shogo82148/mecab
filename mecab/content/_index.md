---
title: "MeCab @shogo82148 flavored: Yet Another Part-of-Speech and Morphological Analyzer"
date: 2020-01-05T07:41:50+09:00
---

## MeCab @shogo82148 flavored とは {#about-mecab-shogo82148}
MeCabはオープンソースの形態素解析エンジンです。
[KyTea](http://www.phontron.com/kytea/index-ja.html), [JUMAN++](http://nlp.ist.i.kyoto-u.ac.jp/index.php?JUMAN++), 
[Sudachi](https://github.com/WorksApplications/Sudachi), [SentencePiece](https://github.com/google/sentencepiece) 等の
後発の形態素解析エンジンの開発も進んでいますが、使い勝手の良さや使用例の多さから今なお現役で利用されています。
しかし、2020年1月現在、[オリジナルのMeCab](http://taku910.github.io/mecab/)の最新リリースは **2013-02-18** MeCab 0.996 です。
OSやコンパイラのアップデートへの追従、バグフィックス等のメンテナンス作業がほぼ止まっています。

このままではマズいということで
[@shogo82148](https://github.com/shogo82148/)がオリジナルのMeCabをフォークしてパッチを当てたものが
MeCab @shogo82148 flavored です。
機能の追加等は行わず、バグフィックスのみ行う予定です。

## MeCab (和布蕪)とは {#about-mecab}

MeCabは 京都大学情報学研究科−日本電信電話株式会社コミュニケーション科学基礎研究所 共同研究ユニットプロジェクトを通じて開発されたオープンソース 形態素解析エンジンです。 言語, 辞書,コーパスに依存しない汎用的な設計を 基本方針としています。 パラメータの推定に Conditional Random Fields ([CRF][CRF]) を用 いており, ChaSenが採用している 隠れマルコフモデルに比べ性能が向上しています。また、平均的に ChaSen, Juman, KAKASIより高速に動作します。 ちなみに和布蕪(めかぶ)は, 作者の好物です。

## 目次 {#toc}

- [特徴](#feature)
- [比較](#diff)
- [新着情報](#news)
- [開発までの経緯]({{<relref feature.md>}})
- [ダウンロード](#download)
- [インストール](#install)
  - [Unix](#install-unix)
  - [Windows](#install-windows)
- [使い方](usage-tools)
  - [とりあえず解析する](#parse)
  - [わかち書きをする](#wakati)
  - [出力フォーマットの変更](#format)
- [高度な使い方](#usage-tools2)
  - [文字コードの変更](#charset)
  - [N-Best 解の出力](#nbest)
  - [単語の追加方法]({{<relref dic.md>}})
  - [出力フォーマットの詳細定義]({{<relref format.md>}})
  - [品詞IDの定義]({{<relref posid.md>}})
  - [制約付き解析(部分解析)]({{<relref partial.md>}})
  - [ソフトわかち書き]({{<relref soft.md>}})
  - [C/C++ ライブラリインタフェース]({{<relref libmecab.md>}})
  - [その他のコマンドラインオプション]({{<relref mecab.md>}})
  - [MeCab の辞書構造と汎用テキスト変換ツールとしての利用]({{<relref dic-detail.md>}})
  - [未知語処理の再定義]({{<relref unk.md>}})
  - [オリジナル辞書/コーパスからのパラメータ推定]({{<relref learn.md>}})
  - [スクリプト言語(perl/ruby/python/Java) バインディング]({{<relref bindings.md>}})
- [謝辞](#thanks)

## 特徴 {#feature}

- 辞書, コーパスに依存しない汎用的な設計
- 条件付き確率場([CRF][CRF])に基づく高い解析精度
- [ChaSen][ChaSen] や [KAKASI][KAKASI] に比べ高速
- 辞書引きアルゴリズム/データ構造に, 高速な TRIE 構造である [Double-Array](http://chasen.org/~taku/software/darts/)を採用.
- 再入可能なライブラリ
- 各種スクリプト言語バインディング(perl/ruby/python/java/C#)

## 比較 {#diff}

|MeCab|[ChaSen][ChaSen]|[JUMAN][JUMAN]|[KAKASI][KAKASI]
-|-|-|-
解析モデル|bi-gram マルコフモデル|可変長マルコフモデル|bi-gram マルコフモデル|最長一致
コスト推定|コーパスから学習|コーパスから学習|人手|コストという概念無し
学習モデル|[CRF][CRF] (識別モデル)|HMM (生成モデル)||
辞書引きアルゴリズム|Double Array|Double Array|パトリシア木|Hash?
解探索アルゴリズム|Viterbi|Viterbi|Viterbi|決定的?
連接表の実装|2次元 Table|オートマトン|2次元 Table?|連接表無し?
品詞の階層|無制限多階層品詞|無制限多階層品詞|2段階固定|品詞という概念無し?
未知語処理|字種 (動作定義を変更可能)|字種 (変更不可能)|字種 (変更不可能)|
制約つき解析|可能|2.4.0で可能|不可能|不可能
N-best解|可能|不可能|不可能|不可能

MeCab に至るまでの形態素解析器開発の歴史等は[こちら]({{<relref feature.md>}})をご覧ください

## メーリングリスト

- [一般ユーザ向けメーリングリスト](http://lists.sourceforge.jp/mailman/listinfo/mecab-users)
- [開発者向けメーリングリスト](http://lists.sourceforge.jp/mailman/listinfo/mecab-devel)

## 新着情報 {#news}

- **2020-10-09** MeCab 0.996.5
  - [C++11が利用可能な環境では `thread_local` キーワードを利用](https://github.com/shogo82148/mecab/pull/54)
  - [C++11で非推奨,C++17で削除された `register` キーワードを削除](https://github.com/shogo82148/mecab/pull/54)
  - [`mecab_new` が正しくエラーを返さない問題を修正](https://github.com/shogo82148/mecab/pull/53)
- **2020-10-07** MeCab 0.996.4
  - [mingw-w64 でコンパイルできない問題を修正](https://github.com/shogo82148/mecab/pull/50)
- **2020-02-21** MeCab 0.996.3
  - [Ruby 2.7 のサポート](https://github.com/shogo82148/mecab/pull/42)
  - コンパイル時の警告を修正
- **2020-02-14** MeCab 0.996.2
  - [C++11が利用可能な環境ではatomicヘッダーを利用](https://github.com/shogo82148/mecab/pull/24)
  - [未使用の変数・関数を削除](https://github.com/shogo82148/mecab/pull/25)
  - [出力フォーマットに不正なメタ文字が含まれていた場合にSEGVする不具合を修正](https://github.com/shogo82148/mecab/pull/26)
  - [Cygwinでビルドできない不具合を修正](https://github.com/shogo82148/mecab/pull/28)
  - [MinGWでビルドできない不具合を修正](https://github.com/shogo82148/mecab/pull/32)
- **2020-02-03** MeCab 0.996.1
  - [SWIG 4.0.1 にアップデート](https://github.com/shogo82148/mecab/pull/11)
  - Python3系でコンパイルできない不具合修正
  - [64bitMSVCでコンパイルできない不具合修正](https://github.com/shogo82148/mecab/pull/6)
  - [意図しないタイミングで表層系のバッファがガーベジコレクションされてしまう不具合修正](https://github.com/taku910/mecab/pull/24)
- **2013-02-18** MeCab 0.996
  - configure script の不備によりこiconvへのリンクに失敗する問題を修正
  - ユーザ辞書用CSVファイルのコストと左/右文脈IDを付与し, 新たなCSVファイルを生成する機能の追加
  - 解析結果からLattice を作成する Lattice::set_result() メソッドを追加. 単体テスト時のスタブの作成等に利用可能
- **2013-01-24** MeCab 0.995
  - [部分解析機能]({{<relref partial.md>}})の再実装
  - [部分解析機能]({{<relref partial.md>}})のためのAPI (Lattice:set_boundary_constraint, Lattice::set_feature_constraint) の追加
- **2012-06-03** MeCab 0.994
  - [再学習機能]({{<relref "learn.md#retrain">}})の追加 (少量のコーパスと既存モデルを使ったCRFパラメータの更新)
  - ユーザ辞書の単語コストの自動推定機能の追加 (CRFモデルが必要)
- **2012-01-27** MeCab 0.993
  - MeCab::Tagger::formatNode()が正しく動いていなかった問題の修正
  - スタックの消費を抑えるため、ほとんどのローカル変数(配列)をヒープ上に退避
- **2012-01-14** MeCab 0.992
  - ソースコード中のTypoの修正
- **2012-01-14** MeCab 0.991
  - 空文字列もしくは空白文字列を解析した時に解析エラーとなる問題を修正
  - ユーザ辞書の作成に失敗する場合がある問題を修正
- **2011-12-24** MeCab 0.99
  - MeCab::Model, MeCab::Lattice クラスを追加
  - マルチスレッド環境でのユーザビリティの向上。複数スレッドが同一辞書を共有しながら解析可能に
  - 同一辞書を参照する場合、辞書へのリファレンスを自動的に共有する機能の削除。 (MeCab::Model を使って同一のことが可能なため)
  - 解析中にアトミックに辞書をアップデートする機能の追加 Model::swap()
  - Windows版のバイナリを Unicodeバイナリに変更
  - online learning, HMM, EM learning の各実験コードを削除
  - MeCab::Node:(begin|end)_node_list メンバの削除
  - 細かいバグの修正
- **2009-09-27** MeCab 0.98
- UTF16のサポート(実験的)
  - Windows版での文字コード変換に MultiByteToWideChar等の Native APIを使うように変更
  - ソースコードを Google coding style に変更
  - フォーマット指定で EON (end of N-best) の追加 (-S or --eon-format)
  - Shift-JIS環境で半角カタカナの扱いに問題があったのを修正
  - online learning のサポート (実験的)
  - Wno-deprecatedをつけなくてもコンパイルできるようにした
  - 細かいバグの修正
- **2008-02-03** MeCab 0.97
  - マルチスレッド環境で辞書を開くときの排他制御がうまくいっていなかったバグの修正
  - Windows版でインストール時に辞書の文字コードを指定できるようになった
  - 一部のコンパイラで正しくコンパイルできなかった問題の修正
  - 部分解析モードを変更するAPI の追加 (Tagger::set_partial())
  - ラティスの生成レベルを変更するAPI の追加 (Tagger::set_lattice_level())
  - 温度パラメータを変更するAPIの追加 (Tagger::set_theta())
  - 全候補出力モードを変更するAPIの追加 (Tagger::set_all_morphs())
- **2007-06-10** MeCab 0.96
  - バッファオーバフローのバグを修正
  - 常にPOS-IDを作成するようにした (-p オプションの廃止)
  - ユーザ辞書のデリミタを : から , (CSV) に変更 (Windows対策)
  - charsetの判定にバグがあり, ある条件でユーザ辞書とシステム辞書が非互換になるバグを修正
  - ユーザ辞書ファイルの文字コードとシステム辞書ファイルの文字コー ドが異なる場合, 辞書の構築がうまくいかなかった問題の修正
  - コマンドラインオプションをダンプする --dump-config オプションの追加
  - EMベースのHMM学習をサポートできるような学習ルーチンの追加 (experimental)
- **2007-03-11** MeCab 0.95
  - 古いコンパイラでコンパイルできない問題を修正
  - csvのエスケープの不具合で ","を含む単語が追加できなかった問題を修正
  - UTF8辞書が一部正常に作成できなかったバグの修正
  - recall/precisionの表示が反対になっていたバグの修正
  - コマンドライン解析の不具合の修正
  - その他細かなバグの修正
- **2007-02-24** MeCab 0.94
  - 多くのバグフィックス
  - HMMによる学習をサポート (実験的)
  - 解析結果の全情報を取得できるAPIを追加 (begin_node_list, end_node_list)
  - char.def, unk.def, matrix.def が未定義の場合でも辞書が作成できるよう変更
  - Windows版の iconv.dllへの依存を廃止
  - コードのクリーンアップ
- **2006-07-30** MeCab 0.93
  - ライセンスをLGPLからBSD,LGPL,GPLのトリプルライセンスに変更
- **2006-07-10** MeCab 0.92
  - 辞書コンパイラ等, 一部Perlで実装されていたコードをC++で再実装. Perlへの依存性の排除
  - 辞書コンパイラ (mecab-dict-index) の高速化
  - rewrite.def のシンタックスの変更
  - -x "未知語品詞" オプションの追加: 未知語推定を行わず, ユーザが指定した "未知語品詞" を出力
  - [品詞 id]({{<relref posid.md>}}) のサポート
  - 文字種情報が一部学習できていなかったバグの修正
  - 学習の際, 頻度による足切りができていなかったバグの修正
  - その他細いバグの修正
- **2006-04-30** MeCab 0.91
  - Windows 環境で文字列の最後が半角スペースの時に落ちるバグの修正
  - 連接表の前件と後件のサイズが異なるときに正しく解析できないバグ の修正
  - mecab-dict-index に -f オプションを追加し, CSV の文字コードをユー ザが指定できるようにした
  - 一部の API関数が export されていない問題の修正
  - CRFの学習を pthread を使って並列に行えるようにした (experimental)
  - ユーザ辞書が作成できない問題の修正
  - example ディレクトリに MeCabの応用例を追加 (unittest)
  - その他細いバグの修正
- **2006-03-26** MeCab 0.90
  - Initial release!

## ダウンロード {#download}

- **MeCab** はフリーソフトウェアです．[GPL v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt)(the GNU General Public License Version 2.0), [LGPL](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt)(Lesser GNU General Public License Version 2.1), または [三条項BSD](https://spdx.org/licenses/BSD-3-Clause.html) ライセンスに従って本ソフトウェアを使用,再配布することができます。 詳細は COPYING, GPL, LGPL, BSD各ファイルを参照して下さい．

- [v0.996.5](https://github.com/shogo82148/mecab/releases/tag/v0.996.5)

### MeCab 本体

- Source
  - mecab-0.996.5.tar.gz: [ダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-0.996.5.tar.gz)
  - 辞書は含まれていません. 動作には別途辞書が必要です。
- Binary package for MS-Windows
  - mecab-msvc-x64-0.996.5.zip: [64bit版ダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-msvc-x64-0.996.5.zip)
  - mecab-msvc-x86-0.996.5.zip: [32bit版ダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-msvc-x86-0.996.5.zip)
  - Windows 版には コンパイル済みの IPA 辞書が含まれています

### MeCab 用の辞書

- IPA 辞書
  - IPA 辞書, IPAコーパス に基づき [CRF][CRF] でパラメータ推定した辞書です。 **(推奨)** [ダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-ipadic-2.7.0-20070801.tar.gz)
- JUMAN 辞書
  - JUMAN 辞書, 京都コーパスに基づき [CRF][CRF] でパラメータ推定した辞書です。 [ダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-jumandic-7.0-20130310.tar.gz)
- Unidic 辞書
  - Unidic 辞書, BCCWJコーパスに基づき CRF でパラーメータ推定した辞書です。ダウンロード

### perl/ruby/python/java バインディング

- [Perlダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-perl-0.996.5.tar.gz)
- [Rubyダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-ruby-0.996.5.tar.gz)
- [Pythonダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-python-0.996.5.tar.gz)
  - Windowsに関してはコンパイル済みのwheelもあります。
- [Javaダウンロード](https://github.com/shogo82148/mecab/releases/download/v0.996.5/mecab-java-0.996.5.tar.gz)

## インストール {#install}

### UNIX {#install-unix}

- 動作に必要なもの 
  - C++ コンパイラ (g++ 3.4.3 と VC7 でのコンパイルを確認しています)
  - iconv (libiconv): 辞書のコード変換に使います

#### インストール手順

一般的なフリーソフトウェアと同じ手順でインストールできます。

```
% tar zxfv mecab-X.X.tar.gz
% cd mecab-X.X
% ./configure 
% make
% make check
% su
# make install
```

辞書のインストール

```
% tar zxfv mecab-ipadic-2.7.0-XXXX.tar.gz
% mecab-ipadic-2.7.0-XXXX
% ./configure
% make
% su
# make install
```

### Windows {#windows}

バイナリをインストールする場合は,
自己解凍インストーラ (mecab-X.X.exe)
を実行してください. 辞書も同時にインストールされます。

## 使い方 {#usage-tools}

### とりあえず解析してみる {#parse}

mecab を起動して,生文を標準入力から入力してみてください.MeCab では, 一行一文を前提として解析を行ないます。

```
% mecab
すもももももももものうち
すもも  名詞,一般,*,*,*,*,すもも,スモモ,スモモ
も      助詞,係助詞,*,*,*,*,も,モ,モ
もも    名詞,一般,*,*,*,*,もも,モモ,モモ
も      助詞,係助詞,*,*,*,*,も,モ,モ
もも    名詞,一般,*,*,*,*,もも,モモ,モモ
の      助詞,連体化,*,*,*,*,の,ノ,ノ
うち    名詞,非自立,副詞可能,*,*,*,うち,ウチ,ウチ
EOS
```

出力フォーマットは, ChaSen のそれと大きく異なります。左から,

```
表層形\t品詞,品詞細分類1,品詞細分類2,品詞細分類3,活用形,活用型,原形,読み,発音
```

となっています。

引数にファイルを与えると,
そのファイルが解析対象となります。
また, `-o` オプションにて,
別のファイルに結果を出力することも可能です。

```
% mecab INPUT -o OUTPUT
```

### わかち書きをする {#wakati}

以下のように `-O` オプションを使います。

```
% mecab -O wakati
太郎はこの本を二郎を見た女性に渡した。
太郎 は この 本 を 二郎 を 見 た 女性 に 渡し た 。
```

### 出力フォーマットの変更 {#format}

以下のように `-O` オプションを使います。

```
% mecab -Oyomi (ヨミ付与)
% mecab -Ochasen (ChaSen互換)
% mecab -Odump (全情報を出力)
```

これらの出力フォーマットは, `/usr/local/lib/mecab/ipadic/dicrc` に定義されています。
さらに,ユーザがこれらのフォーマットを自由に定義することが可能です。
[こちら]({{<relref format.md>}})をご覧ください.

## 高度な使い方 {#usage-tools2}

### 文字コード変更 {#charset}

特に指定しない限り, euc が使用されます。
もし, shift-jis や utf8 を 使いたい場合は, 辞書の configure オプションにて charset を変更し,
辞書を再構築してください.
これで, shift-jis や, utf8 の辞書が作成されます。

```
% tar zxfv mecab-ipadic-2.7.0-xxxx
% cd mecab-ipadic-2.7.0-xxxx
% ./configure --with-charset=sjis
% make

% tar zxfv mecab-ipadic-2.7.0-xxxx
% ./configure --with-charset=utf8
% make
```

また, `mecab-dict-index` の `-t` オプションを使って直接文字コードの異なる
辞書を再構築できます。 `-f` オプションはオリジナルのテキスト辞書の文字コードです。

```
% cd mecab-ipadic-2.7.0-xxxx
% /usr/local/libexec/mecab/mecab-dict-index -f euc-jp -t utf-8
# make install
```

### UTF-8 only mode {#utf-8}

configure option で `--enable-utf8-only` を指定すると. MeCab が扱う
文字コードを utf8 に固定します。 euc-jp や shift-jis をサポートする場合, 
MeCab 内部に変換用のテーブルを埋めこみます。 `--enable-utf8-only` を
指定することでテーブルの埋めこみを抑制し, 結果として実行バイナリを
小さくすることができます。

### 未知語推定 {#unk}

MeCab は, 辞書に単語が未登録の場合でも適当にその品詞を推定します。

```
ホリエモン市
ホリエモン      名詞,固有名詞,地域,一般,*,*,*
市      名詞,接尾,地域,*,*,*,市,シ,シ
EOS
ホリエモンさん
ホリエモン      名詞,固有名詞,人名,一般,*,*,*
さん    名詞,接尾,人名,*,*,*,さん,サン,サン
```

ただし, その精度は正確ではありません. 品詞推定をやめ, 未知語は常に 
"未知語" 品詞を出力したい場合は `-x` (`--unk-feature`) オプションを使います。
オプションで指定された文字列が品詞として使われます。

```
%mecab --unk-feature "未知語" 
ホリエモンさん
ホリエモン      未知語
さん    名詞,接尾,人名,*,*,*,さん,サン,サン
```

### N-Best 解の出力 {#nbest}

`-N #NUM` オプションを使うことで, 確からしいものから#NUM 個解析結果を出力
します。 理論的にはすべての可能な解析解を出力することが
可能ですが, 出力バッファのかねあいから, -N の最大値を 512 に制限しています。

```
% mecab -N2
今日もしないとね。
今日    名詞,副詞可能,*,*,*,*,今日,キョウ,キョー
も      助詞,係助詞,*,*,*,*,も,モ,モ
し      動詞,自立,*,*,サ変・スル,未然形,する,シ,シ
ない    助動詞,*,*,*,特殊・ナイ,基本形,ない,ナイ,ナイ
と      助詞,接続助詞,*,*,*,*,と,ト,ト
ね      助詞,終助詞,*,*,*,*,ね,ネ,ネ
。      記号,句点,*,*,*,*,。,。,。
EOS
今日    名詞,副詞可能,*,*,*,*,今日,キョウ,キョー
もし    副詞,一般,*,*,*,*,もし,モシ,モシ
ない    形容詞,自立,*,*,形容詞・アウオ段,基本形,ない,ナイ,ナイ
と      助詞,接続助詞,*,*,*,*,と,ト,ト
ね      助詞,終助詞,*,*,*,*,ね,ネ,ネ
。      記号,句点,*,*,*,*,。,。,。
EOS
```

## 謝辞 {#thanks}

CRF のパラメータ推定に [Jorge Nocedal](http://www.ece.nwu.edu/~nocedal) 氏が考案した L-BFGS
と同氏が公開している FORTRAN 実装を使わせていただいております。ありがとうございます。

<http://www.ece.northwestern.edu/~nocedal/lbfgs.html>

- J. Nocedal. Updating Quasi-Newton Matrices with Limited Storage (1980), Mathematics of Computation 35, pp. 773-782.
- D.C. Liu and J. Nocedal. On the Limited Memory Method for Large Scale Optimization (1989), Mathematical Programming B, 45, 3, pp. 503-528.

[CRF]: https://repository.upenn.edu/cgi/viewcontent.cgi?article=1162&context=cis_papers "CRF"
[ChaSen]: https://chasen-legacy.osdn.jp/ "ChaSen"
[JUMAN]: http://nlp.ist.i.kyoto-u.ac.jp/index.php?JUMAN "JUMAN"
[KAKASI]: http://kakasi.namazu.org/index.html.en "KAKASI"
