CA65=/usr/bin/ca65
LD65=/usr/bin/ld65
MINIPRO=minipro

rom.bin: main.o multitasking_test.o uart_test.o system.cfg
	$(LD65) -o rom.bin \
		-C system.cfg \
		main.o \
		multitasking_test.o \
		uart_test.o
	# padded to 32k
	truncate -s 32768 rom.bin

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
