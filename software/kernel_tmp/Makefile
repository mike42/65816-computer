.PHONY: run clean

CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

SOURCES = $(wildcard *.s)
OBJECTS = $(SOURCES:.s=.o)

kernel_tmp.bin: $(OBJECTS) kernel_tmp.cfg
	$(LD65) -o kernel_tmp.bin \
		-Ln kernel_tmp.lbl \
		-C kernel_tmp.cfg \
		$(OBJECTS)

%.o: %.s
	$(CA65) --feature string_escapes --cpu 65816 --debug-info $<

clean:
	rm -f kernel_tmp.bin $(OBJECTS)

