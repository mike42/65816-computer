.PHONY: run clean

CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

SOURCES = $(wildcard *.s)
OBJECTS = $(SOURCES:.s=.o)

hexdump.bin: $(OBJECTS) hexdump.cfg
	$(LD65) -o hexdump.bin \
		-C hexdump.cfg \
		-Ln hexdump.lbl \
		$(OBJECTS)

%.o: %.s
	$(CA65) --cpu 65816 --debug-info $<

clean:
	rm -f hexdump.bin hexdump.lbl $(OBJECTS)

