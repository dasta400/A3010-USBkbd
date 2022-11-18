# Teensy++ 2.0 based keyboard controller for Acorn Archimedes A3010 keyboard to USB

![A3010 keyboard to USB](https://github.com/dasta400/A3010-USBkbd/blob/master/A3010kbd2USB.jpg "A3010 keyboard to USB")

The Teensy board is connected to an Acorn Archimedes A3010 membrane switch matrix and acts as microcontroller and USB HID, thus allowing the connections of this keyboard to any computer through USB.

## The Acorn Archimedes A3010 keyboard

The A3010 keyboard has a key layout very similar to the industry standard PC keyboard. With 103 keys, 3 status LEDs (Caps Lock, Scroll Lock, Num Lock) and 2 extra LEDs (Power On, Disk Activity).

On the original A3010 board the matrix was driven by a 87C51 8-bit microcontroller, except the Disk Activity LED. As the Teensy does not know anything about the disk activity on a computer, this LED can be used for another purpose. For now, I am not using it.

## Materials used

* 1x Acorn Archimedes A3010 keyboard
* 1x Teensy++ 2.0
* 2x AMP 7-520355-0 connectors
* For the LEDs
  * 1x 220R carbon resistor 1/4 W
  * 2x 270R carbon resistors 1/4 W
  * 2x 330R carbon resistors 1/4 W
* 1x Mini-b to A-Type USB cable
* Soldering iron, solder, cable, pin headers and perfboard

## Current status

* All keys, except pound and keypad # implemented.
* All LEDs (CapsLock, ScrollLock, NumLock) implemented.
* Power On LED is connected to +5V
* Disk Activity LED is connected to a pin header, so it can be connected to a disk drive busy signal.

## Tested systems

* Reportedly works (tested by me) on:
  * Windows 10
  * Linux Mint 20.3 Cinnamon
  * Debian Bullseye (32-bit) on Raspberry Pi
  * RISC OS 5.28 on Raspberry Pi
