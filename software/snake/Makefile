.PHONY: run clean test

CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

# Source/obj files excluding those used for tests
SRC_FILES = $(filter-out $(wildcard *_test.s), $(wildcard *.s))
OBJ_FILES = $(SRC_FILES:.s=.o)

default: snake.bin

run: snake.bin
	(cd ../../ &&./emulator/build/release/emulator --kernel software/snake/snake.bin --rom rom/rom_emulator.bin --trace)

test: random_test.bin
	../../emulator/build/release/emulator --test random_test.bin

%_test.bin: %_test.o %.o
	$(LD65) -o $@ \
		-C test.cfg \
        -Ln $(@:.bin=.lbl) \
		$^

snake.bin: $(OBJ_FILES) snake.cfg
	$(LD65) -o snake.bin \
		-C snake.cfg \
        -Ln snake.lbl \
		$(OBJ_FILES)

%.o: %.s
	$(CA65) --feature string_escapes --cpu 65816 --debug-info $<

clean:
	rm -f *.bin *.o *.lbl
