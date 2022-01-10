CA65=/usr/bin/ca65
LD65=/usr/bin/ld65
MINIPRO=minipro

rom.bin: main.o system.cfg
	$(LD65) -o rom.bin \
		-C system.cfg \
		main.o
	# padded to 32k
	truncate -s 32768 rom.bin

main.o: main.s
	$(CA65) main.s

clean:
	rm -f main.o rom.bin

flashrom: rom.bin
	$(MINIPRO) -p AT28C256 -w rom.bin

