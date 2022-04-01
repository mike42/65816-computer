# Programmable logic for 65816 computer

This project PLD's to implement some functionality.

## PLD definitions

- `address_decode` - Provides chip select, read and write signals based on a memory address.

## Required tools

You will need quite a few tools to work with these files. Take a look at the `Makefile` in each directory to see how they are used.

### galette

Galette compiles the logic functions to a binary file which can be written to the PLD. See [simon-frankau/galette](https://github.com/simon-frankau/galette).

### pin_to_csv

This converts the output of galette to an input for KiPart. It was written for this project, and is included in the repo.

### KiPart

KiPart generates KiCAD symbols for the programmed PLD's. See [devbisme/KiPart](https://github.com/devbisme/KiPart)

```
pip3 install kipart
```

### minipro

This is a command-line utility which I use to program these chips. See [DavidGriffith/minipro/](https://gitlab.com/DavidGriffith/minipro/)

