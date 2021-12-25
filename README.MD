# x86 kernel

## Roadmap
- [x] Boots
- [x] VGA Text mode
- [x] GDT
  - [x] Detect existing GDT
  - [x] Set our own GDT
- [x] Interrupts
  - [x] Set up the IDT
  - [x] Set up the universal interrupt handler
  - [ ] Set up the PIC
   - [x] Init the PIC
   - [ ] Finish the driver
  - [x] Add the runtime-defined handlers
- [x] Timer
  - [x] Set up the PIT
  - [x] Write the IRQ0 handler
  - [x] Display the seconds since boot
- [ ] PS/2 Keyboard
  - [ ] Init the keyboard
  - [ ] Set up the IRQ
  - [ ] Write a command queue
  - [ ] Write an input buffer
- [ ] Shell
  - [ ] Display input to screen
  - [ ] React to commands 
- [ ] PCI
- [ ] USB

## Build instructions
Requires `meson` and `ninja` build tools and an `i686-elf` gcc cross-compilation
toolchain located in `/usr/bin`.

First, setup the builddir **in the root of this repository** like this:
```shell
meson setup builddir --cross-file cross_build.ini
```

Then, you can build the kernel like this:
```shell
cd builddir # go into the build directory
meson compile # compile the kernel
```
### Running in an emulator
Requires `qemu-system-x86_64`.

Run `ninja qemu` in the build directory.

### Making an .iso file for flashing onto a flash drive/CDROM
Requires `grub-mkrescue` (a part of GRUB2) and all its dependencies (the major one seems to be `xorriso`).

Run `ninja iso` in the build directory.

### Nix Shell
You can get a nix shell via `nix-shell`. On the first run it might have to compile a gcc cross-compiler for you, which will take time. On the bright side, you won't have to do it by hand, which you would otherwise pretty much have to do.

### What if I'm on Windows though?
You can probably set up the minGW toolchain and QEMU. I believe in you.

Also, you can use WSL, which is included by default in Win11, as far as I've heard.