# Nano
組み込み開発に頻出の構造・デバイスの抽象化をまとめたライブラリです

## コンポーネント

- `Nano`: C/C++ 組み込み開発で便利でかつ広く使えるライブラリをまとめたものです
- `NanoHW`: ハードウェア抽象化 (静的・動的ディスパッチどちらもできます)
- `MbedIF`: `NanoHW` の機能を `MbedOS` のインターフェースで扱えるもの (後方互換性)
- `MbedImpl`: `NanoHW` の動的ディスパッチの対象を `MbedOS` のものにするターゲット
- `StubImpl`: `NanoHW` の動的ディスパッチの対象を C++ 標準機能のものにするターゲット

## インストール
以下に `flake.nix` の編集例を示します
`...` は省略です

```diff
{
  ...
+ inputs.nano.url = "git+ssh://git@github.com/nnctroboticsclub/Nano";
  ...

  outputs =
    { ... }:
    let
      ...
    in
    {
      devShells.x86_64-linux.default = roboenvPackages.roboenv {
        ...
        cmakeInputs = [
          ...
+         nano.packages.x86_64-linux.default
          ...
        ];
        ...
      };
    };
}
```
