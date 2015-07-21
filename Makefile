kixos: kernel.o boot.o linker.ld
	i686-elf-gcc -T linker.ld -o kixos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o

kernel.o: kernel.c
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o: boot.s
	i686-elf-as boot.s -o boot.o

clean:
	rm -f kernel.o boot.o kixos.bin
	rm -rf isodir/

iso: kixos
	mkdir -p isodir/boot/grub
	cp kixos.bin isodir/boot/kixos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kixos.iso isodir

iso-run: iso
	qemu-system-i386 -cdrom kixos.iso

run: kixos
	qemu-system-i386 -kernel kixos.bin

cd: iso
	sudo dd if=kixos.iso of=/tmp/blah && sync

all: kixos

