{ pkgs }: {
	deps = [
		pkgs.unzip
  pkgs.zip
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}