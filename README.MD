# x86 kernel

## Roadmap
- [x] Boots
- [x] VGA Text mode
- [ ] GDT
  - [x] Detect existing GDT
  - [ ] Set our own GDT
- [x] Interrupts
  - [x] Set up the IDT
  - [x] Set up the universal interrupt handler
  - [ ] Set up the PIC
   - [x] Init the PIC
   - [ ] Finish the driver
  - [ ] Add the runtime-defined handlers
- [ ] Timer
  - [ ] Set up the PIT
  - [ ] Write the IRQ0 handler
  - [ ] Display the seconds since boot
- [ ] PS/2 Keyboard
  - [ ] Init the keyboard
  - [ ] Set up the IRQ
  - [ ] Write a command queue
  - [ ] Write an input buffer
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
Requires `grub-mkrescue` and all its dependencies.

Run `ninja iso` in the build directory.

### What if I'm on Windows though?
You can probably set up the minGW toolchain and QEMU. I believe in you.

Also, you can use WSL, which is included by default in Win11, as far as I've heard.