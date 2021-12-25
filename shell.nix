{ pkgs ? import <nixpkgs> {}, 
  pkgsCross ? import <nixpkgs> {
    crossSystem = {
      config = "i686-elf";
    };
  } }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = [ 
      pkgs.meson 
      pkgs.ninja 
      pkgs.qemu
      pkgs.grub2
      pkgs.xorriso
      pkgs.gdb
      pkgsCross.buildPackages.gcc
    ];
    
}
