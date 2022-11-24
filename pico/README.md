# RTOSPerf-PICO
TOPPERS-OS用性能評価プログラム(PICO用フォルダ)

## 注意点
計測結果は10ns単位である．そのため，結果が233の場合は，2,330nsととなる．

## フォルダ
- asp3_3.5
	- https://www.toppers.jp/download.cgi/asp3_raspberrypi_pico_gcc-20211016.tar.gz から取得
	- obj : プロジェクトフォルダ
		- obj_sample1 : sample1用フォルダ
		- obj_perf_act_tsk_single : perf_act_tsk_single用フォルダ
		- obj_perf_sig_sem : perf_sig_sem用フォルダ
		- obj_perf_slp_tsk : perf_slp_tsk用フォルダ

- fmp3_3.2
	- https://www.toppers.jp/download.cgi/fmp3_raspberrypi_pico_gcc-20211016.tar.gz から取得
	- obj : プロジェクトフォルダ
		- obj_sample1 : sample1用フォルダ

- openocd-w64
	- MSYS2でビルドしたもの．

## ビルド及び実行

- 各 obj_xxx フォルダにおいて，次のコマンドによりビルド

	```bash
	make 
	```
- 次のコマンドでダウンロード及び実行．
	```bash
	make run
	```

## 開発環境の構築
- WSLのインストール
 - Ubuntu 20.04 をインストール(WSL1で良い)
 	- https://se-abeaver.com/wsl2-guide/
 - ターミナルを起動してビルドに必要なパッケージのインストール
	```bash
	sudo apt update
	sudo apt install -y build-essential gcc-arm-none-eabi ruby git
	```
 - rubyのパッケージのインストール
	```bash
	sudo gem install shell
	```

- ボードの接続
	- https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf の Appendix A: Using Picoprobe に従ってボードを接続．

- PCへ接続とドライバのインストール
	- COMポートが認識されない場合は以下にあるドライバーをインストール
		- https://developer.mbed.org/handbook/Windows-serial-configuration
	- Picoprobeのドライバは以下のページを参照にWinUSBをインストール
 		- https://qiita.com/yunkya2/items/4e3d89f08b2237ef551f

- COMポートはTeratermを115200bpsで接続する		