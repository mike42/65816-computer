.PHONY: run clean

CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

SOURCES = $(wildcard *.s) $(wildcard device/*.s)
OBJECTS = $(SOURCES:.s=.o)

kernel.bin: $(OBJECTS) system.cfg
	$(LD65) -o kernel.bin \
		-C system.cfg \
		$(OBJECTS)

%.o: %.s
	$(CA65) --feature string_escapes --cpu 65816 --debug-info $<

clean:
	rm -f kernel.bin $(OBJECTS)
