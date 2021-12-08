mkdir -p isodir/boot/grub
cp kernel isodir/boot/kernel
cp ../scripts/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir