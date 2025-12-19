#!/bin/bash
set -e

KERNEL=kernel
BUILD=build

echo "== Compilando kernel =="

i686-elf-gcc -c $KERNEL/kernel.c   -o $BUILD/kernel.o   -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$KERNEL
i686-elf-gcc -c $KERNEL/terminal.c -o $BUILD/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$KERNEL
i686-elf-gcc -c $KERNEL/keyboard.c -o $BUILD/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$KERNEL
i686-elf-gcc -c $KERNEL/shell.c    -o $BUILD/shell.o    -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$KERNEL
i686-elf-gcc -c $KERNEL/util.c     -o $BUILD/util.o     -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$KERNEL

echo "== Linkando kernel =="

i686-elf-gcc -T linker.ld -o $BUILD/myos.bin \
    -ffreestanding -O2 -nostdlib \
    $BUILD/boot.o \
    $BUILD/kernel.o \
    $BUILD/terminal.o \
    $BUILD/keyboard.o \
    $BUILD/shell.o \
    $BUILD/util.o

echo "== Montando ISO =="

cp build/myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg

grub-mkrescue -o $BUILD/myos.iso isodir