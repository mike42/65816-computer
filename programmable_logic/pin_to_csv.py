#!/usr/bin/env python3
"""
pin_to_csv.py: Convert Galette .pin file to .csv file suitable for KiPart.

This may be used to generate a KiCAD .lib (symbol library) file for the parts
in this repository.
"""
import argparse
import csv
import sys
from dataclasses import dataclass
from typing import List


@dataclass
class PinDef:
    pin: int
    name: str
    type: str
    side: str

    @staticmethod
    def from_str(inp: str):
        # Parse one line of pin file to PinDef
        parts = [x.strip() for x in inp.split("|")]
        if len(parts) != 3:
            # blank lines, etc.
            return None
        if not parts[0].isnumeric():
            # Header line
            return None
        # Should have a pin definition here
        pin_number = int(parts[0])
        # Mapping for pin name
        pin_name = parts[1]
        if pin_name[0] == "/":
            # Active-low /FOO becomes ~FOO in KiCad
            pin_name = "~" + pin_name[1:]
        # Mapping for pin type and side
        pin_type_lookup = {
            "Clock/Input": "input",
            "Input": "input",
            "GND": "power_in",
            "VCC": "power_in",
            "Output": "output",
            "NC": "no_connect"
        }
        pin_side_lookup = {
            "Clock/Input": "left",
            "Input": "left",
            "GND": "bottom",
            "VCC": "top",
            "Output": "right",
            "NC": "left"
        }
        raw_pin_type = parts[2]
        pin_type = pin_type_lookup.get(raw_pin_type)
        pin_side = pin_side_lookup.get(raw_pin_type)
        if pin_type is None or pin_side is None:
            raise ValueError(f"Pin type '{raw_pin_type}' is not supported by this tool")
        return PinDef(pin_number, pin_name, pin_type, pin_side)

@dataclass
class PinDefList:
    definitions: List[PinDef]

    @staticmethod
    def from_str_list(inp: List[str]):
        """ Convert list of pin definition strings"""
        pin_definitions = [PinDef.from_str(x) for x in inp]
        # Filter out lines that did not contain a pin definition (headers/formatting)
        pin_definitions = [x for x in pin_definitions if x is not None]
        # Check for duplicate pin names/numbers
        pin_number_list = [x.pin for x in pin_definitions]
        pin_number_set = set(pin_number_list)
        if len(pin_number_set) != len(pin_number_list):
            raise ValueError("Duplicate pin number in input file.")
        pin_name_list = [x.name for x in pin_definitions if x.name != "NC"]
        pin_name_set = set(pin_name_list)
        if len(pin_name_set) != len(pin_name_list):
            raise ValueError("Duplicate pin name in input file.")
        # Everything is OK! :)
        return PinDefList(pin_definitions)


def main(argv: List[str]):
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="Convert Galette .pin file to .csv file suitable for KiPart.")
    parser.add_argument('infile', nargs='?', type=argparse.FileType('r'), default=sys.stdin)
    parser.add_argument('outfile', nargs='?', type=argparse.FileType('w'), default=sys.stdout)
    parser.add_argument('--name', type=str)
    args = parser.parse_args(argv)
    try:
        # Parse inputs
        pin_definitions = PinDefList.from_str_list(args.infile.readlines())
        # Deliver outputs
        args.outfile.write(f"{args.name}\n\n")
        fieldnames = ["Pin", "Type", "Name", "Side"]
        writer = csv.DictWriter(args.outfile, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows([{'Pin': x.pin, "Type": x.type, "Name": x.name, "Side": x.side} for x in pin_definitions.definitions])
    finally:
        args.infile.close()
        args.outfile.close()


if __name__ == "__main__":
    #eg. main(["irq_controller.pin", "irq_controller.csv", "--name", "IRQ_Controller"])
    main(sys.argv[1:])
