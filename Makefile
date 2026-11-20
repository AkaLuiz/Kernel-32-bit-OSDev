KERNEL_DIR := kernel
BUILD_DIR := build
ISO_DIR := isodir

CC := i686-elf-gcc
ASFLAGS := -ffreestanding -O2 -Wall -Wextra
CFLAGS := -std=gnu99 $(ASFLAGS) -I$(KERNEL_DIR)
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib

# macOS (Homebrew) usa i686-elf-grub-mkrescue; Linux usa grub-mkrescue
GRUB_MKRESCUE ?= $(shell command -v i686-elf-grub-mkrescue 2>/dev/null || echo grub-mkrescue)

BIN := $(BUILD_DIR)/myos.bin
ISO := $(BUILD_DIR)/myos.iso

OBJS := \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/terminal.o \
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/shell.o \
	$(BUILD_DIR)/util.o \
	$(BUILD_DIR)/pong.o

.PHONY: all iso clean

all: $(BIN)

iso: $(ISO)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: $(KERNEL_DIR)/boot.s | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(ASFLAGS)

$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/terminal.o: $(KERNEL_DIR)/terminal.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/keyboard.o: $(KERNEL_DIR)/keyboard.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/shell.o: $(KERNEL_DIR)/shell.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/util.o: $(KERNEL_DIR)/util.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/pong.o: $(KERNEL_DIR)/pong.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN): $(OBJS) linker.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(ISO): $(BIN) grub.cfg | $(BUILD_DIR)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BIN) $(ISO_DIR)/boot/myos.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)

clean:
	rm -f $(BUILD_DIR)/*.o $(BIN) $(ISO)
