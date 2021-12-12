project('my-os', 'c')

kernel_files = []
kernel_objs = []

subdir('arch')
subdir('drivers')
subdir('third_party/printf')
subdir('services')

kernel = executable('kernel', 'main.c', kernel_files, include_directories: include_directories('.'), objects: kernel_objs)
# Run the kernel in QEMU
qemu_args = ['-D', '/tmp/qemu_log']
run_target('qemu', command: ['qemu-system-i386', qemu_args, '-kernel', kernel], depends: [kernel])
run_target('qemu-dbg', command: ['qemu-system-i386', qemu_args, '-s', '-S', '-kernel', kernel])
# Build an iso from the kernel
run_target('iso', command: ['sh', '../scripts/make_iso.sh'], depends: [kernel])

create_gdt = executable('create_gdt', 'utils/create_gdt.c', native: true)