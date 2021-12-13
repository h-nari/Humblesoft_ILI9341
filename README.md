# Humblesoft_ILI9341

Adafruit_ILI9341の機能拡張版のArduinoライブラリ

<div align="center"><img src="https://github.com/h-nari/Humblesoft_ILI9341/blob/master/img/160907a0.jpg?raw=true" width="512"></div>

## 機能拡張項目

* fontxによる日本語表示
 * wrap, getTextBoundsも対応
 * SPIFFS内fontxファイルにも対応
* writedata(uint8_t *data, uint32_t len)追加
* VerticalScrollAreaサポート

## 必要な他のライブラリ

* Adafruit_GFX:  https://github.com/adafruit/Adafruit-GFX-Library
* Fontx:  https://github.com/h-nari/Fontx
* Humblsefot_GFX: https://github.com/h-nari/Humblesoft_GFX

## 対象ハードウェア

* ESP8266 Ardino + ILI9341をコントローラとするLCD
 * 例えば、[ハンブルソフト HSES-LCD24](http://www.humblesoft.com/products/HSES-LCD24/ "HSES-LCD24/ 製品情報/ ハンブルソフト")
* 他のArduinoでも動くかもしれませんが、テストしていません。

## fontx使用例

サンプルプログラム
[kanjiDisp](https://github.com/h-nari/Humblesoft_ILI9341/blob/master/examples/kanjiDisp/kanjiDisp.ino "kanjiDisp.ino")をご覧ください

##  同梱しているfontxファイル

fontxディレクトリに同梱させていただいているfontxファイルは、文市(あやち)さんが [blog](http://ayati.cocolog-nifty.com/blog/2012/08/ipalx322416-64a.html "blog")で公開されているILフォントです。ILフォントはIPAフォントをFONTX形式に変換したフォントです。IPAフォントのLicense Agreementも同梱しています。

同梱しているのは16dot ゴシックの半角と全角の個のファイルですが、ILフォントでは16,24,32ドットの明朝、ゴシック、太字処理したものなど多くのファイルが配布されていますので、興味がある人は文市さんの[blog](http://ayati.cocolog-nifty.com/blog/2012/08/ipalx322416-64a.html"blog")からダウンロードしてみてください。

## 謝辞

- Adafruit_ILI9341ライブラリを参考にさせて頂きました。
- fontx関連の多くの情報はChaNさんのサイトのテクニカルノート"[FONTXの使いかた](http://elm-chan.org/docs/dosv/fontx.html "FONTXの使いかた")"等から得ております。ありがとうございます。
- その他 fontxファイルを公開された方も、ありがとうございます。

## 変更履歴

* 1.3.0 2021/12/13(Mon) ESP32対応、 Adafruit-GFX-Library 1.10.12で動作確認
* 1.2.0 2019/09/12(Thr) Adafuit-GFX-Library 1.5.6対応
* 1.1.0 2017/06/27(Tue) Adafuit_ILI9341の機能を取り込み、includeしなくなった。
 * 1.5～5倍程度の高速化を行った。
 * settAddrWindow, writedata, writecommand等はspi_begin,spi_endで囲まないと動かなくなった。
* 1.0.1 2016/11/02(Wed) Fontx/FontxGfxライブラリ分離、VerticalScrollAreaサポート
* 1.0.0 2016/09/07(Wed) 初版リリース
