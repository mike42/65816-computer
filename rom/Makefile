CA65=/usr/bin/ca65
LD65=/usr/bin/ld65
MINIPRO=minipro

.PHONY: all

all: rom.bin nop.bin

rom.bin: main.o multitasking_test.o uart_test.o system.cfg
	$(LD65) -o rom.bin \
		-C system.cfg \
		main.o \
		multitasking_test.o \
		uart_test.o
	# padded to 32k
	truncate -s 32768 rom.bin

nop.bin:
	python3 -c "open('nop.bin', 'wb').write(b'\xea' * 32768)"

main.o: main.s
	$(CA65) --cpu 65816 main.s

multitasking_test.o: multitasking_test.s
	$(CA65) --cpu 65816 multitasking_test.s

uart_test.o: uart_test.s
	$(CA65) --cpu 65816 uart_test.s

clean:
	rm -f main.o multitasking_test.o uart_test.o rom.bin

flashrom: rom.bin
	$(MINIPRO) -p AT28C256 -w rom.bin
