CA65=/usr/bin/ca65
LD65=/usr/bin/ld65
MINIPRO=minipro

.PHONY: all clean flashrom_at28c256 flashrom_sst39lf010

all: rom.bin nop.bin rom_at28c256.bin rom_sst39lf010.bin

SOURCES = $(wildcard *.s)
OBJECTS = $(SOURCES:.s=.o)

rom.bin: $(OBJECTS) system.cfg
	$(LD65) -o rom.bin \
		-Ln rom.txt \
		-C system.cfg \
		$(OBJECTS)

nop.bin:
	python3 -c "open('nop.bin', 'wb').write(b'\xea' * 8192)"

%.o: %.s
	$(CA65) --feature string_escapes --cpu 65816 --debug-info $<

clean:
	rm -f rom.bin rom_at28c256.bin rom_sst39lf010.bin $(OBJECTS) nop.bin rom.txt

# AT28C256 - Used in 5 V system.
rom_at28c256.bin: rom.bin
	cp -f rom.bin rom_at28c256.bin
	# padded to 32 KiB
	truncate -s 32768 rom_at28c256.bin

flashrom_at28c256: rom_at28c256.bin
	$(MINIPRO) -p AT28C256 -w rom_at28c256.bin

# SST39lF010 - Used in 3.3 V system
rom_sst39lf010.bin: rom.bin
	cp -f rom.bin rom_sst39lf010.bin
	# padded to 128 KiB
	truncate -s 131072 rom_sst39lf010.bin

flashrom_sst39lf010: rom_sst39lf010.bin
	$(MINIPRO) -p "SST39LF010@PLCC32" -w rom_sst39lf010.bin

