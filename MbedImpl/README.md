# MbedImpl

MbedOS の機能で HW を実装したものを提供します

## 使い方
### 1. プロジェクトルートの CMakeLists.txt で MbedImpl を StaticMbedOS で生成する

以下のような記述をして MbedOS のターゲットに対応する MbedImpl のターゲットを作成してください

※以下の例は開発環境に NUCLEO_F446RE 向けの StaticMbedOS がインストールされていることが前提です
```cmake
# 以下の行が無い場合は nano_hw_mbed_impl 呼び出しよりも前の好きな場所に追加する
find_package(Nano REQUIRED)

# StaticMbedOS-NUCLEO_F446RE (NUCLEO_F446RE 向けの StaticMbedOS) を使える状態にしておく
find_package(StaticMbedOS-NUCLEO_F446RE REQUIRED)

# 構文: nano_hw_mbed_impl(ImplTarget MbedTarget)
# 役割: MbedTarget に対応する MbedImpl ターゲットを ImplTarget という名前で作成する
nano_hw_mbed_impl(NanoHWImpl_F446RE StaticMbedOS-NUCLEO_F446RE)
```

### 2. MbedImpl ターゲットを**実行ファイル**にリンクするよう指示する

例

```cmake
# 実行ファイル定義
add_executable(MyRoboticsApplication ...)

# MbedImpl を使うのでこれが必須
# NanoHWImpl_F446RE は nano_hw_mbed_impl で作成した MbedImpl ターゲットであれば何でもよい
target_link_libraries(MyRoboticsApplication PRIVATE NanoHWImpl_F446RE)

# StaticMbedOS で作成した MbedImpl の場合これが必須
static_mbed_os_nucleo_f446re_app_target(MyRoboticsApplication)

...
```
