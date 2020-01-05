---
title: "MeCab の辞書構造と汎用テキスト変換ツールとしての利用"
date: 2020-01-05T15:19:29+09:00
---

## 概要

単語辞書の構造を理解することで, MeCab を汎用的なテキスト変換ツールとして利用することができます. 例えば, 
ひらがな to カタカナ変換, ローマ字 to ひらがな変換, Auto Link等を MeCab だけで実行できます

## ファイル

単語辞書を構築するには, 最低以下のファイルを作成する必要があります. 

- `*.csv` ファイル (単語辞書)
- `matrix.def` (連接表)
- `unk.def` (未知語用品詞定義)
- `char.def` (未知語の文字定義)
- `dicrc` (設定ファイル)

### *.csv ファイル

単語辞書です

エントリは, 以下のような CSV で追加します.

```css
test,1223,1223,6058,foo,bar,baz
```

最初の4つは必須エントリで, それぞれ

- 表層形
- 左文脈ID (単語を左から見たときの文脈 ID)
- 右文脈ID (単語を右から見たときの文脈 ID)
- 単語コスト (小さいほど出現しやすい)

となっています. コスト値は `short int` (16bit 整数)
の範囲におさめる必要があります.

```
5カラム目以降は, ユーザ定義の CSV フィールドです. 基本的に
どんな内容でも CSV の許す限り追加することができます. 
```

### matrix.def

最初の行に連接表のサイズ(前件サイズ, 後件サイズ)を書きます. その後は, 
連接表の前件の文脈 ID, 後件の文脈IDと, それに対応するコストを書きます. 

ある単語 A, B が連接をなすとき,

- 前件文脈ID = 単語Aの右文脈ID 
- 後件文脈ID = 単語Bの左文脈ID

となります. つまり, 単語辞書に登録した ID が連接表を参照する際の
キーとなります. コスト値は short int (16bit 整数)
の範囲におさめる必要があります.

```
100 120
0 0 1
0 1 10
0 2 5
```

上記の例では, 前件の文脈のサイズが100, 後件の文脈のサイズが 120 となって
います. また, 前件文脈 0 から 後件文脈 1 への遷移コストが 10 となっています. 

### char.def

未知語処理のルールです. [こちら]({{<relref learn.md>}})を御覧ください.

以下が最低限の設定 (DEFAULT と SPACE) です

```
DEFAULT 1 0 0
SPACE   0 1 0
0x0020 SPACE
```

### unk.def

未知語に対する品詞列のテーブルです. [こちら]({{<relref learn.md>}})を
御覧ください.

以下が最低限の設定 (DEFAULT と SPACE) です

```
DEFAULT,0,0,0,*
SPACE,0,0,0,*
```

## 辞書のコンパイル
次のコマンドを実行することで, 解析用のバイナリ辞書を作成します.

```
% /usr/local/libexec/mecab/mecab-dict-index 
```

## ケーススタディ

example ディレクトリにいくつかの応用例があります.

### Auto Link

Hatena Keyword のような Auto Link を実装してみます

#### url.csv

単語としてキーワード, 品詞としてキーワードに対応する URL を書きます.
連接の状態は1状態で十分なので, 左文脈/右文脈IDともに 0 とします. 
コスト値は長いキーワードが優先されるよう設定します. 例えば以下のような関数を使うとよいでしょう.

```
cost = (int)max(-36000, -400 * (length^1.5))
```

```
Google,0,0,-5878,http://www.google.com/
Yahoo,0,0,-4472,http://www.yahoo.com/
ChaSen,0,0,-5878,http://chasen.org/
京都,0,0,-3200,http://www.city.kyoto.jp/
...
```

#### matrix.def
1 状態なので, 連接表のサイズは 1 x 1 となります. 
後件 0 から前件 0 の連接コストは 0 とします.

```
1 1
0 0 0
```

#### char.def
最低限の設定 (DEFAULT と SPACE) です

```
DEFAULT 1 0 0
SPACE   0 1 0
0x0020 SPACE
```

#### unk.def
最低限の設定 (DEFAULT と SPACE) です

```
DEFAULT,0,0,0,*
SPACE,0,0,0,*
```

#### dicrc
autolink というフォーマットを作成し, それがデフォルトの出力になるようにします

```
cost-factor = 800
bos-feature = BOS/EOS
output-format-type=autolink

node-format-autolink = <a href="%H">%M</a>
unk-format-autolink = %M
eos-format-autolink = \n
```

#### コンパイル + テスト

```
% /usr/local/libexec/mecab/mecab-dict-index -f euc-jp -c euc-jp
reading ./unk.def ..  2
emitting double-array: 100% |###########################################| 
reading ./dic.csv ..  4
emitting double-array: 100% |###########################################| 
emitting matrix      : 100% |###########################################
done!

% mecab -d .
京都に行った. 
<a href="http://www.city.kyoto.jp/">京都</a>に行った。
YahooとGoogle
<a href="http://www.yahoo.com/">Yahoo</a>と<a href="http://www.google.com/">Google</a>
```

### ひらがな to カタカナ変換ツール

#### dic.csv

単語としてひらがな1文字, 品詞として各ひらがな対応するカタカナ1文字を書きます.
連接の状態は1状態で十分なので, 左文脈/右文脈IDともに 0 とします. 
曖昧性がないため コスト値は 0 とします

```
う゛,0,0,0,ヴ
あ,0,0,0,ア
い,0,0,0,イ
う,0,0,0,ウ
え,0,0,0,エ
お,0,0,0,オ
ぁ,0,0,0,ァ
ぃ,0,0,0,ィ
ぅ,0,0,0,ゥ
ぇ,0,0,0,ェ
ぉ,0,0,0,ォ
か,0,0,0,カ
き,0,0,0,キ
く,0,0,0,ク
け,0,0,0,ケ
こ,0,0,0,コ
が,0,0,0,ガ
ぎ,0,0,0,ギ
ぐ,0,0,0,グ
げ,0,0,0,ゲ
ご,0,0,0,ゴ
さ,0,0,0,サ
し,0,0,0,シ
す,0,0,0,ス
せ,0,0,0,セ
そ,0,0,0,ソ
ざ,0,0,0,ザ
じ,0,0,0,ジ
ず,0,0,0,ズ
ぜ,0,0,0,ゼ
ぞ,0,0,0,ゾ
た,0,0,0,タ
ち,0,0,0,チ
つ,0,0,0,ツ
て,0,0,0,テ
と,0,0,0,ト
だ,0,0,0,ダ
ぢ,0,0,0,ヂ
づ,0,0,0,ヅ
で,0,0,0,デ
ど,0,0,0,ド
っ,0,0,0,ッ
な,0,0,0,ナ
に,0,0,0,ニ
ぬ,0,0,0,ヌ
ね,0,0,0,ネ
の,0,0,0,ノ
は,0,0,0,ハ
ひ,0,0,0,ヒ
ふ,0,0,0,フ
へ,0,0,0,ヘ
ほ,0,0,0,ホ
ば,0,0,0,バ
び,0,0,0,ビ
ぶ,0,0,0,ブ
べ,0,0,0,ベ
ぼ,0,0,0,ボ
ぱ,0,0,0,パ
ぴ,0,0,0,ピ
ぷ,0,0,0,プ
ぺ,0,0,0,ペ
ぽ,0,0,0,ポ
ま,0,0,0,マ
み,0,0,0,ミ
む,0,0,0,ム
め,0,0,0,メ
も,0,0,0,モ
ゃ,0,0,0,ャ
や,0,0,0,ヤ
ゅ,0,0,0,ュ
ゆ,0,0,0,ユ
ょ,0,0,0,ョ
よ,0,0,0,ヨ
ら,0,0,0,ラ
り,0,0,0,リ
る,0,0,0,ル
れ,0,0,0,レ
ろ,0,0,0,ロ
ゎ,0,0,0,ヮ
わ,0,0,0,ワ
ゐ,0,0,0,ヰ
ゑ,0,0,0,ヱ
を,0,0,0,ヲ
ん,0,0,0,ン
```

#### matrix.def

1 状態なので, 連接表のサイズは 1 x 1 となります. 
後件 0 から前件 0 の連接コストは 0 とします.

```
1 1
0 0 0
```

#### char.def

最低限の設定 (DEFAULT と SPACE) です

```
DEFAULT 1 0 0
SPACE   0 1 0
0x0020 SPACE
```

#### unk.def

最低限の設定 (DEFAULT と SPACE) です

```
DEFAULT,0,0,0,*
SPACE,0,0,0,*
```

#### dicrc
katakana というフォーマットを作成し, それがデフォルトの出力になるようにします

```
dictionary-charset = euc-jp
cost-factor = 800
bos-feature = BOS/EOS
output-format-type=katakana

node-format-katakana = %H
unk-format-katakana = %M
eos-format-katakana  = \n
```

#### コンパイル + テスト

```
% /usr/local/libexec/mecab/mecab-dict-index -f euc-jp -c euc-jp
reading ./unk.def ..  2
emitting double-array: 100% |###########################################| 
reading ./dic.csv ..  4
emitting double-array: 100% |###########################################| 
emitting matrix      : 100% |###########################################
done!
% mecab -d .
これはてすとです
コレハテストデス
```