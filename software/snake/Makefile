.PHONY: run clean

CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

SOURCES = $(wildcard *.s)
OBJECTS = $(SOURCES:.s=.o)

snake.bin: $(OBJECTS) snake.cfg
	$(LD65) -o snake.bin \
		-C snake.cfg \
		$(OBJECTS)

%.o: %.s
	$(CA65) --feature string_escapes --cpu 65816 --debug-info $<

clean:
	rm -f snake.bin $(OBJECTS)
