# RTOSPerf
TOPPERS-OS用性能評価プログラム

## フォルダ

- fmp_1.4.0
  - バージョン 1.4.0 
    - [オリジナルのソース](https://toppers.jp/download.cgi/fmp_zynq_gcc-20190929.zip)
  - テストプログラムフォルダ
    - perf
  - Zybo Z7 用プロジェクトフォルダ
    - target\zynq_gcc\zybo_z7_sdk

- asp_1.9.3
  - バージョン 1.9.3
    - [オリジナルのソース](https://www.toppers.jp/download.cgi/asp_zynq_gcc-20190929.zip)
  - オリジナルからの変更点
    - GTMで性能評価が可能とするよう変更．
  - テストプログラムフォルダ
    - perf
  - Zybo Z7 用プロジェクトフォルダ
    - target\zynq_gcc\zybo_z7_sdk 

- asp3_3.6
  - テストプログラムフォルダ
    - perf
  - Zybo Z7 用プロジェクトフォルダ
    - target\zynq_gcc\zybo_z7_sdk 

## 時間計測方法

グローバルタイマー（GTM）で計測しナノ秒で取得する．


## テストプログラム

| 名称 | 説明2 |
----|---- 
| perf_act_tsk        | act_tskのマルチプロセッサ用評価プログラム  |
| perf_act_tsk_single | act_tskのシングルプロセッサ用評価プログラム |
| perf_sig_sem        | sig_semのシングルプロセッサ用評価プログラム |
| perf_slp_tsk        | slp_tskのシングルプロセッサ用評価プログラム |
