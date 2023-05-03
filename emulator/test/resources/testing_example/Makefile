.PHONY: run clean

CA65=/usr/bin/ca65
LD65=/usr/bin/ld65

test.bin: example.s system.cfg
	$(CA65) --cpu 65816 --debug-info example.s
	$(LD65) -o test.bin \
		-C system.cfg \
		--dbgfile  test.dbg \
		-Ln test.lbl \
		example.o

clean:
	rm -f test.bin example.o test.dbg test.lbl

