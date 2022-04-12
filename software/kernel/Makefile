CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

SOURCES = $(wildcard *.s) $(wildcard device/*.s)
OBJECTS = $(SOURCES:.s=.o)

kernel.bin: $(OBJECTS) system.cfg
	$(LD65) -o kernel.bin \
		-C system.cfg \
		main.o \
		device/uart.o

%.o: %.s
	$(CA65) --cpu 65816 $<

clean:
	rm -f kernel.bin $(OBJECTS)
