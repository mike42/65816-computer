EMULATOR=../emulator/cmake-build-debug/emulator

.PHONY: kernel clean all

all: kernel ramfs.bin

kernel:
	make -C kernel kernel.bin

ramfs.bin:
	(cd ramfs && mkdir -p bin/ dev/ &&  python3 ../build_tools/ramfs.py --create --file ../ramfs.bin *)

clean:
	make -C kernel clean
	rm ramfs.bin

run: kernel ramfs.bin
	$(EMULATOR) kernel/kernel.bin ramfs.bin
