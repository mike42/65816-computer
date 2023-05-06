EMULATOR=../emulator/cmake-build-debug/emulator

.PHONY: bootloader kernel kernel_tmp snake clean all

all: kernel disk.img ramfs.bin snake

bootloader:
	make -C bootloader boot.bin

kernel_tmp:
	make -C kernel_tmp kernel_tmp.bin

kernel:
	make -C kernel kernel.bin

snake:
	make -C snake snake.bin
	
hexdump:
	make -C hexdump hexdump.bin

ramfs.bin:
	(cd ramfs && mkdir -p bin/ dev/ &&  python3 ../build_tools/ramfs.py --create --file ../ramfs.bin *)

disk.img: bootloader kernel_tmp
	cat bootloader/boot.bin kernel_tmp/kernel_tmp.bin > disk.img

snake.img: bootloader snake
	cat bootloader/boot.bin snake/snake.bin > snake.img

hexdump.img: bootloader hexdump
	cat bootloader/boot.bin hexdump/hexdump.bin > hexdump.img


clean:
	make -C bootloader clean
	make -C kernel clean
	make -C kernel_tmp clean
	rm -f ramfs.bin disk.img

run: kernel ramfs.bin
	$(EMULATOR) kernel/kernel.bin ramfs.bin

