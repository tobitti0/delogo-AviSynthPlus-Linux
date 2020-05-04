#  透過性ロゴ フィルタ for AviSynth+ 3.5.x & Linux
## 概要
AviSynth+はVer3.5.0からNative Linuxをサポートした。  
本フィルタは「透過性ロゴ フィルタ for AviSynth 2.5」をLinuxのAviSynth+で使用できるように改造したものである。  
現状`logo data file ver0.1`のみ対応を確認。（他のVersionがある？持っていないので未確認)  
## 機能
BS･CSでよく見かけるような半透明ロゴを付加または除去します。  
## 書式
* ロゴ付加
````
AddLOGO(logofile="", logoname="",
          pos_x=0, pos_y=0, depth=128,
          yc_y=0,  yc_u=0,  yc_v=0,
          start=0, fadein=0, fadeout=0, end=-1,
          interlaced=false)
````
* ロゴ除去
````
EraseLOGO(logofile="", logoname="",
          pos_x=0, pos_y=0, depth=128,
          yc_y=0,  yc_u=0,  yc_v=0,
          start=0, fadein=0, fadeout=0, end=-1,
          interlaced=false)
````
| オプション| 指定内容 |説明 |
|:---|:---|:---|
|logofile         |ロゴファイル名|*.ldp、*.lgdのどちらも使用できます。|
|logoname         |ロゴ名|省略した場合はロゴデータファイルの先頭に保存されているロゴを使用します。|
|pos_x, pox_y     |位置調整|1/4ピクセル単位に調整します。|
|yc_y, yc_u, yc_v |色調整|ロゴの色を調整します。|
|depth            |不透明度（深度）調整|128で100%として調整します。|
|start, end       |開始･終了フレーム|フレーム番号で指定してください。end<startの時はstart以降の全てのフレームで実行されます。|
|fadein, fadeout  |フェードフレーム数|ロゴをフェードさせます。|
|interlaced       |インターレースフラグ|YV12をインターレースとして扱います。YUY2では無視されます。|
## 利用方法
AviSynth+3.5.x、FFmpegを[公式手順][1]に従ってインストールしてください。  
本フィルタは次の手順で導入できます。  

[1]:https://github.com/AviSynth/AviSynthPlus/blob/3.5/distrib/docs/english/source/avisynthdoc/contributing/posix.rst

````
git clone https://github.com/tobitti0/delogo-AviSynthPlus-Linux.git
cd delogo-AviSynthPlus-Linux/src
make
make install
````
導入確認は`ls -la /usr/local/lib/avisynth`で表示された内容に、  
`libdelogo.so`があれば導入できています。
## おまけ
Dockerを用いて、環境を汚さずに利用できるDockerfileをつけています。  
（適当に作ったのでイメージサイズは2.4GBになります。）  
環境の構築方法は次のとおりです。  
````
git clone https://github.com/tobitti0/delogo-AviSynthPlus-Linux.git
cd delogo-AviSynthPlus-Linux/
docker build -t delogo_trial .
````
初回のみFFmpeg他ライブラリ郡をビルドします。環境によりますが、最低10分程度はかかります。  
次にロゴファイルを`docker/logo`、動画を`docker/input`に入れます。  
cloneしたフォルダで次のコマンドを実行すると、delogoをつけてFFmpegを実行します。  
````
docker run --rm \
           -v $(pwd)/docker:/tmp/delogotrial \
           -it delogo_trial \
           ./delogotrial \
                    -i 「inputに入れた動画のファイル名（拡張子含む）」 \
                    -l 「logoに入れたロゴデータのファイル名（拡張子含む）」 \
                    -o 「出力される動画のファイル名（拡張子含む）」 \
                    -f "「（任意）FFmpegにわたすオプション」"
````
出力されたファイルは`docker/output`に保存されます。  
動作自体は、`docker/delogotrial`のシェルスクリプトが動作するだけです。  
シェルスクリプトはavsファイルを作成し、FFmpegに食わせて、生成されたゴミを消すだけです。  
l-smash-sourceで読める形式ならば、動作すると思います。  
TSをそのまま食わせるとドロップがあると失敗する可能性があります。  
fオプションでコーデックを指定するなど適当な対策を講じてください。  
(そのままでは音ズレ等で使い物にはならないと思いますが、あくまでロゴを消すことができることの確認用です。）
## 謝辞
オリジナルの製作者MakKi氏、  
AviSynth+への対応改修をされた「Avisynthを絶讃ιょぅょ Part31」の126氏、  
に深く感謝いたします。
## 更新履歴
| 更新日 | Version | 更新内容 | 更新者 |
|:---|:---|:---|:---:|
|2020/04/29 |0.05c |・Linuxに対応(Windowsは切り捨て) |tobitti0|
|2013/12/04 |0.05b※ |・AviSynth+に対応 |Avisynthを絶讃ιょぅょ Part31 126氏|
|2008/01/07 |0.05a |・YUY2の時、左端が処理できていなかったバグを修正 |MakKi氏|
|2008/01/07 |0.05 |・ロゴファイルの最大データ数拡張に対応  |MakKi氏|
|2007/06/06 |0.04 |・depthが効かない場合があるバグを修正<br>・yc_y,yc_u,yc_vの効きが弱いバグを修正|MakKi氏|             
|2007/03/21 |0.03 |・全部書き直し<br>・色差の計算での不要な処理を削除<br>・内部処理をAviUtl版とほぼ同じにした<br>・YV12に対応|MakKi氏|
|2004/02/11 |0.02 |・ロゴの色の調整機能を追加  |MakKi氏|
|2003/11/05 |0.01d |・細かなバグ修正  |MakKi氏|
|2003/11/02 |0.01c |・pos_x,pos_yの挙動がおかしかったのを修正<br>・Cropしてある時ロゴが崩れるバグ修正|MakKi氏|
|2003/11/01 |0.01b |・pos_x,pos_yに負の値を入れても正方向になっていたバグ修正  |MakKi氏|
|2003/10/26 |0.01a |・pos_x,pos_yが-200未満の時エラーになることがあったのを修正  |MakKi氏|
|2003/10/01 |0.01 |・公開  |MakKi氏|

※0.05bはバージョンが振られていないのでtobitti0が設定
