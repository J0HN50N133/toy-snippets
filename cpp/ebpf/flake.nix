{
  description = "eBPF Development Environment (libbpf + Clang)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = import nixpkgs { inherit system; };
      in {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            pkg-config
            gnumake
            clang_18
            llvm_18
            llvmPackages_18.clang-tools
          ];

          buildInputs = with pkgs; [
            libbpf
            bpftools
            elfutils
            zlib
            #glibc.static
          ];

          # 禁用 hardening 以允许 -target bpf
          hardeningDisable = [ "all" ];

          shellHook = ''
            export CLANG=clang
            export BPFTOOL=bpftool

            # 自动配置 libbpf 路径
            export LIBBPF_INCLUDE=$(pkg-config --cflags-only-I libbpf | sed 's/-I//')
            export LIBBPF_LIB=$(pkg-config --libs-only-L libbpf | sed 's/-L//')

            generate_vmlinux_h() {
              bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
            }

            echo "🚀 eBPF Environment Ready"
            echo "   Clang Version: $(clang --version | head -n1)"
            echo "   Bpftool Version: $(bpftool --version | head -n1)"
          '';
        };
      });
}
