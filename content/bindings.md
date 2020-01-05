---
title: "スクリプト言語のバインディング"
date: 2020-01-05T15:19:21+09:00
---

## 概要

各種スクリプト言語 [perl][perl],
[ruby][ruby],
[python][python],
[Java][java])
から, MeCab が提供する形態素解析の機能を利用可能です.
各バインディングは [SWIG][SWIG] というプログラムを用いて, 自動生成されています.
[SWIG][SWIG] がサポートする他の言語も
生成可能だと思われますが, 現在は, 作者の管理できる範囲内ということで, 
上記の4つの言語のみを提供しております.

## インストール

各言語バイディングのインストール方法は, perl/README, ruby/README, python/README,
java/README を御覧下さい.

## とりあえず解析する

`MeCab::Tagger` というクラスのインスタンスを生成し, `parse` (もしくは
`parseToString`) というメソッドを呼ぶことで, 解析結果が文字列として取得できます.
`MeCab::Tagger` のコンストラクタの引数は, 基本的に mecab の実行形式に与え
るパラメータと同一で, それらを文字列として与えます.

### perl

```perl
use MeCab;
$m = new MeCab::Tagger ("-Ochasen");
print $m->parse ("今日もしないとね");
```

### ruby

```ruby
require 'MeCab'
m = MeCab::Tagger.new ("-Ochasen")
print m.parse ("今日もしないとね")
```

### python

```python
import sys
import MeCab
m = MeCab.Tagger ("-Ochasen")
print m.parse ("今日もしないとね")
```

### Java

```java
import org.chasen.mecab.Tagger;
import org.chasen.mecab.Node;
public static void main(String[] argv) {
    Tagger tagger = new Tagger ("-Ochasen");
    System.out.println (tagger.parse ("太郎は二郎にこの本を渡した.")); 
}
```

## 各形態素の詳細情報を取得する

`MeCab::Tagger` クラスの, `parseToNode` という
メソッドを呼ぶことで, 「文頭」という特別な形態素が `MeCab::Node` クラスのインスタンスとして
取得できます.

`MeCab::Node` は, 双方向リストとして表現されており, `next`, `prev` というメンバ変数があります.
それぞれ, 次の形態素, 前の形態素を `MeCab::Node` クラスのインスタンスとして
返します. 全形態素には, `next` を順次呼ぶことでアクセスできます.

`MeCab::Node` は C 言語のインタフェイスで提供している `mecab_node_t` をラップしたクラスです.
`mecab_node_t` が持つほぼすべてのメンバ変数にアクセスすることができます.
ただし, `surface` のみ, 単語そのものが返るように変更しています.

以下に [perl][perl] の例を示します. この例では, 
各形態素を順次にアクセスし,形態素の表層文字列, 品詞, その形態素までのコストを表示します.

```perl
use MeCab;
my $m = new MeCab::Tagger ("");

for (my $n = $m->parseToNode ("今日もしないとね"); $n ; $n = $n->{next}) {
   printf ("%s\t%s\t%d\n",
            $n->{surface},          # 表層
            $n->{feature},          # 現在の品詞
            $n->{cost}              # その形態素までのコスト
            );
}
```

## エラー処理

もし, コンストラクタや, 解析途中でエラーが起きた場合は, 
RuntimeError 例外が発生します. 
例外のハンドリングの方法は, 各言語のリファレンスマニュアルを
ごらんください. 以下は, [python][python] の例です

```python
try:
    m = MeCab.Tagger ("-d .")
    print m.parse ("今日もしないとね")
except RuntimeError, e:
    print "RuntimeError:", e;
```


## 注意事項

### 文頭,文末形態素

`parseToNode` の返り値は, 「文頭」という特別な形態素を示す `MeCab::Node`
インタンスです. さらに, 「文末」という特別な形態素も存在いたしますので,
注意してください. もし, これらを無視したい場合は, 以下のように
`next` でそれぞれを読み飛ばしてください.

```perl
my $n = $m->parseToNode ("今日もしないとね"); 
$n = $n->{next}; # 「文頭」を無視

while ($n->{next}) { # next を調べる
  printf ("%s\n", $n->{surface});
  $n = $n->{next}; # 次に移動
}
```

### MeCab::Node の振舞い

`MeCab::Node` の実体(メモリ上にある形態素情報)は, 
`MeCab::Tagger` インスタンスが管理しています. `MeCab::Node` は,
Node の実体を指している **参照** にすぎせん. そのために, `parseToNode` が
呼ばれる度に, 実体そのものが, 上書きされていきます. 以下のような例はソースの意図する通りには動きません.
 
```python
m = MeCab.Tagger ("")
n1 = m.parseToNode ("今日もしないとね") 
n2 = m.parseToNode ("さくさくさくら")

# n1 の内容は無効になっている
while (n1.hasNode () != 0):
   print n1.getSurface ()
   n1 = n1.next ()
```

上記の例では, `n1` の指す中身が, 「さくさくさくら」を解析した時点で
上書きされており, 使用できなくなっています. 

複数の Node を同時にアクセスしたい場合は, 複数の `MeCab::Tagger` インスタンスを生成してください.

## 全メソッド

以下に, [SWIG][SWIG] 用のインタフェースファイル
の一部を示します. バイディングの実装言語の都合上, C++ のシンタックスで
表記されていますが, 適宜読みかえてください. また, 各メソッドの動作も添え
ていますので参考にしてください.

```cpp
namespace MeCab {

  class Tagger {

     // str を解析して文字列として結果を得ます. len は str の長さ(省略可能)
     string parse(string str, int len);
  
     // parse と同じ
     string parseToString(string str, int len);
  
     // str を解析して MeCab::Node 型の形態素を返します. 
     // この形態素は文頭を示すもので, next を順に辿ることで全形態素にアクセスできます
     Node parseToNode(string str, int len);
  
     // parse の Nbest 版です. N に nbest の個数を指定します.
     // この機能を使う場合は, 起動時オプションとして -l 1 を指定する必要があります
     string parseNBest(int N, string str, int len);
  
     // 解析結果を, 確からしいものから順番に取得する場合にこの関数で初期化を行います.
     bool  parseNBestInit(string str, int len);
  
     // parseNbestInit() の後, この関数を順次呼ぶことで, 確からしい解析結果を, 順番に取得できます.
     string next();
  
     // next() と同じですが, MeCab::Node を返します.
     Node  nextNode();
  };
  
  #define MECAB_NOR_NODE  0
  #define MECAB_UNK_NODE  1
  #define MECAB_BOS_NODE  2
  #define MECAB_EOS_NODE  3
  
  struct Node {

    struct Node  prev;  // 一つ前の形態素へのポインタ
    struct Node  next;  // 一つ先の形態素へのポインタ
    
    struct Node  enext; // 同じ位置で終わる形態素へのポインタ
    struct Node  bnext; // 同じ開始位置で始まる形態素へのポインタ
  
    string surface;             // 形態素の文字列情報 
  			      
    string feature;             // CSV で表記された素性情報
    unsigned int   length;      // 形態素の長さ
    unsigned int   rlength;     // 形態素の長さ(先頭のスペースを含む)
    unsigned int   id;          // 形態素に付与される ユニークID
    unsigned short rcAttr;      // 右文脈 id 
    unsigned short lcAttr;      // 左文脈 id
    unsigned short posid;       // 形態素 ID (未使用)
    unsigned char  char_type;   // 文字種情報
    unsigned char  stat;        // 形態素の種類: 以下のマクロの値
                                // #define MECAB_NOR_NODE  0
                                // #define MECAB_UNK_NODE  1
                                // #define MECAB_BOS_NODE  2
                                // #define MECAB_EOS_NODE  3
    unsigned char  isbest;      // ベスト解の場合 1, それ以外 0
  
    float          alpha;       // forward backward の foward log 確率
    float          beta;        // forward backward の backward log 確率 
    float          prob;        // 周辺確率
                                // alpha, beta, prob は -l 2 オプションを指定した時に定義されます
  
    short          wcost;       // 単語生起コスト
    long           cost;        // 累積コスト
  };
}
```

## サンプルプログラム

perl/test.pl, ruby/test.rb, python/test.py, java/test.java
にそれぞれの言語のサンプルがありますので, 参考にしてください.

[perl]: http://www.perl.com "perl"
[ruby]: http://www.ruby-lang.org "ruby"
[python]: http://www.python.org "python"
[Java]: http://java.sun.com "java"
[SWIG]: http://www.swig.org "SWIG"
