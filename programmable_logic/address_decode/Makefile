GALETTE=galette
MINIPRO=minipro

all: address_decode.jed

address_decode.jed: address_decode.pld
	$(GALETTE) $^
	../pin_to_csv.py address_decode.pin address_decode.csv --name Address_Decoder
	kipart --overwrite -r generic --output address_decode.lib address_decode.csv

clean:
	rm -f *.jed *.fus *.chp *.pin *.csv *.lib

# Write JED to address decode PLD
write_address_decode: address_decode.jed
	$(MINIPRO) -p ATF22V10CQZ -w address_decode.jed

