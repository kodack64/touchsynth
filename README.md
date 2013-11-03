﻿touchsynth
==========

五月祭で作ったタッチパネルとの通信とそこで動作するプログラムとか

・動作環境
DirectX9.0
最低ひとつ以上のオーディオ出力
TUIOメッセージを送る外部ソフト


・各フォルダ

*Synth
 タッチディスプレイの画像を受け取ったCommunity Core VisionからのTUIOメッセージを受け取り、処理した上でディスプレイに描画を行う
 Synthフォルダ内部のマネージャは上記の入出力情報をPC上の操作と統合した上で入出力、オーディオ、描画のコンテキストをAPIの形で利用できるようにして、Packageのサブクラスに渡す。
 各Packageサブクラス(Master,Smoke,Piano,Map,Game)はdllとして呼び出され、Packageサブクラスを提供する。

*Master
 他のデモを表示するためのメニュー

*Smoke
 FTIRタッチパネルのデモで度々使用されるものを見よう見まねで移植した。

*Piano
 ASIO出力を用いてタッチされたピアノの音声を低レイテンシで出力する。

*Map
 複数の画像を画面を使う複数人が移動拡大縮小回転出来るようなデモ。
 
*Game
 Pong。自陣の二点をタッチすると一定時間壁を作って玉を反射できる。
 
