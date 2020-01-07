---
title: "制約付き解析(部分解析)"
date: 2020-01-05T15:29:11+09:00
---

## 概要

入力文の一部の形態素情報が既知である、あるいは境界がわかっているときに、
それを満たすように解析する機能です。



たとえば、「にわにはにわにわとりがいる。」という文に対して、
「はにわ」の部分が名詞であるとか、「にわとり」の部分が一つの形態素
であるというように指定した上で解析することができます。このとき、
制約に反する4文字目の「は」が単独で形態素となったり、「にわとり」が「にわ」と「とり」
に分割されるような解析候補は排除されます。


## 入力フォーマット


MeCabのデフォルト出力と同じようなフォーマットで制約を記述します。
MeCabは -p (--partial) オプション付きで起動する必要があります。


各行は以下のいずれかに該当します.
 - 文断片\
     文の断片です。制約がないときと同じように通常の形態素解析が行われます。ただし文断片をまたぐような
     形態素は出力されません。
 - 形態素断片\
     それ以上分割されない、ただ一つの形態素です。この断片がそのまま出力されます。
     形態素断片は必ず
     ```表層\t素性パターン```
     という形で表記する必要があります。\t がない場合は文断片として処理されます。
 - EOS\
     文の終わりを示すマークです。文の終わりには必ず指定してください。

## 素性パターンの記述方法

CSV で記述します。* をワイルドカードとして使うことができます。

- \* : すべての素性
- 名詞: すべての名詞
- *,非自立: 品詞の第二分類が非自立のもの

## 例

品詞の部分に * が指定されると、その単語で切り出し、品詞は適当に最適なものを付与します。

```
にわ    *
に      *
はにわ  *
にわとり        *
が      *
いる    *
EOS
```

品詞そのものを指定することができます。( "*" はワイルドカード)

```にわ  *
に      助詞
はにわ  *
にわとり        *
が      接続詞
いる    *,非自立
EOS
```

品詞のカラムを指定しないと、そのトークンは文断片となり、制約がないときと同じように解析されます。ただし、文断片をまたぐような形態素は出力されません。

```
にわ
には
にわ
にわとり
がいる
EOS
```

## 制限
### 制約の限界

制約付きの解析は、未知語処理を含め可能な限りラティスを作って、制約を満たさないものを枝刈りするという方法で実装されています。
もし制約を満たすものが一つもない場合、ダミーの形態素を作成します。ただしダミーの形態素の品詞情報(素性)は
制約の情報がそのまま使われます。以下の例では「こんな長い入力を一形態素にしてみる」を
動詞の一形態素と指定していますが、動詞を生成するような未知語処理ルールが存在しないため、制約の品詞をそのまま出力しています。


```
% mecab -p
こんな長い入力を一形態素にしてみる      動詞
EOS
こんな長い入力を一形態素にしてみる      動詞
EOS
```

### APIによる制約の指定

-pオプションを用いた場合、任意の場所に単語境界が存在しない・必ず存在するといったような
細かな制限は行えません。より細かい制御を行いたい場合は、Lattice::set_boundary_constraint, Lattice::set_feature_constraint APIを用います。

```cpp
MeCab::Lattice *lattice = MeCab::createLattice();

// boundary の指定 (byte単位で指定)
lattice->set_sentence("thisis");

// |this|is| で分割されるように強制
lattice->set_boundary_constraint(0, MECAB_TOKEN_BOUNDARY);
lattice->set_boundary_constraint(1, MECAB_INSIDE_TOKEN);
lattice->set_boundary_constraint(2, MECAB_INSIDE_TOKEN);
lattice->set_boundary_constraint(3, MECAB_INSIDE_TOKEN);
lattice->set_boundary_constraint(4, MECAB_TOKEN_BOUNDARY);
lattice->set_boundary_constraint(5, MECAB_INSIDE_TOKEN);
lattice->set_boundary_constraint(6, MECAB_TOKEN_BOUNDARY);
tagger->parse(lattice);

// feature の指定 (byte単位で指定)
lattice->set_sentence("thisisatest");

// |this|is|a|test| で分割され、かつ品詞大分類をすべて名詞にするように強制
lattice->set_feature_constraint(0,4,"名詞");
lattice->set_feature_constraint(4,6,"名詞");
lattice->set_feature_constraint(6,7,"名詞");
lattice->set_feature_constraint(7,11,"名詞");

tagger->parse(lattice);
```
