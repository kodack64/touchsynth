マルチタッチスクリーン
==========

赤外光を利用したマルチタッチスクリーン上で動作するソフトウェアです。五月祭で展示されました。「infrared multitouch screen」で検索するといろいろな製作記が出てきます。

プログラムはsynthプロジェクトが本体で、タッチ情報の取得や描画/音声デバイスの初期化やAPIの提供などを行います。他のプロジェクトgame/map/piano/smokeの四つはsynthから呼び出されるアプリケーション用のDLLになります。gameはタッチして球を打ち返すホッケーゲーム、mapは展示会場の地図画像などを複数人が並行して確認できる地図アプリ、pianoはASIOデバイスを用いて低レイテンシで音声入出力を行うピアノアプリ、smokeはreal-time fluid dynamics simulationを用いてスモークを用いたデモです。

ビルドの際には以下を必要とします。
- IR multitouch displayの実機（筐体、プロジェクタ、カメラなどを必要とします）
- Community Core Vision (カメラの情報からタッチ情報をラベリングしてトラックし、通信を介してプログラムとやり取りできます)
- reacTIVision TUIO Client (Community Core Visionから送られてきたタッチ情報をクライアントとして受信して処理するためのツールキットです)
- portaudio (ASIOデバイスを介して音声を入出力するのに利用します)
- DirectX9 SDK (描画処理に必要です)
- fluid2d (Gustav Taxen氏によるfluid dynamics simulationの実装です)
- fftw (fluid simulationから呼び出されます)
- OpenCV (カメラ情報の取得と画像処理に利用します)
個々のアプリケーションはDLLMain.cppの音声、画像、入力に対するコールバックを記述することで作成します。
