# unor4-kanji
kanji viewer for Arduino UNO R4 WiFi matrix LED

# 概要
- Arduino UNO R4 WiFiのマトリクスLEDに漢字を表示するデモプログラムです．
- かなり雑な作りなので公開する予定はなかったのですが，Twitterで共有してほしいとのメッセージをいただいたので公開します．

# 使い方
- ソースコード(ViewBDF.ino)を見て下さい．

# フォント
- フォントは門真なむさんによる[k8x12](https://littlelimit.net/k8x12.htm)を使用しています．再配布可ということなのでこのレポジトリにもそのまま置いてあります．
- bdfフォントをbdftoc.plでCの配列に変換して使っています．(BBXあたりはかなり雑に変換しています．)メモリが足りないせいか，全文字だとコンパイルできないので上の方のコードを削っています．(bdftoc.plのソース参照) ．


