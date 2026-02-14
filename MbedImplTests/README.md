# MbedImpl Tests

このディレクトリには、MbedImpl (Mbed OS を使用した Nano HAL の実装) のテストが含まれています。

## 概要

MbedImpl は ARM Cortex-M マイコン上で実行されるため、これらのテストは通常の x86/x64 ホストではビルド・実行できません。
代わりに、ARM toolchain を使用してビルドし、実際のハードウェア (NUCLEO_F446RE) 上で実行する必要があります。

## 現在の状態

### ビルド成功 ✅

以下の4つのテストが正常にビルドできます:

1. **Test_MbedImpl_Timer** (883 KB) - タイマー機能のテスト
2. **Test_MbedImpl_PWM** (894 KB) - PWM 出力のテスト
3. **Test_MbedImpl_DigitalOut** (848 KB) - デジタル出力のテスト
4. **Test_MbedImpl_CAN** (920 KB) - CAN 通信のテスト

これらのテストは ARM Cortex-M4 用にコンパイルされ、NUCLEO_F446RE ボードにフラッシュして実行できます。

### ビルドエラーのあるテスト ⚠️

以下のテストは依存関係の問題によりビルドできません:

- **Test_MbedImpl_UART** - `NanoMbed/thread.hpp` への参照エラー (uart.hpp の依存関係)
- **Test_MbedImpl_SPI** - SPIConfig の構造不一致 (vector vs ポインタパラメータ)

これらの問題は、Nano パッケージを最新バージョンに更新することで解決される予定です。

## 必要なもの

- ARM toolchain (Clang ARM または GCC ARM)
- NUCLEO_F446RE ボード
- StaticMbedOS パッケージ
- ST-LINK USB ドライバ

## ビルド方法

### 方法1: メインの Nano プロジェクトから (ExternalProject として)

```bash
cd /path/to/libs-dev/libs/Nano
cmake -G Ninja -B build -DBUILD_MBED_IMPL_TESTS=ON
cmake --build build
```

### 方法2: 直接ビルド

```bash
cd /path/to/libs-dev/libs/Nano/MbedImplTests
./build.sh
```

ビルドが成功すると、`build/tests/` ディレクトリに以下のファイルが生成されます:
- `Test_MbedImpl_*.elf` - ELF 実行ファイル
- `Test_MbedImpl_*.bin` - バイナリファイル (フラッシュ書き込み用)
- `Test_MbedImpl_*.hex` - Intel HEX ファイル

## テスト一覧

### Test_MbedImpl_SPI
SPI 通信のテスト。NUCLEO_F446RE の SPI1 ピンを使用:
- MOSI: PA_7
- MISO: PA_6
- SCK: PA_5
- CS: PA_4

### Test_MbedImpl_UART
UART 通信のテスト。ST-LINK 経由の仮想 COM ポートを使用:
- TX: PA_2
- RX: PA_3

### Test_MbedImpl_CAN
CAN 通信のテスト:
- TX: PA_12
- RX: PA_11

### Test_MbedImpl_Timer
タイマー機能のテスト。

### Test_MbedImpl_PWM
PWM 出力のテスト:
- PWM 出力: PA_8

### Test_MbedImpl_DigitalOut
デジタル出力のテスト。オンボード LED を使用:
- LED: LED1 (PA_5)

## 実行方法

1. `.bin` ファイルをボードにフラッシュ:
   ```bash
   cp build/tests/Test_MbedImpl_*.bin /path/to/NODE_F446RE/
   ```

2. シリアルターミナル (115200 baud) でボードに接続:
   ```bash
   screen /dev/ttyACM0 115200
   # または
   minicom -D /dev/ttyACM0 -b 115200
   ```

3. ボードをリセットして、テスト出力を確認

## トラブルシューティング

### ビルドエラー: "StaticMbedOS-NUCLEO_F446RE not found"

StaticMbedOS パッケージがインストールされていることを確認してください。
roboenv-nix を使用している場合、Nix shell 内でビルドしてください。

### ビルドエラー: "ARM toolchain not found"

ARM toolchain がインストールされ、PATH に含まれていることを確認してください。

### ビルドエラー: "NanoMbed/thread.hpp file not found"

インストールされている Nano パッケージが古い可能性があります。
最新の Nano を再インストールしてください:

```bash
cd /path/to/libs-dev/libs/Nano
run build-nano
cmake --install build --prefix ~/.local  # または適切なインストールパス
```

### ビルドエラー: SPI Config 構造の不一致

SPI テストは現在の Nano パッケージと互換性がありません。
Nano パッケージの更新が必要です。

### 実行時エラー: シリアル出力が表示されない

- ST-LINK ドライバが正しくインストールされているか確認
- ボードが正しく接続されているか確認
- ボーレートが 115200 に設定されているか確認
- ボードをリセット (黒いボタンを押す)

## 成功事例

2026年2月14日時点で、以下のテストが正常にビルドされています:

```bash
$ ls -lh build/tests/Test_MbedImpl_*
-rwxr-xr-x  920k  Test_MbedImpl_CAN
-rwxr-xr-x  848k  Test_MbedImpl_DigitalOut
-rwxr-xr-x  894k  Test_MbedImpl_PWM
-rwxr-xr-x  883k  Test_MbedImpl_Timer
```

これらの .elf ファイルは NUCLEO_F446RE ボードにフラッシュして実行可能です。

## 注意事項

- これらのテストは実機上でのみ実行可能です
- CI/CD パイプラインでは自動実行できません (ビルドのみ可能)
- テスト用のハードウェアセットアップが必要な場合があります (CAN トランシーバーなど)
