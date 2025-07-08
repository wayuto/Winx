SRC            := ./src
BUILD          := ./build
SRC_BOOT       := $(SRC)/boot
SRC_KERNEL     := $(SRC)/kernel
SRC_LIB        := ./lib
SRC_DEVICE     := $(SRC)/device
SRC_THREAD     := $(SRC)/thread
SRC_USERPROG   := $(SRC)/userprog
SRC_FS         := $(SRC)/fs
SRC_SHELL      := $(SRC)/shell
SRC_IMPL       := $(SRC)/impl
SRC_USER       := $(SRC)/user
SRC_TAU        := $(SRC)/tau
BUILD_BOOT     := $(BUILD)/boot
BUILD_DEVICE   := $(BUILD)/device
BUILD_KERNEL   := $(BUILD)/kernel
BUILD_THREAD   := $(BUILD)/thread
BUILD_USERPROG := $(BUILD)/userprog
BUILD_FS       := $(BUILD)/fs
BUILD_SHELL    := $(BUILD)/shell
BUILD_IMPL     := $(BUILD)/impl
BUILD_USER     := $(BUILD)/user

CC             := i686-elf-gcc
AS             := nasm
ASFLAGS        := -I $(SRC_BOOT) -f bin
CFLAGS         := -c -m32 -I $(SRC_LIB) -I $(SRC_LIB)/kernel -I $(SRC_LIB)/device -I $(SRC_LIB)/thread -I $(SRC_LIB)/userprog -I $(SRC_LIB)/shell -I $(SRC_LIB)/user -I $(SRC_LIB)/fs -fno-stack-protector -fno-builtin -nostdlib
LDFLAGS        := -Ttext 0xc0001500 -e main -m elf_i386

KERNEL_OBJS := \
  $(BUILD_KERNEL)/main.o \
  $(BUILD_KERNEL)/interrupt.o \
  $(BUILD_DEVICE)/timer.o \
  $(BUILD_DEVICE)/console.o \
  $(BUILD_KERNEL)/print.o \
  $(BUILD_KERNEL)/kernel.o \
  $(BUILD_KERNEL)/init.o \
  $(BUILD_KERNEL)/debug.o \
  $(BUILD_KERNEL)/memory.o \
  $(BUILD_KERNEL)/bitmap.o \
  $(BUILD_IMPL)/string.o \
  $(BUILD_THREAD)/switch.o \
  $(BUILD_THREAD)/thread.o \
  $(BUILD_THREAD)/sync.o \
  $(BUILD_KERNEL)/list.o \
  $(BUILD_DEVICE)/keyboard.o \
  $(BUILD_DEVICE)/ioqueue.o \
  $(BUILD_USERPROG)/tss.o \
  $(BUILD_USERPROG)/process.o \
  $(BUILD_USERPROG)/syscall-init.o \
  $(BUILD_USER)/syscall.o \
  $(BUILD_IMPL)/stdio.o \
  $(BUILD_KERNEL)/stdio-kernel.o \
  $(BUILD_DEVICE)/ide.o \
  $(BUILD_FS)/fs.o \
  $(BUILD_FS)/dir.o \
  $(BUILD_FS)/file.o \
  $(BUILD_FS)/inode.o \
  $(BUILD_USERPROG)/fork.o \
  $(BUILD_USERPROG)/exec.o \
  $(BUILD_USER)/assert.o \
  $(BUILD_SHELL)/shell.o \
  $(BUILD_SHELL)/builtin_cmd.o \
  $(BUILD_SHELL)/pipe.o \
  $(BUILD_USERPROG)/wait_exit.o \
  $(BUILD_DEVICE)/cmos.o \
  $(BUILD_IMPL)/math.o \

$(BUILD_KERNEL)/%.o: $(SRC_KERNEL)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_KERNEL)/%.o: $(SRC_KERNEL)/%.s
	mkdir -p $(dir $@)
	nasm -f elf $< -o $@

$(BUILD_DEVICE)/%.o: $(SRC_DEVICE)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_THREAD)/%.o: $(SRC_THREAD)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_THREAD)/%.o: $(SRC_THREAD)/%.s
	mkdir -p $(dir $@)
	nasm -f elf $< -o $@

$(BUILD_SHELL)/%.o: $(SRC_SHELL)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_USERPROG)/%.o: $(SRC_USERPROG)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_FS)/%.o: $(SRC_FS)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_IMPL)/%.o: $(SRC_IMPL)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_USER)/%.o: $(SRC_USER)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_TAU)/%.o: $(SRC_TAU)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_BOOT)/%.bin: $(SRC_BOOT)/%.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_KERNEL)/kernel.bin: $(KERNEL_OBJS)
	mkdir -p $(dir $@)
	ld $(LDFLAGS) $(KERNEL_OBJS) -o $@

$(BUILD)/winx.img: $(BUILD_BOOT)/mbr.bin $(BUILD_BOOT)/loader.bin $(BUILD_KERNEL)/kernel.bin
	./disk.sh
	yes | bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $(BUILD)/winx.img
	dd if=$(BUILD_BOOT)/mbr.bin of=$(BUILD)/winx.img bs=512 count=1 conv=notrunc
	dd if=$(BUILD_BOOT)/loader.bin of=$(BUILD)/winx.img bs=512 count=4 conv=notrunc seek=2
	dd if=$(BUILD_KERNEL)/kernel.bin of=$(BUILD)/winx.img bs=512 count=200 conv=notrunc seek=10

.PHONY: build clean qemu bochs
build: $(BUILD)/winx.img

clean:
	rm -rf $(BUILD)

bochs: $(BUILD)/winx.img
	rm -rf $(BUILD)/winx.img.lock
	echo c | bochs -q

all: clean build
