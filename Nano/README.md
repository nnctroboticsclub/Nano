# Nano

組み込み開発向けのデータ構造や Utility クラスをまとめたライブラリです

## 機能

### セマンティクス関係
- [non_copyable.hpp](./include/Nano/non_copyable.hpp): コピー不可にする CRTP
- [singleton.hpp](./include/Nano/singleton.hpp): シングルトンを構成する CRTP

### Utility
- [clock.hpp](./include/Nano/clock.hpp): STL 互換の Clock 型を作成する Utility
- [result.hpp](./include/Nano/result.hpp): エラー付きで処理の結果を表せるクラス
- [scratch.hpp](./include/Nano/scratch.hpp): スクラッチバッファを提供する静的クラス

### データ構造
- [fixed_map.hpp](./include/Nano/fixed_map.hpp): KV 構造をメモリアロケーション無しで扱えるクラス
- [linked_list.hpp](./include/Nano/linked_list.hpp): リンクリストを Iterator として扱えるクラス
- [managed_list.hpp](./include/Nano/managed_list.hpp): 静的領域を Allocate できる配列 (delete は出来ない)
- [no_mutex_lifo.hpp](./include/Nano/no_mutex_lifo.hpp): Mutex を使わない Queue (single-producer single-consumer 前提)
- [span.hpp](./include/Nano/span.hpp): 連続コンテナへの参照を表す型
