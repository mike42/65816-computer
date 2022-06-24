CA65=/usr/bin/ca65
LD65=/usr/bin/ld65
MINIPRO=minipro

.PHONY: all clean flashrom

all: rom.bin nop.bin

SOURCES = $(wildcard *.s)
OBJECTS = $(SOURCES:.s=.o)

rom.bin: $(OBJECTS) system.cfg
	$(LD65) -o rom.bin \
		-Ln rom.txt \
		-C system.cfg \
		$(OBJECTS)
	# padded to 32k
	truncate -s 32768 rom.bin

nop.bin:
	python3 -c "open('nop.bin', 'wb').write(b'\xea' * 32768)"

%.o: %.s
	$(CA65) --feature string_escapes --cpu 65816 --debug-info $<

clean:
	rm -f rom.bin $(OBJECTS) nop.bin rom.txt

flashrom: rom.bin
	$(MINIPRO) -p AT28C256 -w rom.bin
