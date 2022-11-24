# RTOSPerf-PICO
TOPPERS-OS用性能評価プログラム(PICO用フォルダ)


## フォルダ
- asp3_3.5
	- https://www.toppers.jp/download.cgi/asp3_raspberrypi_pico_gcc-20211016.tar.gz から取得
	- obj : プロジェクトフォルダ
		- obj_sample1 : sample1用フォルダ

- fmp3_3.2
	- https://www.toppers.jp/download.cgi/fmp3_raspberrypi_pico_gcc-20211016.tar.gz から取得
	- obj : プロジェクトフォルダ
		- obj_sample1 : sample1用フォルダ

- openocd-w64
	- MSYS2でビルドしたもの．

## 開発環境の構築
- WSLのインストール
 - Ubuntu 20.04 をインストール(WSL1で良い)
 	- https://se-abeaver.com/wsl2-guide/
 - ターミナルを起動してビルドに必要なパッケージのインストール
	- sudo apt update
	- sudo apt install -y build-essential gcc-arm-none-eabi ruby git
 - rubyのパッケージのインストール
	- sudo gem install shell

- ボードの接続
	- https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf の Appendix A: Using Picoprobe に従ってボードを接続．

- PCへ接続とドライバのインストール
	- COMポートが認識されない場合は以下にあるドライバーをインストール
		- https://developer.mbed.org/handbook/Windows-serial-configuration
	- Picoprobeのドライバは以下のページを参照にWinUSBをインストール
 		- https://qiita.com/yunkya2/items/4e3d89f08b2237ef551f