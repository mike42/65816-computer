; address.s: Base addresses for I/O devices
.export VIA_BASE
.export UART_BASE
.export BOOTLOADER_BASE

BOOTLOADER_BASE = $7c00
VIA_BASE = $c000
UART_BASE = $c800
