project('my-os', 'c')

drivers = ['drivers/tty.c', 'drivers/tty.h', 'drivers/pit.c', 'drivers/pit.h']
arch_i686 = ['arch/i686/boot.s', 'arch/i686/a20.s', 'arch/i686/a20.h', 
            'arch/i686/gdt.c', 'arch/i686/gdt.h', 'arch/i686/idt.h', 'arch/i686/idt.c', 'arch/i686/interrupt.s', 'arch/i686/helpers.h', 'arch/i686/pic.c', 'arch/i686/pic.h']
printf = ['third_party/printf/printf.c', 'third_party/printf/printf.h']

kernel = executable('kernel', 'main.c', arch_i686, drivers, printf, include_directories: include_directories('.'))
# Run the kernel in QEMU
qemu_args = ['-D', '/tmp/qemu_log']
run_target('qemu', command: ['qemu-system-i386', qemu_args, '-kernel', kernel], depends: [kernel])
run_target('qemu-dbg', command: ['qemu-system-i386', qemu_args, '-s', '-S', '-kernel', kernel])
# Build an iso from the kernel
run_target('iso', command: ['sh', '../scripts/make_iso.sh'], depends: [kernel])

create_gdt = executable('create_gdt', 'utils/create_gdt.c', native: true)