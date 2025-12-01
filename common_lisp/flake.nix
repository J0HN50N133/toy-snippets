{
  description = "Common Lisp Dev Environment (SBCL + CFFI Ready)";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        # 在这里定义你需要通过 CFFI 加载的 C/Rust 库
        # 例如: openssl 用于 cl+ssl, sqlite 用于 cl-dbi, 或者是你自己编译的 .so
        nativeLibs = with pkgs; [ openssl sqlite zlib libuv ];

      in {
        devShells.default = pkgs.mkShell {
          # 编译时工具
          nativeBuildInputs = with pkgs; [ pkg-config ];

          # 运行时工具
          buildInputs = with pkgs;
            [
              sbcl # 编译器
              rlwrap # 交互式体验增强
            ] ++ nativeLibs;

          # 核心魔法：让 CFFI 能找到 Nix Store 里的 .so 文件
          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath nativeLibs;

          # 环境变量配置
          # CL_SOURCE_REGISTRY: 告诉 ASDF 优先在当前目录查找 .asd 文件
          shellHook = ''
            export CL_SOURCE_REGISTRY=$PWD/

            echo "==== Common Lisp Dev Environment ===="
            echo "Compiler: $(sbcl --version)"
            echo "CFFI Lib Path Set: ${
              builtins.toString (builtins.length nativeLibs)
            } libraries included."
            echo "Source Registry: $CL_SOURCE_REGISTRY"
          '';
        };
      });
}
